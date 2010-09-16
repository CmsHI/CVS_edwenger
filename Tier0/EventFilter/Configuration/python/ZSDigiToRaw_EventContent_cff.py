import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContentHeavyIons_cff import RAWEventContent,RAWSIMEventContent

OutZSRAW = cms.PSet(
    SelectEvents = cms.untracked.PSet(
      SelectEvents = cms.vstring()
    ),
    outputCommands = cms.untracked.vstring(
      'drop FEDRawDataCollection_*_*_*',
      'keep FEDRawDataCollection_source_ZSRAW_*', 
      'keep FEDRawDataCollection_rawDataCollector_ZSRAW_*',
      'keep *_*_APVCM_*')
    )

import copy
OutZSRAWData =copy.deepcopy(OutZSRAW)
OutZSRAWData.outputCommands = RAWEventContent.outputCommands + OutZSRAWData.outputCommands

OutZSRAWMC = copy.deepcopy(OutZSRAW)
OutZSRAWMC.outputCommands = RAWSIMEventContent.outputCommands + OutZSRAWMC.outputCommands
