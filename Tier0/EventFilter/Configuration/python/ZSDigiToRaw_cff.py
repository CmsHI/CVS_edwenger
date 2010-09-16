import FWCore.ParameterSet.Config as cms

## Remake RAW from ZS tracker digis
import EventFilter.SiStripRawToDigi.SiStripDigiToRaw_cfi
SiStripDigiToZSRaw = EventFilter.SiStripRawToDigi.SiStripDigiToRaw_cfi.SiStripDigiToRaw.clone(
    InputModuleLabel = 'siStripZeroSuppression',
    InputDigiLabel = cms.string('VirginRaw'),
    FedReadoutMode = cms.string('ZERO_SUPPRESSED')
    )

## Combine new ZS RAW from tracker with existing RAW for other FEDs

# (MC)
from EventFilter.RawDataCollector.rawDataCollectorByLabel_cfi import rawDataCollector
rawDataCollector.verbose = 0

# (Data)
source = rawDataCollector.clone(
    RawCollectionList = cms.VInputTag( cms.InputTag('SiStripDigiToZSRaw'),
                                       cms.InputTag('source'))
    )

## ZSDigiToRaw Sequences

# (MC)
zsDigiToRawMC = cms.Sequence( SiStripDigiToZSRaw * rawDataCollector )

# (Data)
zsDigiToRawData = cms.Sequence( SiStripDigiToZSRaw * source )
