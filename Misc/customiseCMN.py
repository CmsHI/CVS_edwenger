import FWCore.ParameterSet.Config as cms

##############################################################################
def customiseIteratedMedian(process):

    process = customiseStripDB(process)
    
    process.siStripZeroSuppression.Algorithms.CommonModeNoiseSubtractionMode=cms.string("IteratedMedian")
    process.siStripZeroSuppression.Algorithms.CutToAvoidSignal = cms.double(2.0)
    process.siStripZeroSuppression.Algorithms.Iterations = cms.int32(3)
    process.siStripZeroSuppression.storeCM = cms.bool(True)

    return process

##############################################################################
def customisePercentile(process):

    process = customiseStripDB(process)
    
    process.siStripZeroSuppression.Algorithms.CommonModeNoiseSubtractionMode=cms.string("Percentile")
    process.siStripZeroSuppression.Algorithms.Percentile=cms.double(0.25)
    process.siStripZeroSuppression.storeCM = cms.bool(True)

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
