import FWCore.ParameterSet.Config as cms

trackAna = cms.EDAnalyzer('TrackSpectraAnalyzer',
        src = cms.untracked.InputTag("selectTracks"),
        vsrc = cms.untracked.InputTag("offlinePrimaryVertices"),
        jsrc = cms.untracked.InputTag("selectedPatJets"),
        doOutput = cms.untracked.bool(True),
        isGEN = cms.untracked.bool(False),
        doJet = cms.untracked.bool(True),
        etaMax = cms.untracked.double(3.0)
)
