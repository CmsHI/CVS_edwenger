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
process.GlobalTag.globaltag = 'MC_37Y_V4::All'

process.load("RecoVertex.BeamSpotProducer.d0_phi_analyzer_cff")
#process.load("RecoVertex.BeamSpotProducer.d0_phi_analyzer_pixelLess_cff")
process.d0_phi_analyzer.BeamFitter.Debug=True
process.d0_phi_analyzer.BeamFitter.MinimumInputTracks = 2
process.d0_phi_analyzer.BeamFitter.MinimumTotalLayers = 6
process.d0_phi_analyzer.BeamFitter.MinimumPixelLayers = -1
process.d0_phi_analyzer.BeamFitter.MaximumNormChi2 = 10
process.d0_phi_analyzer.BeamFitter.MinimumPt = 1.0
process.d0_phi_analyzer.BeamFitter.MaximumImpactParameter = 1.0
process.d0_phi_analyzer.BeamFitter.InputBeamWidth = -1

process.d0_phi_analyzer.PVFitter.Debug=True
process.d0_phi_analyzer.PVFitter.minNrVerticesForFit = 10
process.d0_phi_analyzer.PVFitter.minVertexNdf = 4
process.d0_phi_analyzer.BSAnalyzerParameters.RunBeamWidthFit=True
process.MessageLogger.debugModules = ['BeamSpotAnalyzer','PVFitter','BeamFitter']

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [    
    #'/store/mc/Spring10/MinimumBiasHI/RAW/JulyAnalysisExercise_MC_37Y_V4-v3/0002/02614D5A-A285-DF11-840A-00E08147F80E.root',
    #'/store/mc/Spring10/MinimumBiasHI/RAW/JulyAnalysisExercise_MC_37Y_V4-v3/0002/02F69476-C285-DF11-9BBA-0019B9CADC3D.root',
    #'/store/mc/Spring10/MinimumBiasHI/RAW/JulyAnalysisExercise_MC_37Y_V4-v3/0002/10DA04A9-BF85-DF11-A56B-0026B9392629.root',
    #'/store/mc/Spring10/MinimumBiasHI/RAW/JulyAnalysisExercise_MC_37Y_V4-v3/0002/485FD26A-8C85-DF11-ADBB-001C23C0B763.root',
    #'/store/mc/Spring10/MinimumBiasHI/RAW/JulyAnalysisExercise_MC_37Y_V4-v3/0002/50AB285B-FB85-DF11-81F4-001EC94BFDB6.root'

    "file:/net/hisrv0001/home/edwenger/scratch/jex/0004/44FB7814-0C87-DF11-A682-0026B93785F5.root",
    "file:/net/hisrv0001/home/edwenger/scratch/jex/0004/62485CB2-F786-DF11-AA24-001EC94BA3E0.root",
    "file:/net/hisrv0001/home/edwenger/scratch/jex/0004/649E7D7F-E386-DF11-AA50-A4BADB22B414.root",
    "file:/net/hisrv0001/home/edwenger/scratch/jex/0004/72E5F9D3-0887-DF11-8328-0019B9CABE48.root",
    "file:/net/hisrv0001/home/edwenger/scratch/jex/0004/A8629E90-0887-DF11-821C-001C23BED7DE.root",
    "file:/net/hisrv0001/home/edwenger/scratch/jex/0004/B2F8D9FB-0787-DF11-9DAA-001EC94BFB57.root"

    
] );

process.source = source

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Centrality Producer
process.load("RecoHI.HiCentralityAlgos.HiCentrality_cfi")
process.hiCentrality.produceHFtowers=cms.bool(False)
process.hiCentrality.produceBasicClusters=cms.bool(False)

# Centrality Filter
process.load("edwenger.JulyExercise.CentralityFilter_cff")
process.centralityFilter.selectedBins = [25,26,27,28,29,30,31,32,33,34,35,36,37,38,39]


# Path and EndPath definitions
process.centralityFilter_step = cms.Sequence(process.hcalDigis*
                                             process.hcalLocalRecoSequence*
                                             process.hiCentrality*
                                             process.centralityFilter)

process.pixelLessTrack_step = cms.Sequence(process.siPixelDigis*
                                           process.siStripDigis*
                                           process.trackerlocalreco*
                                           process.offlineBeamSpot*
                                           process.ctfTracksPixelLess)

process.tracking_step = cms.Sequence(process.siPixelDigis*
                                     process.siStripDigis*
                                     process.trackerlocalreco*
                                     process.offlineBeamSpot*
                                     process.recopixelvertexing*
                                     process.ckftracks)

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
