import FWCore.ParameterSet.Config as cms

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

    # digitize in Virgin Raw
    process.simSiStripDigis.ZeroSuppression = cms.bool(False)
    process.SiStripDigiToRaw.InputDigiLabel = cms.string('VirginRaw')
    process.SiStripDigiToRaw.FedReadoutMode = cms.string('VIRGIN_RAW')
    process.simSiStripDigis.TrackerConfigurationFromDB = cms.bool(True)

    # turn silicon effects off unless otherwise specified later
    process.simSiStripDigis.BaselineShift = cms.bool(False)
    process.simSiStripDigis.APVSaturationFromHIP = cms.bool(False)
    process.simSiStripDigis.CommonModeNoise = cms.bool(False)

    # only do redigi of strip tracker
    process.pdigi.remove(process.addPileupInfo)
    process.pdigi.remove(process.trackingParticles)
    process.pdigi.replace(process.doAllDigi,process.simSiStripDigis)

    # drop existing tracker digis and raw data
    process.source.inputCommands = cms.untracked.vstring('keep *',
                                                         'drop FEDRawDataCollection_*_*_*',
                                                         'drop *_simSiStripDigis_*_*')
    process.source.dropDescendantsOfDroppedBranches=cms.untracked.bool(False)

    return process


##############################################################################
def customiseStripCMNOnly(process):
    process = customiseStripCommon(process)
    process.simSiStripDigis.CommonModeNoise = cms.bool(True)

    return process


##############################################################################
def customiseStripHIPOnly(process):
    process = customiseStripCommon(process)
    process.simSiStripDigis.APVSaturationFromHIP = cms.bool(True)

    return process


##############################################################################
def customiseStripBSOnly(process):
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
