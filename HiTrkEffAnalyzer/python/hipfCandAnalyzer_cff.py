import FWCore.ParameterSet.Config as cms

from SimTracker.TrackAssociation.TrackAssociatorByHits_cfi import *
from SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi import *
TrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')

#myTrackAssociatorByHitsRecoDenom = cms.Sequence(TrackAssociatorByHitsRecoDenom)

from edwenger.HiTrkEffAnalyzer.hipfCandAnalyzer_cfi import *

pfCandidateAnalyzer.ptMin = cms.untracked.double(4.0)
pfCandidateAnalyzer.Tracks = cms.InputTag("hiSelectedTracks")
pfCandidateAnalyzer.isData = cms.untracked.bool(False)

hipfCandAnalyzer = cms.Sequence(#myTrackAssociatorByHitsRecoDenom*
                                pfCandidateAnalyzer)


