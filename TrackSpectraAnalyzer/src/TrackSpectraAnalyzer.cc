//
// Original Author:  Andre Yoon,32 4-A06,+41227676980,
//         Created:  Wed Apr 28 16:18:39 CEST 2010
// $Id: TrackSpectraAnalyzer.cc,v 1.5 2010/05/12 15:27:22 frankma Exp $
//

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "edwenger/TrackSpectraAnalyzer/interface/TrackSpectraAnalyzer.h"


TrackSpectraAnalyzer::TrackSpectraAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   src_ = iConfig.getUntrackedParameter<edm::InputTag>("src",edm::InputTag("generalTracks"));
   vsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("vsrc",edm::InputTag("offlinePrimaryVertices"));
   jsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("jsrc",edm::InputTag("ak5CaloJets"));
   doOutput_ = iConfig.getUntrackedParameter<bool>("doOutput", true);
   isGEN_ = iConfig.getUntrackedParameter<bool>("isGEN", true);
   doJet_ = iConfig.getUntrackedParameter<bool>("doJet", true);
   etaMax_ = iConfig.getUntrackedParameter<double>("etaMax", 5.0);
   hltNames_ = iConfig.getUntrackedParameter<std::vector <std::string> >("hltNames");

}

// ------------ method called to for each event  ------------
void
TrackSpectraAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;

   const string qualityString = "highPurity";
   
   // get hlt bit
   Handle<edm::TriggerResults> triggerResults;
   iEvent.getByLabel(edm::InputTag("TriggerResults","","HLT"), triggerResults);

   const edm::TriggerNames triggerNames = iEvent.triggerNames(*triggerResults); 
   std::vector<bool> accept(5,false);

   for(unsigned i=0; i<hltNames_.size(); i++) { 

     unsigned index = triggerNames.triggerIndex(hltNames_[i]);
     if(index < triggerResults->size())
       accept[i] = triggerResults->accept(index);
     else 
       edm::LogWarning("TrackSpectraAnalyzer")
	 << "Index returned by TriggerNames object for trigger '"
	 << hltNames_[i]
	 << "' is out of range (" 
	 << index << " >= " << triggerResults->size() << ")";
     
   } 
   
   //----- loop over pat jets and store in a vector -----
   Handle<std::vector<pat::Jet> > pjets;
   iEvent.getByLabel(jsrc_, pjets);

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
	 if (accept[0]) hJet0Pt_HltMB->Fill(sortedpJets[it]->et());
	 if (accept[1]) hJet0Pt_HltJet6U->Fill(sortedpJets[it]->et());
	 if (accept[2]) hJet0Pt_HltJet15U->Fill(sortedpJets[it]->et());
	 if (accept[3]) hJet0Pt_HltJet30U->Fill(sortedpJets[it]->et());
	 if (accept[4]) hJet0Pt_HltJet50U->Fill(sortedpJets[it]->et());
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

}


// ------------ method called once each job just before starting event loop  ------------
void 
TrackSpectraAnalyzer::beginJob()
{
   if(doOutput_){
      nt_dndptdeta = fs->make<TNtuple>("nt_dndptdeta","eta vs pt","pt:eta");
      hTrkPtMB = fs->make<TH1D>("hTrkPtMB","track p_{T}; p_{T} [GeV/c]", 1000, 0.0, 200.0);
      if(isGEN_) nt_gen_dndptdeta = fs->make<TNtuple>("nt_gen_dndptdeta","eta vs pt","pt:eta");
      if(doJet_) {
	 nt_jet = fs->make<TNtuple>("nt_jet","jet spectra ntuple","jet:jeta:jphi:mb:jet6:jet15:jet30:jet50");
	 nt_jettrack = fs->make<TNtuple>("nt_jettrack","jet tracks correlation ntuple","pt:eta:jet:mb:jet6:jet15:jet30:jet50");
	 // jet histograms
	 hJet0Pt_HltMB = fs->make<TH1D>("hJet0Pt_HltMB","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltJet6U = fs->make<TH1D>("hJet0Pt_HltJet6U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltJet15U = fs->make<TH1D>("hJet0Pt_HltJet15U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltJet30U = fs->make<TH1D>("hJet0Pt_HltJet30U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltJet50U = fs->make<TH1D>("hJet0Pt_HltJet50U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
      }
   }
}


//define this as a plug-in
DEFINE_FWK_MODULE(TrackSpectraAnalyzer);
