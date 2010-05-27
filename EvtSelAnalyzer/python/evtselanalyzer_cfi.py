import FWCore.ParameterSet.Config as cms

evtselanalyzer = cms.EDAnalyzer('EvtSelAnalyzer',
      gtlabel=cms.untracked.InputTag('gtDigis'),
      towerslabel=cms.untracked.InputTag('towerMaker'),
      hfEThreshold=cms.untracked.double(3.0),
      trackslabel=cms.untracked.InputTag('generalTracks'),
      qualityString=cms.untracked.string('highPurity'),
      triglabel=cms.untracked.InputTag('TriggerResults','','HLT'),
      trignames=cms.untracked.vstring('HLT_MinBiasBSC',
                                      'HLT_L1Jet6U',
                                      'HLT_Jet15U',
                                      'HLT_Jet30U',
                                      'HLT_Jet50U'),
      isGEN=cms.untracked.bool(False),
      includeSelTrk=cms.untracked.bool(False),
      etaMaxSpec=cms.untracked.double(1.0),
      ptMin=cms.untracked.double(0.5)
)
