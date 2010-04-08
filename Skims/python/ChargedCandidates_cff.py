import FWCore.ParameterSet.Config as cms

#goodTracks = cms.EDFilter()

allTracks = cms.EDProducer("ConcreteChargedCandidateProducer",
                           src = cms.InputTag("generalTracks"),
                           #src = cms.InputTag("goodTracks),
                           particleType = cms.string('pi+')
                           )

chargedCandidates = cms.Sequence(#goodTracks *
                                 allTracks)
