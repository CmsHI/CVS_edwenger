// -*- C++ -*-
//
// Package:    HiEvtSelAnalyzer
// Class:      HiEvtSelAnalyzer
// 
/**\class HiEvtSelAnalyzer HiEvtSelAnalyzer.cc edwenger/HiEvtSelAnalyzer/src/HiEvtSelAnalyzer.cc

 Description: An event selection analyzer that models after 
 UserCode/edwenger/EvtSelAnalyzer/src/EvtSelAnalyzer.cc

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Andre Yoon,32 4-A06,+41227676980,
//         Created:  Tue Mar 15 14:07:45 CET 2011
// $Id: HiEvtSelAnalyzer.cc,v 1.7 2011/03/19 19:05:55 sungho Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "edwenger/HiVertexAnalyzer/interface/HiVertexComparator.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"


#include <TH1.h>
#include <TH2.h>
#include "TF1.h"


//
// class declaration
//

class HiEvtSelAnalyzer : public edm::EDAnalyzer {
   public:
      explicit HiEvtSelAnalyzer(const edm::ParameterSet&);
      ~HiEvtSelAnalyzer();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

   // ----------member data ---------------------------
   edm::Service<TFileService> f;

   edm::InputTag gtlabel_;
   edm::InputTag triglabel_;
   edm::InputTag trklabel_;
   edm::InputTag vtxlabel_;
   edm::InputTag gtrklabel_;
   
   bool isGEN_;
   double ptMin_;

   std::vector<int32_t> neededCentBins_;
   
   std::vector<std::string> trignames_;

   CentralityProvider * centrality_;

   TH1F *hL1TechBits;
   TH1F *hL1AlgoBits;
   TH1F *hHLTPaths;

   TH1F *hVtxSize;
   TH1F *hVtxTracks;
   TH1F *hVtxZ;

   // for M = 0 not so peripheral events (<70%)
   TH1F *hVtxSizeZero;
   TH1F *hVtxTracksZero;
   TH1F *hVtxZZero;

   TH1F *hCentDist;
   TH1F *hPixelMultDst;

   TH1F *hRecMult;
   TH1F *hGenMult;

   TH2F *hVtxZTrkEta;
   
   std::vector<TH1F*> hRecMult_Cent;
   std::vector<TH1F*> hGenMult_Cent;

   std::vector<int32_t> nREC_Cent;
   std::vector<int32_t> nGEN_Cent;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HiEvtSelAnalyzer::HiEvtSelAnalyzer(const edm::ParameterSet& iConfig)
:
   gtlabel_(iConfig.getUntrackedParameter<edm::InputTag>("gtlabel")),
   triglabel_(iConfig.getUntrackedParameter<edm::InputTag>("triglabel")),
   trklabel_(iConfig.getUntrackedParameter<edm::InputTag>("trklabel")),
   vtxlabel_(iConfig.getUntrackedParameter<edm::InputTag>("vtxlabel")),
   gtrklabel_(iConfig.getUntrackedParameter<edm::InputTag>("gtrklabel")),
   isGEN_(iConfig.getUntrackedParameter<bool>("isGEN")),
   neededCentBins_(iConfig.getUntrackedParameter<std::vector<int> >("neededCentBins")),
   trignames_(iConfig.getUntrackedParameter<std::vector <std::string> >("trignames")),
   ptMin_(iConfig.getUntrackedParameter<double>("ptMin", 0.5)),
   centrality_(0)
{
   //now do what ever initialization is needed

}


HiEvtSelAnalyzer::~HiEvtSelAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HiEvtSelAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   bool warningM = false;

   // -------------- L1 trigger bits -------------------------
   edm::Handle<L1GlobalTriggerReadoutRecord> gtH;
   iEvent.getByLabel(gtlabel_,gtH);
  
   // technical bits before mask
   const TechnicalTriggerWord& tword = gtH->technicalTriggerWord();
   for(int tbit=0; tbit<64; tbit++) hL1TechBits->Fill(tbit,tword.at(tbit));
   
   // algo bits before mask
   const DecisionWord& word = gtH->decisionWord();
   for(int bit=0; bit<128; bit++) hL1AlgoBits->Fill(bit,word.at(bit));

   // -------------- HLT -------------------------------------
   edm::Handle<edm::TriggerResults> trigH;
   bool isTrigThere = iEvent.getByLabel(triglabel_,trigH);
    
   // jet triggers

   if(isTrigThere){ // just a protection
      const edm::TriggerNames names = iEvent.triggerNames(*trigH);
      for(unsigned i=0; i<trignames_.size(); i++) {
	 unsigned index = names.triggerIndex(trignames_[i]);
	 if(index < trigH->size()){
	    if(trigH->accept(index)) hHLTPaths->Fill(trignames_[i].c_str(),1);  
	 }else if(warningM){
	    edm::LogWarning("HiEvtSelAnalyzer") 
	       << "Index returned by TriggerNames object for trigger '"
	       << trignames_[i]
	       << "' is out of range (" 
	       << index << " >= " << trigH->size() << ")";
	 }
      }
   }


   //---------------- Vertex distribution -------------------
   edm::Handle<reco::VertexCollection> vtxsH;
   iEvent.getByLabel(vtxlabel_,vtxsH);

   reco::VertexCollection vtxs = *vtxsH;
   std::sort(vtxs.begin(),vtxs.end(),MoreTracksThenLowerChi2<reco::Vertex>());

   float vtxz = -999;

   hVtxSize->Fill(vtxs.size());
   if(vtxs.size()!=0){
      vtxz = vtxs[0].z();
      hVtxTracks->Fill(vtxs[0].tracksSize());
      hVtxZ->Fill(vtxz);
   }


   // -------------- Event centrality -----------------------
   if(!centrality_) centrality_ = new CentralityProvider(iSetup);
   centrality_->newEvent(iEvent,iSetup);

   int cbin = centrality_->getBin();
   float pixelhit = centrality_->raw()->multiplicityPixel();
   pixelhit = pixelhit/100.; // renormaliztion so that 120K -> 1200 

   hCentDist->Fill(cbin);
   hPixelMultDst->Fill(pixelhit);


   // -------------- Event multiplicity ----------------------
   edm::Handle<std::vector<reco::Track> > tracks;
   iEvent.getByLabel(trklabel_, tracks);

   int nREC=0;
   for(unsigned i=0;i<neededCentBins_.size()-1;i++)
      nREC_Cent[i]=0; // intialize


   for(unsigned it=0; it<tracks->size(); ++it){
      const reco::Track & trk = (*tracks)[it];

      hVtxZTrkEta->Fill(vtxz,trk.eta()); // acceptance 

      if(fabs(trk.eta())>2.4) continue;
      nREC++;
      
      // centrality binned multiplicity
      for(unsigned i=0;i<neededCentBins_.size()-1;i++){
	 if(i==0){
	    if(cbin<=neededCentBins_[i+1]) nREC_Cent[i]++;
	 }else{
	    if(cbin>neededCentBins_[i] && cbin<=neededCentBins_[i+1]) nREC_Cent[i]++;
	 }
      }
   }
   
   hRecMult->Fill(nREC);

   for(unsigned i=0;i<neededCentBins_.size()-1;i++){
      if(i==0){
	 if(cbin<=neededCentBins_[i+1])
	    hRecMult_Cent[i]->Fill(nREC_Cent[i]);
      }else{
	 if(cbin>neededCentBins_[i] && cbin<=neededCentBins_[i+1])
	    hRecMult_Cent[i]->Fill(nREC_Cent[i]);
      }
   }


   // invetigation plot

   if(nREC==0 && cbin<28){
      hVtxSizeZero->Fill(vtxs.size());
      if(vtxs.size()!=0){
	 hVtxTracksZero->Fill(vtxs[0].tracksSize());
	 hVtxZZero->Fill(vtxs[0].z());
      }
   }


   // -----------  Gen level information ---------------------
   if(isGEN_){
      
      edm::Handle<reco::GenParticleCollection> genParticles;
      iEvent.getByLabel(gtrklabel_, genParticles);
      const reco::GenParticleCollection *genCollect = genParticles.product();

      int nGEN=0;
      for(unsigned i=0;i<neededCentBins_.size()-1;i++)
	 nGEN_Cent[i]=0; // intialize                            

      for(unsigned i=0; i<genCollect->size();i++){
	 const reco::GenParticle & gen = (*genCollect)[i];

	 if(gen.status()!=1) continue;
	 if(gen.charge()==0) continue;
	 if(gen.pt()<ptMin_) continue;
	 if(fabs(gen.eta())>2.4) continue;

	 nGEN++;
	 // centrality binned multiplicity
	 for(unsigned i=0;i<neededCentBins_.size()-1;i++){
	    if(i==0){
	       if(cbin<=neededCentBins_[i+1]) nGEN_Cent[i]++;
	    }else{
	       if(cbin>neededCentBins_[i] && cbin<=neededCentBins_[i+1]) nGEN_Cent[i]++;
	    }
	 }
      }

      hGenMult->Fill(nGEN);
      for(unsigned i=0;i<neededCentBins_.size()-1;i++){
	 if(i==0){
	    if(cbin<=neededCentBins_[i+1])
	       hGenMult_Cent[i]->Fill(nGEN_Cent[i]);
	 }else{
	    if(cbin>neededCentBins_[i] && cbin<=neededCentBins_[i+1])
	       hGenMult_Cent[i]->Fill(nGEN_Cent[i]);
	 }
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
HiEvtSelAnalyzer::beginJob()
{

   hL1TechBits = f->make<TH1F>("hL1TechBits","L1 technical trigger bits before mask",64,-0.5,63.5);
   hL1AlgoBits = f->make<TH1F>("hL1AlgoBits","L1 algorithm trigger bits before mask",128,-0.5,127.5);
   hHLTPaths = f->make<TH1F>("hHLTPaths","HLT Paths",5,0,5);
   hHLTPaths->SetBit(TH1::kCanRebin);

   hVtxSize = f->make<TH1F>("hVtxSize","number of reconstructed vertices",10,-0.5,9.5);
   hVtxTracks = f->make<TH1F>("hVtxTracks","number of tracks fitted to vertex",120,-0.5,119.5);
   hVtxZ = f->make<TH1F>("hVtxZ","z position of best reconstructed vertex",120,-30.0,30.0);

   hVtxSizeZero = f->make<TH1F>("hVtxSizeZero","number of reconstructed vertices",10,-0.5,9.5);
   hVtxTracksZero = f->make<TH1F>("hVtxTracksZero","number of tracks fitted to vertex",120,-0.5,119.5);
   hVtxZZero = f->make<TH1F>("hVtxZZero","z position of best reconstructed vertex",120,-30.0,30.0);

   hCentDist = f->make<TH1F>("hCentDist","Centrality bin distribution; centrality bin",40,-0.5,39.5);
   hPixelMultDst = f->make<TH1F>("hPixelMultDst","Pixel hit distribution; first layer pixel hits",600,0,1200);

   hRecMult = f->make<TH1F>("hRecMult","Charged mult. |#eta|<2.4)",600,-0.5,599.5);
   hGenMult = f->make<TH1F>("hGenMult","Charged mult. |#eta|<2.4)",600,-0.5,599.5);

   hVtxZTrkEta = f->make<TH2F>("hVtxZTrkEta","vertex vz vs track eta",120,-30.0,30.0, 60,-2.65,2.65);

   // Centrality binned multiplicity

   for(unsigned i=0;i<neededCentBins_.size()-1;i++){

      nREC_Cent.push_back(0);
      nGEN_Cent.push_back(0);

      hRecMult_Cent.push_back(f->make<TH1F>("","Charged mult. |#eta|<2.4)",600,-0.5,599.5));
      hGenMult_Cent.push_back(f->make<TH1F>("","Charged mult. |#eta|<2.4)",600,-0.5,599.5));

      if(i==0){
	 hRecMult_Cent[i]->SetName(Form("hRecMult_cbin%dto%d",neededCentBins_[i],neededCentBins_[i+1]));
	 hGenMult_Cent[i]->SetName(Form("hGenMult_cbin%dto%d",neededCentBins_[i],neededCentBins_[i+1]));
      }else{
	 hRecMult_Cent[i]->SetName(Form("hRecMult_cbin%dto%d",neededCentBins_[i]+1,neededCentBins_[i+1]));
	 hGenMult_Cent[i]->SetName(Form("hGenMult_cbin%dto%d",neededCentBins_[i]+1,neededCentBins_[i+1]));
      }
   }

}

// ------------ method called once each job just after ending the event loop  ------------
void 
HiEvtSelAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HiEvtSelAnalyzer);
