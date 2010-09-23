import FWCore.ParameterSet.Config as cms

RepackRAW = cms.PSet(
    outputCommands = cms.untracked.vstring(
      'drop FEDRawDataCollection_*_*_*',
      'keep FEDRawDataCollection_rawDataRepacker_*_*'
      )
    )
