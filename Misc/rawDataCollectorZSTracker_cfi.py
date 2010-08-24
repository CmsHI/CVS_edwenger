import FWCore.ParameterSet.Config as cms

rawDataCollector = cms.EDProducer("RawDataCollectorZSTracker",
    currentProcessOnly = cms.bool(True),
    preferLaterProcess = cms.bool(False),
    verbose = cms.untracked.bool(True)
)


