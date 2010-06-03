//
// Original Author:  Andre Yoon,32 4-A06,+41227676980,
//         Created:  Wed Apr 28 16:18:39 CEST 2010
// $Id: TrackSpectraAnalyzer.cc,v 1.35 2010/06/02 14:19:27 edwenger Exp $
//

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "edwenger/TrackSpectraAnalyzer/interface/TrackSpectraAnalyzer.h"
#include <TF1.h>

TrackSpectraAnalyzer::TrackSpectraAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   src_ = iConfig.getUntrackedParameter<edm::InputTag>("src",edm::InputTag("generalTracks"));
   vsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("vsrc",edm::InputTag("offlinePrimaryVertices"));
   jsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("jsrc",edm::InputTag("ak5CaloJets"));
   gjsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("gjsrc",edm::InputTag("ak5GenJets"));
   src_evtCorr_ = iConfig.getUntrackedParameter<edm::InputTag>("src_evtCorr",edm::InputTag("generalTracks"));
   isGEN_ = iConfig.getUntrackedParameter<bool>("isGEN", true);
   pureGENmode_ = iConfig.getUntrackedParameter<bool>("pureGENmode", false);
   nsdOnly_ = iConfig.getUntrackedParameter<bool>("nsdOnly", false);
   doJet_ = iConfig.getUntrackedParameter<bool>("doJet", true);
   histOnly_ = iConfig.getUntrackedParameter<bool>("histOnly", false);
   includeExtra_ = iConfig.getUntrackedParameter<bool>("includeExtra",false);
   etaMax_ = iConfig.getUntrackedParameter<double>("etaMax", 5.0);
   ptMin_ = iConfig.getUntrackedParameter<double>("ptMin", 0.5);
   applyEvtEffCorr_ = iConfig.getUntrackedParameter<bool>("applyEvtEffCorr", true);
   evtEffCorrType_ = iConfig.getUntrackedParameter<int>("evtEffCorrType_", 0);
   efit_type_ = iConfig.getUntrackedParameter<int>("efit_type", 0);
   evtSelEffv_ = iConfig.getUntrackedParameter< std::vector<double> >("evtSelEffv");
   evtSelEffCut_ = iConfig.getUntrackedParameter<double>("evtSelEffCut", 0.05);
   efit_para_ = iConfig.getUntrackedParameter< std::vector<double> >("efit_para");
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

   float etaCut_evtSel = 0.0;
   bool skipEvt = false;

   if(evtEffCorrType_==0)
      etaCut_evtSel = 2.4;
   else if(evtEffCorrType_==1)
      etaCut_evtSel = 1.0;
   else if(evtEffCorrType_==2)
      etaCut_evtSel = 0.8;

   if(!pureGENmode_){  // if pure GEN, skip through to the GEN ana part

     //float nevt = 1.0; // comment out unused variable (EAW)
      
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
	    if(fabs(sortedJets[it]->eta())>etaMax_) continue;
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
      
      // Get multiplicity dist from track collection
      int mult = 0;
      
      Handle<vector<Track> > etracks;
      iEvent.getByLabel(src_evtCorr_, etracks);
      
      for(unsigned it=0; it<etracks->size(); ++it){
	 const reco::Track & etrk = (*etracks)[it];
	 if(!etrk.quality(reco::TrackBase::qualityByName(qualityString))) continue;
	 if(fabs(etrk.eta())<etaCut_evtSel && etrk.pt()>ptMin_) mult++;
      }
      
      // get evt sel eff
      double evt_sel_eff = 1;
      double evt_sel_corr = 1;

      if(applyEvtEffCorr_) {
         if(mult+1>(int)evtSelEffv_[0]) evt_sel_eff = 1.0;
	 else evt_sel_eff = evtSelEffv_[(unsigned)(mult+1)];
	 if(evt_sel_eff<evtSelEffCut_) skipEvt = true;
         if(includeExtra_ && !skipEvt) {
	    hRecMult_STD->Fill(mult);
	    hRecMult_STD_corr->Fill(mult,(1./evt_sel_eff));
	 }
      }


      // get track collection                
      if(!skipEvt){

	 Handle<vector<Track> > tracks;
	 iEvent.getByLabel(src_, tracks);
	 
	 for(unsigned it=0; it<tracks->size(); ++it){
	    const reco::Track & trk = (*tracks)[it];
	    
	    if(!trk.quality(reco::TrackBase::qualityByName(qualityString))) continue;
	    
	    if(accept[0]==1) hTrkPtMB->Fill(trk.pt(),evt_sel_corr);
	    if(!histOnly_) nt_dndptdeta->Fill(trk.pt(),trk.eta());
	    
	    if(doJet_ && (!histOnly_)) nt_jettrack->Fill(trk.pt(),trk.eta(),jet_et,
							 accept[0],accept[1],accept[2],accept[3],accept[4]); 
	    
	    hTrkPtEta->Fill(trk.eta(),trk.pt(),1./evt_sel_eff);
	    hTrkPtEtaJetEt->Fill(trk.eta(),trk.pt(),jet_et,1./evt_sel_eff);
	    
	    if(includeExtra_) {
	       hTrkPtEtaJetEtW->Fill(trk.eta(),trk.pt(),jet_et,(evt_sel_corr/trk.pt())); // weighted by pT
	       if(mult==1) hTrkPtEtaJetEt_mult1->Fill(trk.eta(),trk.pt(),jet_et,1./evt_sel_eff);
	       if(mult==2) hTrkPtEtaJetEt_mult2->Fill(trk.eta(),trk.pt(),jet_et,1./evt_sel_eff);
	       if(mult==3) hTrkPtEtaJetEt_mult3->Fill(trk.eta(),trk.pt(),jet_et,1./evt_sel_eff);
	    }
	    
	    if (accept[1]) hTrkPtEtaJetEt_HltJet6U->Fill(trk.eta(),trk.pt(),jet_et,1./evt_sel_eff);
	    if (accept[2]) hTrkPtEtaJetEt_HltJet15U->Fill(trk.eta(),trk.pt(),jet_et,1./evt_sel_eff);
	    if (accept[3]) hTrkPtEtaJetEt_HltJet30U->Fill(trk.eta(),trk.pt(),jet_et,1./evt_sel_eff);
	    if (accept[4]) hTrkPtEtaJetEt_HltJet50U->Fill(trk.eta(),trk.pt(),jet_et,1./evt_sel_eff);
	    
	 }
	 
	 hNevt->Fill(evt_sel_eff);
	 if(mult==1) hNevt_mult1->Fill(evt_sel_eff);
	 if(mult==2) hNevt_mult2->Fill(evt_sel_eff);
	 if(mult==3) hNevt_mult3->Fill(evt_sel_eff);

      }// end of skip evt

   } // end of if(pureGENmode_)
   
   
   if(isGEN_ && !skipEvt){
      float nevtGEN = 1.0;
	 
      edm::Handle<reco::CandidateView> gjets;
      iEvent.getByLabel(gjsrc_,gjets);
      
      vector<const reco::Candidate *> sortedGJets;
      
      for(unsigned it=0; it<gjets->size(); ++it){
	 const reco::Candidate* jet = &((*gjets)[it]);
	 sortedGJets.push_back(jet);
	 sortByEtRef (&sortedGJets);
      }
      // Get Leading jet energy
      float gjet_et = 0, gjet_eta = 0;
      unsigned index = 0; 
      if(sortedGJets.size()==0) gjet_et = 0,gjet_eta = 0; 
      else gjet_et = sortedGJets[index]->et(), gjet_eta = sortedGJets[index]->eta(); 
      
      // Gen event info
      edm::Handle<GenEventInfoProduct> genEvtInfo;
      iEvent.getByLabel("generator", genEvtInfo);
      int pid = genEvtInfo->signalProcessID();
      
      bool isWanted = false;
      
      if(nsdOnly_){
	 if( ((pid>=11) && (pid<=68)) || (pid==94) || (pid==95)) isWanted = true;
      }else{
	 isWanted = true;
      }
      
      // Gen track
      if(isWanted){
	 Handle<GenParticleCollection> genParticles;
	 iEvent.getByLabel("genParticles", genParticles);
	 const GenParticleCollection *genCollect = genParticles.product();
	 
	 for(unsigned i=0; i<genCollect->size();i++){
	    const GenParticle & gen = (*genCollect)[i];
	    if(gen.status() != 1) continue;
	    if(gen.charge() == 0) continue;
	    if(fabs(gen.eta())>etaMax_) continue;
	    if(!histOnly_) nt_gen_dndptdeta->Fill(gen.pt(),gen.eta());
	    
	    hGenTrkPtEta->Fill(gen.eta(),gen.pt());
	    hGenTrkPtEtaJetEt->Fill(gen.eta(),gen.pt(),gjet_et); 
	    //if(includeExtra_) hGenTrkPtEtaJetEtW->Fill(gen.eta(),gen.pt(),gjet_et,(1./gen.pt())); // weighted by pT
	 }
	 hGenNevt->Fill(nevtGEN);
      }
   } // isGEN
   
}
// ------------ method called once each job just before starting event loop  ------------
void 
TrackSpectraAnalyzer::beginJob()
{

   int numBins = 300;
   double xmax = 299.5;

   float etaHistMax = 2.4;
   int nbinsEta = 25;
   
   // Defin Histograms
   TFileDirectory subDir = fs->mkdir( "threeDHist" );
   
   if(!pureGENmode_){
      hNevt = fs->make<TH1F>("hNevt","evt sel eff", 510, -0.01, 1.01);
      hNevt_mult1 = fs->make<TH1F>("hNevt_mult1","evt sel eff", 510, -0.01, 1.01);
      hNevt_mult2 = fs->make<TH1F>("hNevt_mult2","evt sel eff", 510, -0.01, 1.01);
      hNevt_mult3 = fs->make<TH1F>("hNevt_mult3","evt sel eff", 510, -0.01, 1.01);

      if(!histOnly_) nt_dndptdeta = fs->make<TNtuple>("nt_dndptdeta","eta vs pt","pt:eta");
      hTrkPtMB = fs->make<TH1F>("hTrkPtMB","track p_{T}; p_{T} [GeV/c]", 1000, 0.0, 200.0);
      hTrkPtEta = fs->make<TH2F>("hTrkPtEta","eta vs pt;#eta;p_{T} (GeV/c)", nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0);
      
      // memory consumption limits the number of bins...
      hTrkPtEtaJetEt = subDir.make<TH3F>("hTrkPtEtaJetEt","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
					 nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0); 
      hTrkPtEtaJetEt->Sumw2();

      if(includeExtra_) {
	 hRecMult_STD = fs->make<TH1F>("hRecMult_STD","Charged mult. |#eta|<|#eta_{max}|)",numBins,-0.5,xmax);
	 hRecMult_STD_corr = fs->make<TH1F>("hRecMult_STD_corr","Charged mult. |#eta|<|#eta_{max}|)",numBins,-0.5,xmax);
	 hTrkPtEtaJetEtW = subDir.make<TH3F>("hTrkPtEtaJetEtW","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
	 				     nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
	 hTrkPtEtaJetEt_mult1 = subDir.make<TH3F>("hTrkPtEtaJetEt_mult1","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						  nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
	 hTrkPtEtaJetEt_mult2 = subDir.make<TH3F>("hTrkPtEtaJetEt_mult2","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
                                                  nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
	 hTrkPtEtaJetEt_mult3 = subDir.make<TH3F>("hTrkPtEtaJetEt_mult3","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
                                                  nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
      }
      hTrkPtEtaJetEt_HltJet6U = subDir.make<TH3F>("hTrkPtEtaJetEt_HltJet6U","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						  nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
      hTrkPtEtaJetEt_HltJet15U = subDir.make<TH3F>("hTrkPtEtaJetEt_HltJet15U","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						   nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0); 
      hTrkPtEtaJetEt_HltJet30U = subDir.make<TH3F>("hTrkPtEtaJetEt_HltJet30U","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						   nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
      hTrkPtEtaJetEt_HltJet50U = subDir.make<TH3F>("hTrkPtEtaJetEt_HltJet50U","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						   nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
      
      if(doJet_) {
	 if(!histOnly_) nt_jet = fs->make<TNtuple>("nt_jet","jet spectra ntuple","jet:jeta:jphi:mb:jet6:jet15:jet30:jet50");
	 if(!histOnly_) nt_jettrack = fs->make<TNtuple>("nt_jettrack","jet tracks correlation ntuple","pt:eta:jet:mb:jet6:jet15:jet30:jet50");
	 hNumJets = fs->make<TH1F>("hNumJets",";# jets in evt;# evts", 100, 0, 100);
	 hJet0Pt = fs->make<TH1F>("hJet0Pt","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltMB = fs->make<TH1F>("hJet0Pt_HltMB","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltJet6U = fs->make<TH1F>("hJet0Pt_HltJet6U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltJet15U = fs->make<TH1F>("hJet0Pt_HltJet15U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltJet30U = fs->make<TH1F>("hJet0Pt_HltJet30U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 hJet0Pt_HltJet50U = fs->make<TH1F>("hJet0Pt_HltJet50U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
      }
   } // end of pureGENmode
   
   if(isGEN_) {
      hGenNevt = fs->make<TH1F>("hGenNevt","evt sel eff", 51, -0.01, 1.01);
      
      if(!histOnly_) nt_gen_dndptdeta = fs->make<TNtuple>("nt_gen_dndptdeta","eta vs pt","pt:eta");
      hGenTrkPtEta = fs->make<TH2F>("hGenTrkPtEta","eta vs pt;#eta;p_{T} (GeV/c)", nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0);
      hGenTrkPtEtaJetEt = subDir.make<TH3F>("hGenTrkPtEtaJetEt","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
					    nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
      //if(includeExtra_) hGenTrkPtEtaJetEtW = subDir.make<TH3F>("hGenTrkPtEtaJetEtW","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
      //50, -2.5, 2.5, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
   }
    
}


//define this as a plug-in
DEFINE_FWK_MODULE(TrackSpectraAnalyzer);
