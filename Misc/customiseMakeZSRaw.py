##############################################################################
def customiseMakeZSRaw(process):

    # Secondary RAW output definition
    process.outputZSraw = cms.OutputModule("PoolOutputModule",
        splitLevel = cms.untracked.int32(0),
        outputCommands = cms.untracked.vstring('keep *_hltTriggerSummaryAOD_*_*',
                                               'keep *_TriggerResults_*_*',
                                               'keep FEDRawDataCollection_*_*_RECO'),
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
        InputModuleLabel = 'siStripDigis'
        )

    process.load("EventFilter.RawDataCollector.rawDataCollector_cfi")
    process.rawDataCollector.currentProcessOnly = False

    process.zsDigiToRaw_step = cms.Path(process.SiStripDigiToZSRaw *
                                        process.rawDataCollector)
   
    # Extend schedule with ZS RAW + secondary RAW output 
    process.schedule.extend([process.zsDigiToRaw_step,process.outZSraw_step])

    return process

