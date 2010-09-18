import FWCore.ParameterSet.Config as cms

# Virgin Raw + ZS SiStripDigis --> ZS Raw sequence
from EventFilter.Configuration.ZSDigiToRaw_cff import *

ZSDigiToRaw = cms.Sequence(zsDigiToRawData)
