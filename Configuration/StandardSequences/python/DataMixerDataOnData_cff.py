import FWCore.ParameterSet.Config as cms

# Start with Standard Digitization:

from SimCalorimetry.Configuration.SimCalorimetry_cff import *

from SimGeneral.DataMixingModule.mixOne_data_on_data_cfi import *

# Run after the DataMixer only.
#
# Calorimetry Digis (Ecal + Hcal) - * unsuppressed *
# 
#
# clone these sequences:

DMEcalTriggerPrimitiveDigis = simEcalTriggerPrimitiveDigis.clone()
DMEcalDigis = simEcalDigis.clone()

# Re-define inputs to point at DataMixer output
DMEcalTriggerPrimitiveDigis.Label = cms.string('mixData')
DMEcalTriggerPrimitiveDigis.InstanceEB = cms.string('EBDigiCollectionDM')
DMEcalTriggerPrimitiveDigis.InstanceEE = cms.string('EEDigiCollectionDM')
#
DMEcalDigis.digiProducer = cms.string('mixData')
DMEcalDigis.EBdigiCollection = cms.string('EBDigiCollectionDM')
DMEcalDigis.EEdigiCollection = cms.string('EEDigiCollectionDM')

ecalDigiSequenceDM = cms.Sequence(DMEcalTriggerPrimitiveDigis*DMEcalDigis)

# same for Hcal:

# clone these sequences:

DMHcalTriggerPrimitiveDigis = simHcalTriggerPrimitiveDigis.clone()
DMHcalDigis = simHcalDigis.clone()

# Re-define inputs to point at DataMixer output
DMHcalTriggerPrimitiveDigis.inputLabel = cms.VInputTag(cms.InputTag('mixData'),cms.InputTag('mixData'))
DMHcalDigis.digiLabel = cms.InputTag("mixData")

hcalDigiSequenceDM = cms.Sequence(DMHcalTriggerPrimitiveDigis+DMHcalDigis)

postDMDigi = cms.Sequence(ecalDigiSequenceDM+hcalDigiSequenceDM)

pdatamix = cms.Sequence(mixData+postDMDigi)

