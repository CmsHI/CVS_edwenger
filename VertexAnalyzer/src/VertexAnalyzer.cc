// Original Author:  Edward Allen Wenger,32 4-A06,+41227676980,
//         Created:  Fri May  7 13:11:39 CEST 2010
// $Id: VertexAnalyzer.cc,v 1.7 2011/01/05 16:10:53 sungho Exp $
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

   //-- same vertex collection for sqrt(pt*pt) based sorting
   edm::Handle<reco::VertexCollection> vtxsH_sortPt;
   iEvent.getByLabel(vtxlabel_,vtxsH_sortPt);
   
   reco::VertexCollection vtxs_sortPt = *vtxsH_sortPt;

   //std::sort(vtxs_sortPt.begin(),vtxs_sortPt.end(), PrimaryVertexSorter);

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
	 
	 hTrkPtFromAV->Fill(sortedTrks[j]->pt());

	 if(fabs(dzPV)<=fabs(dzSV)) hTrkPtFromPV->Fill(sortedTrks[j]->pt());
	 else hTrkPtFromSV->Fill(sortedTrks[j]->pt());

      }
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
     
     hTrkPtFromAV = f->make<TH1F>("hTrkPtFromAV","track p_{T} distribution from all vertices;p_{T} (GeV/c)",ptBins.size()-1, &ptBins[0]);
     hTrkPtFromPV = f->make<TH1F>("hTrkPtFromPV","track p_{T} distribution from PV;p_{T} (GeV/c)",ptBins.size()-1, &ptBins[0]);
     hTrkPtFromSV = f->make<TH1F>("hTrkPtFromSV","track p_{T} distribution from SV;p_{T} (GeV/c)",ptBins.size()-1, &ptBins[0]);

     hVtxZTrkEta = f->make<TH2F>("hVtxZTrkEta","vtx vz vs track eta",80,-20.,20.,24,-3.0,3.0);
  }
}

// ------------ method called once each job just after ending the event loop  ------------
void 
VertexAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(VertexAnalyzer);
