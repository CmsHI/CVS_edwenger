// Usage:
// .x rootlogon.C
// .x analyzeTracks.C++

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include "math.h"

#include <TH1D.h>
#include <TH2D.h>
#include <TNtuple.h>
#include <TFile.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

#if !defined(__CINT__) && !defined(__MAKECINT__)

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#endif

void analyzeTracks(bool debug=false){

  // is gen track needed
  const bool isGEN = false;

  // event cuts
  const unsigned int maxEvents = -1;
  const float hpFracCut = 0.25;
  const unsigned nTrackCut = 10;
  const double hfEThreshold = 3.0;
  const int nTowerThreshold = 1;
  // track cuts
  const string qualityString = "highPurity";
  const double normD0Cut = 3.0;
  const double normDZCut = 3.0;
  const double ptErrCut = 0.1;
  const unsigned nHitsCut = 1; // at least this many hits on track
  const double ptDebug = 3.0;  // fill debug ntuple for this selection

  //----- input files (900 GeV data) -----
  vector<string> fileNames;
  string fileDir = "/d101/edwenger/data/v4";       // data skim
  //string fileDir = "/d101/y_alive/mc/crab/v3";   // mc skim
  cout << "directory: '" << fileDir << "'" << endl;
  for(int ifile=1; ifile<=31; ifile++) {
    TString name = Form("trkAnaSkimAOD_%d.root",ifile);
    cout << "  adding file: " << name.Data() << endl;
    fileNames.push_back(fileDir + "/" + name.Data());
  }
  fwlite::ChainEvent event(fileNames);

  //----- define output hists/trees in directories of output file -----
  TFile *outFile = new TFile("trackHists.root", "recreate" );
  TH1D::SetDefaultSumw2();

  // evt hists
  outFile->cd(); outFile->mkdir("evt"); outFile->cd("evt");
  TH2D *hRunLumi    = new TH2D("hRunLumi","Event information; run number; luminosity block",500,123549.5,124049.5,200,-0.5,199.5);
  TH1D *hL1TechBits = new TH1D("hL1TechBits","L1 technical trigger bits before mask",64,-0.5,63.5);
  TH2D *hHPFracNtrk = new TH2D("hHPFracNtrk","High purity fraction vs. # of tracks; number of tracks; highPurity fraction",50,0,500,50,0,1);
  TH2D *hHfTowers   = new TH2D("hHfTowers","Number of HF tower above threshold; positive side; negative side",80,-0.5,79.5,80,-0.5,79.5);

  // vtx hists
  outFile->cd(); outFile->mkdir("vtx"); outFile->cd("vtx");
  TH1D *hVtxSize    = new TH1D("hVtxSize","number of reconstructed pixel vertices",10,-0.5,9.5);
  TH1D *hVtxTrks    = new TH1D("hVtxTrks","number of tracks used to fit pixel vertex",50,-0.5,49.5);
  TH1D *hVtxZ       = new TH1D("hVtxZ","z position of best reconstructed pixel vertex", 80,-20,20);
  TH2D *hBeamXRun   = new TH2D("hBeamXRun","x position of beamspot; run number",500,123549.5,124049.5,80,-0.2,0.2);
  TH2D *hBeamYRun   = new TH2D("hBeamYRun","y position of beamspot; run number",500,123549.5,124049.5,80,-0.2,0.2);
  TH2D *hBeamZRun   = new TH2D("hBeamZRun","z position of beamspot; run number",500,123549.5,124049.5,80,-2,2);

  // track hists
  outFile->cd(); outFile->mkdir("trk"); outFile->cd("trk");
  TH1D *hTrkQual    = new TH1D("hTrkQual","track quality", 20, -0.5, 19.5);
  TH1D *hTrkDxyBeam = new TH1D("hTrkDxyBeam","track dxy from beamspot; dxy [cm]", 80, -2.0, 2.0);
  TH1D *hTrkDzVtx   = new TH1D("hTrkDzVtx","track dz from vertex; dz [cm]", 80, -2.0, 2.0);
  TH2D *hTrkPtErrNhits = new TH2D("hTrkPtErrNhits","track relative pt-error vs. nhits; number of valid hits on track; #sigma(p_{T})/p_{T}",30,-0.5,29.5,80,0.0,0.5);
  TH2D *hTrkPtErrEta = new TH2D("hTrkPtErrEta","track relative pt-error vs. #eta; #eta; #sigma(p_{T})/p_{T}",60,-3,3,80,0.0,0.5);
  TH1D *hTrkNhits   = new TH1D("hTrkNhits", "number of valid hits on track", 30,-0.5,29.5);
  TH1D *hTrkPt      = new TH1D("hTrkPt","track p_{T}; p_{T} [GeV/c]", 80, 0.0, 20.0);
  TH1D *hTrkEta     = new TH1D("hTrkEta","track #eta; #eta", 60, -3.0, 3.0);
  TH1D *hTrkPhi     = new TH1D("hTrkPhi","track #phi; #phi [radians]", 56, -3.5, 3.5);

  // gen track hists
  TH1D *hGenTrkPt=0;      
  if(isGEN) hGenTrkPt = new TH1D("hGenTrkPt","track p_{T}; p_{T} [GeV/c]", 80, 0.0, 20.0);

  // debug ntuple
  outFile->cd();
  TNtuple *nt=0;
  if(debug) nt = new TNtuple("nt","track debug ntuple","pt:eta:phi:qual:algo:hits:pterr:d0:d0err:dz:dzerr");

  //----- loop over events -----
  unsigned int iEvent=0;
  for(event.toBegin(); !event.atEnd(); ++event, ++iEvent){

    if( iEvent == maxEvents ) break;
    if( iEvent % 1000 == 0 ) cout << "Processing " << iEvent<< "th event: "
				  << "run " << event.id().run() 
				  << ", lumi " << event.luminosityBlock() 
				  << ", evt " << event.id().event() << endl;

    // fill event info
    hRunLumi->Fill(event.id().run(),event.luminosityBlock());

    // select on L1 trigger bits
    fwlite::Handle<L1GlobalTriggerReadoutRecord> gt;
    gt.getByLabel(event, "gtDigis");
    const TechnicalTriggerWord&  word = gt->technicalTriggerWord(); //before mask
    for(int bit=0; bit<64; bit++) hL1TechBits->Fill(bit,word.at(bit));
    if(!word.at(0)) continue;  // BPTX coincidence
    if(!word.at(34)) continue; // BSC single-side
    if(word.at(36) || word.at(37) || word.at(38) || word.at(39)) continue; // BSC halo
    
    // select on coincidence of HF towers above threshold
    fwlite::Handle<CaloTowerCollection> towers;
    towers.getByLabel(event, "towerMaker");
    int nHfTowersN=0, nHfTowersP=0;
    for(CaloTowerCollection::const_iterator calo = towers->begin(); calo != towers->end(); ++calo) {
      if(calo->energy() < hfEThreshold) continue;
      if(calo->eta()>3) nHfTowersP++;
      if(calo->eta()<-3) nHfTowersN++;
    }
    hHfTowers->Fill(nHfTowersP,nHfTowersN);
    if(nHfTowersP < nTowerThreshold || nHfTowersN < nTowerThreshold) continue;

    // select on high-purity track fraction
    fwlite::Handle<std::vector<reco::Track> > tracks;
    tracks.getByLabel(event, "generalTracks");
    int numhighpurity = 0;
    float fraction = 0;
    for(unsigned it=0; it<tracks->size(); ++it)
      if((*tracks)[it].quality(reco::TrackBase::qualityByName(qualityString))) numhighpurity++;
    if(tracks->size() > 0) fraction = (float)numhighpurity/(float)tracks->size();
    hHPFracNtrk->Fill(tracks->size(),fraction);
    if(fraction<hpFracCut && tracks->size()>nTrackCut) continue;

    // select on requirement of valid vertex
    fwlite::Handle<std::vector<reco::Vertex> > vertices;
    vertices.getByLabel(event, "pixel3Vertices");
    hVtxSize->Fill(vertices->size());
    if(!vertices->size()) continue;
    size_t maxtracks=0; double bestvz=-999.9, bestvx=-999.9, bestvy=-999.9, bestNchi2=999.9;
    for(unsigned it=0; it<vertices->size(); ++it) {
      const reco::Vertex & vtx = (*vertices)[it];
      if(vtx.tracksSize() > maxtracks
	 || (vtx.tracksSize() == maxtracks && vtx.normalizedChi2() < bestNchi2) ) {
	maxtracks = vtx.tracksSize();
	bestvz = vtx.z(); bestvx = vtx.x(); bestvy = vtx.y();
	bestNchi2 = vtx.normalizedChi2();
      }	
    }
    hVtxTrks->Fill(maxtracks);
    hVtxZ->Fill(bestvz);

    // get beamspot
    fwlite::Handle<reco::BeamSpot> beamspot;
    beamspot.getByLabel(event, "offlineBeamSpot");
    hBeamXRun->Fill(event.id().run(),beamspot->x0());
    hBeamYRun->Fill(event.id().run(),beamspot->y0());
    hBeamZRun->Fill(event.id().run(),beamspot->z0());

    // TO DO: get hlt bits

    //----- loop over tracks -----
    for(unsigned it=0; it<tracks->size(); ++it){
      
      const reco::Track & trk = (*tracks)[it];
      
      // select high purity tracks
      hTrkQual->Fill(trk.qualityMask());
      if(!trk.quality(reco::TrackBase::qualityByName(qualityString))) continue;

      // select tracks based on transverse proximity to beamspot
      double dxybeam = trk.dxy(beamspot->position());
      hTrkDxyBeam->Fill(dxybeam);
      if(fabs(dxybeam/trk.d0Error()) > normD0Cut) continue;

      // select tracks based on z-proximity to best vertex 
      math::XYZPoint bestvtx(bestvx,bestvy,bestvz);
      double dzvtx = trk.dz(bestvtx);
      hTrkDzVtx->Fill(dzvtx);
      if(fabs(dzvtx/trk.dzError()) > normDZCut) continue;

      // select tracks based on relative pt error
      double pterr = trk.ptError()/trk.pt();
      unsigned nhits = trk.numberOfValidHits();
      hTrkPtErrNhits->Fill(nhits,pterr);
      hTrkPtErrEta->Fill(trk.eta(),pterr);
      if(debug && trk.pt() > ptDebug) // fill debug ntuple for selection of tracks
	nt->Fill(trk.pt(),trk.eta(),trk.phi(),trk.qualityMask(),trk.algo(),nhits,trk.ptError(),dxybeam,trk.d0Error(),dzvtx,trk.dzError());
      if(pterr > ptErrCut) continue;

      // select tracks based on number of valid rechits
      hTrkNhits->Fill(nhits);
      if(nhits < nHitsCut) continue;

      // fill selected track histograms
      hTrkPt->Fill(trk.pt());
      hTrkEta->Fill(trk.eta());
      hTrkPhi->Fill(trk.phi());

    }

    //---- loop over MC gen level track ---- 
    if(!isGEN) continue;
    fwlite::Handle<std::vector<reco::GenParticle> > genTracks;
    genTracks.getByLabel(event, "genParticles");
    
    for(unsigned ip=0; ip<genTracks->size(); ++ip){
       const reco::GenParticle & p = (*genTracks)[ip];
       if(p.status() != 1) continue;
       if(p.collisionId() != 0) continue;
       if(p.charge() == 0) continue;
       if(fabs(p.eta())>2.5) continue;
       // fill selected GEN track histograms
       hGenTrkPt->Fill(p.pt());
    }
    

    
  }
  
  cout << "Number of events processed : " << iEvent << endl;
  cout << "Number passing all event selection cuts: " << hVtxZ->GetEntries() << endl;

  // write to output file
  outFile->Write();
  outFile->ls();
  outFile->Close();

}
