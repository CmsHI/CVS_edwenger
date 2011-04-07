def customise_MC_on_Data_RECO(process):

    # prepend mixing step to remake crossing frame of simdigis in input file
    print "Prepend 'mix' path to schedule to remake crossing frame of simdigis..."
    process.crossing_step = cms.Path(process.mix)
    process.schedule.insert(0,process.crossing_step)

    # specify the secondary source input, which must line up event by event with pool source
    print "Ensure that we are mixing digis with the same background event as the signal was matched to..."
    process.mixData.input.sequential = True # overlay signal on same background event used to match vertex 
    #process.mixData.input.fileNames = process.source.fileNames # if all objects were kept in DIGI file
    process.mixData.input.fileNames = ["secInput.root"] # this needs to be changed to the background digi file

    # mixing is done with VR background
    print "Mixing of silicon tracker digis done with Virgin Raw background..."
    process.mixData.SiStripRawDigiSource = "PILEUP" # use raw digis from "PILEUP" or "SIGNAL"
    process.SiStripDigiToRaw.FedReadoutMode = cms.string('VIRGIN_RAW') # pack rawdigis as VR

    # custom beamspot shifted into MC coordinate frame
    print "Using custom beamspot from data shifted into MC frame..."
    process.GlobalTag.toGet = cms.VPSet(
        cms.PSet(record = cms.string("BeamSpotObjectsRcd"),
                 tag = cms.string("Realistic2.76ATeVCollisions_STARTUP_v0_mc"),
                 connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_BEAMSPOT")
                 )
        )

    # Dummy GEN -> RECO vertex
    print "Using dummy reco vertex based on smeared GEN position..."
    process.hiSelectedVertex = cms.EDProducer("GenToRecoVtxProducer",
        signalLabel=cms.InputTag("generator"),
        dummyVtxError=cms.vdouble(0.0,0.0,0.0),
        useBkgdVtxError=cms.bool(True),
        bkgdVtxLabel=cms.InputTag("hiSelectedVertex"),     
        smearVtx=cms.bool(True)
    )

    # SiStrip conditions from data
    print "Using SiStrip conditions from data tags..."
    process.stripConditions = cms.ESSource("PoolDBESSource",
    process.CondDBSetup,
    timetype = cms.untracked.string('runnumber'),
    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_STRIP'),
    toGet = cms.VPSet(
        cms.PSet(
        record = cms.string('SiStripNoisesRcd'),
        tag = cms.string('SiStripNoise_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripPedestalsRcd'),
        tag = cms.string('SiStripPedestals_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripFedCablingRcd'),
        tag = cms.string('SiStripFedCabling_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripBadChannelRcd'),
        tag = cms.string('SiStripBadChannel_FromOnline_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripLatencyRcd'),
        tag = cms.string('SiStripLatency_GR10_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripThresholdRcd'),
        tag = cms.string('SiStripThreshold_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripBadFiberRcd'),
        tag = cms.string('SiStripBadChannel_FromOfflineCalibration_GR10_v2_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripBadModuleRcd'),
        tag = cms.string('SiStripBadChannel_FromEfficiencyAnalysis_GR10_v1_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripConfObjectRcd'),
        tag = cms.string('SiStripShiftAndCrosstalk_GR10_v1_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripLorentzAngleRcd'),
        tag = cms.string('SiStripLorentzAngle_GR10_v1_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripApvGain2Rcd'),
        tag = cms.string('SiStripApvGain_FromParticles_GR10_v2_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripApvGainRcd'),
        tag = cms.string('SiStripApvGain_GR10_v1_hlt')
        )
        )
    )
    
    process.es_prefer_strips = cms.ESPrefer("PoolDBESSource","stripConditions")

    # calorimeter conditions from data
    print "Using calorimeter conditions from data tags..."
    from SimGeneral.DataMixingModule.DataMixer_DataConditions_3_8_X_data2010 import customise
    process=customise(process)
    
    return process
