import FWCore.ParameterSet.Config as cms

process = cms.Process("RECO")

process.load("SimGeneral.TrackingAnalysis.trackingParticles_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpotFakeConditionsEarlyCollision_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.Digi_cff")
process.load("Configuration.StandardSequences.FakeConditions_cff")
#process.load("MitHig.Configuration.FakeConditions_TempFix_cff") # temp fix to MIT issues
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
process.load("RecoTracker.TkSeedingLayers.TTRHBuilderWithoutAngle4PixelTriplets_cfi")
#process.load("HeavyIonsAnalysis.Configuration.Tracking_PbPb_cff")
process.load("HeavyIonsAnalysis.Configuration.HighPtTracking_PbPb_cff") #updated config
process.load("Configuration.EventContent.EventContent_cff")
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")

process.load("Configuration.StandardSequences.MixingNoPileUp_cff") #no pileup

#from HeavyIonsAnalysis.Configuration.EventEmbedding_cff import *
#process.mix=mixSim
#process.mix.input.fileNames = cms.untracked.vstring('dcache:/pnfs/cmsaf.mit.edu/hibat/cms/users/yetkin/sim/hydjet_sim_x2_c2_d20080425/hydjet_sim_x2_c2_d20080425_r000084.root')

process.MessageLogger = cms.Service("MessageLogger",
                                    destinations = cms.untracked.vstring("cerr"),
                                    cerr = cms.untracked.PSet( threshold = cms.untracked.string("DEBUG") ),
                                    debugModules = cms.untracked.vstring()
                                    )

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                                   moduleSeeds = cms.PSet(simMuonRPCDigis = cms.untracked.uint32(6),
                                                                          simEcalUnsuppressedDigis = cms.untracked.uint32(8),
                                                                          simSiStripDigis = cms.untracked.uint32(7),
                                                                          mix = cms.untracked.uint32(4),
                                                                          simHcalUnsuppressedDigis = cms.untracked.uint32(9),
                                                                          simMuonCSCDigis = cms.untracked.uint32(6),
                                                                          VtxSmeared = cms.untracked.uint32(2),
                                                                          g4SimHits = cms.untracked.uint32(3),
                                                                          simMuonDTDigis = cms.untracked.uint32(6),
                                                                          simSiPixelDigis = cms.untracked.uint32(7)
                                                                          ),
                                                   sourceSeed = cms.untracked.uint32(1)
                                                   )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource", 
                            fileNames = cms.untracked.vstring("dcache:/pnfs/cmsaf.mit.edu/hibat/cms/users/yetkin/sim/pythia_900GeV_d20080526/pythia_900GeV_d20080526_r000179.root")
                            )

process.GlobalTrackingGeometryESProducer = cms.ESProducer("GlobalTrackingGeometryESProducer")

process.TransientTrackBuilderESProducer = cms.ESProducer("TransientTrackBuilderESProducer",
                                                         ComponentName = cms.string("TransientTrackBuilder")
                                                         )

process.output = cms.OutputModule("PoolOutputModule",
                                  fileName = cms.untracked.string("Hydjet_full_test.root")
                                  )

process.p = cms.Path(process.mix * process.trDigi * process.trackingParticles * process.offlineBeamSpot * process.trackerlocalreco * process.heavyIonTracking)

process.outpath = cms.EndPath(process.output)
