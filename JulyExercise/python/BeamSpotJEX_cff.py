import FWCore.ParameterSet.Config as cms
from CondCore.DBCommon.CondDBSetup_cfi import        *
from RecoVertex.BeamSpotProducer.BeamSpot_cfi import *

BeamSpotDBSource = cms.ESSource("PoolDBESSource",
                                CondDBSetup,
                                toGet = cms.VPSet(cms.PSet(
    record = cms.string('BeamSpotObjectsRcd'),
    tag = cms.string('JEX_BeamSpot_Preliminary'))),
                                connect = cms.string('sqlite_file:../data/PreliminaryBeamSpot.db')
                                )
es_prefer_beamspot = cms.ESPrefer("PoolDBESSource","BeamSpotDBSource")

