import FWCore.ParameterSet.Config as cms

# import p+p collision sequences
from DQM.SiStripMonitorClient.SiStripSourceConfigTier0_cff import *

# make heavy-ion modifications
hiTracks = "hiGlobalPrimTracks"
MonitorTrackResiduals.Tracks                      = hiTracks
MonitorTrackResiduals.trajectoryInput             = hiTracks
SiStripMonitorTrack.TrackProducer                 = hiTracks
