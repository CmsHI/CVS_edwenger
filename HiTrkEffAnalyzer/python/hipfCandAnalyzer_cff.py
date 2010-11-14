import FWCore.ParameterSet.Config as cms

from SimTracker.TrackAssociation.TrackAssociatorByHits_cfi import *
from SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi import *
TrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')

#myTrackAssociatorByHitsRecoDenom = cms.Sequence(TrackAssociatorByHitsRecoDenom)

from edwenger.HiTrkEffAnalyzer.hipfCandAnalyzer_cfi import *

pfCandidateAnalyzer.ptMin = cms.untracked.double(3.0)
pfCandidateAnalyzer.hasSimInfo = cms.untracked.bool(True)
pfCandidateAnalyzer.Tracks = cms.InputTag("hiSelectedTracks")

hipfCandAnalyzer = cms.Sequence(#myTrackAssociatorByHitsRecoDenom*
                                pfCandidateAnalyzer)


