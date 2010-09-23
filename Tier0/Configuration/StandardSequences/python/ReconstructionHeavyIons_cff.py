import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------
# LOCAL RECO

# Tracker
from RecoVertex.BeamSpotProducer.BeamSpot_cfi import *
from RecoLocalTracker.Configuration.RecoLocalTracker_cff import *

# Ecal
from RecoLocalCalo.Configuration.ecalLocalRecoSequence_cff import *

# Hcal
from RecoLocalCalo.Configuration.hcalLocalReco_cff import *
from RecoLocalCalo.Configuration.hcalLocalRecoNZS_cff import *

# Muons
from RecoLocalMuon.Configuration.RecoLocalMuon_cff import *
from RecoLuminosity.LumiProducer.lumiProducer_cff import *

#--------------------------------------------------------------------------
# HIGH LEVEL RECO

from RecoHI.Configuration.Reconstruction_HI_cff import *

#--------------------------------------------------------------------------

caloReco = cms.Sequence(ecalLocalRecoSequence*hcalLocalRecoSequence)
#hbhereco = hbheprereco.clone()
#hcalLocalRecoSequence.replace(hbheprereco,hbhereco)
muonReco = cms.Sequence(trackerlocalreco+muonlocalreco+lumiProducer)
localReco = cms.Sequence(offlineBeamSpot*muonReco*caloReco)

caloRecoNZS = cms.Sequence(caloReco+hcalLocalRecoSequenceNZS)
localReco_HcalNZS = cms.Sequence(offlineBeamSpot*muonReco*caloRecoNZS)

#--------------------------------------------------------------------------
# Main Sequence

reconstruct_PbPb = cms.Sequence(localReco*globalRecoPbPb)
reconstructionHeavyIons = cms.Sequence(reconstruct_PbPb)
reconstructionHeavyIons_HcalNZS = cms.Sequence(localReco_HcalNZS*globalRecoPbPb)

#--------------------------------------------------------------------------
