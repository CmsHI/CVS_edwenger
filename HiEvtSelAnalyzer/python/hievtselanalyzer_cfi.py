import FWCore.ParameterSet.Config as cms

hievtselanalyzer = cms.EDAnalyzer('HiEvtSelAnalyzer',
       gtlabel=cms.untracked.InputTag('gtDigis'),
       triglabel=cms.untracked.InputTag('TriggerResults','','HLT'),
       trklabel=cms.untracked.InputTag('hiSelectedTracks'),
       gtrklabel=cms.untracked.InputTag('hiGenParticles'),
       isGEN=cms.untracked.bool(False),
       ptMin=cms.untracked.double(0.9),
       neededCentBins = cms.untracked.vint32(0,1,3,11,19,39), # 0~5% (0,1), 5~10% (2,3), 10~30% (4,11) etc..
       trignames=cms.untracked.vstring() # no triggers
)
