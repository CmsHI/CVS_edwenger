// Original Author:  Edward Allen Wenger,32 4-A06,+41227676980,
//         Created:  Fri May  7 13:11:39 CEST 2010
// $Id: VertexAnalyzer.cc,v 1.4 2010/08/22 19:39:29 edwenger Exp $
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
   std::sort(vtxs.begin(),vtxs.end(),MoreTracksThenLowerChi2<reco::Vertex>()); 


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
      double leadJetEt_=0, leadJetEta_=0;

      unsigned index = 0;
      if(sortedJets.size()==0) leadJetEt_ = 0,leadJetEta_ = 0;
      else leadJetEt_ = sortedJets[index]->et(), leadJetEta_ = sortedJets[index]->eta();

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


      // Tracks/Vertices corrleation 
      for(unsigned j=0; j<sortedTrks.size(); j++){

	 double dzPV = sortedTrks[j]->dz(vtxs[0].position());
	 double dzSV = sortedTrks[j]->dz(vtxs[1].position());

	 if(j==0) hLeadingTrkPVdZ->Fill(dzPV), hLeadingTrkSVdZ->Fill(dzSV); // leading
	 else if(j==1) hSLeadingTrkPVdZ->Fill(dzPV), hSLeadingTrkSVdZ->Fill(dzSV); // sub-leading
	 else if(j==2) hSSLeadingTrkPVdZ->Fill(dzPV), hSSLeadingTrkSVdZ->Fill(dzSV); // s-sub-leading
	 
	 if(fabs(dzPV)<=fabs(dzSV)) hTrkPtFromPV->Fill(sortedTrks[j]->pt());
	 else hTrkPtFromSV->Fill(sortedTrks[j]->pt());

      }
   } // end of if(jetTrkVerticesCorr_)
}


// ------------ method called once each job just before starting event loop  ------------
void 
VertexAnalyzer::beginJob()
{

  hVtxSize = f->make<TH1D>("hVtxSize","number of reconstructed vertices",10,-0.5,9.5);
  hVtxTracks = f->make<TH1D>("hVtxTracks","number of tracks fitted to vertex",nTracksBins_,-0.5,(float)nTracksBins_-0.5);
  hVtxZ = f->make<TH1D>("hVtxZ","z position of best reconstructed vertex",80,-20.0,20.0);
  hMultVtxTracks = f->make<TH2D>("hMultVtxTracks","number of tracks fitted to vertex; most populated; 2nd most populated",nTracksBins_,-0.5,(float)nTracksBins_-0.5,nTracksBins_,-0.5,(float)nTracksBins_-0.5);
  hMultVtxZ = f->make<TH2D>("hMultVtxZ","z position of reconstructed vertex; most populated; 2nd most populated",80,-20.0,20.0,80,-20.0,20.0);
  hAllVtxZ = f->make<TH1D>("hAllVtxZ","z position of all reconstructed vertices",80,-20.0,20.0);

  if(jetTrkVerticesCorr_){
     hLeadingTrkPVdZ = f->make<TH1F>("hLeadingTrkPVdZ","dz of leading track with primary vertex",150,-30,30);
     hSLeadingTrkPVdZ = f->make<TH1F>("hSLeadingTrkPVdZ","dz of sub-leading track with primary vertex",150,-30,30);
     hSSLeadingTrkPVdZ = f->make<TH1F>("hSSLeadingTrkPVdZ","dz of sub-sub-leading track with primary vertex",150,-30,30);
     
     hLeadingTrkSVdZ = f->make<TH1F>("hLeadingTrkSVdZ","dz of leading track with 2nd vertex",150,-30,30);
     hSLeadingTrkSVdZ = f->make<TH1F>("hSLeadingTrkSVdZ","dz of sub-leading track with 2nd vertex",150,-30,30);
     hSSLeadingTrkSVdZ = f->make<TH1F>("hSSLeadingTrkSVdZ","dz of sub-sub-leading track with 2nd vertex",150,-30,30);
     
     hTrkPtFromPV = f->make<TH1F>("hTrkPtFromPV","track p_{T} distribution from PV;p_{T} (GeV/c)",220,0.0,330.);
     hTrkPtFromSV = f->make<TH1F>("hTrkPtFromSV","track p_{T} distribution from SV;p_{T} (GeV/c)",220,0.0,330.);

     hVtxZTrkEta = f->make<TH2F>("hVtxZTrkEta","vtx vz vs track eta",80,-20.,20.,24,-3.0,3.0);
  }
}

// ------------ method called once each job just after ending the event loop  ------------
void 
VertexAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(VertexAnalyzer);
