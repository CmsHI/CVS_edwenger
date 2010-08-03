import FWCore.ParameterSet.Config as cms

DefaultAlgorithms = cms.PSet(
    SiStripFedZeroSuppressionMode = cms.uint32(4),
    CommonModeNoiseSubtractionMode = cms.string('Median') ##Supported modes: Median, Percentile, TT6, FastLinear
    #CutToAvoidSignal = cms.double(3.0), ## for TT6
    #Percentile = cms.double(25.0),      ## for Percentile
    )