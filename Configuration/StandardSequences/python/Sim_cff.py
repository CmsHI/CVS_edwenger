import FWCore.ParameterSet.Config as cms

# Hector transport of particles along the beam pipe for very forward detectors
# from SimTransport.HectorProducer.HectorTransportZDC_cfi import *

# CMSSW/Geant4 interface
from SimG4Core.Configuration.SimG4Core_cff import *

#psim = cms.Sequence(cms.SequencePlaceholder("randomEngineStateProducer")*LHCTransport*g4SimHits)
psim = cms.Sequence(cms.SequencePlaceholder("randomEngineStateProducer")*g4SimHits)


