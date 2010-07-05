
import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

#process.load("RecoHI.HiCentralityAlgos.HiTrivialCondRetriever_cfi")

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

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    "file:/net/hisrv0001/home/edwenger/jex/edwenger/JulyExercise/python/hiRecoJEX_RAW2DIGI_RECO.root"
    )
                            )

process.analyze = cms.EDAnalyzer("AnalyzerWithCentrality")

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string("histograms.root")
                                   )

process.p = cms.Path(process.analyze)

