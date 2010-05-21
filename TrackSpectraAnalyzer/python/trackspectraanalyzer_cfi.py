import FWCore.ParameterSet.Config as cms

trackAna = cms.EDAnalyzer('TrackSpectraAnalyzer',
        src = cms.untracked.InputTag("generalTracks"),
        vsrc = cms.untracked.InputTag("offlinePrimaryVertices"),
        jsrc = cms.untracked.InputTag("selectedPatJets"),
        doOutput = cms.untracked.bool(True),
        isGEN = cms.untracked.bool(False),
        doJet = cms.untracked.bool(True),                          
        histOnly = cms.untracked.bool(False),
        etaMax = cms.untracked.double(2.5),
        hltNames = cms.untracked.vstring('HLT_MinBiasBSC',
                                         'HLT_L1Jet6U',
                                         'HLT_Jet15U',
                                         'HLT_Jet30U',
                                         'HLT_Jet50U'),
        triglabel = cms.untracked.InputTag('TriggerResults','','HLT')
)

trackAna_mc = trackAna.clone(
  isGEN = cms.untracked.bool(True)
)

trackAna_mc_genjet = trackAna_mc.clone(
  jsrc = cms.untracked.InputTag("ak5GenJets"),
)
