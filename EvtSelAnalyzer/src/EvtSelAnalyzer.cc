//
// Original Author:  Edward Wenger
//         Created:  Fri May  7 10:33:49 CEST 2010
// $Id: EvtSelAnalyzer.cc,v 1.2 2010/05/10 14:56:00 edwenger Exp $
//

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "edwenger/EvtSelAnalyzer/interface/EvtSelAnalyzer.h"

EvtSelAnalyzer::EvtSelAnalyzer(const edm::ParameterSet& iConfig)
:
  gtlabel_(iConfig.getUntrackedParameter<edm::InputTag>("gtlabel")),
  towerslabel_(iConfig.getUntrackedParameter<edm::InputTag>("towerslabel")),
  hfEThreshold_(iConfig.getUntrackedParameter<double>("hfEThreshold")),
  trackslabel_(iConfig.getUntrackedParameter<edm::InputTag>("trackslabel")),
  qualityString_(iConfig.getUntrackedParameter<std::string>("qualityString")),
  triglabel_(iConfig.getUntrackedParameter<edm::InputTag>("triglabel")),
  trignames_(iConfig.getUntrackedParameter<std::vector <std::string> >("trignames")),
  isGEN_(iConfig.getUntrackedParameter<bool>("isGEN"))
{

}

// ------------ method called to for each event  ------------
void
EvtSelAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  //------- L1 trigger bits --------------------
  edm::Handle<L1GlobalTriggerReadoutRecord> gtH;
  iEvent.getByLabel(gtlabel_,gtH);
  
  // technical bits before mask
  const TechnicalTriggerWord& tword = gtH->technicalTriggerWord();
  for(int tbit=0; tbit<64; tbit++) hL1TechBits->Fill(tbit,tword.at(tbit));
    
  // algo bits before mask
  const DecisionWord& word = gtH->decisionWord();
  for(int bit=0; bit<128; bit++) hL1AlgoBits->Fill(bit,word.at(bit));

  //------- HLT -------------------------------

    edm::Handle<edm::TriggerResults> trigH;
    iEvent.getByLabel(triglabel_,trigH);
    
    // jet triggers
    const edm::TriggerNames names = iEvent.triggerNames(*trigH);
    for(unsigned i=0; i<trignames_.size(); i++) {
      unsigned index = names.triggerIndex(trignames_[i]);
      if(index < trigH->size()) {
	if(trigH->accept(index)) hHLTPaths->Fill(trignames_[i].c_str(),1);  
      } else {
	edm::LogWarning("EvtSelAnalyzer") 
	  << "Index returned by TriggerNames object for trigger '"
	  << trignames_[i]
	  << "' is out of range (" 
	  << index << " >= " << trigH->size() << ")";
      }
    }

  //------- Calotowers ------------------------
  edm::Handle<CaloTowerCollection> towersH;
  iEvent.getByLabel(towerslabel_,towersH);
  
  // HF towers above threshold
  int nHfTowersN=0, nHfTowersP=0;
  for(CaloTowerCollection::const_iterator calo = towersH->begin(); calo != towersH->end(); ++calo) {
    if(calo->energy() < hfEThreshold_) continue;
    if(calo->eta()>3) nHfTowersP++;
    if(calo->eta()<-3) nHfTowersN++;
  }
  hHfTowers->Fill(nHfTowersP,nHfTowersN);
  
  //------- Tracks ----------------------------
  edm::Handle< std::vector <reco::Track> > tracksH;
  iEvent.getByLabel(trackslabel_,tracksH);

  // high-purity fraction
  int numhighpurity=0;
  float fraction=0;
  for(unsigned it=0; it<tracksH->size(); it++)
    if((*tracksH)[it].quality(reco::TrackBase::qualityByName(qualityString_))) numhighpurity++;
  if(tracksH->size()) fraction = (float) numhighpurity / (float) tracksH->size();
  hHPFracNtrk->Fill(tracksH->size(),fraction);

  //------- GEN --------------------------------
  if(isGEN_) {
    edm::Handle<reco::GenParticleCollection> genParticles;
    iEvent.getByLabel("genParticles", genParticles);
    
    int nGEN=0;
    const reco::GenParticleCollection *genCollect = genParticles.product();
    for(unsigned i=0; i<genCollect->size();i++){
      const reco::GenParticle & gen = (*genCollect)[i];
      if(gen.status() != 1) continue;
      if(gen.collisionId() != 0) continue;
      if(gen.charge() == 0) continue;
      if(fabs(gen.eta())>2.5) continue;
      nGEN++;
    }
    
    edm::Handle<GenEventInfoProduct> genEvtInfo;
    iEvent.getByLabel("generator", genEvtInfo);
    int pid = genEvtInfo->signalProcessID();
    edm::LogVerbatim("EvtSelAnalyzer") 
      << "Process ID = " << pid << ", GEN multiplicity = " << nGEN;
    
    hGenMultInel->Fill(nGEN);

    switch(pid) {
    case 94:
      hGenMultDD->Fill(nGEN);
      hGenMultNSD->Fill(nGEN);
      break;
    case 92:
    case 93:
      hGenMultSD->Fill(nGEN);
      break;
    case 11:
    case 12:
    case 13:
    case 28:
    case 53:
    case 68:
    case 95:
      hGenMultND->Fill(nGEN);
      hGenMultNSD->Fill(nGEN);
      break;
    default:
      edm::LogWarning("EvtSelAnalyzer") 
	<< "Is this PYTHIA?! This code doesn't know what to do for process ID = " << pid;
    }

  }
  
}
  
// ------------ method called once each job just before starting event loop  ------------
void 
EvtSelAnalyzer::beginJob()
{

  hL1TechBits = f->make<TH1D>("hL1TechBits","L1 technical trigger bits before mask",64,-0.5,63.5);
  hL1AlgoBits = f->make<TH1D>("hL1AlgoBits","L1 algorithm trigger bits before mask",128,-0.5,127.5);
  hHPFracNtrk = f->make<TH2D>("hHPFracNtrk","High purity fraction vs. # of tracks; number of tracks; highPurity fraction",50,0,500,50,0,1);
  hHfTowers   = f->make<TH2D>("hHfTowers","Number of HF towers above threshold; positive side; negative side",80,-0.5,79.5,80,-0.5,79.5);
  hHLTPaths = f->make<TH1D>("hHLTPaths","HLT Paths",5,0,5);
  hHLTPaths->SetBit(TH1::kCanRebin);

  if(isGEN_) {
    hGenMultInel = f->make<TH1D>("hGenMultInel","Charged mult. (inel.) |#eta|<2.5)",100,-0.5,99.5);
    hGenMultNSD = f->make<TH1D>("hGenMultNSD","Charged mult. (NSD) |#eta|<2.5)",100,-0.5,99.5);
    hGenMultSD = f->make<TH1D>("hGenMultSD","Charged mult. (SD) |#eta|<2.5)",100,-0.5,99.5);
    hGenMultDD = f->make<TH1D>("hGenMultDD","Charged mult. (DD) |#eta|<2.5)",100,-0.5,99.5);
    hGenMultND = f->make<TH1D>("hGenMultND","Charged mult. (ND) |#eta|<2.5)",100,-0.5,99.5);
  }

}

// ------------ method called once each job after completing the event loop  ------------
void 
EvtSelAnalyzer::endJob()
{

  hHLTPaths->LabelsDeflate();

}

//define this as a plug-in
DEFINE_FWK_MODULE(EvtSelAnalyzer);
