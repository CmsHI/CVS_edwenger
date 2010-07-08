import FWCore.ParameterSet.Config as cms

# Track efficiency analyzer
hitrkEffAnalyzer = cms.EDAnalyzer('HiTrkEffAnalyzer',
    tracks = cms.untracked.InputTag('hiSelectedTracks'),
    jets = cms.untracked.InputTag('patJets'),
    label_tp_effic = cms.untracked.InputTag('mergedtruth','MergedTrackTruth'),
    label_tp_fake = cms.untracked.InputTag('mergedtruth','MergedTrackTruth'),                 
    associatormap = cms.untracked.InputTag('trackingParticleRecoTrackAsssociation'),
    vertices = cms.untracked.InputTag('hiSelectedVertex',''),
    beamspot = cms.untracked.InputTag('offlineBeamSpot'),                         
    fillHistograms = cms.bool(True),
    fillNtuples = cms.bool(False),
    constPtBins = cms.bool(False),
    lowPtMode = cms.bool(False),
    doAssociation = cms.untracked.bool(True),
    hasSimInfo = cms.untracked.bool(False),
    pixelMultMode =  cms.untracked.bool(False)
)
