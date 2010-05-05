import FWCore.ParameterSet.Config as cms

# Track efficiency analyzer
trkEffAnalyzer = cms.EDAnalyzer('TrkEffAnalyzer',
    tracks = cms.untracked.InputTag('generalTracks'),
    label_tp_effic = cms.untracked.InputTag('mergedtruth','MergedTrackTruth'),
    label_tp_fake = cms.untracked.InputTag('mergedtruth','MergedTrackTruth'),                 
    associatormap = cms.untracked.InputTag('trackingParticleRecoTrackAsssociation'),
    vertices = cms.untracked.InputTag('offlinePrimaryVertices',''),
    beamspot = cms.untracked.InputTag('offlineBeamSpot'),                         
    fillHistograms = cms.bool(True),
    fillNtuples = cms.bool(True),
    histoFile = cms.string("hists.root"),
    ntupleFile = cms.string("trees.root")                         
)
