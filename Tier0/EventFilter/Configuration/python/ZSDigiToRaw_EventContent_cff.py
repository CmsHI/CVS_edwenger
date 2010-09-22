import FWCore.ParameterSet.Config as cms

ZSDigiToRawContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
      'drop FEDRawDataCollection_*_*_*',
      'keep FEDRawDataCollection_zsSource_*_RECO'
      )
    )
