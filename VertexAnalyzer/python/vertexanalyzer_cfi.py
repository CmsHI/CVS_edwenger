import FWCore.ParameterSet.Config as cms

vertexanalyzer = cms.EDAnalyzer('VertexAnalyzer',
      vtxlabel=cms.untracked.InputTag("pixel3Vertices"),
      nTracksBins=cms.untracked.int32(50)
)
