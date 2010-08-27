import FWCore.ParameterSet.Config as cms

process = cms.Process("SIPIXELDQM")

##----## Geometry and other global parameters:
process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
###process.load("DQM.Integration.test.FrontierCondition_GT_cfi")

##----## Reco:
#process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")

#process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
process.siPixelDigis.InputLabel = 'rawDataCollector' ## was 'source'
process.siPixelDigis.IncludeErrors = True

process.load("RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizer_cfi")

#process.load("EventFilter.SiStripRawToDigi.SiStripDigis_cfi")
process.load("EventFilter.SiStripRawToDigi.SiStripRawToDigis_standard_cff")
process.siStripDigis.ProductLabel = 'rawDataCollector' ## was 'source'

process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")


##----## Central DQM:
process.load("DQMServices.Core.DQM_cfg")
#process.DQMStore.referenceFileName = 'tmp/pixel_reference.root'
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.DQM.collectorHost = ''
process.dqmSaver.convention = 'Online'
process.dqmSaver.producer = 'DQM'
process.dqmEnv.subSystemFolder = 'Pixel'
process.dqmSaver.dirName = '.'
process.dqmSaver.saveByLumiSection = 1
process.dqmSaver.saveByRun = 1
process.dqmSaver.saveAtJobEnd = True
process.qTester = cms.EDAnalyzer("QualityTester",
    #qtList = cms.untracked.FileInPath('DQM/SiPixelMonitorClient/test/sipixel_qualitytest_config.xml'),
    qtList = cms.untracked.FileInPath('DQM/SiPixelMonitorClient/test/sipixel_tier0_qualitytest_heavyions.xml'),
    prescaleFactor = cms.untracked.int32(1),
    getQualityTestsFromFile = cms.untracked.bool(True),
    testInEventloop = cms.untracked.bool(False),
    qtestOnEndRun = cms.untracked.bool(True),
    qtestOnEndJob = cms.untracked.bool(True),
    qtestOnEndLumi = cms.untracked.bool(True),
    verboseQT = cms.untracked.bool(True)
)

##----## Pixel DQM P5/OFFLINE:
#process.load("DQM.SiPixelCommon.SiPixelOfflineDQM_source_cff")
#process.load("DQM.SiPixelCommon.SiPixelOfflineDQM_client_cff")
process.load("DQM.SiPixelCommon.SiPixelP5DQM_source_cff")
process.load("DQM.SiPixelCommon.SiPixelP5DQM_client_cff")
## the following sequences are declared therein:
## siPixelOfflineDQM_source, siPixelOfflineDQM_cosmics_source, siPixelOfflineDQM_heavyions_source, siPixelOfflineDQM_source_woTrack
## PixelOfflineDQMClient, PixelOfflineDQMClientWithDataCertification, PixelOfflineDQMClientWithDataCertificationHI
## siPixelP5DQM_source, siPixelP5DQM_cosmics_source, siPixelP5DQM_heavyions_source,siPixelP5DQM_source_woTrack
## PixelP5DQMClient, PixelP5DQMClientWithDataCertification



##----## Other stuff:
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.debugModules = ['*']  
process.MessageLogger.categories = ['HeavyIonVertexing','heavyIonHLTVertexing','MinBiasTracking']
process.MessageLogger.cerr = cms.untracked.PSet(
    threshold = cms.untracked.string('DEBUG'),
    DEBUG = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    INFO = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    HeavyIonVertexing = cms.untracked.PSet(
        limit = cms.untracked.int32(-1)
	),
    heavyIonHLTVertexing = cms.untracked.PSet(
        limit = cms.untracked.int32(-1)
    )
)
process.Timing=cms.Service("Timing")
process.AdaptorConfig = cms.Service("AdaptorConfig")
process.ModuleWebRegistry = cms.Service("ModuleWebRegistry")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

##----## Global tag and input data:
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = "GR_R_38X_V8::All"
process.GlobalTag.globaltag = "MC_38Y_V8::All"

process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_3_8_1/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V8-v1/0011/CE30528D-F3A1-DF11-9D39-001A92971ACE.root'
	    )
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

##----## Sequences and Paths:
process.LocalReco = cms.Sequence(process.siPixelDigis*process.siPixelClusters*process.siPixelRecHits)
process.TrackReco = cms.Sequence(process.siStripDigis*process.offlineBeamSpot*process.trackerlocalreco*process.heavyIonTracking) #*process.recopixelvertexing*process.ckftracks)
process.DQMmodules = cms.Sequence(process.qTester*process.dqmEnv*process.dqmSaver)


process.SiPixelHLTSource.RawInput=cms.InputTag("rawDataCollector") ### was implicitly 'source'


#put proces.dump in the path where you want to print all event content
#process.dump=cms.EDAnalyzer('EventContentAnalyzer')

process.p = cms.Path(
		     process.LocalReco*
		     process.TrackReco*
		     process.DQMmodules*
		     #process.siPixelP5DQM_source_woTrack*
		     process.siPixelP5DQM_heavyions_source*
		     process.PixelP5DQMClientWithDataCertification
		     #process.siPixelOfflineDQM_source_woTrack*
		     #process.PixelOfflineDQMClientWithDataCertification
		    )
