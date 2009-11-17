import FWCore.ParameterSet.Config as cms

fakeTrkAnalyzer = cms.EDAnalyzer('FakeTrkAnalyzer',
                                 tracks = cms.untracked.InputTag('generalTracks'),
                                 vertex = cms.untracked.InputTag('offlinePrimaryVerticesWithBS'),
                                 jets = cms.untracked.InputTag('iterativeCone5CaloJets'),
                                 muons = cms.untracked.InputTag('muons'),
                                 ptMin = cms.untracked.double(15.0)
)
