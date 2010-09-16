import FWCore.ParameterSet.Config as cms

# ZSDIGI2RAW sequences and output commands
from EventFilter.Configuration.ZSDigiToRaw_cff import *
from EventFilter.Configuration.ZSDigiToRaw_EventContent_cff import *

# ZSDIGI2RAW path definition
zsDigiToRaw_step = cms.Path(zsDigiToRawMC)

# ZSDIGI2RAW stream definition
ZSRAWStreamZSDigiToRaw = cms.FilteredStream(
	responsible = 'Edward Wenger',
	name = 'ZSDigiToRaw',
	paths  = (zsDigiToRaw_step),
	content = OutZSRAWMC.outputCommands,
	selectEvents = OutZSRAWMC.SelectEvents,
	dataTier = cms.untracked.string('ZSRAW')
	)
