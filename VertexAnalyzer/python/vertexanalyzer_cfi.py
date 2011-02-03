import FWCore.ParameterSet.Config as cms

vertexanalyzer = cms.EDAnalyzer('VertexAnalyzer',
      vtxlabel=cms.untracked.InputTag("pixel3Vertices"),
      trklabel=cms.untracked.InputTag("selectTracks"),
      jetlabel=cms.untracked.InputTag("cleanedPatJets"),
      dzcut=cms.untracked.double(0.9), # |dz|<dz_cut 
      minJetEt=cms.untracked.double(0.0),
      nTracksBins=cms.untracked.int32(50),
      jetTrkVerticesCorr=cms.untracked.bool(False)
)
