import FWCore.ParameterSet.Config as cms

from edwenger.Misc.customiseStripDB import *
from RecoLocalTracker.SiStripZeroSuppression.customiseCMN import *

##############################################################################
def useIteratedMedianAlgo(process):

    process = customiseStripDB(process)
    process = customiseIteratedMedian(process)
    return process

##############################################################################
def usePercentileAlgo(process):

    process = customiseStripDB(process)
    process = customisePercentile(process)
    return process

