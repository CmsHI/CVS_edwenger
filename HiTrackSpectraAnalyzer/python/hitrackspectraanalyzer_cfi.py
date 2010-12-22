import FWCore.ParameterSet.Config as cms

hitrackAna = cms.EDAnalyzer('HiTrackSpectraAnalyzer',
                          src = cms.untracked.InputTag("hiGoodTracks"),
                          vsrc = cms.untracked.InputTag("hiSelectedVertex"),
                          jsrc = cms.untracked.InputTag("patJets"),
                          gsrc = cms.untracked.InputTag("hiGenParticles"),
                          gjsrc = cms.untracked.InputTag("ak5HiGenJets"),
                          src_evtCorr = cms.untracked.InputTag("hiGoodTracks"),
                          setQualityBit = cms.untracked.bool(False),
                          isGEN = cms.untracked.bool(False),
                          pureGENmode = cms.untracked.bool(False),
                          nsdOnly = cms.untracked.bool(False), # look at only NSD GEN
                          pythia6 = cms.untracked.bool(True), # false => pythia8
                          pthatCut = cms.untracked.double(0.0),
                          doJet = cms.untracked.bool(False),
                          histOnly = cms.untracked.bool(True),
                          includeExtra = cms.untracked.bool(False),
                          etaMax = cms.untracked.double(2.5),
                          ptMin = cms.untracked.double(0.0),
                          applyEvtEffCorr = cms.untracked.bool(False),
                          evtEffCorrType = cms.untracked.int32(0), # 0 = STD, 1 = SPEC, 2 = AGR
                          efit_type = cms.untracked.int32(0), # 0 = 1 pixel,  1 = 4 tracks
                          evtMultCut = cms.untracked.int32(3), # skip events with mult <= evtMultCut
                          evtSelEffv = cms.untracked.vdouble(19,0.0119454,0.212963,0.376093,0.450549,0.535354,0.607345,0.684058,0.723837,0.805643,0.83391,
                                                             0.880702,0.93007,0.959839,0.972656,0.966942,0.965812,0.982609,0.985646,0.978495),
                          triggerNeeded = cms.untracked.bool(False),
                          hltNames = cms.untracked.vstring('HLT_HIMinBiasHF', # the 1st 5 paths are hardcoded in nt, please add more HLT paths after these
                                                           'HLT_HIJet35U',
                                                           'HLT_HIJet50U',
                                                           'HLT_HIJet75U',
                                                           'HLT_HIJet90U'),
                          neededTrigSpectra = cms.untracked.vint32(0,0,0,0,0), # 1st, 3rd, in the hltNames list 
                          triglabel = cms.untracked.InputTag('TriggerResults','','HLT'),
                          pixelMultMode = cms.untracked.bool(False)
                          )

