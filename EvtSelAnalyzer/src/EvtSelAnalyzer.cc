//
// Original Author:  Edward Wenger
//         Created:  Fri May  7 10:33:49 CEST 2010
// $Id$
//

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "edwenger/EvtSelAnalyzer/interface/EvtSelAnalyzer.h"

EvtSelAnalyzer::EvtSelAnalyzer(const edm::ParameterSet& iConfig)
:
  gtlabel_(iConfig.getUntrackedParameter<edm::InputTag>("gtlabel")),
  towerslabel_(iConfig.getUntrackedParameter<edm::InputTag>("towerslabel")),
  hfEThreshold_(iConfig.getUntrackedParameter<double>("hfEThreshold")),
  trackslabel_(iConfig.getUntrackedParameter<edm::InputTag>("trackslabel")),
  qualityString_(iConfig.getUntrackedParameter<std::string>("qualityString")),
  triglabel_(iConfig.getUntrackedParameter<edm::InputTag>("triglabel")),
  trignames_(iConfig.getUntrackedParameter<std::vector <std::string> >("trignames"))
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
  for(unsigned i=0; i<trignames_.size(); i++)
    if(trigH->accept(names.triggerIndex(trignames_[i])))
      hHLTPaths->Fill(trignames_[i].c_str(),1);


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

}

// ------------ method called once each job aftre completing the event loop  ------------
void 
EvtSelAnalyzer::endJob()
{

  hHLTPaths->LabelsDeflate();

}

//define this as a plug-in
DEFINE_FWK_MODULE(EvtSelAnalyzer);
