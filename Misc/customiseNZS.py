import FWCore.ParameterSet.Config as cms

##############################################################################
def customiseHcalNZS(process):
    process.hcalRawData.HBHE = cms.untracked.InputTag("simHcalUnsuppressedDigis")
    process.hcalRawData.HF = cms.untracked.InputTag("simHcalUnsuppressedDigis")
    process.hcalRawData.HO = cms.untracked.InputTag("simHcalUnsuppressedDigis")
    process.hcalRawData.ZDC = cms.untracked.InputTag("simHcalUnsuppressedDigis")

    return process


##############################################################################
def customiseEcalNZS(process):
    process.simEcalDigis.srpBarrelLowInterestChannelZS = cms.double(-1.e9)
    process.simEcalDigis.srpEndcapLowInterestChannelZS = cms.double(-1.e9)

    return process

##############################################################################
def customiseStripDB(process):
    process.poolDBESSource = cms.ESSource("PoolDBESSource",
                                          BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
                                          DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(2),
        authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
        ),
                                          timetype = cms.untracked.string('runnumber'),
                                          connect = cms.string('frontier://FrontierProd/CMS_COND_31X_STRIP'),
                                          toGet = cms.VPSet(
        cms.PSet(
        record = cms.string('SiStripNoisesRcd'),
        tag = cms.string('SiStripNoise_CRAFT09_DecMode_ForTrackerSim')
        ),
        cms.PSet(
        record = cms.string('SiStripPedestalsRcd'),
        tag = cms.string('SiStripPedestal_CRAFT09_DecMode_ForTrackerSim')
        ),
        cms.PSet(
        record = cms.string('SiStripFedCablingRcd'),
        tag = cms.string('SiStripFedCabling_CRAFT09_ForTrackerSim')
        ),
        cms.PSet(
        record = cms.string('SiStripBadChannelRcd'),
        tag = cms.string('SiStripBadChannelsFromO2O_CRAFT09_DecMode_ForTrackerSim')
        )
        )
                                          )
    
    process.es_prefer_my =cms.ESPrefer("PoolDBESSource","poolDBESSource")

    return process


##############################################################################
def customiseStripCommon(process):

    process = customiseStripDB(process)
    
    process.simSiStripDigis.ZeroSuppression = cms.bool(False)
    process.SiStripDigiToRaw.InputDigiLabel = cms.string('VirginRaw')
    process.SiStripDigiToRaw.FedReadoutMode = cms.string('VIRGIN_RAW')
    
    process.simSiStripDigis.Noise = cms.bool(True)
    process.simSiStripDigis.TrackerConfigurationFromDB = cms.bool(True)

    process.pdigi.remove(process.addPileupInfo)
    process.pdigi.remove(process.trackingParticles)
    process.pdigi.replace(process.doAllDigi,process.simSiStripDigis)

    return process


##############################################################################
def customiseStripCMN(process):
    process = customiseStripCommon(process)
    process.simSiStripDigis.CommonModeNoise = cms.bool(True)

    return process


##############################################################################
def customiseStripHIP(process):
    process = customiseStripCommon(process)
    process.simSiStripDigis.APVSaturationFromHIP = cms.bool(True)

    return process


##############################################################################
def customiseStripBaselineShift(process):
    process = customiseStripCommon(process)
    process.simSiStripDigis.BaselineShift = cms.bool(True)

    return process


##############################################################################
def customiseStripAll(process):
    process = customiseStripCommon(process)
    process.simSiStripDigis.BaselineShift = cms.bool(True)
    process.simSiStripDigis.APVSaturationFromHIP = cms.bool(True)
    process.simSiStripDigis.CommonModeNoise = cms.bool(True)

    return process
