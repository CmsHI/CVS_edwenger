import FWCore.ParameterSet.Config as cms

from edwenger.Misc.customiseStripDB import *

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
