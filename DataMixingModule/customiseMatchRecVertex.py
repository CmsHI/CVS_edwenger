import FWCore.ParameterSet.Config as cms

##############################################################################
def customiseDataMixerOutput(process):

    process.DATAMIXERoutput.outputCommands.extend(
        ['keep *_offlinePrimaryVertices_*_*',
         'keep *_siStripDigis_VirginRaw_*'])

    return process

##############################################################################
def customiseMatchRecVertex(process):

    process.load("SimGeneral.MixingModule.MatchRecVtx_cfi")
    process.pgen.replace(process.VertexSmearing,process.matchRecVtx)
    process.mixData.input.sequential = True

    return process

##############################################################################
def customiseSiStripRawDigi(process):

    process.mixData.SiStripRawDigiBkgd = True # use VirginRaw background digis
    process.SiStripDigiToRaw.FedReadoutMode = cms.string('VIRGIN_RAW') # pack rawdigis as VR

    return process

##############################################################################
def customiseBeamSpot(process):

    # Loosen beamspot-dependent cuts
    # Probably better to do ESPrefer of custom offlineBeamSpot object
    
    #process.hiPixel3ProtoTracks.RegionFactoryPSet.RegionPSet.useFoundVertices = False
    process.hiSelectedProtoTracks.maxD0Significance = 1000.0
    process.hiSelectedProtoTracks.minZCut = 0.5
    process.hiSelectedTracks.d0_par2 = [300.0,0.3]
    process.hiSelectedTracks.dz_par2 = [300.0,0.3]

    return process
