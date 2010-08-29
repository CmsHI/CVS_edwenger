import FWCore.ParameterSet.Config as cms

process = cms.Process("SiStrpDQMLive")

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

#----------------------------
# Event Source
#-----------------------------

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_3_8_1/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V8-v1/0011/CE30528D-F3A1-DF11-9D39-001A92971ACE.root'
	    )
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#----------------------------
# DQM Environment
#-----------------------------
process.load("DQMServices.Core.DQM_cfg")
#process.DQMStore.referenceFileName = '/dqmdata/dqm/reference/sistrip_reference.root'
#process.DQM.filter = '^(SiStrip|Tracking)(/[^/]+){0,5}$'

process.load("DQMServices.Components.DQMEnvironment_cfi")

#----------------------------
# DQM Live Environment
#-----------------------------
process.load("DQM.Integration.test.environment_cfi")
process.dqmEnv.subSystemFolder    = "SiStrip"
process.dqmSaver.producer = "Playback"
process.dqmSaver.saveByTime = 16
process.dqmSaver.saveByMinute = 16
process.dqmSaver.dirName = '.'
process.dqmSaver.saveByRun         = cms.untracked.int32(1)

process.dqmEnvTr = cms.EDAnalyzer("DQMEventInfo",
                 subSystemFolder = cms.untracked.string('Tracking'),
                 eventRateWindow = cms.untracked.double(0.5),
                 eventInfoFolder = cms.untracked.string('EventInfo')
)

#-----------------------------
# Magnetic Field
#-----------------------------
# 0T field
#process.load("Configuration.StandardSequences.MagneticField_0T_cff")
# 3.8T field
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
# 4.0T field
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
#process.prefer("VolumeBasedMagneticFieldESProducer")

#process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

#-------------------------------------------------
# GEOMETRY
#-------------------------------------------------
process.load("Configuration.StandardSequences.Geometry_cff")

#--------------------------
# Calibration
#--------------------------
#process.load("DQM.Integration.test.FrontierCondition_GT_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "MC_38Y_V8::All"
#--------------------------------------------
## Patch to avoid using Run Info information in reconstruction
#
process.siStripQualityESProducer.ListOfRecordToMerge = cms.VPSet(
   cms.PSet( record = cms.string("SiStripDetVOffRcd"),    tag    = cms.string("") ),
   cms.PSet( record = cms.string("SiStripDetCablingRcd"), tag    = cms.string("") ),
#  cms.PSet( record = cms.string("RunInfoRcd"),           tag    = cms.string("") ),
   cms.PSet( record = cms.string("SiStripBadChannelRcd"), tag    = cms.string("") ),
   cms.PSet( record = cms.string("SiStripBadFiberRcd"),   tag    = cms.string("") ),
   cms.PSet( record = cms.string("SiStripBadModuleRcd"),  tag    = cms.string("") )
   )
#-------------------------------------------
                                                                                           
#-----------------------
#  Reconstruction Modules
#-----------------------
## Collision Reconstruction
#process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')

# offline beam spot
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")

#--------------------------
# Strip DQM Source and Client
#--------------------------
process.load("DQM.SiStripMonitorClient.SiStripSourceConfigP5_cff")
process.TrackMon_hi.doLumiAnalysis = False

process.load("DQM.SiStripMonitorClient.SiStripClientConfigP5_cff")
process.SiStripAnalyser.TkMapCreationFrequency  = -1
process.SiStripAnalyser.ShiftReportFrequency = -1
process.SiStripAnalyser.StaticUpdateFrequency = 5

process.siStripFEDMonitor.RawDataTag='rawDataCollector'

#--------------------------
# Quality Test
#--------------------------
process.qTester = cms.EDAnalyzer("QualityTester",
    qtList = cms.untracked.FileInPath('DQM/SiStripMonitorClient/data/sistrip_qualitytest_config_tier0_heavyions.xml'),
    prescaleFactor = cms.untracked.int32(5),                               
    getQualityTestsFromFile = cms.untracked.bool(True),
    qtestOnEndLumi = cms.untracked.bool(True),
    qtestOnEndRun = cms.untracked.bool(True)
)

#--------------------------
# Web Service
#--------------------------
process.ModuleWebRegistry = cms.Service("ModuleWebRegistry")

process.AdaptorConfig = cms.Service("AdaptorConfig")

#--------------------------
# Producers
#--------------------------
# Event History Producer
process.load("DPGAnalysis.SiStripTools.eventwithhistoryproducerfroml1abc_cfi")

# APV Phase Producer
process.load("DPGAnalysis.SiStripTools.apvcyclephaseproducerfroml1abc_GR09_cfi")

#--------------------------
# Scheduling
#--------------------------
process.SiStripSources_LocalReco = cms.Sequence(process.siStripFEDMonitor*process.SiStripMonitorDigi*process.SiStripMonitorClusterReal)
process.SiStripSources_TrkReco   = cms.Sequence(process.SiStripMonitorTrack_hi*process.MonitorTrackResiduals_hi*process.TrackMon_hi)
process.SiStripClients           = cms.Sequence(process.SiStripAnalyser)
process.DQMCommon                = cms.Sequence(process.qTester*process.dqmEnv*process.dqmEnvTr*process.dqmSaver)
process.RecoForDQM_LocalReco     = cms.Sequence(process.siPixelDigis*process.siStripDigis*process.gtDigis*process.trackerlocalreco)
process.RecoForDQM_TrkReco       = cms.Sequence(process.offlineBeamSpot*process.heavyIonTracking)

process.p = cms.Path(process.APVPhases*
                     process.consecutiveHEs*
                     process.RecoForDQM_LocalReco*
                     process.DQMCommon*
                     process.SiStripClients*
                     process.SiStripSources_LocalReco*
                     process.RecoForDQM_TrkReco*
                     process.SiStripSources_TrkReco
)

