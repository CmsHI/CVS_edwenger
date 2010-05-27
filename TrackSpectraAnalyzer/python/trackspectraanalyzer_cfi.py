import FWCore.ParameterSet.Config as cms

trackAna = cms.EDAnalyzer('TrackSpectraAnalyzer',
                          src = cms.untracked.InputTag("generalTracks"),
                          vsrc = cms.untracked.InputTag("offlinePrimaryVertices"),
                          jsrc = cms.untracked.InputTag("selectedPatJets"),
                          gjsrc = cms.untracked.InputTag("ak5GenJets"),
                          doOutput = cms.untracked.bool(True),
                          isGEN = cms.untracked.bool(False),
                          pureGENmode = cms.untracked.bool(False),
                          doJet = cms.untracked.bool(True),
                          histOnly = cms.untracked.bool(True),
                          includeExtra = cms.untracked.bool(True),
                          etaMax = cms.untracked.double(2.5),
                          ptMin = cms.untracked.double(0.0),
                          applyEvtEffCorr = cms.untracked.bool(False),
                          evtEffCorrType = cms.untracked.int32(0), # 0 = STD, 1 = SPEC, 2 = AGR
                          efit_para = cms.untracked.vdouble(2.31590e+00,4.58625e+00,-5.37943e+01,3.80259e-02,3.83653e-02,3.48486e+00,3.72379e-01),
                          hltNames = cms.untracked.vstring('HLT_MinBiasBSC',
                                                           'HLT_L1Jet6U',
                                                           'HLT_Jet15U',
                                                           'HLT_Jet30U',
                                                           'HLT_Jet50U'),
                          triglabel = cms.untracked.InputTag('TriggerResults','','HLT')
                          )

