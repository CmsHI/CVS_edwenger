import FWCore.ParameterSet.Config as cms

#
# Geometry master configuration
from Geometry.MuonNumbering.muonNumberingInitialization_cfi import *

#  Tracking Geometry
from Geometry.CommonDetUnit.globalTrackingGeometryDB_cfi import *

#Tracker
from RecoTracker.GeometryESProducer.TrackerRecoGeometryESProducer_cfi import *

#Muon
from RecoMuon.DetLayers.muonDetLayerGeometry_cfi import *

#  Calorimeters
from Geometry.CaloEventSetup.CaloTopology_cfi import *
from Geometry.CaloEventSetup.CaloGeometryDBReader_cfi import *
from Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi import *
from Geometry.EcalMapping.EcalMapping_cfi import *
from Geometry.EcalMapping.EcalMappingRecord_cfi import *

#  Alignment
from Geometry.TrackerGeometryBuilder.idealForDigiTrackerGeometryDB_cff import *
from Geometry.CSCGeometryBuilder.idealForDigiCscGeometryDB_cff import *
from Geometry.DTGeometryBuilder.idealForDigiDtGeometryDB_cff import *
