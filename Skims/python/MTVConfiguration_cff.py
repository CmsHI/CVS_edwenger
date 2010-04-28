import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQM_cfg import *
DQMStore.collateHistograms =cms.untracked.bool(True)
from Validation.RecoTrack.MultiTrackValidator_cfi import *
from SimTracker.TrackAssociation.LhcParametersDefinerForTP_cfi import *
from SimTracker.TrackAssociation.CosmicParametersDefinerForTP_cfi import *

# MTV
multiTrackValidator.skipHistoFit=cms.untracked.bool(False)
multiTrackValidator.useLogPt=cms.untracked.bool(False)
multiTrackValidator.minpT = cms.double(0.1)
multiTrackValidator.maxpT = cms.double(1000)
multiTrackValidator.nintpT = cms.int32(2000)
multiTrackValidator.UseAssociators = cms.bool(True)
multiTrackValidator.nint = cms.int32(200) #eta interval (0.025) 
