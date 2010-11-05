import FWCore.ParameterSet.Config as cms

hivertexanalyzer = cms.EDAnalyzer('HiVertexAnalyzer',
      vtxlabel=cms.untracked.InputTag("hiSelectedVertex"),
      nTracksBins=cms.untracked.int32(50)
)
