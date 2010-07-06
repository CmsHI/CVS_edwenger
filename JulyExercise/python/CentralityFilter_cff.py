import FWCore.ParameterSet.Config as cms
from RecoHI.HiCentralityAlgos.CentralityFilter_cfi import *

from CondCore.DBCommon.CondDBCommon_cfi import *
CondDBCommon.connect = "sqlite_file:../data/CentralityTables.db"
PoolDBESSource = cms.ESSource("PoolDBESSource",
                              CondDBCommon,
                              toGet = cms.VPSet(
    cms.PSet(
    record = cms.string('HeavyIonRcd'),
    tag = cms.string('HFhits40_DataJulyExercise_Hydjet2760GeV_MC_37Y_V5_v0'))
    )
                              )
