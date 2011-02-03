// Original Author:  Edward Allen Wenger,32 4-A06,+41227676980,
//         Created:  Fri May  7 13:11:39 CEST 2010
// $Id: VertexAnalyzer.cc,v 1.14 2011/02/03 18:59:44 sungho Exp $
//

#include "edwenger/VertexAnalyzer/interface/VertexAnalyzer.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "edwenger/VertexAnalyzer/interface/VertexComparator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

VertexAnalyzer::VertexAnalyzer(const edm::ParameterSet& iConfig)
:
  vtxlabel_(iConfig.getUntrackedParameter<edm::InputTag>("vtxlabel")),
  trklabel_(iConfig.getUntrackedParameter<edm::InputTag>("trklabel")),
  jetlabel_(iConfig.getUntrackedParameter<edm::InputTag>("jetlabel")),
  dzcut_(iConfig.getUntrackedParameter<double>("dzcut")),
  minJetEt_(iConfig.getUntrackedParameter<double>("minJetEt")),
  nTracksBins_(iConfig.getUntrackedParameter<int>("nTracksBins")),
  jetTrkVerticesCorr_(iConfig.getUntrackedParameter<bool>("jetTrkVerticesCorr"))

{

}


// ------------ method called to for each event  ------------
void
VertexAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   using namespace edm;
   using namespace std;
   using namespace reco;
   
   //------- Vertices ---------------------
   edm::Handle<reco::VertexCollection> vtxsH;
   iEvent.getByLabel(vtxlabel_,vtxsH);

   reco::VertexCollection vtxs = *vtxsH;
   if(jetTrkVerticesCorr_) std::sort(vtxs.begin(),vtxs.end(),VertexHigherPtSquared()); // sorted by sum pt^2 for jet-vtx correlation
   else std::sort(vtxs.begin(),vtxs.end(),MoreTracksThenLowerChi2<reco::Vertex>());

   //------- Vertex analyzer ---------------
   hVtxSize->Fill(vtxs.size());

   if(!vtxs.size()) return;  // this protects from an event with an empty vertex

   hVtxTracks->Fill(vtxs[0].tracksSize());
   hVtxZ->Fill(vtxs[0].z());

   for(unsigned i=0; i<vtxs.size(); i++)
      hAllVtxZ->Fill(vtxs[i].z());

   if(vtxs.size()<2) return; // downstream below is gauranteed to deal with at least 2 vertices

   hMultVtxTracks->Fill(vtxs[0].tracksSize(),vtxs[1].tracksSize());
   hMultVtxZ->Fill(vtxs[0].z(),vtxs[1].z());
      
   for(unsigned i=0; i<vtxs.size(); i++)
      edm::LogVerbatim("VertexAnalyzer") << "vtx #" << i << " has "
                                         << vtxs[i].tracksSize() << " tracks";

   
   // -------- Vertex,Track,Jet analyzer --------
   if(jetTrkVerticesCorr_){  // this is to check jet,track,vertices correlation

      //------ Jets ---------------------------- 
      edm::Handle<reco::CandidateView> jets;
      iEvent.getByLabel(jetlabel_,jets);

      std::vector<const reco::Candidate *> sortedJets;

      for(unsigned it=0; it<jets->size(); ++it){
         const reco::Candidate* jet = &((*jets)[it]);
         sortedJets.push_back(jet);
         sortByEtRef (&sortedJets);
      }

      // Get Leading jet energy 
      double leadJetEt_=0, leadJetEta_=0, leadJetPhi_=0;

      unsigned index = 0;
      if(sortedJets.size()==0) leadJetEt_ = 0,leadJetEta_ = 100, leadJetPhi_ = 0; // so that dr > 10 for events with no pat jet
      else leadJetEt_ = sortedJets[index]->et(), leadJetEta_ = sortedJets[index]->eta(), leadJetPhi_ = sortedJets[index]->phi();

      if(minJetEt_>leadJetEt_) return;  // if jet 
 
      if(sortedJets.size()>1) 
	 hLeadnSLeadJetEta->Fill(leadJetEta_,sortedJets[1]->eta());

      //------ Tracks -------------------------- 
      edm::Handle<std::vector<reco::Track> > tracks;
      iEvent.getByLabel(trklabel_, tracks);

      const std::string qualityString = "highPurity";

      std::vector<const reco::Track *> sortedTrks;

      for(unsigned it=0; it<tracks->size(); ++it){
         const reco::Track & trk = (*tracks)[it];

         // track selections - do we need eta cut? 
         if(!trk.quality(reco::TrackBase::qualityByName(qualityString))) continue;

         sortedTrks.push_back(&(*tracks)[it]);
         sortByPtRef (&sortedTrks);

         hVtxZTrkEta->Fill(vtxs[0].z(),trk.eta());
      }

      // Tracks/Vertices corrleation ========================
      double dzPV_leading=0, dzSV_leading=0;
      double dzPV_sleading=0, dzSV_sleading=0;

      if(sortedTrks.size()>0){
	 dzPV_leading = sortedTrks[0]->dz(vtxs[0].position());
	 dzSV_leading = sortedTrks[0]->dz(vtxs[1].position());
      }

      if(sortedTrks.size()>1){
	 dzPV_sleading = sortedTrks[1]->dz(vtxs[0].position());
	 dzSV_sleading = sortedTrks[1]->dz(vtxs[1].position());
      }

      double ptSum_PV=0, ptSum_SV=0;
      
      for(unsigned j=0; j<sortedTrks.size(); j++){
	 
	 double dzPV = sortedTrks[j]->dz(vtxs[0].position());
	 double dzSV = sortedTrks[j]->dz(vtxs[1].position());
	 
	 double deta = leadJetEta_ - sortedTrks[j]->eta();
	 double dphi = leadJetPhi_ - sortedTrks[j]->phi();

	 if(fabs(dphi)>(TMath::Pi()) && dphi>0) dphi = dphi - (float) 2.*TMath::Pi();
         if(fabs(dphi)>(TMath::Pi()) && dphi<0) dphi = dphi + (float) 2.*TMath::Pi();

	 double dr = TMath::Sqrt(deta*deta + dphi*dphi); // dr < pi

	 if(j==0) {
	    hLeadingTrkPVdZ->Fill(dzPV), hLeadingTrkSVdZ->Fill(dzSV), hLeadingPVdZdR->Fill(dzPV,dr), hLeadingSVdZdR->Fill(dzSV,dr); // leading
	    if(fabs(leadJetEta_)<2.0) hLeadingTrkPVdZ_narrowEta->Fill(dzPV), hLeadingTrkSVdZ_narrowEta->Fill(dzSV); 
	 }
	 if(j==1) {
	    hSLeadingTrkPVdZ->Fill(dzPV), hSLeadingTrkSVdZ->Fill(dzSV), hSLeadingPVdZdR->Fill(dzPV,dr), hSLeadingSVdZdR->Fill(dzSV,dr);
	    hLeadingAndSLeadingTrkPVdZ->Fill(dzPV_leading,dzPV), hLeadingAndSLeadingTrkSVdZ->Fill(dzSV_leading,dzSV);
	 }
	 if(j==2) {
	    hSSLeadingTrkPVdZ->Fill(dzPV), hSSLeadingTrkSVdZ->Fill(dzSV), hSSLeadingPVdZdR->Fill(dzPV,dr), hSSLeadingSVdZdR->Fill(dzSV,dr);
	    hLeadingAndSSLeadingTrkPVdZ->Fill(dzPV_leading,dzPV), hLeadingAndSSLeadingTrkSVdZ->Fill(dzSV_leading,dzSV);
	    hSLeadingAndSSLeadingTrkPVdZ->Fill(dzPV_sleading,dzPV), hSLeadingAndSSLeadingTrkSVdZ->Fill(dzSV_sleading,dzSV);
	 }
	 
	 hTrkPtFromAV->Fill(sortedTrks[j]->pt());

	 if(fabs(dzPV)<=dzcut_) {
	    hTrkPtFromPV->Fill(sortedTrks[j]->pt());
	    if(j==0) hLeadingJetEtaFromPV->Fill(leadJetEta_), hLeadingdRFromPV->Fill(dr); 
	    if(j==1) hSLeadingJetEtaFromPV->Fill(leadJetEta_), hSLeadingdRFromPV->Fill(dr);
	    if(j==2) hSSLeadingJetEtaFromPV->Fill(leadJetEta_), hSSLeadingdRFromPV->Fill(dr);
	    ptSum_PV = ptSum_PV + sortedTrks[j]->pt();
	 }else if(fabs(dzSV)<=dzcut_) {
	    hTrkPtFromSV->Fill(sortedTrks[j]->pt());
	    if(j==0) hLeadingJetEtaFromSV->Fill(leadJetEta_), hLeadingdRFromSV->Fill(dr);
            if(j==1) hSLeadingJetEtaFromSV->Fill(leadJetEta_), hSLeadingdRFromSV->Fill(dr);
            if(j==2) hSSLeadingJetEtaFromSV->Fill(leadJetEta_), hSSLeadingdRFromSV->Fill(dr);
	    ptSum_SV = ptSum_SV+ sortedTrks[j]->pt();
	 }
      }

      hJetEtTrkPtSumPV->Fill(ptSum_PV,leadJetEta_);
      hJetEtTrkPtSumSV->Fill(ptSum_SV,leadJetEta_);

   } // end of if(jetTrkVerticesCorr_)
}


// ------------ method called once each job just before starting event loop  ------------
void 
VertexAnalyzer::beginJob()
{

   // pt bins
   const double small = 1e-3;
   double ptb;

   // simple rebinning possible with a rebinning factor n = 2, 3, 4 !
   for(ptb =   0  ; ptb <   1.2-small; ptb +=  0.05) ptBins.push_back(ptb); // 24 bins
   for(ptb =   1.2; ptb <   2.4-small; ptb +=  0.1 ) ptBins.push_back(ptb); // 12 bins
   for(ptb =   2.4; ptb <   7.2-small; ptb +=  0.2 ) ptBins.push_back(ptb); // 24 bins
   for(ptb =   7.2; ptb <  13.2-small; ptb +=  0.5 ) ptBins.push_back(ptb); // 12 bins
   for(ptb =  13.2; ptb <  25.2-small; ptb +=  1.0 ) ptBins.push_back(ptb); // 12 bins
   for(ptb =  25.2; ptb <  61.2-small; ptb +=  3.0 ) ptBins.push_back(ptb); // 12 bins
   for(ptb =  61.2; ptb < 121.2-small; ptb +=  5.0 ) ptBins.push_back(ptb); // 12 bins
   for(ptb = 121.2; ptb < 361.2-small; ptb += 10.0 ) ptBins.push_back(ptb); // 24 bins
   ptBins.push_back(361.2);

   // constant pt bins
   static float ptMin;
   static float ptMax;
   static float ptWidth;

   ptMin = 0, ptMax = 1200, ptWidth = 10;

   for(double pt = ptMin; pt < ptMax + ptWidth/2; pt += ptWidth)
      cptBins.push_back(pt);


   // jet et bins
   static float jetMin;
   static float jetMax; 
   static float jetWidth;

   jetMin = 0, jetMax = 2400, jetWidth = 20;

   for(double jet = jetMin; jet < jetMax + jetWidth/2; jet += jetWidth)
      jetBins.push_back(jet);


  hVtxSize = f->make<TH1D>("hVtxSize","number of reconstructed vertices",10,-0.5,9.5);
  hVtxTracks = f->make<TH1D>("hVtxTracks","number of tracks fitted to vertex",nTracksBins_,-0.5,(float)nTracksBins_-0.5);
  hVtxZ = f->make<TH1D>("hVtxZ","z position of best reconstructed vertex",80,-20.0,20.0);
  hMultVtxTracks = f->make<TH2D>("hMultVtxTracks","number of tracks fitted to vertex; most populated; 2nd most populated",
				 nTracksBins_,-0.5,(float)nTracksBins_-0.5,nTracksBins_,-0.5,(float)nTracksBins_-0.5);
  hMultVtxZ = f->make<TH2D>("hMultVtxZ","z position of reconstructed vertex; most populated; 2nd most populated",80,-20.0,20.0,80,-20.0,20.0);
  hAllVtxZ = f->make<TH1D>("hAllVtxZ","z position of all reconstructed vertices",80,-20.0,20.0);

  if(jetTrkVerticesCorr_){
     hLeadingTrkPVdZ = f->make<TH1F>("hLeadingTrkPVdZ","dz of leading track with primary vertex",300,-30,30);
     hSLeadingTrkPVdZ = f->make<TH1F>("hSLeadingTrkPVdZ","dz of sub-leading track with primary vertex",300,-30,30);
     hSSLeadingTrkPVdZ = f->make<TH1F>("hSSLeadingTrkPVdZ","dz of sub-sub-leading track with primary vertex",300,-30,30);
     
     hLeadingTrkSVdZ = f->make<TH1F>("hLeadingTrkSVdZ","dz of leading track with 2nd vertex",300,-30,30);
     hSLeadingTrkSVdZ = f->make<TH1F>("hSLeadingTrkSVdZ","dz of sub-leading track with 2nd vertex",300,-30,30);
     hSSLeadingTrkSVdZ = f->make<TH1F>("hSSLeadingTrkSVdZ","dz of sub-sub-leading track with 2nd vertex",300,-30,30);

     hLeadingAndSLeadingTrkPVdZ = f->make<TH2F>("hLeadingAndSLeadingTrkPVdZ","dz of leading track vs dz of sub-leading tracks",300,-30,30, 300,-30,30);
     hLeadingAndSSLeadingTrkPVdZ = f->make<TH2F>("hLeadingAndSSLeadingTrkPVdZ","dz of leading track vs dz of ssub-leading tracks",300,-30,30, 300,-30,30);
     hSLeadingAndSSLeadingTrkPVdZ = f->make<TH2F>("hSLeadingAndSSLeadingTrkPVdZ","dz of sub-leading track vs dz of ssub-leading tracks",300,-30,30, 300,-30,30);

     hLeadingAndSLeadingTrkSVdZ = f->make<TH2F>("hLeadingAndSLeadingTrkSVdZ","dz of leading track vs dz of sub-leading tracks",300,-30,30, 300,-30,30);
     hLeadingAndSSLeadingTrkSVdZ = f->make<TH2F>("hLeadingAndSSLeadingTrkSVdZ","dz of leading track vs dz of ssub-leading tracks",300,-30,30, 300,-30,30);
     hSLeadingAndSSLeadingTrkSVdZ = f->make<TH2F>("hSLeadingAndSSLeadingTrkSVdZ","dz of sub-leading track vs dz of ssub-leading tracks",300,-30,30, 300,-30,30);

     hLeadingTrkPVdZ_narrowEta = f->make<TH1F>("hLeadingTrkPVdZ_narrowEta","dz of leading track with primary vertex",300,-30,30);
     hLeadingTrkSVdZ_narrowEta = f->make<TH1F>("hLeadingTrkSVdZ_narrowEta","dz of leading track with 2nd vertex",300,-30,30);

     hLeadingJetEtaFromPV = f->make<TH1F>("hLeadingJetEtaFromPV","leading jet et whean l-track is from PV",60,-5.0,5.0);
     hLeadingJetEtaFromSV = f->make<TH1F>("hLeadingJetEtaFromSV","leading jet et whean l-track is from SV",60,-5.0,5.0);
     hSLeadingJetEtaFromPV = f->make<TH1F>("hSLeadingJetEtaFromPV","leading jet et whean sl-track is from PV",60,-5.0,5.0);
     hSLeadingJetEtaFromSV = f->make<TH1F>("hSLeadingJetEtaFromSV","leading jet et whean sl-track is from SV",60,-5.0,5.0);
     hSSLeadingJetEtaFromPV = f->make<TH1F>("hSSLeadingJetEtaFromPV","leading jet et whean ssl-track is from PV",60,-5.0,5.0);
     hSSLeadingJetEtaFromSV = f->make<TH1F>("hSSLeadingJetEtaFromSV","leading jet et whean ssl-track is from SV",60,-5.0,5.0);

     hLeadingdRFromPV = f->make<TH1F>("hLeadingdRFromPV","dr of leading track w.r.t leading jet when from PV",50,0.0,3.2);
     hLeadingdRFromSV =f->make<TH1F>("hLeadingdRFromSV","dr of leading track w.r.t leading jet when from SV",50,0.0,3.2);
     hSLeadingdRFromPV =f->make<TH1F>("hSLeadingdRFromPV","dr of s-leading track w.r.t leading jet when from PV",50,0.0,3.2);
     hSLeadingdRFromSV =f->make<TH1F>("hSLeadingdRFromSV","dr of s-leading track w.r.t leading jet when from SV",50,0.0,3.2);
     hSSLeadingdRFromPV =f->make<TH1F>("hSSLeadingdRFromPV","dr of ss-leading track w.r.t leading jet when from PV",50,0.0,3.2);
     hSSLeadingdRFromSV =f->make<TH1F>("hSSLeadingdRFromSV","dr of ss-leading track w.r.t leading jet when from SV",50,0.0,3.2);

     hTrkPtFromAV = f->make<TH1F>("hTrkPtFromAV","track p_{T} distribution from all vertices;p_{T} (GeV/c)",ptBins.size()-1, &ptBins[0]);
     hTrkPtFromPV = f->make<TH1F>("hTrkPtFromPV","track p_{T} distribution from PV;p_{T} (GeV/c)",ptBins.size()-1, &ptBins[0]);
     hTrkPtFromSV = f->make<TH1F>("hTrkPtFromSV","track p_{T} distribution from SV;p_{T} (GeV/c)",ptBins.size()-1, &ptBins[0]);

     hVtxZTrkEta = f->make<TH2F>("hVtxZTrkEta","vtx vz vs track eta",80,-20.,20.,24,-3.0,3.0);
     hLeadnSLeadJetEta = f->make<TH2F>("hLeadnSLeadJetEta","leading jet eta vs sleading jet eta", 60,-5.0,5.0, 60,-5.0,5.0);

     hLeadingPVdZdR = f->make<TH2F>("hLeadingPVdZdR","dz vs dr of leading track when from PV",60,-30,30, 50,0.0,3.2);
     hSLeadingPVdZdR = f->make<TH2F>("hSLeadingPVdZdR","dz vs dr of s-leading track when from PV",60,-30,30, 50,0.0,3.2);
     hSSLeadingPVdZdR = f->make<TH2F>("hSSLeadingPVdZdR","dz vs dr ss-leading track when from PV",60,-30,30, 50,0.0,3.2);
     hLeadingSVdZdR = f->make<TH2F>("hLeadingSVdZdR","dz vs dr of leading track when from SV",60,-30,30, 50,0.0,3.2);
     hSLeadingSVdZdR = f->make<TH2F>("hSLeadingSVdZdR","dz vs dr of s-leading track when from SV",60,-30,30, 50,0.0,3.2);
     hSSLeadingSVdZdR = f->make<TH2F>("hSSLeadingSVdZdR","dz vs dr ss-leading track when from SV",60,-30,30, 50,0.0,3.2);

     hJetEtTrkPtSumPV = f->make<TH2F>("hJetEtTrkPtSumPV","track sum pt (PV originated) vs leading jet et",cptBins.size()-1, &cptBins[0], jetBins.size()-1, &jetBins[0]);
     hJetEtTrkPtSumSV = f->make<TH2F>("hJetEtTrkPtSumSV","track sum pt (SV originated) vs leading jet et",cptBins.size()-1, &cptBins[0], jetBins.size()-1, &jetBins[0]);
  }
}

// ------------ method called once each job just after ending the event loop  ------------
void 
VertexAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(VertexAnalyzer);
