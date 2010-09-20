import FWCore.ParameterSet.Config as cms

ZSDigiToRawContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
      'drop FEDRawDataCollection_*_*_*',
      'keep FEDRawDataCollection_source_*_RECO', 
      'keep FEDRawDataCollection_rawDataCollector_*_RECO'
      )
    )
