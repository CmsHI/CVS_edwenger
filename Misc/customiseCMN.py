import FWCore.ParameterSet.Config as cms

from edwenger.Misc.customiseMakeZSRaw import *
from edwenger.Misc.customiseStripDB import *

##############################################################################
def customiseIteratedMedian(process):

    process = customiseStripDB(process)
    process = customiseMakeZSRaw(process)
    
    process.siStripZeroSuppression.Algorithms.CommonModeNoiseSubtractionMode=cms.string("IteratedMedian")
    process.siStripZeroSuppression.Algorithms.CutToAvoidSignal = cms.double(2.0)
    process.siStripZeroSuppression.Algorithms.Iterations = cms.int32(3)
    process.siStripZeroSuppression.storeCM = cms.bool(True)

    return process

##############################################################################
def customisePercentile(process):

    process = customiseStripDB(process)
    process = customiseMakeZSRaw(process)

    process.siStripZeroSuppression.Algorithms.CommonModeNoiseSubtractionMode=cms.string("Percentile")
    process.siStripZeroSuppression.Algorithms.Percentile=cms.double(25.0)
    process.siStripZeroSuppression.storeCM = cms.bool(True)

    return process

