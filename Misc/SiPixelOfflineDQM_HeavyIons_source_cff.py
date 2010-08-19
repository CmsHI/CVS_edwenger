import FWCore.ParameterSet.Config as cms

# import p+p sequence
from DQM.SiPixelCommon.SiPixelOfflineDQM_source_cff import *

# clone and modify modules 
hiTracks = "hiGlobalPrimTracks"

SiPixelTrackResidualSource_HeavyIons = SiPixelTrackResidualSource.clone(
    TrackCandidateProducer = 'hiPrimTrackCandidates',
    trajectoryInput = hiTracks
    )

SiPixelHitEfficiencySource_HeavyIons = SiPixelHitEfficiencySource.clone(
    trajectoryInput = hiTracks
    )

# define new HI sequence
siPixelOfflineDQM_heavyions_source = cms.Sequence(SiPixelHLTSource +
                                                  SiPixelRawDataErrorSource +
                                                  SiPixelDigiSource +
                                                  SiPixelRecHitSource +
                                                  SiPixelClusterSource +
                                                  SiPixelTrackResidualSource_HeavyIons +
                                                  SiPixelHitEfficiencySource_HeavyIons +
                                                  dqmInfo)
