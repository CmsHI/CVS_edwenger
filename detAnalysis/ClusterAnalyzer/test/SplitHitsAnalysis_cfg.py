import FWCore.ParameterSet.Config as cms

process = cms.Process("AnalyzeSplitClusters")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Services_cff")

#process.Timing = cms.Service("Timing")
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck")

### conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'MC_3XY_V21::All'

### input source
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    #'file:HYDJET_B0_DiJet80_Split.root'
    'file:HYDJET_MB_Split.root'
    ))

### mixing and full track reconstruction
process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
#process.load("Configuration.StandardSequences.HiEventMixing_cff")
process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")

### use split clusters in making rechits and ckf trajectories
process.siStripMatchedRecHits.ClusterProducer = 'siStripSplitClusters'
from RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi import *
MeasurementTracker.stripClusterProducer = 'siStripSplitClusters'

### cluster analyzer
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.myClusterAnalysis = cms.EDFilter("ClusterAnalyzer",
 outputFilename = cms.untracked.string('SplitHitsAnalysisMB.root'),
 associatePixel = cms.bool(False),
 ROUList = cms.vstring('g4SimHitsTrackerHitsTIBLowTof', 
  'g4SimHitsTrackerHitsTIBHighTof', 
  'g4SimHitsTrackerHitsTIDLowTof', 
  'g4SimHitsTrackerHitsTIDHighTof', 
  'g4SimHitsTrackerHitsTOBLowTof', 
  'g4SimHitsTrackerHitsTOBHighTof', 
  'g4SimHitsTrackerHitsTECLowTof', 
  'g4SimHitsTrackerHitsTECHighTof'),
 trackTrackInfoAssocLabel = cms.InputTag("trackinfoCTF"),
 trackSourceLabel = cms.InputTag("hiSelectedTracks"),
 modulesToBeExcluded = cms.vuint32(369199109, 419561989, 419627960),
 clusterSourceLabel = cms.InputTag("siStripSplitClusters"),
 associateRecoTracks = cms.bool(False),
 SecondMinDistanceRecHitSimHit = cms.double(1000.0),
 associateStrip = cms.bool(True),
 ClusterConditions = cms.PSet(
  minWidth = cms.double(0.0),
  On = cms.bool(True),
  maxStoN = cms.double(2000.0),
  minStoN = cms.double(0.0),
  maxWidth = cms.double(200.0)
 ),
 TrackThresholds = cms.PSet(
  minRecHit = cms.double(5.0),
  maxChi2 = cms.double(100.0)
 ),
 ShowMechanicalStructureView = cms.bool(True)
)

### output
process.out = cms.OutputModule("PoolOutputModule",
 outputCommands = cms.untracked.vstring('keep *_*_*_*'),
 fileName = cms.untracked.string(
    #'HYDJET_B0_DiJet80_Split_Validator.root'
    'HYDJET_MB_Split_Validator.root'
    )
)

### paths
process.rechits = cms.Sequence(process.siPixelRecHits*process.siStripMatchedRecHits)
process.p1 = cms.Path(process.mix
                      *process.rechits
                      *process.offlineBeamSpot
                      *process.heavyIonTracking
                      *process.myClusterAnalysis
                      )
process.e = cms.EndPath(process.out)
