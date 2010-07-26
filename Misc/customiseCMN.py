import FWCore.ParameterSet.Config as cms

def customise(process):
    process.siStripZeroSuppression.Algorithms.CommonModeNoiseSubtractionMode=cms.string("Percentile")
    process.siStripZeroSuppression.Algorithms.Percentile=cms.double(0.25)
