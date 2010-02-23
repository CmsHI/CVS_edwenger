#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include "math.h"

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TROOT.h>
#include <TSystem.h>

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"

#if !defined(__CINT__) && !defined(__MAKECINT__)

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#endif

void analyzeTracks(){
  
  // event cuts
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

  //----- histograms -----
  TH1D *hL1TechBits = new TH1D("hL1TechBits","L1 technical trigger bits before mask",64,-0.5,63.5);
  TH2D *hHPFracNtrk = new TH2D("hHPFracNtrk","High purity fraction vs. # of tracks; number of tracks; highPurity fraction",25,0,500,25,0,1);
  TH2D *hHfTowers   = new TH2D("hHfTowers","Number of HF tower above threshold; positive side; negative side",20,-0.5,19.5,20,-0.5,19.5);

  TH1D *hVtxSize    = new TH1D("hVtxSize","number of reconstructed pixel vertices",10,-0.5,9.5);
  TH1D *hVtxTrks    = new TH1D("hVtxTrks","number of tracks used to fit pixel vertex",50,-0.5,49.5);
  TH1D *hVtxZ       = new TH1D("hVtxZ","z position of best reconstructed pixel vertex", 80,-20,20);
  
  TH1D *hTrkQual    = new TH1D("hTrkQual","track quality", 20, -0.5, 19.5);
  TH1D *hTrkDxyBeam = new TH1D("hTrkDxyBeam","track dxy from beamspot; dxy [cm]", 20, -2.0, 2.0);
  TH1D *hTrkDzVtx   = new TH1D("hTrkDzVtx","track dz from vertex; dz [cm]", 20, -2.0, 2.0);
  TH2D *hTrkPtErrNhits = new TH2D("hTrkPtErrNhits","track relative pt-error vs. nhits; number of valid hits on track; #sigma(p_{T})/p_{T}",30,-0.5,29.5,20,0.0,0.5);
  TH1D *hTrkNhits   = new TH1D("hTrkNhits", "number of valid hits on track", 30,-0.5,29.5);
  TH1D *hTrkPt      = new TH1D("hTrkPt","track p_{T}; p_{T} [GeV/c]", 80, 0.0, 20.0);
  TH1D *hTrkEta     = new TH1D("hTrkEta","track #eta; #eta", 30, -3.0, 3.0);
  TH1D *hTrkPhi     = new TH1D("hTrkPhi","track #phi; #phi [radians]", 28, -3.5, 3.5);
  
  //----- input files (run 124023) -----
  vector<string> fileNames;
  string fileDir = "rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/Feb9ReReco_v2/0025";
  fileNames.push_back(fileDir + "/F68A5ABB-E515-DF11-8A77-0026189437F0.root");
  fileNames.push_back(fileDir + "/F67155E0-E815-DF11-B478-003048678B30.root");
  fileNames.push_back(fileDir + "/F4893FBB-E515-DF11-883A-0030486792B4.root");
  /*fileNames.push_back(fileDir + "/D8FC2BFE-E615-DF11-9533-003048678B5E.root");
  fileNames.push_back(fileDir + "/D0C1373B-EB15-DF11-8BF7-0026189438CF.root");
  fileNames.push_back(fileDir + "/CE78C9FD-E615-DF11-A6F5-00261894394F.root");
  fileNames.push_back(fileDir + "/C8283EFD-E615-DF11-A603-00261894383B.root");
  fileNames.push_back(fileDir + "/C69943C1-E515-DF11-B659-00304867920C.root");
  fileNames.push_back(fileDir + "/C0DB53DA-E915-DF11-9252-002618943921.root");
  fileNames.push_back(fileDir + "/C022BEB9-E515-DF11-8A2B-003048679168.root");
  fileNames.push_back(fileDir + "/AE991602-EA15-DF11-BD87-003048B95B30.root");
  fileNames.push_back(fileDir + "/A08741B4-E615-DF11-A7AA-002618943977.root");
  fileNames.push_back(fileDir + "/9CAF22B4-E615-DF11-9374-0026189438B3.root");
  fileNames.push_back(fileDir + "/90A835B4-E615-DF11-8A69-00261894395A.root");
  fileNames.push_back(fileDir + "/864065D7-E715-DF11-A278-0026189438DA.root");
  fileNames.push_back(fileDir + "/804FF8B4-E615-DF11-BCD2-00261894392B.root");
  fileNames.push_back(fileDir + "/765C2490-E115-DF11-853E-003048679168.root");
  fileNames.push_back(fileDir + "/6AE396F0-ED15-DF11-8B1E-0026189438AD.root");
  fileNames.push_back(fileDir + "/66CB14B4-E615-DF11-9786-0026189438CC.root");
  fileNames.push_back(fileDir + "/643DB5B2-E615-DF11-8D6B-003048D42D92.root");
  fileNames.push_back(fileDir + "/62E0C7B9-E515-DF11-82C2-002618943866.root");
  fileNames.push_back(fileDir + "/5E0C3B3C-EB15-DF11-A99B-003048678BAA.root");
  fileNames.push_back(fileDir + "/58F48BF9-EB15-DF11-B1E2-00261894389E.root");
  fileNames.push_back(fileDir + "/3C3A7DF6-EC15-DF11-A34B-003048678EE2.root");
  fileNames.push_back(fileDir + "/3C0E0491-E315-DF11-B3C2-0026189437F0.root");
  fileNames.push_back(fileDir + "/3A991163-DA15-DF11-A28B-0026189438D6.root");
  fileNames.push_back(fileDir + "/2E9847BB-E515-DF11-AE34-0026189438E1.root");
  fileNames.push_back(fileDir + "/287BF4FD-E615-DF11-9013-00248C0BE005.root");
  fileNames.push_back(fileDir + "/26DAA4BA-E515-DF11-961B-002618943977.root");
  fileNames.push_back(fileDir + "/226244B4-E615-DF11-AA1F-00248C0BE005.root");
  fileNames.push_back(fileDir + "/124B7EBB-E515-DF11-BFF8-003048678B5E.root");
  fileNames.push_back(fileDir + "/0CFA07DC-E715-DF11-A51D-002618943845.root");
  fileNames.push_back(fileDir + "/0A76F1E0-E815-DF11-9023-002618943921.root");*/
  fwlite::ChainEvent event(fileNames);

  //----- loop over events -----
  unsigned int iEvent=0;
  for(event.toBegin(); !event.atEnd(); ++event, ++iEvent){

    //if( iEvent == 1000 ) break;
    if( iEvent % 1000 == 0 ) cout<<"Processing event : "<<iEvent<<endl;

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
    vertices.getByLabel(event, "pixelVertices");
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

    // get beamspot if there are any tracks in the event
    fwlite::Handle<reco::BeamSpot> beamspot;
    if(tracks->size()) beamspot.getByLabel(event, "offlineBeamSpot");

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
      if(pterr > ptErrCut) continue;

      // select tracks based on number of valid rechits
      hTrkNhits->Fill(nhits);
      if(nhits < nHitsCut) continue;

      // fill selected track histograms
      hTrkPt->Fill(trk.pt());
      hTrkEta->Fill(trk.eta());
      hTrkPhi->Fill(trk.phi());

    }
    
  }
  
  cout<<"Number of events processed : "<<iEvent<<endl;

  // write to output file
  TFile outFile("trackHists.root", "recreate" );
  outFile.cd(); outFile.mkdir("evt"); outFile.cd("evt");
  hL1TechBits->Write();
  hHPFracNtrk->Write();
  hHfTowers->Write();

  outFile.cd(); outFile.mkdir("vtx"); outFile.cd("vtx");
  hVtxSize->Write();
  hVtxTrks->Write();
  hVtxZ->Write();
  
  outFile.cd(); outFile.mkdir("trk"); outFile.cd("trk");
  hTrkQual->Write();
  hTrkDxyBeam->Write();
  hTrkDzVtx->Write();
  hTrkPtErrNhits->Write();
  hTrkNhits->Write();
  hTrkPt->Write();
  hTrkEta->Write();
  hTrkPhi->Write();
  
  outFile.ls();
  outFile.Close();

  // free allocated space
  delete hL1TechBits; 
  delete hHPFracNtrk;
  delete hHfTowers;

  delete hVtxSize;
  delete hVtxTrks;
  delete hVtxZ;

  delete hTrkQual; 
  delete hTrkDxyBeam;
  delete hTrkDzVtx;
  delete hTrkPtErrNhits; 
  delete hTrkNhits;
  delete hTrkPt;
  delete hTrkEta;
  delete hTrkPhi;

}
