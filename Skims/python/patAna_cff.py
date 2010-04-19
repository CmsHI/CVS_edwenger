import FWCore.ParameterSet.Config as cms

# Build the Objects from AOD (Jets, Muons, Electrons, METs, Taus)
from PhysicsTools.PatAlgos.patSequences_cff import *

# turn off btagging related
patJets.addBTagInfo = cms.bool(False)
patJets.addDiscriminators = cms.bool(False)

# Select jets
selectedPatJets.cut = cms.string('pt > 2 & abs(eta) < 3.0')

# Define Sequence
patAnaSequence = cms.Sequence(
   makePatJets *
   selectedPatJets
)
