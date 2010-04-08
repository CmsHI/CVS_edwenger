import FWCore.ParameterSet.Config as cms

# todo: quality selections on general tracks and pass to allTracks.src

allTracks = cms.EDProducer("ConcreteChargedCandidateProducer",
                           src = cms.InputTag("generalTracks"),
                           particleType = cms.string('pi+')
                           )

chargedCandidates = cms.Sequence(allTracks)
