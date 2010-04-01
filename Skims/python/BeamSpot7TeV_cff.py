import FWCore.ParameterSet.Config as cms
from CondCore.DBCommon.CondDBSetup_cfi import        *
from RecoVertex.BeamSpotProducer.BeamSpot_cfi import *

BeamSpotDBSource = cms.ESSource("PoolDBESSource",
                                CondDBSetup,
                                toGet = cms.VPSet(cms.PSet(
    record = cms.string('BeamSpotObjectsRcd'),
    tag = cms.string('Early7TeVCollision_STARTUP_DATA'))),
                                connect = cms.string('sqlite_file:EarlyCollision.db')
                                )
es_prefer_beamspot = cms.ESPrefer("PoolDBESSource","BeamSpotDBSource")
