import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQM_cfg import *
DQMStore.collateHistograms =cms.untracked.bool(True)
from Validation.RecoTrack.MultiTrackValidator_cfi import *
from SimTracker.TrackAssociation.LhcParametersDefinerForTP_cfi import *
from SimTracker.TrackAssociation.CosmicParametersDefinerForTP_cfi import *

from Validation.RecoTrack.cuts_cff import *
from CmsHi.TrackAnalysis.findableSimTracks_cfi import *

# TrackingParticle selection
#cutsTPEffic.minRapidity  = cms.double(-2.5)
#cutsTPEffic.maxRapidity  = cms.double(2.5)

findableSimTracks.ptMin = 0.9
findableSimTracks.minHit = 8
findableSimTracks.minRapidity = cms.double(-2.5)
findableSimTracks.maxRapidity = cms.double(2.5)
findableSimTracks.tip = cms.double(30)
findableSimTracks.lip = cms.double(30)

# reco::Track selection
cutsRecoTracks.quality = ['highPurity']
cutsRecoTracks.minHit   = cms.int32(8)
cutsRecoTracks.minRapidity  = cms.double(-2.5)
cutsRecoTracks.maxRapidity  = cms.double(2.5)

# MTV
multiTrackValidator.outputFile = 'multitrackvalidator.root'
multiTrackValidator.associators = ['TrackAssociatorByHits']
multiTrackValidator.label_tp_effic = cms.InputTag("findableSimTracks")
multiTrackValidator.label_tp_fake  = cms.InputTag("cutsTPFake")
multiTrackValidator.label = ['cutsRecoTracks']
multiTrackValidator.skipHistoFit=cms.untracked.bool(False)
multiTrackValidator.useLogPt=cms.untracked.bool(False)
multiTrackValidator.minpT = cms.double(0.1)
multiTrackValidator.maxpT = cms.double(1000)
multiTrackValidator.nintpT = cms.int32(2000)
multiTrackValidator.UseAssociators = cms.bool(True)
multiTrackValidator.nint = cms.int32(200) #eta interval (0.025)

mtv = cms.Sequence(
    findableSimTracks *
    #cutsTPEffic *
    cutsTPFake  *
    cutsRecoTracks *
    multiTrackValidator
)
