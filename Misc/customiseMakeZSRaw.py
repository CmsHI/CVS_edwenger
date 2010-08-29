import FWCore.ParameterSet.Config as cms

##############################################################################
def customiseMakeZSRaw(process):

    # Secondary RAW output definition
    process.outputZSraw = cms.OutputModule("PoolOutputModule",
        splitLevel = cms.untracked.int32(0),
        outputCommands = cms.untracked.vstring('keep *_hltTriggerSummaryAOD_*_*',
                                               'keep *_TriggerResults_*_*',
                                               'keep *_*_APVCM_*',
                                               'keep FEDRawDataCollection_rawDataCollector_*_RECO'),
        fileName = cms.untracked.string('output_ZS_RAW.root'),
        dataset = cms.untracked.PSet(
            dataTier = cms.untracked.string('RAW'),
            filterName = cms.untracked.string('')
        )
    )

    process.outZSraw_step = cms.EndPath(process.outputZSraw)

    # Remake RAW from ZS tracker digis
    process.load("EventFilter.SiStripRawToDigi.SiStripDigiToRaw_cfi")
    process.SiStripDigiToZSRaw=process.SiStripDigiToRaw.clone(
        InputModuleLabel = 'siStripZeroSuppression',
        InputDigiLabel = cms.string('VirginRaw'),
        FedReadoutMode = cms.string('ZERO_SUPPRESSED')
        )

    # Combine new ZS RAW from tracker with existing RAW for other FEDs
    #process.load("EventFilter.RawDataCollector.rawDataCollectorZSTracker_cfi")
    process.load("EventFilter.RawDataCollector.rawDataCollectorByLabel_cfi")
    process.rawDataCollector.verbose = 1

    process.zsDigiToRaw_step = cms.Path(process.SiStripDigiToZSRaw *
                                        process.rawDataCollector)
   
    # Extend schedule with ZS RAW + secondary RAW output 
    process.schedule.extend([process.zsDigiToRaw_step,process.outZSraw_step])

    return process

