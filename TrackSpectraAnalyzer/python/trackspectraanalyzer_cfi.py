import FWCore.ParameterSet.Config as cms

trackAna = cms.EDAnalyzer('TrackSpectraAnalyzer',
        src = cms.untracked.InputTag("selectTracks"),
        vsrc = cms.untracked.InputTag("offlinePrimaryVertices"),
        jsrc = cms.untracked.InputTag("iterativeCone5CaloJets"),
        doOutput = cms.untracked.bool(True),
        isGEN = cms.untracked.bool(False),
        doJet = cms.untracked.bool(True),
        etaMax = cms.untracked.double(3.0),
        nVtxTrkCut = cms.untracked.int32(3)
)
