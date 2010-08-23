import FWCore.ParameterSet.Config as cms

from edwenger.Misc.customiseMakeZSRaw import *
from edwenger.Misc.customiseStripDB import *
from RecoLocalTracker.SiStripZeroSuppression.customiseCMN import *

##############################################################################
def useIteratedMedianAlgo(process):

    process = customiseStripDB(process)
    process = customiseMakeZSRaw(process)
    process = customiseIteratedMedian(process)

    process.outputZSraw.fileName = cms.untracked.string('dijet80_ZS_RAW.root')

    return process

##############################################################################
def usePercentileAlgo(process):

    process = customiseStripDB(process)
    process = customiseMakeZSRaw(process)
    process = customisePercentile(process)

    process.outputZSraw.fileName = cms.untracked.string('dijet80_ZS_RAW.root')

    return process

