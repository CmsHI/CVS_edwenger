//
// Original Author:  Andre Yoon,32 4-A06,+41227676980,
//         Created:  Wed Apr 28 16:18:39 CEST 2010
// $Id: HiTrackSpectraAnalyzer.cc,v 1.27 2011/03/23 20:00:39 sungho Exp $
//

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "edwenger/HiTrackSpectraAnalyzer/interface/HiTrackSpectraAnalyzer.h"
#include <TF1.h>

HiTrackSpectraAnalyzer::HiTrackSpectraAnalyzer(const edm::ParameterSet& iConfig) :
  pixelMult_(0),
  leadJetEt_(0),
  leadJetEta_(0), 
  leadJetOccEt_(0),
  leadJetOccEta_(0),
  leadGJetEt_(0),
  leadGJetEta_(0),
  occHandle_(0),
  occGENHandle_(0),
  hltAccept_(5,false),
  centrality_(0)
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
   histOnly_ = iConfig.getUntrackedParameter<bool>("histOnly", false);
   includeExtra_ = iConfig.getUntrackedParameter<bool>("includeExtra",false);
   etaMax_ = iConfig.getUntrackedParameter<double>("etaMax", 5.0);
   ptMin_ = iConfig.getUntrackedParameter<double>("ptMin", 0.5);
   applyEvtEffCorr_ = iConfig.getUntrackedParameter<bool>("applyEvtEffCorr", true);
   evtEffCorrType_ = iConfig.getUntrackedParameter<int>("evtEffCorrType", 0);
   evtSelEffv_ = iConfig.getUntrackedParameter< std::vector<double> >("evtSelEffv");
   evtMultCut_ = iConfig.getUntrackedParameter<int>("evtMultCut", 0);
   triggerNeeded_ = iConfig.getUntrackedParameter<bool>("triggerNeeded",false);
   hltNames_ = iConfig.getUntrackedParameter<std::vector <std::string> >("hltNames");
   neededTrigSpectra_ = iConfig.getUntrackedParameter<std::vector<int> >("neededTrigSpectra");
   jetEtCuts_ = iConfig.getUntrackedParameter<std::vector<double> >("jetEtCuts");
   triglabel_ = iConfig.getUntrackedParameter<edm::InputTag>("triglabel");
   neededCentBins_ = iConfig.getUntrackedParameter<std::vector<int> >("neededCentBins");
   pixelMultMode_ = iConfig.getUntrackedParameter<bool>("pixelMultMode",true);
   closestJets_ = iConfig.getUntrackedParameter<bool>("closestJets",true);
   trkAcceptedJet_ = iConfig.getUntrackedParameter<bool>("trkAcceptedJet",false);
   useSubLeadingJet_ = iConfig.getUntrackedParameter<bool>("useSubLeadingJet",false);
}

// ------------ method called to for each event  ------------
void
HiTrackSpectraAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;

   const string qualityString = "highPurity";

   float etaCut_evtSel = 0.0;
   bool skipEvt = false;
   bool warningM = false;

   if(evtEffCorrType_==0)
      etaCut_evtSel = 2.4;
   else if(evtEffCorrType_==1)
      etaCut_evtSel = 1.0;
   else if(evtEffCorrType_==2)
      etaCut_evtSel = 0.8;

   //----- intialization of variables ------------------
   pixelMult_ = 0.;
   leadJetEt_ = 0.,  leadJetEta_ = -999.; // so that if no jet present, put it outisde of scope
   leadJetOccEt_ = 0., leadJetOccEta_ = -999.; //
   leadGJetEt_ = 0., leadGJetEta_ = -999.;
   occHandle_ = 0.,  occGENHandle_ = 0.;

   //----- centrality information ----------------------
   centrality_ = new CentralityProvider(iSetup);
   centrality_->newEvent(iEvent,iSetup);
   int cbin = centrality_->getBin();

   if(pixelMultMode_){
      pixelMult_ = centrality_->raw()->multiplicityPixel();
      pixelMult_ = pixelMult_/100.; // scale it (120K -> 1200)
      hPxlMultDist->Fill(pixelMult_);
   }

   if(!pureGENmode_){  // if pure GEN, skip through to the GEN ana part
      // get hlt bit
      if(triggerNeeded_){
	 Handle<edm::TriggerResults> triggerResults;
	 iEvent.getByLabel(triglabel_, triggerResults);
	 
	 const edm::TriggerNames triggerNames = iEvent.triggerNames(*triggerResults); 
	 
	 for(unsigned i=0; i<hltNames_.size(); i++) { 
	    unsigned index = triggerNames.triggerIndex(hltNames_[i]);
	    if(index < triggerResults->size())
	       hltAccept_[i] = triggerResults->accept(index);
	    else if(warningM)
	       edm::LogWarning("HiTrackSpectraAnalyzer")
		  << "Index returned by TriggerNames object for trigger '"
		  << hltNames_[i]
		  << "' is out of range (" 
		  << index << " >= " << triggerResults->size() << ")";
	 } 
      }

      //----- loop over pat jets and store in a vector -----
      edm::Handle<reco::CandidateView> jets;
      iEvent.getByLabel(jsrc_,jets);
      hNumJets->Fill(jets->size()); // check # of jets found in event
      
      vector<const reco::Candidate *> sortedJets;         // jets for event normalization
      vector<const reco::Candidate *> sortedJets_occHand; // jets for event classfication (i.e. occupancy handle)
      
      for(unsigned it=0; it<jets->size(); ++it){
	 const reco::Candidate* jet = &((*jets)[it]);
	 
	 sortedJets.push_back(jet);
	 sortByEtRef (&sortedJets);
	 
	 if(trkAcceptedJet_) { // fill the jet pull only when the jet axes are within trk acceptance
	    if(fabs(jet->eta())<2.0) {
	       sortedJets_occHand.push_back(jet);
	       sortByEtRef (&sortedJets_occHand);
	    }
	 }else{
	    sortedJets_occHand.push_back(jet);
	    sortByEtRef (&sortedJets_occHand);
	 }
	 
      }
      
      for(unsigned it=0; it<sortedJets.size(); ++it){
	 // break statement below makes it iterate only once!
	 
	 if(!histOnly_) nt_jet->Fill(sortedJets[it]->et(),sortedJets[it]->eta(),sortedJets[it]->phi(),
				     hltAccept_[0],hltAccept_[1],hltAccept_[2],hltAccept_[3],hltAccept_[4]); 

	 if(fabs(sortedJets[it]->eta())>6.5) continue; // should be same as eta cut in jet trigger

	 // leading jet en only!, (break statement below)
	 leadJetEt_  = sortedJets[it]->et(), leadJetEta_ = sortedJets[it]->eta();
	 hJet0Pt->Fill(leadJetEt_), hJet0Eta->Fill(leadJetEta_);

	 // triggered jet et
	 for(unsigned i=0;i<hltNames_.size();i++){
	    if(hltAccept_[i]) hJet0Pt_Trig[i]->Fill(sortedJets[it]->et());
	 }
	 // centrality binned jet et
	 for(unsigned i=0;i<neededCentBins_.size()-1;i++){
	    if(i==0){
	       if(cbin<=neededCentBins_[i+1])
		  hJet0Pt_Cent[i]->Fill(sortedJets[it]->et());
	    }else{
	       if(cbin>neededCentBins_[i] && cbin<=neededCentBins_[i+1])
		  hJet0Pt_Cent[i]->Fill(sortedJets[it]->et());
	    }
	 }
	 // centrality bin vs jet et
	 hCentJetEt->Fill(cbin,sortedJets[it]->et());
	 break;             
      }
      
      // Get Leading jet energy from sortedJets_occHand
      for(unsigned it=0; it<sortedJets_occHand.size(); ++it){
	 if(useSubLeadingJet_){ // use sub-leading jet
	    if(sortedJets_occHand.size()>1) it++;
	    else break; // if not sub-leading jet, break
	 }
	 leadJetOccEt_ = sortedJets_occHand[it]->et();
	 leadJetOccEta_ = sortedJets_occHand[it]->eta();
	 break;
      }

      // Placing jet Et cuts such that the events are reqiured to have jet ET:[low,high]
      //if(jetEtCuts_.size()!=0)
      //std::cout<<"leading jet et = "<<leadJetEt_<<" jet et cut min = "<<jetEtCuts_[0]<<" max = "<<jetEtCuts_[1]<<std::endl;
      if(jetEtCuts_.size()!=0 && (leadJetEt_<jetEtCuts_[0] || leadJetEt_>jetEtCuts_[1])) skipEvt=true;
      
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
      double evt_sel_eff = 1.0;

      /*
	// remove later !
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
      */

      // occupancy handle
      if(pixelMultMode_) occHandle_ = pixelMult_;
      else occHandle_ = leadJetOccEt_;


      if(!skipEvt){
	 
	 hJet0Pt_jetCut->Fill(leadJetEt_), hJet0Eta_jetCut->Fill(leadJetEta_);
	 hJet0Pt_occHand->Fill(leadJetOccEt_), hJet0Eta_occHand->Fill(leadJetOccEta_);

	 // get track collection 
	 Handle<vector<Track> > tracks;
	 iEvent.getByLabel(src_, tracks);
	 
	 int ntrk_none_jet_ass=0, ntrk_lead_jet_ass=0, ntrk_slead_jet_ass=0;
	 
	 for(unsigned it=0; it<tracks->size(); ++it){
	    const reco::Track & trk = (*tracks)[it];
	    
	    if(setQualityBit_ && !trk.quality(reco::TrackBase::qualityByName(qualityString))) continue;
	    
	    if(!histOnly_){
	       nt_dndptdeta->Fill(trk.pt(),trk.eta()); 
	       nt_jettrack->Fill(trk.pt(),trk.eta(),leadJetEt_,
				 hltAccept_[0],hltAccept_[1],hltAccept_[2],hltAccept_[3],hltAccept_[4]); 
	    }

	    // Classfy tracks by leading jet ET, sub-leading jet ET, non-jet 
	    // by their proximity to a leading jet, sub-leading jet, and non of the two
	    if(closestJets_){
	       float closestJetdR = 99, dR=99;
	       int closestJetInd = -99;
	       unsigned int maxnjet 
		  = (sortedJets_occHand.size()<2) ?  sortedJets_occHand.size() : 2;
		  
	       for(unsigned k=0;k<maxnjet;k++){ // only leading and sub-leading
		  if(sortedJets_occHand[k]->et()<40) continue; // fake jet meaningless
		  dR=deltaR(*sortedJets_occHand[k],trk);
		  if(dR<0.8 && dR<closestJetdR){ // dR>0.8, should not influence efficiency..
		     closestJetdR=dR, closestJetInd=k;
		  }
	       }

	       if(closestJetInd<0){
		  occHandle_ = 0.0;
		  hdNdPt_none_jet->Fill(trk.pt());
		  if(fabs(trk.eta())<1.0) hdNdPt_none_jet_eta->Fill(trk.pt()); // eta restricted
		  ntrk_none_jet_ass++;
	       }else {
		  occHandle_ = sortedJets_occHand[closestJetInd]->et();
		  hClosestJetInd->Fill(closestJetInd);
		  if(closestJetInd==0){
		     hClosestJetdR_lead->Fill(closestJetdR);
		     hClosestJetdRdPt_lead->Fill(closestJetdR,trk.pt());
		     hClosestJetEta_lead->Fill(sortedJets_occHand[closestJetInd]->eta());
		     hdNdPt_lead_jet->Fill(trk.pt());
		     if(fabs(trk.eta())<1.0) 
			hdNdPt_lead_jet_eta->Fill(trk.pt()), hClosestJetdRdPt_lead_eta->Fill(closestJetdR,trk.pt());
		     ntrk_lead_jet_ass++;
		     if(ntrk_lead_jet_ass==1) hdNdEt_leadjet->Fill(occHandle_); // fill once
		  }else{
		     hClosestJetdR_slead->Fill(closestJetdR);
		     hClosestJetdRdPt_slead->Fill(closestJetdR,trk.pt());
		     hClosestJetEta_slead->Fill(sortedJets_occHand[closestJetInd]->eta());
		     hdNdPt_slead_jet->Fill(trk.pt());
		     if(fabs(trk.eta())<1.0) 
			hdNdPt_slead_jet_eta->Fill(trk.pt()), hClosestJetdRdPt_slead_eta->Fill(closestJetdR,trk.pt());
		     ntrk_slead_jet_ass++;
		     if(ntrk_slead_jet_ass==1) hdNdEt_sleadjet->Fill(occHandle_);
		  }
	       }
	    } // end of if(closestJets_)

	    hTrkPtEtaJetEt->Fill(trk.eta(),trk.pt(),occHandle_,1./evt_sel_eff);
	    hTrkPtEtaJetEt_vbin->Fill(trk.eta(),trk.pt(),occHandle_,1./evt_sel_eff);

	    // triggered spectra 
	    if(includeExtra_) {
	       unsigned ind=0;
	       for(unsigned i=0;i<hltNames_.size();i++){
		  if(neededTrigSpectra_[i]!=1) continue;
		  if(hltAccept_[i]) hTrkPtEtaJetEt_Trig[ind]->Fill(trk.eta(),trk.pt(),occHandle_,1./evt_sel_eff);
		  ind++;
	       }
	    }

	    // centrality binned spectra
	    for(unsigned i=0;i<neededCentBins_.size()-1;i++){
	       if(i==0){
		  if(cbin<=neededCentBins_[i+1]) 
		     hTrkPtEtaJetEt_Cent[i]->Fill(trk.eta(),trk.pt(),occHandle_,1./evt_sel_eff);
	       }else{
		  if(cbin>neededCentBins_[i] && cbin<=neededCentBins_[i+1])
		     hTrkPtEtaJetEt_Cent[i]->Fill(trk.eta(),trk.pt(),occHandle_,1./evt_sel_eff);
	       }
	    }

	    if(includeExtra_) {
	       hTrkPtEta->Fill(trk.eta(),trk.pt(),1./evt_sel_eff);
	       if(mult==1) hTrkPtEtaJetEtW_mult1->Fill(trk.eta(),trk.pt(),occHandle_,1./(evt_sel_eff*trk.pt()));
	       if(mult==2) hTrkPtEtaJetEtW_mult2->Fill(trk.eta(),trk.pt(),occHandle_,1./(evt_sel_eff*trk.pt()));
	       if(mult==3) hTrkPtEtaJetEtW_mult3->Fill(trk.eta(),trk.pt(),occHandle_,1./(evt_sel_eff*trk.pt()));
	       hTrkPtEtaJetEtW->Fill(trk.eta(),trk.pt(),occHandle_,1./(evt_sel_eff*trk.pt())); // weighted by pT  
	       hTrkPtEtaJetEtW_vbin->Fill(trk.eta(),trk.pt(),occHandle_,1./evt_sel_eff);
	    }
	 } // end of track loop
	 
	 // jet-associated track multiplicity
	 hNtrkNoneJet->Fill(ntrk_none_jet_ass);
	 hNtrkLeadJet->Fill(ntrk_lead_jet_ass);
	 hNtrkSLeadJet->Fill(ntrk_slead_jet_ass);

	 hNevt->Fill(evt_sel_eff);

	 // centrality binned number of events
	 for(unsigned i=0;i<neededCentBins_.size()-1;i++){
	    if(i==0){
	       if(cbin<=neededCentBins_[i+1])
		  hNevt_Cent[i]->Fill(evt_sel_eff);
	    }else{
	       if(cbin>neededCentBins_[i] && cbin<=neededCentBins_[i+1])
		  hNevt_Cent[i]->Fill(evt_sel_eff);
	    }
	 }

      }// end of skip evt

   } // end of if(pureGENmode_)
   
   
   if(isGEN_ && !skipEvt){
      float nevtGEN = 1.0;
	 
      edm::Handle<reco::CandidateView> gjets;
      iEvent.getByLabel(gjsrc_,gjets);
      
      vector<const reco::Candidate *> sortedGJets;
      vector<const reco::Candidate *> sortedGJets_occHand;
      
      for(unsigned it=0; it<gjets->size(); ++it){
	 const reco::Candidate* jet = &((*gjets)[it]);

	 sortedGJets.push_back(jet);
         sortByEtRef (&sortedGJets);

	 if(trkAcceptedJet_) { // fill the jet pull only when the jet axes are within trk acceptance 
	    if(fabs(jet->eta())<2.0) {
	       sortedGJets_occHand.push_back(jet);
	       sortByEtRef (&sortedGJets_occHand);
	    }
	 }else{
	    sortedGJets_occHand.push_back(jet);
	    sortByEtRef (&sortedGJets_occHand);
	 }
      }

      // Get Leading jet energy
      for(unsigned it=0; it<sortedGJets_occHand.size(); ++it){
	 leadGJetEt_ = sortedGJets_occHand[it]->et();
	 leadGJetEta_ = sortedGJets_occHand[it]->eta();
	 break;
      }
      
      // Gen event info
      edm::Handle<GenEventInfoProduct> genEvtInfo;
      iEvent.getByLabel("generator", genEvtInfo);
      int pid = genEvtInfo->signalProcessID();
      double pthat = genEvtInfo->qScale();
      
      bool isWanted = true;
      
      // occupancy handle 
      if(pixelMultMode_) occGENHandle_ = pixelMult_;
      else occGENHandle_ = leadJetEt_;


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
	    

	    if(closestJets_){
	       float closestGJetdR = 99, GdR=99;
               int closestGJetInd = -99;

               for(unsigned k=0;k<sortedGJets_occHand.size();k++){
                  if(sortedGJets_occHand[k]->et()<40) continue; // fake jet meaningless                                                                      
                  GdR=deltaR(*sortedGJets_occHand[k],gen);
                  if (GdR<closestGJetdR) {
                     closestGJetdR=GdR;
                     closestGJetInd=k;
		  }
               }

               if(closestGJetInd<0) occGENHandle_ = 0.0;
	       else occGENHandle_ = sortedGJets_occHand[closestGJetInd]->et();
            }


	    hGenTrkPtEta->Fill(gen.eta(),gen.pt());
	    hGenTrkPtEtaJetEt->Fill(gen.eta(),gen.pt(),occGENHandle_); 
	    hGenTrkPtEtaJetEt_vbin->Fill(gen.eta(),gen.pt(),occGENHandle_);

	    // centrality binned spectra
            for(unsigned i=0;i<neededCentBins_.size()-1;i++){
               if(i==0){
                  if(cbin<=neededCentBins_[i+1])
                     hGenTrkPtEtaJetEt_Cent[i]->Fill(gen.eta(),gen.pt(),occGENHandle_);
               }else{
                  if(cbin>neededCentBins_[i] && cbin<=neededCentBins_[i+1])
                     hGenTrkPtEtaJetEt_Cent[i]->Fill(gen.eta(),gen.pt(),occGENHandle_);
               }
            }

	    // extra
	    if(includeExtra_) {
	       hGenTrkPtEtaJetEtW->Fill(gen.eta(),gen.pt(),occGENHandle_,(1./gen.pt())); // weighted by pT
	       hGenTrkPtEtaJetEtW_vbin->Fill(gen.eta(),gen.pt(),occGENHandle_,(1./gen.pt()));
	    }
	    
	 }
	 hGenNevt->Fill(nevtGEN);
      }
   } // isGEN
   
}
// ------------ method called once each job just before starting event loop  ------------
void 
HiTrackSpectraAnalyzer::beginJob()
{

   // define hist ragnes, number of bins, bins
   int numBins = 300;
   double xmax = 299.5;

   float etaHistMax = 2.4;
   int nbinsEta = 24;
   
   // pt bins
   const double small = 1e-3;
   double ptb;

   // simple rebinning possible with a rebinning facto n = 2, 3, 6 !
   for(ptb =   0  ; ptb <   1.2-small; ptb +=  0.05) ptBins.push_back(ptb); // 24 bins
   for(ptb =   1.2; ptb <   2.4-small; ptb +=  0.1 ) ptBins.push_back(ptb); // 12 bins
   for(ptb =   2.4; ptb <   7.2-small; ptb +=  0.4 ) ptBins.push_back(ptb); // 12 bins
   for(ptb =   7.2; ptb <  16.8-small; ptb +=  1.6 ) ptBins.push_back(ptb); // 6 bins
   for(ptb =  16.8; ptb <  55.2-small; ptb +=  6.4 ) ptBins.push_back(ptb); // 6 bins
   for(ptb =  55.2; ptb < 170.4-small; ptb +=  9.6 ) ptBins.push_back(ptb); // 12 bins
   ptBins.push_back(170.4);

   // eta bins
   static double etaMin   = -2.6;
   static double etaMax   =  2.6;
   static double etaWidth =  0.4;

   for(double eta = etaMin; eta < etaMax + etaWidth/2; eta += etaWidth)
      etaBins.push_back(eta);

   // jet et bins
   /*
   double jet;
   for(jet =    0; jet <   50-small; jet +=  50 ) jetBins.push_back(jet);
   for(jet =   50; jet <   80-small; jet +=  30 ) jetBins.push_back(jet);
   for(jet =   80; jet < 1000-small; jet +=  25 ) jetBins.push_back(jet);
   jetBins.push_back(1005);
   */

   static float jetMin = 0.0;
   static float jetMax = 1000; // good to be matched with ana 
   static float jetWidth = 20;

   for(double jet = jetMin; jet < jetMax + jetWidth/2; jet += jetWidth)
      jetBins.push_back(jet);

   static double centMin   =  -0.5;
   static double centMax   =  39.5;
   static double centWidth =   1.0;

   for(double cent = centMin; cent < centMax + centWidth/2; cent += centWidth)
      centBins.push_back(cent);


   // Defin Histograms
   TFileDirectory subDir = fs->mkdir( "threeDHist" );

   if(!pureGENmode_){

      hNevt = fs->make<TH1F>("hNevt","evt sel eff", 102, -0.02, 2.02);

      hCentJetEt = fs->make<TH2F>("hCentJetEt","Centrality vs Jet E_{T};centrality bin; E_{T}", centBins.size()-1,&centBins[0], jetBins.size()-1,&jetBins[0]);

      //if(pixelMultMode_) hPxlMultDist = fs->make<TH1F>("hPxlMultDist","pixel mult dist",50, 0.0, 1000.0);
      //hRecMult_STD = fs->make<TH1F>("hRecMult_STD","Charged mult. |#eta|<|#eta_{max}|)",numBins,-0.5,xmax);
      //hRecMult_STD_corr = fs->make<TH1F>("hRecMult_STD_corr","Charged mult. |#eta|<|#eta_{max}|)",numBins,-0.5,xmax);

      if(!histOnly_) nt_dndptdeta = fs->make<TNtuple>("nt_dndptdeta","eta vs pt","pt:eta");


      hTrkPtEtaJetEt = subDir.make<TH3F>("hTrkPtEtaJetEt","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
					 nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 50, 0.0, 1000.0); 
      hTrkPtEtaJetEt_vbin = subDir.make<TH3F>("hTrkPtEtaJetEt_vbin","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
					      etaBins.size()-1, &etaBins[0],ptBins.size()-1, &ptBins[0],jetBins.size()-1, &jetBins[0]);
      
      // cross check by summing all binned spectra and compare with "MB"
      // test by running on binned selection
      for(unsigned i=0;i<neededCentBins_.size()-1;i++){
	 hTrkPtEtaJetEt_Cent.push_back( subDir.make<TH3F>("","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
                                                          etaBins.size()-1, &etaBins[0],ptBins.size()-1, &ptBins[0],jetBins.size()-1, &jetBins[0]) );
	 if(i==0) hTrkPtEtaJetEt_Cent[i]->SetName(Form("hTrkPtEtaJetEt_vbin_cbin%dto%d",neededCentBins_[i],neededCentBins_[i+1]));  
	 else hTrkPtEtaJetEt_Cent[i]->SetName(Form("hTrkPtEtaJetEt_vbin_cbin%dto%d",neededCentBins_[i]+1,neededCentBins_[i+1]));

	 // this is Nevt histogram for different centrality 
	 hNevt_Cent.push_back(fs->make<TH1F>("","evt sel eff", 102, -0.02, 2.02));
	 if(i==0) hNevt_Cent[i]->SetName(Form("hNevt_cbin%dto%d",neededCentBins_[i],neededCentBins_[i+1]));
	 else hNevt_Cent[i]->SetName(Form("hNevt_cbin%dto%d",neededCentBins_[i]+1,neededCentBins_[i+1]));

	 // this is hJet0Pt histograms for different centrality
	 hJet0Pt_Cent.push_back(fs->make<TH1F>("","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 550, 0.0, 1100.0));
         if(i==0) hJet0Pt_Cent[i]->SetName(Form("hJet0Pt_cbin%dto%d",neededCentBins_[i],neededCentBins_[i+1]));
         else hJet0Pt_Cent[i]->SetName(Form("hJet0Pt_cbin%dto%d",neededCentBins_[i]+1,neededCentBins_[i+1]));
      }

      if(includeExtra_) {
	 hTrkPtEta = fs->make<TH2F>("hTrkPtEta","eta vs pt;#eta;p_{T} (GeV/c)", nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0);
	 hTrkPtEtaJetEtW_mult1 = subDir.make<TH3F>("hTrkPtEtaJetEtW_mult1","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						   nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 50, 0.0, 1000.0);
	 hTrkPtEtaJetEtW_mult2 = subDir.make<TH3F>("hTrkPtEtaJetEtW_mult2","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						   nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 50, 0.0, 1000.0);
	 hTrkPtEtaJetEtW_mult3 = subDir.make<TH3F>("hTrkPtEtaJetEtW_mult3","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						   nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 50, 0.0, 1000.0);
	 hTrkPtEtaJetEtW = subDir.make<TH3F>("hTrkPtEtaJetEtW","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
					     nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 50, 0.0, 1000.0);
	 hTrkPtEtaJetEtW_vbin = subDir.make<TH3F>("hTrkPtEtaJetEtW_vbin","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						  etaBins.size()-1, &etaBins[0],ptBins.size()-1, &ptBins[0],jetBins.size()-1, &jetBins[0]);

	 unsigned index=0;
	 for(unsigned i=0;i<hltNames_.size();i++){
	    if(neededTrigSpectra_[i]!=1) continue;
	    hTrkPtEtaJetEt_Trig.push_back( subDir.make<TH3F>("","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
							     etaBins.size()-1, &etaBins[0],ptBins.size()-1, &ptBins[0],jetBins.size()-1, &jetBins[0]) );
	    hTrkPtEtaJetEt_Trig[index]->SetName(Form("hTrkPtEtaJetEt_%s",(char*) hltNames_[i].c_str()));
	    index++;
	 }

      }
      
      if(!histOnly_) nt_jet = fs->make<TNtuple>("nt_jet","jet spectra ntuple","jet:jeta:jphi:mb:jet6:jet15:jet30:jet50");
      if(!histOnly_) nt_jettrack = fs->make<TNtuple>("nt_jettrack","jet tracks correlation ntuple","pt:eta:jet:mb:jet6:jet15:jet30:jet50");
      hNumJets = fs->make<TH1F>("hNumJets",";# jets in evt;# evts", 100, 0, 100);
      hJet0Pt = fs->make<TH1F>("hJet0Pt","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 550, 0.0, 1100.0);
      hJet0Eta = fs->make<TH1F>("hJet0Eta","jet eta; #eta", 50,-6.0,6.0);
      hJet0Pt_jetCut = fs->make<TH1F>("hJet0Pt_jetCut","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 550, 0.0, 1100.0);
      hJet0Eta_jetCut = fs->make<TH1F>("hJet0Eta_jetCut","jet eta; #eta", 50,-6.0,6.0);
      hJet0Pt_occHand = fs->make<TH1F>("hJet0Pt_occHand","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 550, 0.0, 1100.0);
      hJet0Eta_occHand = fs->make<TH1F>("hJet0Eta_occHand","jet eta; #eta", 50,-6.0,6.0);
      if(closestJets_){
	 hClosestJetInd = fs->make<TH1F>("hClosestJetInd","index of closest jet",20,0.0,20.);
	 hClosestJetdR_lead = fs->make<TH1F>("hClosestJetdR_lead","dR of closest jet",60,0.0,1.0);
	 hClosestJetdR_slead = fs->make<TH1F>("hClosestJetdR_slead","dR of closest jet",60,0.0,1.0);
	 hClosestJetEta_lead = fs->make<TH1F>("hClosestJetEta_lead","jet eta; #eta", 50,-6.0,6.0);
	 hClosestJetEta_slead = fs->make<TH1F>("hClosestJetEta_slead","jet eta; #eta", 50,-6.0,6.0);
	 hdNdPt_none_jet = fs->make<TH1F>("hdNdPt_none_jet","no jet-associated track pT;p_{T} [GeV/c]", 400, 0.0, 200.0);
	 hdNdPt_lead_jet = fs->make<TH1F>("hdNdPt_lead_jet","leading jet-associated track pT;p_{T} [GeV/c]", 400, 0.0, 200.0);
	 hdNdPt_slead_jet = fs->make<TH1F>("hdNdPt_slead_jet","sub-leading jet-associated track pT;p_{T} [GeV/c]", 400, 0.0, 200.0);
	 hdNdPt_none_jet_eta = fs->make<TH1F>("hdNdPt_none_jet_eta","no jet-associated track pT;p_{T} [GeV/c]", 400, 0.0, 200.0);
         hdNdPt_lead_jet_eta = fs->make<TH1F>("hdNdPt_lead_jet_eta","leading jet-associated track pT;p_{T} [GeV/c]", 400, 0.0, 200.0);
         hdNdPt_slead_jet_eta = fs->make<TH1F>("hdNdPt_slead_jet_eta","sub-leading jet-associated track pT;p_{T} [GeV/c]", 400, 0.0, 200.0);
	 hdNdEt_leadjet = fs->make<TH1F>("hdNdEt_leadjet","leading jet energy distribution; E_{T} [GeV]", 50,0.0,1000.0);
	 hdNdEt_sleadjet = fs->make<TH1F>("hdNdEt_sleadjet","sub-leading jet energy distribution; E_{T} [GeV]", 50,0.0,1000.0);
	 hNtrkNoneJet = fs->make<TH1F>("hNtrkNoneJet","track multiplicity",200,0.0,200);
	 hNtrkLeadJet = fs->make<TH1F>("hNtrkLeadJet","track multiplicity",200,0.0,200);
	 hNtrkSLeadJet = fs->make<TH1F>("hNtrkSLeadJet","track multiplicity",200,0.0,200);
	 hClosestJetdRdPt_lead = fs->make<TH2F>("hClosestJetdRdPt_lead","dR of closest jet vs pt;dR;p_{T} [GeV/c]",60,0.0,1.0, 340,0.0,170.0);
         hClosestJetdRdPt_slead = fs->make<TH2F>("hClosestJetdRdPt_slead","dR of closest jet vs pt;dR;p_{T} [GeV/c]",60,0.0,1.0, 340,0.0,170.0);
	 hClosestJetdRdPt_lead_eta = fs->make<TH2F>("hClosestJetdRdPt_lead_eta","dR of closest jet vs pt;dR;p_{T} [GeV/c]",60,0.0,1.0, 340,0.0,170.0);
         hClosestJetdRdPt_slead_eta = fs->make<TH2F>("hClosestJetdRdPt_slead_eta","dR of closest jet vs pt;dR;p_{T} [GeV/c]",60,0.0,1.0, 340,0.0,170.0);
      }
      for(unsigned i=0;i<hltNames_.size();i++){
	 hJet0Pt_Trig.push_back(fs->make<TH1F>("","jet p_{T}; p_{T}^{corr jet} [GeV/c]", 550, 0.0, 1100.0));
	 hJet0Pt_Trig[i]->SetName(Form("hJet0Pt_%s",(char*) hltNames_[i].c_str()));
      }
   } // end of pureGENmode
   
   if(isGEN_) {
      hGenNevt = fs->make<TH1F>("hGenNevt","evt sel eff", 51, -0.01, 1.01);
      
      if(!histOnly_) nt_gen_dndptdeta = fs->make<TNtuple>("nt_gen_dndptdeta","eta vs pt","pt:eta:jet:gjet:jet6:jet15:jet30:jet50");
      hGenTrkPtEta = fs->make<TH2F>("hGenTrkPtEta","eta vs pt;#eta;p_{T} (GeV/c)", nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0);
      hGenTrkPtEtaJetEt = subDir.make<TH3F>("hGenTrkPtEtaJetEt","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
					    nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 50, 0.0, 1000.0);
      hGenTrkPtEtaJetEt_vbin = subDir.make<TH3F>("hGenTrkPtEtaJetEt_vbin","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						 etaBins.size()-1, &etaBins[0],ptBins.size()-1, &ptBins[0],jetBins.size()-1, &jetBins[0]);

      for(unsigned i=0;i<neededCentBins_.size()-1;i++){
         hGenTrkPtEtaJetEt_Cent.push_back( subDir.make<TH3F>("","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
							     etaBins.size()-1, &etaBins[0],ptBins.size()-1, &ptBins[0],jetBins.size()-1, &jetBins[0]) );
         if(i==0) hGenTrkPtEtaJetEt_Cent[i]->SetName(Form("hGenTrkPtEtaJetEt_vbin_cbin%dto%d",neededCentBins_[i],neededCentBins_[i+1]));
         else hGenTrkPtEtaJetEt_Cent[i]->SetName(Form("hGenTrkPtEtaJetEt_vbin_cbin%dto%d",neededCentBins_[i]+1,neededCentBins_[i+1]));
      }

      if(includeExtra_) {
	 hGenTrkPtEtaJetEtW = subDir.make<TH3F>("hGenTrkPtEtaJetEtW","eta vs pt vs jet;#eta;p_{T} (GeV/c);E_{T} (GeV/c)",
						nbinsEta, -1.*etaHistMax, etaHistMax, 1000, 0.0, 200.0, 50, 0.0, 1000.0);
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
DEFINE_FWK_MODULE(HiTrackSpectraAnalyzer);
