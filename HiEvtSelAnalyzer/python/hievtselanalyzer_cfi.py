import FWCore.ParameterSet.Config as cms

hievtselanalyzer = cms.EDAnalyzer('HiEvtSelAnalyzer',
       gtlabel=cms.untracked.InputTag('gtDigis'),
       triglabel=cms.untracked.InputTag('TriggerResults','','HLT'),
       trklabel=cms.untracked.InputTag('hiSelectedTracks'),
       gtrklabel=cms.untracked.InputTag('hiGenParticles'),
       isGEN=cms.untracked.bool(False),
       neededCentBins = cms.untracked.vint32(0,1,3,11,19,39), # 0~5% (0,1), 5~10% (2,3), 10~30% (4,11) etc..
       trignames=cms.untracked.vstring('HLT_HIMinBiasHF',
                                       'HLT_HIJet35U',
                                       'HLT_HIJet50U',
                                       'HLT_HIJet75U',
                                       'HLT_HIJet90U')
)
