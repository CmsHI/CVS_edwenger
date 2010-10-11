import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import os 

process = cms.Process("TEST")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.MixingNoPileUp_cff')

#global tags for conditions data: https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'MC_38Y_V8::All'

##################################################################################

# setup 'standard'  options
options = VarParsing.VarParsing ('standard')

# setup any defaults you want
options.output = 'test_out.root'
options.files = [
    #b=0 Hydjet
    #'/store/relval/CMSSW_3_8_1/RelValHydjetQ_B0_2760GeV/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V8-v1/0011/FE23A1D2-FFA1-DF11-8640-0018F3D09642.root'

    #MB Hydjet
    #'/store/relval/CMSSW_3_8_1/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V8-v1/0011/CE30528D-F3A1-DF11-9D39-001A92971ACE.root'
    
    #pp MinBias
    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_8_1/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V8-v1/0011/32171E7E-28A2-DF11-BE60-001A92810A9A.root'

    #pp QCD80
    #'/store/relval/CMSSW_3_8_1/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V8-v1/0011/DED21844-2DA2-DF11-8547-003048D15D22.root'
    ]
options.maxEvents = 1 

# get and parse the command line arguments
options.parseArguments()


##################################################################################
# Some Services

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.debugModules = ['*']  
process.MessageLogger.categories = ['HeavyIonVertexing','heavyIonHLTVertexing','MinBiasTracking','CkfPattern','CkfDebugger']
process.MessageLogger.cerr = cms.untracked.PSet(
    threshold = cms.untracked.string('DEBUG'),
    DEBUG = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    INFO = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    HeavyIonVertexing = cms.untracked.PSet(
        limit = cms.untracked.int32(-1)
	),
    heavyIonHLTVertexing = cms.untracked.PSet(
        limit = cms.untracked.int32(-1)
    ),
    MinBiasTracking = cms.untracked.PSet(
        limit = cms.untracked.int32(-1)
    ),
    CkfDebugger = cms.untracked.PSet(
        limit = cms.untracked.int32(-1)
    ),
    CkfPattern = cms.untracked.PSet(
        limit = cms.untracked.int32(-1)
    )
)
	   
process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',
                                        ignoreTotal=cms.untracked.int32(0),
                                        oncePerEventMode = cms.untracked.bool(False)
                                        )

process.Timing = cms.Service("Timing")

##################################################################################
# Input Source
process.source = cms.Source('PoolSource',
	fileNames = cms.untracked.vstring(options.files)
)
							
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

##################################################################################
#Reconstruction			
process.load("Configuration.StandardSequences.RawToDigi_cff")		    # RawToDigi
process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff") # full heavy ion reconstruction
process.load("RecoHI.HiTracking.secondStep_cff")                            # pair-seeded step

process.CkfDebugTrajectoryBuilder = cms.ESProducer("CkfDebugTrajectoryBuilderESProducer",
                                                   propagatorAlong = cms.string('PropagatorWithMaterial'),
                                                   trajectoryFilterName = cms.string('ckfBaseTrajectoryFilter'),
                                                   maxCand = cms.int32(5),
                                                   ComponentName = cms.string('CkfDebugTrajectoryBuilder'),
                                                   intermediateCleaning = cms.bool(False),
                                                   MeasurementTrackerName = cms.string(''),
                                                   estimator = cms.string('Chi2'),
                                                   TTRHBuilder = cms.string('WithTrackAngle'),
                                                   updator = cms.string('KFUpdator'),
                                                   alwaysUseInvalidHits = cms.bool(False),
                                                   propagatorOpposite = cms.string('PropagatorWithMaterialOpposite'),
                                                   lostHitPenalty = cms.double(30.0)
                                                   )
                                                   
process.ckfDebugCandidates = cms.EDProducer("CkfDebugTrackCandidateMaker",
                                    RedundantSeedCleaner = cms.string('none'),
                                    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedSeeds'),
                                    cleanTrajectoryAfterInOut = cms.bool(True),
                                    useHitsSplitting = cms.bool(True),
                                    doSeedingRegionRebuilding = cms.bool(False),
                                    maxNSeeds = cms.uint32(100000),
                                    src = cms.InputTag('hiPixelTrackSeeds'),
                                    NavigationSchool = cms.string('SimpleNavigationSchool'),
                                    TrajectoryBuilder = cms.string('CkfDebugTrajectoryBuilder'),
                                    TransientInitialStateEstimatorParameters = cms.PSet(
                                       propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
                                       propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite'),
                                       numberMeasurementsForFit = cms.int32(4)
                                       )
                                    )
 
process.heavyIonTracking.replace(process.hiPrimTrackCandidates,process.ckfDebugCandidates)

##############################################################################
# Output EDM File
process.load("Configuration.EventContent.EventContentHeavyIons_cff")        #load keep/drop output commands
process.output = cms.OutputModule("PoolOutputModule",
                                  process.FEVTDEBUGEventContent,
                                  fileName = cms.untracked.string(options.output)
                                  )
process.output.outputCommands.extend(["keep *_*_*_TEST"])

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('ckfTest.root'))

##################################################################################
# Sub-sequences
process.rechits = cms.Sequence(process.siPixelRecHits*process.siStripMatchedRecHits)
process.vtxreco = cms.Sequence(process.offlineBeamSpot * process.trackerlocalreco * process.hiPixelVertices)
process.pxlreco = cms.Sequence(process.vtxreco * process.hiPixel3PrimTracks)
process.trkreco = cms.Sequence(process.offlineBeamSpot * process.trackerlocalreco * process.heavyIonTracking)

# Paths
process.reco = cms.Path(process.mix * process.RawToDigi * process.trkreco)           # for RECO from RAW
#process.rereco = cms.Path(process.rechits * process.heavyIonTracking    # for reRECO from RECO
                          #* process.secondStep                          # plus pair-seeded iteration
                          #)
#process.save = cms.EndPath(process.output)

