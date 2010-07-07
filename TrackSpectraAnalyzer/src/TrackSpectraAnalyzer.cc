//
// Original Author:  Andre Yoon,32 4-A06,+41227676980,
//         Created:  Wed Apr 28 16:18:39 CEST 2010
// $Id: TrackSpectraAnalyzer.cc,v 1.53 2010/07/07 10:17:09 sungho Exp $
//

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "edwenger/TrackSpectraAnalyzer/interface/TrackSpectraAnalyzer.h"
#include <TF1.h>

TrackSpectraAnalyzer::TrackSpectraAnalyzer(const edm::ParameterSet& iConfig) :
  leadJetEt_(0),
  leadJetEta_(0),
  leadGJetEt_(0),
  leadGJetEta_(0),
  hltAccept_(5,false)
{
   //now do what ever initialization is needed
   src_ = iConfig.getUntrackedParameter<edm::InputTag>("src",edm::InputTag("generalTracks"));
   vsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("vsrc",edm::InputTag("offlinePrimaryVertices"));
   jsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("jsrc",edm::InputTag("ak5CaloJets"));
   gsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("gsrc",edm::InputTag("genParticles"));
   gjsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("gjsrc",edm::InputTag("ak5GenJets"));
   src_evtCorr_ = iConfig.getUntrackedParameter<edm::InputTag>("src_evtCorr",edm::InputTag("generalTracks"));
   setQualityBit_ = iConfig.getUntrackedParameter<bool>("setQualityBit", true);
   isGEN_ = iConfig.getUntrackedParameter<bool>("isGEN", true);
   pureGENmode_ = iConfig.getUntrackedParameter<bool>("pureGENmode", false);
   nsdOnly_ = iConfig.getUntrackedParameter<bool>("nsdOnly", false);
   pythia6_ = iConfig.getUntrackedParameter<bool>("pythia6", true);
   pthatCut_ = iConfig.getUntrackedParameter<double>("pthatCut", 0.0);
   doJet_ = iConfig.getUntrackedParameter<bool>("doJet", true);
   histOnly_ = iConfig.getUntrackedParameter<bool>("histOnly", false);
   includeExtra_ = iConfig.getUntrackedParameter<bool>("includeExtra",false);
   etaMax_ = iConfig.getUntrackedParameter<double>("etaMax", 5.0);
   ptMin_ = iConfig.getUntrackedParameter<double>("ptMin", 0.5);
   applyEvtEffCorr_ = iConfig.getUntrackedParameter<bool>("applyEvtEffCorr", true);
   evtEffCorrType_ = iConfig.getUntrackedParameter<int>("evtEffCorrType", 0);
   efit_type_ = iConfig.getUntrackedParameter<int>("efit_type", 0);
   evtSelEffv_ = iConfig.getUntrackedParameter< std::vector<double> >("evtSelEffv");
   evtMultCut_ = iConfig.getUntrackedParameter<int>("evtMultCut", 0);
   hltNames_ = iConfig.getUntrackedParameter<std::vector <std::string> >("hltNames");
   neededTrigSpectra_ = iConfig.getUntrackedParameter<std::vector<int> >("neededTrigSpectra");
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
      
      for(unsigned i=0; i<hltNames_.size(); i++) { 
	 unsigned index = triggerNames.triggerIndex(hltNames_[i]);
	 if(index < triggerResults->size())
	    hltAccept_[i] = triggerResults->accept(index);
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
					hltAccept_[0],hltAccept_[1],hltAccept_[2],hltAccept_[3],hltAccept_[4]); 
	    if(fabs(sortedJets[it]->eta())>6.5) continue;
	    hJet0Pt->Fill(sortedJets[it]->et());
	    for(unsigned i=0;i<hltNames_.size();i++){
	       if(hltAccept_[i]) hJet0Pt_Trig[i]->Fill(sortedJets[it]->et());
	    }
	    break;             
	 }                     
      }

      // Get Leading jet energy
      unsigned index = 0; 
      if(sortedJets.size()==0) leadJetEt_ = 0,leadJetEta_ = 0; 
      else leadJetEt_ = sortedJets[index]->et(), leadJetEta_ = sortedJets[index]->eta(); 
      
      // Get multiplicity dist from track collection
      int mult = 0;
      
      Handle<vector<Track> > etracks;
      iEvent.getByLabel(src_evtCorr_, etracks);
      
      for(unsigned it=0; it<etracks->size(); ++it){
	 const reco::Track & etrk = (*etracks)[it];
	 if(setQualityBit_ && !etrk.quality(reco::TrackBase::qualityByName(qualityString))) continue;
	 if(fabs(etrk.eta())<etaCut_evtSel && etrk.pt()>ptMin_) mult++;
      }
      
      // get evt sel eff
      double evt_sel_eff = 1;

      if(applyEvtEffCorr_) {
	 hRecMult_STD->Fill(mult);
	 if(mult<=evtMultCut_) { // skip event by hand
	    skipEvt = true;
	 }else{
	    if(mult+1>(int)evtSelEffv_[0]) evt_sel_eff = 1.0; // set eff = 1 
	    else evt_sel_eff = evtSelEffv_[(unsigned)(mult+1)];
	    hRecMult_STD_corr->Fill(mult,(1./evt_sel_eff));
	 }
      }
      
      // get track collection                
      if(!skipEvt){

	 Handle<vector<Track> > tracks;
	 iEvent.getByLabel(src_, tracks);
	 
	 for(unsigned it=0; it<tracks->size(); ++it){
	    const reco::Track & trk = (*tracks)[it];
	    
	    if(setQualityBit_ && !trk.quality(reco::TrackBase::qualityByName(qualityString))) continue;
	    
	    if(!histOnly_) nt_dndptdeta->Fill(trk.pt(),trk.eta());
	    if(doJet_ && (!histOnly_)) nt_jettrack->Fill(trk.pt(),trk.eta(),leadJetEt_,
							 hltAccept_[0],hltAccept_[1],hltAccept_[2],hltAccept_[3],hltAccept_[4]); 
	    
	    hTrkPtEtaJetEt->Fill(trk.eta(),trk.pt(),leadJetEt_,1./evt_sel_eff);
	    hTrkPtEtaJetEt_vbin->Fill(trk.eta(),trk.pt(),leadJetEt_,1./evt_sel_eff);

	    unsigned ind=0;
	    for(unsigned i=0;i<hltNames_.size();i++){
	       if(neededTrigSpectra_[i]!=1) continue;
               if(hltAccept_[i]) hTrkPtEtaJetEtW_Trig[ind]->Fill(trk.eta(),trk.pt(),leadJetEt_,1./(evt_sel_eff*trk.pt()));
	       index++;
            }

	    if(includeExtra_) {
	       hTrkPtEta->Fill(trk.eta(),trk.pt(),1./evt_sel_eff);
	       if(mult==1) hTrkPtEtaJetEtW_mult1->Fill(trk.eta(),trk.pt(),leadJetEt_,1./(evt_sel_eff*trk.pt()));
	       if(mult==2) hTrkPtEtaJetEtW_mult2->Fill(trk.eta(),trk.pt(),leadJetEt_,1./(evt_sel_eff*trk.pt()));
	       if(mult==3) hTrkPtEtaJetEtW_mult3->Fill(trk.eta(),trk.pt(),leadJetEt_,1./(evt_sel_eff*trk.pt()));
	       hTrkPtEtaJetEtW->Fill(trk.eta(),trk.pt(),leadJetEt_,1./(evt_sel_eff*trk.pt())); // weighted by pT  
	       hTrkPtEtaJetEtW_vbin->Fill(trk.eta(),trk.pt(),leadJetEt_,1./evt_sel_eff);
	    }
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
      unsigned index = 0; 
      if(sortedGJets.size()==0) leadGJetEt_ = 0,leadGJetEta_ = 0; 
      else leadGJetEt_ = sortedGJets[index]->et(), leadGJetEta_ = sortedGJets[index]->eta(); 
      
      // Gen event info
      edm::Handle<GenEventInfoProduct> genEvtInfo;
      iEvent.getByLabel("generator", genEvtInfo);
      int pid = genEvtInfo->signalProcessID();
      double pthat = genEvtInfo->qScale();
      
      bool isWanted = false;
      
      if(nsdOnly_){
	 if(pythia6_){
	    if( ((pid>=11) && (pid<=68)) || (pid==94) || (pid==95)) isWanted = true;
	 }else{
	    // SD - 103, 104, DD - 105, MB - 101
	    if(!(pid==103||pid==104)) isWanted = true;
	 }
      }else{
	 isWanted = true;
      }
      
      if((int)pthatCut_!=0){
	 if(pthat>pthatCut_) isWanted = false;
      }

      // Gen track
      if(isWanted){
	 Handle<GenParticleCollection> genParticles;
	 iEvent.getByLabel(gsrc_, genParticles);
	 const GenParticleCollection *genCollect = genParticles.product();
	 
	 for(unsigned i=0; i<genCollect->size();i++){
	    const GenParticle & gen = (*genCollect)[i];
	    if(gen.status() != 1) continue;
	    if(gen.charge() == 0) continue;
	    if(fabs(gen.eta())>etaMax_) continue;
	    if(!histOnly_) nt_gen_dndptdeta->Fill(gen.pt(),gen.eta(),leadJetEt_,leadGJetEt_,
		hltAccept_[1],hltAccept_[2],hltAccept_[3],hltAccept_[4]);
	    
	    hGenTrkPtEta->Fill(gen.eta(),gen.pt());

	    hGenTrkPtEtaJetEt->Fill(gen.eta(),gen.pt(),leadGJetEt_); 
	    hGenTrkPtEtaJetEt_vbin->Fill(gen.eta(),gen.pt(),leadGJetEt_);

	    if(includeExtra_) {
	       hGenTrkPtEtaJetEtW->Fill(gen.eta(),gen.pt(),leadGJetEt_,(1./gen.pt())); // weighted by pT
	       hGenTrkPtEtaJetEtW_vbin->Fill(gen.eta(),gen.pt(),leadGJetEt_,(1./gen.pt()));
	    }
	    
	 }
	 hGenNevt->Fill(nevtGEN);
      }
   } // isGEN
   
}
// ------------ method called once each job just before starting event loop  ------------
void 
TrackSpectraAnalyzer::beginJob()
{

   // define hist ragnes, number of bins, bins
   int numBins = 300;
   double xmax = 299.5;

   float etaHistMax = 2.4;
   int nbinsEta = 24;
   
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
   for(ptb = 121.2; ptb < 241.2-small; ptb += 10.0 ) ptBins.push_back(ptb); // 12 bins


   // eta bins
   static float etaMin   = -2.4;
   static float etaMax   =  2.4;
   static float etaWidth =  0.2;

   for(double eta = etaMin; eta < etaMax + etaWidth/2; eta += etaWidth)
      etaBins.push_back(eta);

   // jet et bins
   static float jetMin = 0.0;
   static float jetMax = 1200; // good to be matched with ana 
   static float jetWidth = 20;

   for(double jet = jetMin; jet < jetMax + jetWidth/2; jet += jetWidth)
      jetBins.push_back(jet);


   // Defin Histograms
   TFileDirectory subDir = fs->mkdir( "threeDHist" );

   //TH1::SetDefaultSumw2(true);
   //TH2::SetDefaultSumw2(true);
   //TH3::SetDefaultSumw2(true);

   if(!pureGENmode_){

      hNevt = fs->make<TH1F>("hNevt","evt sel eff", 102, -0.02, 2.02);
      hNevt_mult1 = fs->make<TH1F>("hNevt_mult1","evt sel eff", 102, -0.02, 2.02);
      hNevt_mult2 = fs->make<TH1F>("hNevt_mult2","evt sel eff", 102, -0.02, 2.02);
      hNevt_mult3 = fs->make<TH1F>("hNevt_mult3","evt sel eff", 102, -0.02, 2.02);

      hRecMult_STD = fs->make<TH1F>("hRecMult_STD","Charged mult. |#eta|<|#eta_{max}|)",numBins,-0.5,xmax);
      hRecMult_STD_corr = fs->make<TH1F>("hRecMult_STD_corr","Charged mult. |#eta|<|#eta_{max}|)",numBins,-0.5,xmax);

      if(!histOnly_) nt_dndptdeta = fs->make<TNtuple>("nt_dndptdeta","eta vs pt","pt:eta");


      hTrkPtEtaJetEt = subDir.make<TH3F>("hTrkPtEtaJetEt","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
					 nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0); 
      hTrkPtEtaJetEt_vbin = subDir.make<TH3F>("hTrkPtEtaJetEt_vbin","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
					      etaBins.size()-1, &etaBins[0],ptBins.size()-1, &ptBins[0],jetBins.size()-1, &jetBins[0]);
      
      unsigned index=0;
      for(unsigned i=0;i<hltNames_.size();i++){
	 if(neededTrigSpectra_[i]!=1) continue;
	 hTrkPtEtaJetEtW_Trig.push_back( subDir.make<TH3F>("","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
							    nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0));
	 hTrkPtEtaJetEtW_Trig[index]->SetName(Form("hTrkPtEtaJetEtW_%s",(char*) hltNames_[i].c_str()));
	 index++;
      }


      if(includeExtra_) {
	 hTrkPtEta = fs->make<TH2F>("hTrkPtEta","eta vs pt;#eta;p_{T} (GeV/c)", nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0);
	 hTrkPtEtaJetEtW_mult1 = subDir.make<TH3F>("hTrkPtEtaJetEtW_mult1","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						   nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
	 hTrkPtEtaJetEtW_mult2 = subDir.make<TH3F>("hTrkPtEtaJetEtW_mult2","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						   nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
	 hTrkPtEtaJetEtW_mult3 = subDir.make<TH3F>("hTrkPtEtaJetEtW_mult3","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						   nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
	 hTrkPtEtaJetEtW = subDir.make<TH3F>("hTrkPtEtaJetEtW","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
					     nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
	 hTrkPtEtaJetEtW_vbin = subDir.make<TH3F>("hTrkPtEtaJetEtW_vbin","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						  etaBins.size()-1, &etaBins[0],ptBins.size()-1, &ptBins[0],jetBins.size()-1, &jetBins[0]);
      }
      
      if(doJet_) {
	 if(!histOnly_) nt_jet = fs->make<TNtuple>("nt_jet","jet spectra ntuple","jet:jeta:jphi:mb:jet6:jet15:jet30:jet50");
	 if(!histOnly_) nt_jettrack = fs->make<TNtuple>("nt_jettrack","jet tracks correlation ntuple","pt:eta:jet:mb:jet6:jet15:jet30:jet50");
	 hNumJets = fs->make<TH1F>("hNumJets",";# jets in evt;# evts", 100, 0, 100);
	 hJet0Pt = fs->make<TH1F>("hJet0Pt","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0);
	 for(unsigned i=0;i<hltNames_.size();i++){
	    hJet0Pt_Trig.push_back(fs->make<TH1F>("","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 600, 0.0, 300.0));
	    hJet0Pt_Trig[i]->SetName(Form("hJet0Pt_%s",(char*) hltNames_[i].c_str()));
	 }
      }
   } // end of pureGENmode
   
   if(isGEN_) {
      hGenNevt = fs->make<TH1F>("hGenNevt","evt sel eff", 51, -0.01, 1.01);
      
      if(!histOnly_) nt_gen_dndptdeta = fs->make<TNtuple>("nt_gen_dndptdeta","eta vs pt","pt:eta:jet:gjet:jet6:jet15:jet30:jet50");
      hGenTrkPtEta = fs->make<TH2F>("hGenTrkPtEta","eta vs pt;#eta;p_{T} (GeV/c)", nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0);
      hGenTrkPtEtaJetEt = subDir.make<TH3F>("hGenTrkPtEtaJetEt","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
					    nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
      hGenTrkPtEtaJetEt_vbin = subDir.make<TH3F>("hGenTrkPtEtaJetEt_vbin","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						 etaBins.size()-1, &etaBins[0],ptBins.size()-1, &ptBins[0],jetBins.size()-1, &jetBins[0]);

      if(includeExtra_) {
	 hGenTrkPtEtaJetEtW = subDir.make<TH3F>("hGenTrkPtEtaJetEtW","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 60, 0.0, 1200.0);
	 hGenTrkPtEtaJetEtW_vbin = subDir.make<TH3F>("hGenTrkPtEtaJetEtW_vbin","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						     etaBins.size()-1, &etaBins[0],ptBins.size()-1, &ptBins[0],jetBins.size()-1, &jetBins[0]);
      }


      // Set Sumw2()
      /*
      hGenNevt->Sumw2();
      hGenTrkPtEta->Sumw2();
      hGenTrkPtEtaJetEt->Sumw2();
      hGenTrkPtEtaJetEtW->Sumw2();
      if(lowPtStudyHist_) hGenTrkLowPtEtaJetEtW->Sumw2();
      */
   }
    
}


//define this as a plug-in
DEFINE_FWK_MODULE(TrackSpectraAnalyzer);
