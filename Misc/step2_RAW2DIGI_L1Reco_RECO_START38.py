# Auto generated configuration file
# using: 
# Revision: 1.222.2.2 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step2 -n 1 --scenario HeavyIons -s RAW2DIGI,L1Reco,RECO --processName TEST --datatier GEN-SIM-RECO --eventcontent FEVTDEBUG --customise SimGeneral/DataMixingModule/DataMixer_DataConditions_3_8_X_data2010.py --geometry DB --filein file:SingleZmumu_FlatPt0to20_cfi_GEN_SIM_DIGI_DATAMIX_L1_DIGI2RAW.root --conditions FrontierConditions_GlobalTag,START38_V8::All --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('TEST')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('Configuration.StandardSequences.MixingNoPileUp_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.debugModules = ['hiPixel3ProtoTracks','hiSelectedProtoTracks','hiSelectedVertex']
process.MessageLogger.categories = ['HeavyIonVertexing','heavyIonHLTVertexing','MinBiasTracking']
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
    )
)

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.222.2.2 $'),
    annotation = cms.untracked.string('step2 nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.options = cms.untracked.PSet(

)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:SingleZmumu_FlatPt0to20_cfi_GEN_SIM_DIGI_DATAMIX_L1_DIGI2RAW.root')
)

# Output definition

process.FEVTDEBUGoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    #outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('step2_RAW2DIGI_L1Reco_RECO.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    )
)

# Additional output definition
#process.hiPixel3ProtoTracks.RegionFactoryPSet.RegionPSet.useFoundVertices = False
process.hiSelectedProtoTracks.maxD0Significance = 1000.0
process.hiSelectedProtoTracks.minZCut = 0.5
process.hiSelectedTracks.d0_par2 = [300.0,0.3]
process.hiSelectedTracks.dz_par2 = [300.0,0.3]

# Other statements
process.GlobalTag.globaltag = 'START38_V8::All'

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstructionHeavyIons)
process.endjob_step = cms.Path(process.endOfProcess)
process.FEVTDEBUGoutput_step = cms.EndPath(process.FEVTDEBUGoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.FEVTDEBUGoutput_step)


# Automatic addition of the customisation function
def customise(process):

#
# IOV set based on GlobalTag GR_R_35X_V8B
#
# placeholder !!!!!! replace with the actual run number of
# the real run to be overlaid

    process.source.firstRun = cms.untracked.uint32(132599)

    process.ecalConditions1 = cms.ESSource("PoolDBESSource",                                          
         process.CondDBSetup,                                                                         
         timetype = cms.string('runnumber'),                                                          
         toGet = cms.VPSet(                                                                           
             cms.PSet(                                                                                
        record = cms.string('EcalADCToGeVConstantRcd'),                                               
        tag = cms.string('EcalADCToGeVConstant_v6_offline')
        ),                                                                                            
             cms.PSet(                                                                                
        record = cms.string('EcalChannelStatusRcd'),                                                  
        tag = cms.string('EcalChannelStatus_v04_offline')                                   
        ),                                                                                            
             cms.PSet(                                                                                
        record = cms.string('EcalGainRatiosRcd'),                                                     
        tag = cms.string('EcalGainRatio_TestPulse2009_offline')                                      
        ),                                                                                            
             cms.PSet(                                                                                
        record = cms.string('EcalIntercalibConstantsRcd'),                                            
        tag = cms.string('EcalIntercalibConstants_v6_offline')                                 
        ),                                                                                            
             cms.PSet(                                                                                
        record = cms.string('EcalIntercalibErrorsRcd'),                                               
        tag = cms.string('EcalIntercalibErrors_mc')                                                   
        ),                                                                                            
             cms.PSet(                                                                                
        record = cms.string('EcalMappingElectronicsRcd'),                                             
        tag = cms.string('EcalMappingElectronics_EEMap')                                              
        ),                                                                                            
             cms.PSet(                                                                                
        record = cms.string('EcalPedestalsRcd'),                                                      
        tag = cms.string('EcalPedestals_2009runs_hlt')                                                
        ),                                                                                            
             cms.PSet(                                                                                
        record = cms.string('EcalTBWeightsRcd'),                                                      
        tag = cms.string('EcalTBWeights_EBEE_v01_offline')                                     
        ),                                                                                            
             cms.PSet(                                                                                
        record = cms.string('EcalTimeCalibConstantsRcd'),                                             
        tag = cms.string('EcalTimeCalibConstants_v02_offline')
        ),                                                                                            
             cms.PSet(                                                                                
        record = cms.string('EcalWeightXtalGroupsRcd'),                                               
        tag = cms.string('EcalWeightXtalGroups_EBEE_offline')                                   
        ),                                                                                            
             cms.PSet(                                                                   
        record = cms.string('EcalLaserAPDPNRatiosRcd'),                                               
        tag = cms.string('EcalLaserAPDPNRatios_p1p2p3_v2_mc')                                        
        ),                                                                                            
             ),                                                                                       
        connect = cms.string('frontier://FrontierProd/CMS_COND_31X_ECAL'),                            
              authenticationMethod = cms.untracked.uint32(0)                                          
    )                                                                                                 
                                                                                                      

    process.ecalConditions2 = cms.ESSource("PoolDBESSource",
                                           process.CondDBSetup,
                                           timetype = cms.string('runnumber'),
                                           toGet = cms.VPSet(
        cms.PSet(
        record = cms.string('EcalTPGCrystalStatusRcd'),
        tag = cms.string('EcalTPGCrystalStatus_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('EcalTPGFineGrainEBGroupRcd'),
        tag = cms.string('EcalTPGFineGrainEBGroup_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('EcalTPGFineGrainEBIdMapRcd'),
        tag = cms.string('EcalTPGFineGrainEBIdMap_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('EcalTPGFineGrainStripEERcd'),
        tag = cms.string('EcalTPGFineGrainStripEE_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('EcalTPGFineGrainTowerEERcd'),
        tag = cms.string('EcalTPGFineGrainTowerEE_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('EcalTPGLinearizationConstRcd'),
        tag = cms.string('EcalTPGLinearizationConst_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('EcalTPGLutGroupRcd'),
        tag = cms.string('EcalTPGLutGroup_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('EcalTPGLutIdMapRcd'),
        tag = cms.string('EcalTPGLutIdMap_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('EcalTPGPedestalsRcd'),
        tag = cms.string('EcalTPGPedestals_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('EcalTPGPhysicsConstRcd'),
        tag = cms.string('EcalTPGPhysicsConst_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('EcalTPGSlidingWindowRcd'),
        tag = cms.string('EcalTPGSlidingWindow_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('EcalTPGTowerStatusRcd'),
        tag = cms.string('EcalTPGTowerStatus_hlt')
        ),
        cms.PSet(
        record = cms.string('EcalTPGWeightGroupRcd'),
        tag = cms.string('EcalTPGWeightGroup_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('EcalTPGWeightIdMapRcd'),
        tag = cms.string('EcalTPGWeightIdMap_v2_hlt')
        ),
        ),
        connect = cms.string('frontier://FrontierProd/CMS_COND_34X_ECAL'),
               authenticationMethod = cms.untracked.uint32(0)
    )

    process.es_prefer_ecal1 = cms.ESPrefer("PoolDBESSource","ecalConditions1")                        
    process.es_prefer_ecal2 = cms.ESPrefer("PoolDBESSource","ecalConditions2")                        

                                                                                                      
    process.hcalConditions = cms.ESSource("PoolDBESSource",                                           
                                          process.CondDBSetup,                          
                                          timetype = cms.string('runnumber'),                         
                                          toGet = cms.VPSet(                                          
        cms.PSet(                                                                                     
        record = cms.string('HcalChannelQualityRcd'),                                                 
        tag = cms.string('HcalChannelQuality_v2.10_offline')                                          
        ),                                                                                            
        cms.PSet(                                                                                     
        record = cms.string('HcalElectronicsMapRcd'),                                                 
        tag = cms.string('HcalElectronicsMap_v7.03_hlt')                                              
        ),                                                                                            
        cms.PSet(                                                                                     
        record = cms.string('HcalGainsRcd'),                                                          
        tag = cms.string('HcalGains_v2.32_offline')                                                   
        ),                                                                                            
        cms.PSet(                                                                                     
        record = cms.string('HcalL1TriggerObjectsRcd'),                                               
        tag = cms.string('HcalL1TriggerObjects_v1.00_hlt')                                            
        ),                                                                                            
        cms.PSet(                                                                                     
        record = cms.string('HcalLUTCorrsRcd'),                                                       
        tag = cms.string('HcalLUTCorrs_v1.01_hlt')                                                    
        ),                                                                                            
        cms.PSet(                                                                                     
        record = cms.string('HcalPedestalsRcd'),                                                      
        tag = cms.string('HcalPedestals_ADC_v9.12_offline')                                        
        ),                                                                                            
        cms.PSet(                                                                                     
        record = cms.string('HcalPedestalWidthsRcd'),                                                 
        tag = cms.string('HcalPedestalWidths_ADC_v7.01_hlt')                                          
        ),                                                                                            
        cms.PSet(                                                                                     
        record = cms.string('HcalPFCorrsRcd'),                                                        
        tag = cms.string('HcalPFCorrs_v2.00_express')                                                 
        ),                                                                                            
        cms.PSet(                                                                                     
        record = cms.string('HcalQIEDataRcd'),                                                        
        tag = cms.string('HcalQIEData_NormalMode_v7.00_hlt')                                          
        ),                                                                                            
        cms.PSet(                                                                                     
        record = cms.string('HcalRespCorrsRcd'),                                                      
        tag = cms.string('HcalRespCorrs_v1.02_express')                                               
        ),                                                                                            
        cms.PSet(                                                                                     
        record = cms.string('HcalTimeCorrsRcd'),                                                      
        tag = cms.string('HcalTimeCorrs_v1.00_express')                                               
        ),                                                                                            
        cms.PSet(                                                                                     
        record = cms.string('HcalZSThresholdsRcd'),                                                   
        tag = cms.string('HcalZSThresholds_v1.01_hlt')                                                
        ),                                                                                            
        ),                                                                                            
             connect = cms.string('frontier://FrontierProd/CMS_COND_31X_HCAL'),                       
                      authenticationMethod = cms.untracked.uint32(0)                                  
    )                                                                                                 
                                                                                                      
    process.es_prefer_hcal = cms.ESPrefer("PoolDBESSource","hcalConditions")                          
                                                                                                      
    try: 
        process.ecalRecHit.ChannelStatusToBeExcluded = [ 1, 2, 3, 4, 8, 9, 10, 11, 12, 13, 14, 78, 142 ]  
    except:
        return(process)
 
    return(process)


# End of customisation function definition

process = customise(process)
