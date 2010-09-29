import FWCore.ParameterSet.Config as cms

from SimTracker.TrackAssociation.TrackAssociatorByHits_cfi import *
from SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi import *
TrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')

from edwenger.HiTrkEffAnalyzer.HiTPCuts_cff import *
from edwenger.HiTrkEffAnalyzer.hitrkEffAnalyzer_cfi import *

fulltrackingParticleRecoTrackAsssociation = trackingParticleRecoTrackAsssociation.clone(label_tr=cms.InputTag("hiSelectedTracks"),
                                                                                        label_tp=cms.InputTag("cutsFullTracks"))

hitrkEffAnalyzer.label_tp_effic = cms.untracked.InputTag("cutsFullTracks")
hitrkEffAnalyzer.label_tp_fake = cms.untracked.InputTag("cutsFullTracks")
hitrkEffAnalyzer.associatormap = cms.untracked.InputTag('fulltrackingParticleRecoTrackAsssociation')
hitrkEffAnalyzer.hasSimInfo = cms.untracked.bool(True) # without this no sim track info
hitrkEffAnalyzer.doAssociation = cms.untracked.bool(False)
hitrkEffAnalyzer.useJetEt = cms.untracked.bool(False)

hitrkEffAna = cms.Sequence(cutsFullTracks*
                           fulltrackingParticleRecoTrackAsssociation*
                           hitrkEffAnalyzer)


