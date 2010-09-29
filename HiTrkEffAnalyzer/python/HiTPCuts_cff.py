import FWCore.ParameterSet.Config as cms

import PhysicsTools.RecoAlgos.trackingParticleSelector_cfi

# -------------- Default cuts -----------------------------
# src = cms.InputTag("mergedtruth","MergedTrackTruth"),
# chargedOnly = cms.bool(True),
# pdgId = cms.vint32(),
# tip = cms.double(3.5),
# signalOnly = cms.bool(True),
# minRapidity = cms.double(-2.4),
# lip = cms.double(30.0),
# ptMin = cms.double(0.9),
# maxRapidity = cms.double(2.4),
# minHit = cms.int32(0)
#-----------------------------------------------------------

cutsFullTracks = PhysicsTools.RecoAlgos.trackingParticleSelector_cfi.trackingParticleSelector.clone(signalOnly = cms.bool(False),
                                                                                                    ptMin=cms.double(0.8),
                                                                                                    minRapidity = cms.double(-2.5),
                                                                                                    maxRapidity = cms.double(2.5))
#cutsPxlLowPtTracks = PhysicsTools.RecoAlgos.trackingParticleSelector_cfi.trackingParticleSelector.clone()


