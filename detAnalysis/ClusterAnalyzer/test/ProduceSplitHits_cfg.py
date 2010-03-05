import FWCore.ParameterSet.Config as cms

process = cms.Process("ProduceSplitHits")
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
    #'/store/relval/CMSSW_3_5_2/RelValPyquen_DiJet_pt80to120_4TeV/GEN-SIM-RECO/MC_3XY_V21-v1/0001/FA961928-A01F-DF11-85F6-0030487CD7C0.root'
    '/store/relval/CMSSW_3_5_2/RelValHydjetQ_MinBias_4TeV/GEN-SIM-RECO/MC_3XY_V21-v1/0016/E2294157-5C1E-DF11-98B6-001731A28BE1.root'
    ))

### mixing and local reco
process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
#process.load("Configuration.StandardSequences.HiEventMixing_cff")
process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")

### cluster splitting
process.siStripSplitClusters = cms.EDProducer("SplitClustersProducer",
                                              splitBy = cms.string('byHits')) #byHits, byTracks

### output
process.out = cms.OutputModule("PoolOutputModule",
 outputCommands = cms.untracked.vstring('drop *',
  'keep *_siStripSplitClusters_*_*',
  'keep *_siStripDigis_*_*',             # Added (wtf)
  'keep *_siPixelClusters_*_*','keep *_offlineBeamSpot_*_*','keep *_pixelVertices_*_*','keep *_g4SimHits_*_*',
  'keep *_simSiPixelDigis_*_*','keep *_simSiStripDigis_*_*','keep *_mergedtruth_*_*',
  'keep *_hiSignal_*_*','keep *_hiSignalG4SimHits_*_*','keep *_generator_*_*'),
 fileName = cms.untracked.string(
    #'HYDJET_B0_DiJet80_Split.root'
    'HYDJET_MB_Split.root'
    )
)

### paths
process.rechits = cms.Sequence(process.siPixelRecHits*process.siStripMatchedRecHits)
process.p1 = cms.Path(process.mix*process.rechits*process.siStripSplitClusters)
process.e = cms.EndPath(process.out)
