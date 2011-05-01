import FWCore.ParameterSet.Config as cms

from SimTracker.TrackAssociation.TrackAssociatorByHits_cfi import *
from SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi import *
TrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')

#myTrackAssociatorByHitsRecoDenom = cms.Sequence(TrackAssociatorByHitsRecoDenom)

from edwenger.HiTrkEffAnalyzer.HiTPCuts_cff import *
from edwenger.HiTrkEffAnalyzer.hitrkEffAnalyzer_cfi import *

from edwenger.HiTrkEffAnalyzer.hipfCandAnalyzer_cfi import *
pfCandidateAnalyzer.SimTracks = cms.InputTag("cutsTPForFak")
pfCandidateAnalyzer.ptMin = cms.untracked.double(4.0)
pfCandidateAnalyzer.Tracks = cms.InputTag("hiHighPtTracks")
pfCandidateAnalyzer.isData = cms.untracked.bool(False)


hipfCandAnalyzer = cms.Sequence(pfCandidateAnalyzer)


pfCandidateAnalyzer_test = pfCandidateAnalyzer.clone(Tracks = cms.InputTag("hiHighPtCaloTracks"))

hipfCandAnalyzer_test = cms.Sequence(pfCandidateAnalyzer_test)
