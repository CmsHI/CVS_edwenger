import FWCore.ParameterSet.Config as cms

##############################################################################

def customiseDataMixerOutput(process):
    process.DATAMIXERoutput.outputCommands.extend(
        ['keep *_hiSelectedVertex_*_*',
         'keep *_siStripDigis_VirginRaw_*'])
    return process

##############################################################################

def customiseCloneGenerator(process):
    process.generator = process.hiSignal.clone(embeddingMode=cms.bool(False))
    process.ProductionFilterSequence.replace(process.hiSignal,process.generator)
    return process

def customiseMatchRecVertex(process):
    process.load("SimGeneral.MixingModule.MatchRecVtx_cfi")
    process.matchRecVtx.heavyIonLabel = "hiSelectedVertex"
    process.pgen.replace(process.VertexSmearing,process.matchRecVtx)
    process.mixData.input.sequential = True
    return process

def customiseSiStripRawDigi(process):
    process.mixData.SiStripRawDigiSource = "PILEUP" # use raw digis from "PILEUP" or "SIGNAL"
    process.SiStripDigiToRaw.FedReadoutMode = cms.string('VIRGIN_RAW') # pack rawdigis as VR

    process.stripConditions = cms.ESSource("PoolDBESSource",
        process.CondDBSetup,
        timetype = cms.untracked.string('runnumber'),
        connect = cms.string('frontier://FrontierProd/CMS_COND_31X_STRIP'),
        toGet = cms.VPSet(cms.PSet(
          record = cms.string('SiStripFedCablingRcd'),
          tag = cms.string('SiStripFedCabling_GR10_v1_hlt')))
    )
    process.es_prefer_strips = cms.ESPrefer("PoolDBESSource","stripConditions")
    return process

def customiseCloneMatchRaw(process):
    customiseCloneGenerator(process)
    customiseMatchRecVertex(process)
    customiseSiStripRawDigi(process)
    return process

def customiseSiStripRawSignal(process):
    customiseSiStripRawDigi(process)
    process.mixData.SiStripRawDigiSource = "SIGNAL"
    process.mixData.ZDCdigiCollectionSig = ""
    process.mixData.ZDCPileInputTag = ""
    return process

##############################################################################

def customiseBeamSpot(process):
    process.GlobalTag.toGet = cms.VPSet(
        cms.PSet(record = cms.string("BeamSpotObjectsRcd"),
                 tag = cms.string("Realistic2.76ATeVCollisions_STARTUP_v0_mc"),
                 connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_BEAMSPOT")
                 )
        )
    return process

def customiseSiStripConditions(process):
    process.stripConditions = cms.ESSource("PoolDBESSource",
    process.CondDBSetup,
    timetype = cms.untracked.string('runnumber'),
    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_STRIP'),
    toGet = cms.VPSet(
        cms.PSet(
        record = cms.string('SiStripNoisesRcd'),
        tag = cms.string('SiStripNoise_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripPedestalsRcd'),
        tag = cms.string('SiStripPedestals_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripFedCablingRcd'),
        tag = cms.string('SiStripFedCabling_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripBadChannelRcd'),
        tag = cms.string('SiStripBadChannel_FromOnline_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripLatencyRcd'),
        tag = cms.string('SiStripLatency_GR10_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripThresholdRcd'),
        tag = cms.string('SiStripThreshold_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripBadFiberRcd'),
        tag = cms.string('SiStripBadChannel_FromOfflineCalibration_GR10_v2_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripBadModuleRcd'),
        tag = cms.string('SiStripBadChannel_FromEfficiencyAnalysis_GR10_v1_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripConfObjectRcd'),
        tag = cms.string('SiStripShiftAndCrosstalk_GR10_v1_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripLorentzAngleRcd'),
        tag = cms.string('SiStripLorentzAngle_GR10_v1_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripApvGain2Rcd'),
        tag = cms.string('SiStripApvGain_FromParticles_GR10_v2_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripApvGainRcd'),
        tag = cms.string('SiStripApvGain_GR10_v1_hlt')
        )
        )
    )
    
    process.es_prefer_strips = cms.ESPrefer("PoolDBESSource","stripConditions")
    return process

def customiseDummyVtx(process):
    # Dummy GEN -> RECO vertex
    process.hiSelectedVertex = cms.EDProducer("GenToRecoVtxProducer",
        signalLabel=cms.InputTag("generator"))

def customiseBeamRaw(process):
    customiseBeamSpot(process)
    customiseSiStripConditions(process)
    customiseDummyVtx(process)
    return process
