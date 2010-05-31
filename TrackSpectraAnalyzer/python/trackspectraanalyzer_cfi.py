import FWCore.ParameterSet.Config as cms

trackAna = cms.EDAnalyzer('TrackSpectraAnalyzer',
                          src = cms.untracked.InputTag("generalTracks"),
                          vsrc = cms.untracked.InputTag("offlinePrimaryVertices"),
                          jsrc = cms.untracked.InputTag("selectedPatJets"),
                          gjsrc = cms.untracked.InputTag("ak5GenJets"),
                          src_evtCorr = cms.untracked.InputTag("generalTracks"),
                          isGEN = cms.untracked.bool(False),
                          pureGENmode = cms.untracked.bool(False),
                          nsdOnly = cms.untracked.bool(False), # look at only NSD GEN
                          doJet = cms.untracked.bool(True),
                          histOnly = cms.untracked.bool(True),
                          includeExtra = cms.untracked.bool(True),
                          etaMax = cms.untracked.double(2.5),
                          ptMin = cms.untracked.double(0.0),
                          applyEvtEffCorr = cms.untracked.bool(False),
                          evtEffCorrType = cms.untracked.int32(0), # 0 = STD, 1 = SPEC, 2 = AGR
                          efit_type = cms.untracked.int32(0), # 0 = 1 pixel,  1 = 4 tracks
                          evtSelEffCut = cms.untracked.double(0.0),
                          evtSelEffv = cms.untracked.vdouble(19,0.0119454,0.212963,0.376093,0.450549,0.535354,0.607345,0.684058,0.723837,0.805643,0.83391,
                                                             0.880702,0.93007,0.959839,0.972656,0.966942,0.965812,0.982609,0.985646,0.978495),
                          efit_para = cms.untracked.vdouble(2.31590e+00,4.58625e+00,-5.37943e+01,3.80259e-02,3.83653e-02,3.48486e+00,3.72379e-01),
                          hltNames = cms.untracked.vstring('HLT_MinBiasBSC',
                                                           'HLT_L1Jet6U',
                                                           'HLT_Jet15U',
                                                           'HLT_Jet30U',
                                                           'HLT_Jet50U'),
                          triglabel = cms.untracked.InputTag('TriggerResults','','HLT')
                          )

