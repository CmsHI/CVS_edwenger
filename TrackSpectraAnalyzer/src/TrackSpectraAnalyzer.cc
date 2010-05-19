//
// Original Author:  Andre Yoon,32 4-A06,+41227676980,
//         Created:  Wed Apr 28 16:18:39 CEST 2010
// $Id: TrackSpectraAnalyzer.cc,v 1.15 2010/05/18 20:29:52 sungho Exp $
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
   histOnly_ = iConfig.getUntrackedParameter<bool>("histOnly", false);
   etaMax_ = iConfig.getUntrackedParameter<double>("etaMax", 5.0);
   hltNames_ = iConfig.getUntrackedParameter<std::vector <std::string> >("hltNames");
   triglabel_ = iConfig.getUntrackedParameter<edm::InputTag>("triglabel");

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
   iEvent.getByLabel(triglabel_, triggerResults);

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
   edm::Handle<reco::CandidateView> jets;
   iEvent.getByLabel(jsrc_,jets);
   hNumJets->Fill(jets->size()); // check # of jets found in event

   vector<const reco::Candidate *> sortedJets;

   for(unsigned it=0; it<jets->size(); ++it){
      const reco::Candidate* jet = &((*jets)[it]);
      sortedJets.push_back(jet);
      sortByEtRef (&sortedJets);
   }
   
   if(doJet_){ 
      for(unsigned it=0; it<sortedJets.size(); ++it){
	 if(!histOnly_) nt_jet->Fill(sortedJets[it]->et(),sortedJets[it]->eta(),sortedJets[it]->phi(),
				     accept[0],accept[1],accept[2],accept[3],accept[4]); 
	 hJet0Pt->Fill(sortedJets[it]->et());
	 if (accept[0]) hJet0Pt_HltMB->Fill(sortedJets[it]->et());
	 if (accept[1]) hJet0Pt_HltJet6U->Fill(sortedJets[it]->et());
	 if (accept[2]) hJet0Pt_HltJet15U->Fill(sortedJets[it]->et());
	 if (accept[3]) hJet0Pt_HltJet30U->Fill(sortedJets[it]->et());
	 if (accept[4]) hJet0Pt_HltJet50U->Fill(sortedJets[it]->et());
	 break;             
      }                     
   }
   // Get Leading jet energy
   float jet_et = 0, jet_eta = 0;
   unsigned index = 0; 
   if(sortedJets.size()==0) jet_et = 0,jet_eta = 0; 
   else jet_et = sortedJets[index]->et(), jet_eta = sortedJets[index]->eta(); 
   
   // get track collection 
   Handle<vector<Track> > tracks;
   iEvent.getByLabel(src_, tracks);

   for(unsigned it=0; it<tracks->size(); ++it){
      const reco::Track & trk = (*tracks)[it];

      if(!trk.quality(reco::TrackBase::qualityByName(qualityString))) continue;

      if(accept[0]==1) hTrkPtMB->Fill(trk.pt());
      if(!histOnly_) nt_dndptdeta->Fill(trk.pt(),trk.eta());
      

      // (leading jet)-track                         
      if(doJet_ && (!histOnly_)) nt_jettrack->Fill(trk.pt(),trk.eta(),jet_et,
				   accept[0],accept[1],accept[2],accept[3],accept[4]); 

      hTrkPtEta->Fill(trk.eta(),trk.pt());
      hTrkPtEtaJetEt->Fill(trk.eta(),trk.pt(),jet_et);
      hTrkPtEtaJetEtW->Fill(trk.eta(),trk.pt(),jet_et,(1./trk.pt())); // weighted by pT

   }
   
   hNevt->Fill(1.0); // put more useful stuff

   if(isGEN_){
      // Gen track
      Handle<GenParticleCollection> genParticles;
      iEvent.getByLabel("genParticles", genParticles);
      const GenParticleCollection *genCollect = genParticles.product();

      for(unsigned i=0; i<genCollect->size();i++){
	 const GenParticle & gen = (*genCollect)[i];
	 if(gen.status() != 1) continue;
	 //if(gen.collisionId() != 0) continue;
	 if(gen.charge() == 0) continue;
	 if(fabs(gen.eta())>etaMax_) continue;
	 if(!histOnly_) nt_gen_dndptdeta->Fill(gen.pt(),gen.eta());
	 hGenTrkPtEta->Fill(gen.eta(),gen.pt());
	 hGenTrkPtEtaJetEt->Fill(gen.eta(),gen.pt(),jet_et);//move to gen jet
	 hGenTrkPtEtaJetEtW->Fill(gen.eta(),gen.pt(),jet_et,(1./gen.pt())); // weighted by pT
      }
   }

}


// ------------ method called once each job just before starting event loop  ------------
void 
TrackSpectraAnalyzer::beginJob()
{

   if(doOutput_){

      TFileDirectory subDir = fs->mkdir( "threeDHist" );
      
      hNevt = fs->make<TH1F>("hNevt","evt counter",10, 0.0, 10);

      if(!histOnly_) nt_dndptdeta = fs->make<TNtuple>("nt_dndptdeta","eta vs pt","pt:eta");
      hTrkPtMB = fs->make<TH1F>("hTrkPtMB","track p_{T}; p_{T} [GeV/c]", 1000, 0.0, 200.0);
      hTrkPtEta = fs->make<TH2F>("hTrkPtEta","eta vs pt;#eta;p_{T} (GeV/c)",50, -2.5, 2.5, 1000, 0.0, 200.0);

      // memory consumption limits the number of bins...
      hTrkPtEtaJetEt = subDir.make<TH3F>("hTrkPtEtaJetEt","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
					 50, -2.5, 2.5, 1000, 0.0, 200.0, 15, 0.0, 300.0); 
      
      hTrkPtEtaJetEtW = subDir.make<TH3F>("hTrkPtEtaJetEtW","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
					  50, -2.5, 2.5, 1000, 0.0, 200.0, 15, 0.0, 300.0);
      
      
      if(isGEN_) {
	 if(!histOnly_) nt_gen_dndptdeta = fs->make<TNtuple>("nt_gen_dndptdeta","eta vs pt","pt:eta");
	 hGenTrkPtEta = fs->make<TH2F>("hGenTrkPtEta","eta vs pt;#eta;p_{T} (GeV/c)",50, -2.5, 2.5, 1000, 0.0, 200.0);
	 hGenTrkPtEtaJetEt = subDir.make<TH3F>("hGenTrkPtEtaJetEt","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
					       50, -2.5, 2.5, 1000, 0.0, 200.0, 15, 0.0, 300.0);

	 hGenTrkPtEtaJetEtW = subDir.make<TH3F>("hGenTrkPtEtaJetEtW","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
	 250, -2.5, 2.5, 1000, 0.0, 200.0, 300, 0.0, 300.0);

      }
      if(doJet_) {
	 if(!histOnly_) nt_jet = fs->make<TNtuple>("nt_jet","jet spectra ntuple","jet:jeta:jphi:mb:jet6:jet15:jet30:jet50");
	 if(!histOnly_) nt_jettrack = fs->make<TNtuple>("nt_jettrack","jet tracks correlation ntuple","pt:eta:jet:mb:jet6:jet15:jet30:jet50");
	 // jet histograms
	 hNumJets = fs->make<TH1F>("hNumJets",";# jets in evt;# evts", 100, 0, 100);
	 hJet0Pt = fs->make<TH1F>("hJet0Pt","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltMB = fs->make<TH1F>("hJet0Pt_HltMB","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltJet6U = fs->make<TH1F>("hJet0Pt_HltJet6U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltJet15U = fs->make<TH1F>("hJet0Pt_HltJet15U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltJet30U = fs->make<TH1F>("hJet0Pt_HltJet30U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltJet50U = fs->make<TH1F>("hJet0Pt_HltJet50U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
      }
   }
}


//define this as a plug-in
DEFINE_FWK_MODULE(TrackSpectraAnalyzer);
