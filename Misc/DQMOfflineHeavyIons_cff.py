import FWCore.ParameterSet.Config as cms

from DQMOffline.Configuration.DQMOffline_cff import *

# pixels
#sipixelQTester.qtList = cms.untracked.FileInPath('')

# strips
#qTester.qtList = cms.untracked.FileInPath('')

# tracks
hiTracks = "hiGlobalPrimTracks"
MonitorTrackResiduals.Tracks                      = hiTracks
MonitorTrackResiduals.trajectoryInput             = hiTracks
SiPixelTrackResidualSource.TrackCandidateProducer = 'hiPrimTrackCandidates'
SiPixelTrackResidualSource.trajectoryInput        = hiTracks
SiStripMonitorTrack.TrackProducer                 = hiTracks
SiPixelHitEfficiencySource.trajectoryInput        = hiTracks

# HCAL
hcalHotCellMonitor.energyThreshold = 50.0 # was 10.0
hcalHotCellMonitor.energyThreshold_HF = 200.0 # was 20.0
hcalDigiMonitor.minDigiSize = 6 # was 10
hcalMonitorTasksOfflineSequence.remove(hcalRawDataMonitor)

DQMOfflineHeavyIonsPreDPG = cms.Sequence(DQMOfflinePreDPG)

DQMOfflineHeavyIonsPrePOG = cms.Sequence(muonMonitors) ## other POG not run yet

DQMOfflineHeavyIons = cms.Sequence(DQMOfflineHeavyIonsPreDPG
                                   * DQMOfflineHeavyIonsPrePOG
                                   * DQMMessageLogger)
