
import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

#process.load("RecoHI.HiCentralityAlgos.HiTrivialCondRetriever_cfi")

process.load("edwenger.JulyExercise.CentralityFilter_cff")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    #"file:/net/hisrv0001/home/edwenger/scratch/jex/599a3a4efd1a164b86e329cbf44e9f6a/hiRecoJEX_RAW2DIGI_RECO_29_1_2Ko.root"
    "dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/kimy/MinimumBiasHI/Spring10-JulyAnalysisExercise_MC_37Y_V4-RECO-prelim-v1/599a3a4efd1a163b86e329cbf44e9f6a/hiRecoJEX_RAW2DIGI_RECO_29_1_2Ko.root"
	)
                            )

process.analyze = cms.EDAnalyzer("AnalyzerWithCentrality")

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string("histograms.root")
                                   )

process.p = cms.Path(process.analyze)

