//
// Original Author:  Andre Yoon,32 4-A06,+41227676980,
//         Created:  Wed Apr 28 16:18:39 CEST 2010
// $Id: TrackSpectraAnalyzer.cc,v 1.7 2010/05/01 15:24:36 sungho Exp $
//

#include "edwenger/TrackSpectraAnalyzer/interface/TrackSpectraAnalyzer.h"


TrackSpectraAnalyzer::TrackSpectraAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   src_ = iConfig.getUntrackedParameter<edm::InputTag>("src",edm::InputTag("selectTracks"));
   vsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("vsrc",edm::InputTag("offlinePrimaryVertices"));
   jsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("jsrc",edm::InputTag("ak5CaloJets"));
   doOutput_ = iConfig.getUntrackedParameter<bool>("doOutput", true);
   isGEN_ = iConfig.getUntrackedParameter<bool>("isGEN", true);
   doJet_ = iConfig.getUntrackedParameter<bool>("doJet", true);
   etaMax_ = iConfig.getUntrackedParameter<double>("etaMax", 5.0);
   nVtxTrkCut_ = iConfig.getUntrackedParameter<int>("nVtxTrkCut", 3);

}

// ------------ method called to for each event  ------------
void
TrackSpectraAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;

   const int nTrigs = 5;

   const string qualityString = "highPurity";

   //-----------------------   (This part will be in an EDFilter later)  
   // get vtx collection 
   Handle<vector<Vertex> > vertices;
   iEvent.getByLabel(vsrc_, vertices);
   if(vertices->size()!=1) return; // one and only one vertex
   int numFake=0, numVtxTrk=0;
   double bestvz=-999.9;
   for(unsigned it=0; it<vertices->size(); ++it) {
      const reco::Vertex & vtx = (*vertices)[it];
      if(vtx.isFake()) numFake++;
      numVtxTrk = vtx.tracksSize();
      bestvz = vtx.z();
   }
   if(numVtxTrk<nVtxTrkCut_) return;
   if(numFake>=1) return;
   hVtxZ->Fill(bestvz);
   //-----------------------   (This part will be in an EDFilter later)    
   
   // get hlt bit
   Handle<edm::TriggerResults> triggerResults;
   iEvent.getByLabel(edm::InputTag("TriggerResults","","HLT"), triggerResults);
   //iEvent.getByLabel("TriggerResults","","HLT",triggerResults);
   const edm::TriggerNames triggerNames = iEvent.triggerNames(*triggerResults); 
   bool accept[nTrigs];
   accept[0]=false; accept[1]=false; accept[2]=false; accept[3]=false; accept[4]=false;
   
   accept[0] = triggerResults->accept(triggerNames.triggerIndex("HLT_MinBiasBSC"));
   accept[1] = triggerResults->accept(triggerNames.triggerIndex("HLT_L1Jet6U"));
   accept[2] = triggerResults->accept(triggerNames.triggerIndex("HLT_Jet15U"));
   accept[3] = triggerResults->accept(triggerNames.triggerIndex("HLT_Jet30U"));
   accept[4] = triggerResults->accept(triggerNames.triggerIndex("HLT_Jet50U"));

   for(int i=0; i<nTrigs; i++) { 
      //cout<<triggerResults->accept(triggerNames.triggerIndex("HLT_MinBiasBSC"))<<endl;
      //accept[i] = triggerResults->accept(triggerNames.triggerIndex(hltNames[i])); 
      //if(accept[i]) hHLTPaths->Fill(hltNames[i],1); 
    } 

   
   
   //----- loop over pat jets and store in a vector -----
   Handle<std::vector<pat::Jet> > pjets;
   iEvent.getByLabel("selectedPatJets", pjets);

   vector<const pat::Jet *> sortedpJets;

   if(doJet_){
      for(unsigned it=0; it<pjets->size(); ++it){
	 const pat::Jet* pjts = &((*pjets)[it]);
	 sortedpJets.push_back( & *pjts);
	 sortByEtRef (&sortedpJets);
      }
      
      for(unsigned it=0; it<sortedpJets.size(); ++it){
	 nt_jet->Fill(sortedpJets[it]->et(),sortedpJets[it]->eta(),sortedpJets[it]->phi(),
		      accept[0],accept[1],accept[2],accept[3],accept[4]); 
	 break;             
      }                     
   }

   // get track collection 
   Handle<vector<Track> > tracks;
   iEvent.getByLabel(src_, tracks);

   for(unsigned it=0; it<tracks->size(); ++it){
      const reco::Track & trk = (*tracks)[it];

      if(!trk.quality(reco::TrackBase::qualityByName(qualityString))) continue;

      if(accept[0]==1) hTrkPtMB->Fill(trk.pt());
      nt_dndptdeta->Fill(trk.pt(),trk.eta());

      // (leading jet)-track                                  
      // even if there's no jet track info saved (needed for MB) 

      if(doJet_){
         double jet_et = 0, jet_eta = 0;
         unsigned index = 0; 
         if(sortedpJets.size()==0) jet_et = 0,jet_eta = 0; 
	 else jet_et = sortedpJets[index]->et(), jet_eta = sortedpJets[index]->eta(); 
         nt_jettrack->Fill(trk.pt(),trk.eta(),jet_et,
                           accept[0],accept[1],accept[2],accept[3],accept[4]); 
      }                                                       
   }

   if(isGEN_){
      Handle<GenParticleCollection> genParticles;
      iEvent.getByLabel("genParticles", genParticles);
      const GenParticleCollection *genCollect = genParticles.product();

      for(unsigned i=0; i<genCollect->size();i++){
	 const GenParticle & gen = (*genCollect)[i];
	 if(gen.status() != 1) continue;
	 if(gen.collisionId() != 0) continue;
	 if(gen.charge() == 0) continue;
	 if(fabs(gen.eta())>etaMax_) continue;
	 nt_gen_dndptdeta->Fill(gen.pt(),gen.eta());
      }
   }

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
TrackSpectraAnalyzer::beginJob()
{
   if(doOutput_){
      nt_dndptdeta = fs->make<TNtuple>("nt_dndptdeta","eta vs pt","pt:eta");
      hTrkPtMB = fs->make<TH1D>("hTrkPtMB","track p_{T}; p_{T} [GeV/c]", 1000, 0.0, 200.0);
      hVtxZ = fs->make<TH1D>("hVtxZ","z position of best reconstructed pixel vertex", 80,-20,20);
      if(isGEN_) nt_gen_dndptdeta = fs->make<TNtuple>("nt_gen_dndptdeta","eta vs pt","pt:eta");
      if(doJet_) {
	 nt_jet = fs->make<TNtuple>("nt_jet","jet spectra ntuple","jet:jeta:jphi:mb:jet6:jet15:jet30:jet50");
	 nt_jettrack = fs->make<TNtuple>("nt_jettrack","jet tracks correlation ntuple","pt:eta:jet:mb:jet6:jet15:jet30:jet50");
      }
   }
}


//define this as a plug-in
DEFINE_FWK_MODULE(TrackSpectraAnalyzer);
