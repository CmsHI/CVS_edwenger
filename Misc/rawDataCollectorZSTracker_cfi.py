import FWCore.ParameterSet.Config as cms

rawDataCollector = cms.EDProducer("RawDataCollectorZSTracker",
    currentProcessOnly = cms.bool(False),
    preferLaterProcess = cms.bool(True),
    verbose = cms.untracked.int32(1)     # 0 = quiet, 1 = collection list, 2 = FED list
)


