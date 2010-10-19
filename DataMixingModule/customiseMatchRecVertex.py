import FWCore.ParameterSet.Config as cms

##############################################################################

def customiseDataMixerOutput(process):
    process.DATAMIXERoutput.outputCommands.extend(
        ['keep *_offlinePrimaryVertices_*_*',
         'keep *_siStripDigis_VirginRaw_*'])
    return process

##############################################################################

def customiseCloneGenerator(process):
    process.generator = process.hiSignal.clone(embeddingMode=cms.bool(False))
    process.ProductionFilterSequence.replace(process.hiSignal,process.generator)
    return process

def customiseMatchRecVertex(process):
    process.load("SimGeneral.MixingModule.MatchRecVtx_cfi")
    process.pgen.replace(process.VertexSmearing,process.matchRecVtx)
    process.mixData.input.sequential = True
    return process

def customiseSiStripRawDigi(process):
    process.mixData.SiStripRawDigiSource = "PILEUP" # use raw digis from "PILEUP" or "SIGNAL"
    process.SiStripDigiToRaw.FedReadoutMode = cms.string('VIRGIN_RAW') # pack rawdigis as VR

    process.stripConditions = cms.ESSource("PoolDBESSource",
        process.CondDBSetup,
        timetype = cms.untracked.string('runnumber'),
        connect = cms.string('frontier://FrontierProd/CMS_COND_31X_STRIP'),
        toGet = cms.VPSet(cms.PSet(
          record = cms.string('SiStripFedCablingRcd'),
          tag = cms.string('SiStripFedCabling_GR10_v1_hlt')))
    )
    process.es_prefer_strips = cms.ESPrefer("PoolDBESSource","stripConditions")
    return process

def customiseCloneMatchRaw(process):
    customiseCloneGenerator(process)
    customiseMatchRecVertex(process)
    customiseSiStripRawDigi(process)
    return process

def customiseSiStripRawSignal(process):
    customiseSiStripRawDigi(process)
    process.mixData.SiStripRawDigiSource = "SIGNAL"
    process.mixData.ZDCdigiCollectionSig = ""
    process.mixData.ZDCPileInputTag = ""
    return process

##############################################################################

def customiseBeamSpot(process):
    # Loosen beamspot-dependent cuts
    # Probably better to do ESPrefer of custom offlineBeamSpot object
    process.hiSelectedProtoTracks.maxD0Significance = 1000.0
    process.hiSelectedProtoTracks.minZCut = 0.5
    process.hiSelectedTracks.d0_par2 = [300.0,0.3]
    process.hiSelectedTracks.dz_par2 = [300.0,0.3]
    #process.hiPixel3ProtoTracks.RegionFactoryPSet.RegionPSet.useFoundVertices = False
    return process
