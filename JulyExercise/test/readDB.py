import FWCore.ParameterSet.Config as cms


process = cms.Process("readDB")
process.load("FWCore.MessageLogger.MessageLogger_cfi")


process.load("CondCore.DBCommon.CondDBSetup_cfi")



process.BeamSpotDBSource = cms.ESSource("PoolDBESSource",
                                        process.CondDBSetup,
                                        toGet = cms.VPSet(cms.PSet(
    record = cms.string('BeamSpotObjectsRcd'),
    tag = cms.string('JEX_BeamSpot_Preliminary')
    )),
                                        connect = cms.string('sqlite_file:JEX_BeamSpot_Preliminary.db')
                                        #connect = cms.string('oracle://cms_orcoff_prod/CMS_COND_31X_BEAMSPOT')
                                        #connect = cms.string('frontier://PromptProd/CMS_COND_31X_BEAMSPOT')
                                        )

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(1)
                    )
process.beamspot = cms.EDAnalyzer("BeamSpotFromDB")


process.p = cms.Path(process.beamspot)

