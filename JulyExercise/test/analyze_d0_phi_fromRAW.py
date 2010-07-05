import FWCore.ParameterSet.Config as cms
process = cms.Process("d0phi")

# initialize MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
#process.load("Configuration.StandardSequences.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.GeometryPilot2_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("RecoTracker.Configuration.RecoTrackerNotStandard_cff")
#process.MeasurementTracker.pixelClusterProducer = cms.string("")

process.load("RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi")
#process.offlinePrimaryVertices.TrackLabel = 'ctfPixelLess'

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.es_prefer_beamspot = cms.ESPrefer("PoolDBESSource","GlobalTag")
process.GlobalTag.globaltag = 'MC_37Y_V5::All'

process.load("RecoVertex.BeamSpotProducer.d0_phi_analyzer_cff")
process.load("RecoVertex.BeamSpotProducer.d0_phi_analyzer_pixelLess_cff")
process.d0_phi_analyzer.BeamFitter.Debug=cms.untracked.bool(True)
process.d0_phi_analyzer.PVFitter.Debug=cms.untracked.bool(True)
process.d0_phi_analyzer.PVFitter.minNrVerticesForFit = 10
process.d0_phi_analyzer.PVFitter.minVertexNdf = 4
process.d0_phi_analyzer.BeamFitter.MinimumInputTracks = 10
process.d0_phi_analyzer.BSAnalyzerParameters.RunBeamWidthFit=True
process.MessageLogger.debugModules = ['BeamSpotAnalyzer','PVFitter','BeamFitter']

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [    
        '/store/relval/CMSSW_3_7_0/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RAW/MC_37Y_V4-v1/0026/80F240B9-8E69-DF11-A1A9-002618943956.root',
        '/store/relval/CMSSW_3_7_0/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RAW/MC_37Y_V4-v1/0025/DC090C58-6669-DF11-AB97-0018F3D09658.root',
        '/store/relval/CMSSW_3_7_0/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RAW/MC_37Y_V4-v1/0025/C2752336-6569-DF11-8BDD-0018F3D0965E.root',
        '/store/relval/CMSSW_3_7_0/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RAW/MC_37Y_V4-v1/0025/A653584F-6969-DF11-B6FF-0026189437FD.root',
        '/store/relval/CMSSW_3_7_0/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RAW/MC_37Y_V4-v1/0025/984ECE84-6C69-DF11-957B-00304867915A.root',
        '/store/relval/CMSSW_3_7_0/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RAW/MC_37Y_V4-v1/0025/8A9B1FB4-6169-DF11-9122-0030486792BA.root',
        '/store/relval/CMSSW_3_7_0/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RAW/MC_37Y_V4-v1/0025/78154E4D-6E69-DF11-8F62-003048678FFA.root',
        '/store/relval/CMSSW_3_7_0/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RAW/MC_37Y_V4-v1/0025/6844C6B1-6869-DF11-ACA0-0026189438F5.root',
        '/store/relval/CMSSW_3_7_0/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RAW/MC_37Y_V4-v1/0025/4C2C6933-7369-DF11-BC9C-001A92971B88.root',
        '/store/relval/CMSSW_3_7_0/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RAW/MC_37Y_V4-v1/0025/46FD9FAE-5F69-DF11-8BD4-003048678ED4.root',
        '/store/relval/CMSSW_3_7_0/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RAW/MC_37Y_V4-v1/0025/0862D7D7-6969-DF11-97D3-0026189438C0.root',
        '/store/relval/CMSSW_3_7_0/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RAW/MC_37Y_V4-v1/0025/00C18D3F-6869-DF11-8687-0026189438AB.root'
] );

secFiles.extend( [
    ] )

process.source = source

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5000)
)


# Centrality Producer
process.load("RecoHI.HiCentralityAlgos.HiCentrality_cfi")
process.hiCentrality.produceHFtowers=cms.bool(False)
process.hiCentrality.produceBasicClusters=cms.bool(False)

# Centrality Filter
process.load("RecoHI.HiCentralityAlgos.CentralityFilter_cfi")

process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.CondDBCommon.connect = "sqlite_file:/home/edwenger/jex/RecoHI/HiCentralityAlgos/data/CentralityTables.db"
process.PoolDBESSource = cms.ESSource("PoolDBESSource",
                                      process.CondDBCommon,
                                      toGet = cms.VPSet(cms.PSet(record = cms.string('HeavyIonRcd'),
                                                                 tag = cms.string('HFhits40_MC_Hydjet2760GeV_MC_3XY_V24_v0')
                                                                 )
                                                        )
                                      )

process.centralityFilter.selectedBins = [25,26,27,28,29,30,31,32,33,34,35,36,37,38,39]


# Path and EndPath definitions
process.centralityFilter_step = cms.Sequence(process.hcalDigis*process.hcalLocalRecoSequence*process.hiCentrality*process.centralityFilter)
process.pixelLessTrack_step = cms.Sequence(process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot*process.ctfTracksPixelLess)
process.tracking_step = cms.Sequence(process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot*process.recopixelvertexing*process.ckftracks)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('test.root'),
                               outputCommands = cms.untracked.vstring(
                                'drop *',
                                'keep *_offlineBeamSpot_*_*',
                                'keep *_offlinePrimaryVertices_*_*',
                                'keep *_ctfPixelLess_*_*'
                               )
                              )
process.end = cms.EndPath(process.out)

#process.schedule = cms.Path(process.pixelLessTrack_step*process.offlinePrimaryVertices*process.d0_phi_analyzer_pixelless)
#process.schedule = cms.Path(process.pixelLessTrack_step*process.offlinePrimaryVertices*process.d0_phi_analyzer_pixelless*process.end)
process.schedule = cms.Path(process.centralityFilter_step*process.tracking_step*process.offlinePrimaryVertices*process.d0_phi_analyzer)
