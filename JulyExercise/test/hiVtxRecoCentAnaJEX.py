# Auto generated configuration file
# using: 
# Revision: 1.182 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: hiRecoJEX -n 2 -s RAW2DIGI,RECO --scenario HeavyIons --geometry DB --conditions auto:mc --datatier RECO --eventcontent=FEVTDEBUG --filein=file.root --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')

process.Timing = cms.Service("Timing")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('hiRecoJEX nevts:2'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    
    "file:/net/hisrv0001/home/edwenger/scratch/jex/0004/44FB7814-0C87-DF11-A682-0026B93785F5.root",
    "file:/net/hisrv0001/home/edwenger/scratch/jex/0004/62485CB2-F786-DF11-AA24-001EC94BA3E0.root",
    "file:/net/hisrv0001/home/edwenger/scratch/jex/0004/649E7D7F-E386-DF11-AA50-A4BADB22B414.root",
    "file:/net/hisrv0001/home/edwenger/scratch/jex/0004/72E5F9D3-0887-DF11-8328-0019B9CABE48.root",
    "file:/net/hisrv0001/home/edwenger/scratch/jex/0004/A8629E90-0887-DF11-821C-001C23BED7DE.root",
    "file:/net/hisrv0001/home/edwenger/scratch/jex/0004/B2F8D9FB-0787-DF11-9DAA-001EC94BFB57.root"

    )
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('hiVtxRecoCentAnaJEX_RAW2DIGI_RECO.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    )
)

# Additional output definition
process.load("edwenger.JulyExercise.BeamSpotJEX_cff")

process.hiCentrality.produceHFtowers=cms.bool(False)
process.hiCentrality.produceBasicClusters=cms.bool(False)

process.load("CondCore.DBCommon.CondDBCommon_cfi")

process.CondDBCommon.connect = 'sqlite_file:CentralityTables.db'

process.PoolDBESSource = cms.ESSource("PoolDBESSource",
                                       process.CondDBCommon,
                                       DumpStat=cms.untracked.bool(True),
                                       toGet = cms.VPSet(cms.PSet(
                                               record = cms.string('HeavyIonRcd'),
                                               tag = cms.string('HFhits40_MC_Hydjet2760GeV_MC_3XY_V24_v0')
                                                )),
                                      )

process.analyze = cms.EDAnalyzer("AnalyzerWithCentrality")
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms.root"))

# Other statements
process.GlobalTag.globaltag = 'MC_37Y_V4::All'
process.rechits = cms.Sequence(process.siPixelRecHits * process.siStripMatchedRecHits)
process.vtxreco = cms.Sequence(process.offlineBeamSpot * process.trackerlocalreco * process.hiPixelVertices)
process.pxlreco = cms.Sequence(process.vtxreco * process.hiPixel3PrimTracks)
process.trkreco = cms.Sequence(process.offlineBeamSpot * process.trackerlocalreco * process.heavyIonTracking)
process.centana = cms.Sequence(process.hcalLocalRecoSequence * process.hiCentrality * process.analyze)

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.vtxreco)
process.centrality_step = cms.Path(process.centana)
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.centrality_step,process.endjob_step,process.out_step)
