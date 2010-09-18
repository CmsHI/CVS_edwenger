import FWCore.ParameterSet.Config as cms

#  Heavy Ions Event Content including:
#    1) common event content from standard file (EventContent_cff)
#    2) heavy-ion specific content from other subsystems (e.g. HiMixing)
#    3) heavy-ion specific reconstruction content from RecoHI

# Common Subsystems
from Configuration.EventContent.EventContent_cff import *
from Configuration.EventContent.EventContent_cff import DQMEventContent

# Heavy-Ion Specific Event Content
from SimGeneral.Configuration.SimGeneral_HiMixing_EventContent_cff import * # heavy ion signal mixing
from RecoHI.Configuration.RecoHI_EventContent_cff import *                  # heavy ion reconstruction
from EventFilter.Configuration.ZSDigiToRaw_EventContent_cff import *        # RAW content after offline ZS

#################################################################

#RECO
RECOEventContent.outputCommands.extend(RecoHIRECO.outputCommands)

#AOD
AODEventContent.outputCommands.extend(RecoHIAOD.outputCommands)

#RAWSIM, RECOSIM, AODSIM
RAWSIMEventContent.outputCommands.extend(HiMixRAW.outputCommands)

RECOSIMEventContent.outputCommands.extend(RecoHIRECO.outputCommands)
RECOSIMEventContent.outputCommands.extend(HiMixRECO.outputCommands)

AODSIMEventContent.outputCommands.extend(RecoHIAOD.outputCommands)
AODSIMEventContent.outputCommands.extend(HiMixAOD.outputCommands)

#FEVT (RAW + RECO), FEVTHLTALL (FEVT + all HLT), FEVTSIM (RAWSIM + RECOSIM)
FEVTEventContent.outputCommands.extend(RecoHIFEVT.outputCommands)

FEVTHLTALLEventContent.outputCommands.extend(RecoHIFEVT.outputCommands)

FEVTSIMEventContent.outputCommands.extend(HiMixRAW.outputCommands)
FEVTSIMEventContent.outputCommands.extend(RecoHIFEVT.outputCommands)

#add (HLT)DEBUG content to RAW, RECO, FEVT (e.g. mergedtruth from trackingParticles)
RAWDEBUGEventContent.outputCommands.extend(HiMixRAW.outputCommands)
RAWDEBUGHLTEventContent.outputCommands.extend(HiMixRAW.outputCommands)

FEVTDEBUGEventContent.outputCommands.extend(HiMixRAW.outputCommands)
FEVTDEBUGEventContent.outputCommands.extend(RecoHIFEVT.outputCommands)
FEVTDEBUGHLTEventContent.outputCommands.extend(HiMixRAW.outputCommands)
FEVTDEBUGHLTEventContent.outputCommands.extend(RecoHIFEVT.outputCommands)

RECODEBUGEventContent.outputCommands.extend(HiMixRAW.outputCommands)
RECODEBUGEventContent.outputCommands.extend(RecoHIRECO.outputCommands)

#now define ZSRAW as an extension of RAW
ZSRAWEventContent = RAWEventContent.clone()
ZSRAWEventContent.outputCommands.extend(ZSDigiToRawContent.outputCommands)

#and similarly ZSRAWSIM
ZSRAWSIMEventContent = RAWSIMEventContent.clone()
ZSRAWSIMEventContent.outputCommands.extend(ZSDigiToRawContent.outputCommands)