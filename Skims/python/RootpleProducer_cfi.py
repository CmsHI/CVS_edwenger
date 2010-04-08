import FWCore.ParameterSet.Config as cms

rootpleProducer = cms.EDProducer("AnalysisRootpleProducer",
                                 TracksCollectionName      = cms.InputTag("goodTracks"),
                                 VertexCollectionName      = cms.InputTag("offlinePrimaryVertices"),
                                 MCEvent                   = cms.InputTag("generator"),
                                 triggerEvent              = cms.InputTag("hltTriggerSummaryAOD"),
                                 ChgGenPartCollectionName  = cms.InputTag("chargeParticles"),
                                 RecoCaloJetCollectionName = cms.InputTag("ak7CaloJets"),
                                 ChgGenJetCollectionName   = cms.InputTag("ak7GenJets"),
                                 TracksJetCollectionName   = cms.InputTag("ak7PFJets"),
                                 GenJetCollectionName      = cms.InputTag("ak7GenJets"),
                                 OnlyRECO                  = cms.bool(True),
                                 triggerResults            = cms.InputTag("TriggerResults","","HLT"),
                                 )
