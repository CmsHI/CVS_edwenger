import FWCore.ParameterSet.Config as cms

# Virgin Raw + ZS SiStripDigis --> ZS Raw sequence
from EventFilter.Configuration.ZSDigiToRaw_cff import *

# Modify input for MC
zsSource.RawCollectionList = cms.VInputTag( cms.InputTag('SiStripDigiToZSRaw'),
                                            cms.InputTag('rawDataCollector'))

ZSDigiToRaw = cms.Sequence(zsDigiToRaw)
