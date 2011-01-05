import FWCore.ParameterSet.Config as cms

vertexanalyzer = cms.EDAnalyzer('VertexAnalyzer',
      vtxlabel=cms.untracked.InputTag("pixel3Vertices"),
      trklabel=cms.untracked.InputTag("selectTracks"),
      jetlabel=cms.untracked.InputTag("cleanedPatJets"),
      nTracksBins=cms.untracked.int32(50),
      jetTrkVerticesCorr=cms.untracked.bool(False)
)
