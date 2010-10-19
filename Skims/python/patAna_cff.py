import FWCore.ParameterSet.Config as cms

# Build the Objects from AOD (Jets, Muons, Electrons, METs, Taus)
from PhysicsTools.PatAlgos.patSequences_cff import *

# turn off btagging related
patJets.addBTagInfo = cms.bool(False)
patJets.addDiscriminators = cms.bool(False)

# Select jets
selectedPatJets.cut = cms.string('pt > 10')

# clean jets
cleanedPatJets = selectedPatJets.clone(
  src = cms.InputTag("selectedPatJets"),
  cut = cms.string('jetID().n90Hits>1 && jetID().fHPD<0.98 && emEnergyFraction>0.01')
  )

# Define Sequence
patAnaSequence = cms.Sequence(
   makePatJets *
   selectedPatJets
)
