import FWCore.ParameterSet.Config as cms

# PF candidate analyzer
pfCandidateAnalyzer = cms.EDAnalyzer("HiPFCandidateTrackAnalyzer",
               PFCandidates = cms.InputTag("particleFlow",""),
               Vertices = cms.InputTag("hiSelectedVertex"),
               verbose = cms.untracked.bool(False), ## print candidate info
               printBlocks = cms.untracked.bool(True), ## print block/element info
               ptMin = cms.untracked.double(3.0), ## of PF candidate
               SimTracks = cms.InputTag("mergedtruth","MergedTrackTruth"),
               Tracks = cms.InputTag("hiSelectedTracks"),
               hasSimInfo = cms.untracked.bool(True),
               minHits = cms.untracked.double(5),
               maxPtErr = cms.untracked.double(0.05),
               maxD0 = cms.untracked.double(0.2),
               maxDZ = cms.untracked.double(0.2),
               maxD0Norm = cms.untracked.double(3.0),
               maxDZNorm = cms.untracked.double(3.0),
               pixelSeedOnly = cms.untracked.bool(True)
               )
