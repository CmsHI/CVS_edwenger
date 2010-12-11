import FWCore.ParameterSet.Config as cms

process = cms.Process("DEDX")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.load("RecoTracker.DeDx.dedxEstimators_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'MC_38Y_V8::All'
process.GlobalTag.globaltag = 'GR10_P_V12::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1)) 

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    
    '/store/hidata/HIRun2010/HICorePhysics/RECO/PromptReco-v3/000/151/353/FCB9D5C6-2EF2-DF11-B795-003048D2BE12.root'
    
    )
)

#process.Timing = cms.Service("Timing")

## Redo tracking (trajectories needed by dE/dx) and low-pt pixel tracking
process.rechits = cms.Sequence(process.siPixelRecHits*process.siStripMatchedRecHits)

process.hiLowPtPixelTracks = process.hiPixel3PrimTracks.clone()
process.hiLowPtPixelTracks.RegionFactoryPSet.RegionPSet.ptMin = cms.double(0.2)
process.hiLowPtPixelTracks.FilterPSet.ptMin = cms.double(0.2)
process.hiLowPtPixelTracks.FilterPSet.nSigmaTipMaxTolerance = cms.double(4.0)
process.hiLowPtPixelTracks.FilterPSet.tipMax = cms.double(0.2)
process.hiLowPtPixelTracks.FilterPSet.nSigmaLipMaxTolerance = cms.double(4.0)
process.hiLowPtPixelTracks.FilterPSet.lipmax = cms.double(0.2)
process.hiLowPtPixelTracks.OrderedHitsFactoryPSet.GeneratorPSet.maxElement = 5000000

process.load("Appeltel.PixelTracksRun2010.HILowPtConformalPixelTracks_cfi")

#pxlTrkCollection="hiLowPtPixelTracks"
pxlTrkCollection="hiConformalPixelTracks"

#trackCollection = "refittedPixelTracks"
#trackCollection = "hiLowPtPixelTracks"
trackCollection = "hiSelectedTracks"

process.refittedTracks = process.TrackRefitter.clone(
   #src = pxlTrkCollection
   src = trackCollection
   )

## dE/dx estimators for pixel tracks
process.dedxPTHarm2 = cms.EDProducer("DeDxEstimatorProducerPixelTripplet",
    tracks                     = cms.InputTag(pxlTrkCollection),
    trajectoryTrackAssociation = cms.InputTag(pxlTrkCollection),

    estimator      = cms.string('generic'),
    exponent       = cms.double(-2.0),

    UseStrip       = cms.bool(False),
    UsePixel       = cms.bool(True),
    MeVperADCStrip = cms.double(3.61e-06*250),
    MeVperADCPixel = cms.double(3.61e-06),

    MisCalib_Mean      = cms.untracked.double(1.0),
    MisCalib_Sigma     = cms.untracked.double(0.00),

    UseCalibration  = cms.bool(False),
    calibrationPath = cms.string(""),
    ShapeTest       = cms.bool(False),
)

process.dedxPTTru40 = cms.EDProducer("DeDxEstimatorProducerPixelTripplet",
    tracks                     = cms.InputTag(pxlTrkCollection),
    trajectoryTrackAssociation = cms.InputTag(pxlTrkCollection),

    estimator      = cms.string('truncated'),
    fraction       = cms.double(0.4),

    UseStrip       = cms.bool(False),
    UsePixel       = cms.bool(True),
    MeVperADCStrip = cms.double(3.61e-06*250),
    MeVperADCPixel = cms.double(3.61e-06),

    MisCalib_Mean      = cms.untracked.double(1.0),
    MisCalib_Sigma     = cms.untracked.double(0.00),

    UseCalibration  = cms.bool(False),
    calibrationPath = cms.string(""),
    ShapeTest       = cms.bool(False),
)

## dE/dx estimators for full tracks
process.dedxHarmonic2.tracks = cms.InputTag(trackCollection)
process.dedxHarmonic2.trajectoryTrackAssociation = cms.InputTag(trackCollection)

process.dedxTruncated40.tracks = cms.InputTag(trackCollection)
process.dedxTruncated40.trajectoryTrackAssociation = cms.InputTag(trackCollection)

process.dedxMedian.tracks = cms.InputTag(trackCollection)
process.dedxMedian.trajectoryTrackAssociation = cms.InputTag(trackCollection)

## Caio's analyzer
process.ana = cms.EDAnalyzer('DeDxHi',
                             TrackCollection = cms.InputTag(trackCollection),
                             TrackDeDxMap = cms.InputTag("dedxHarmonic2"),
                             )

process.anapxl = process.ana.clone(
    TrackCollection = cms.InputTag(pxlTrkCollection),
    TrackDeDxMap = cms.InputTag("dedxPTHarm2"))

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('dEdx_HI_output.root'))


## Paths and EndPaths
process.rereco_step = cms.Path(process.rechits *
                               #process.heavyIonTracking *
                               #process.refittedTracks *
                               #process.hiLowPtPixelTracks *
                               process.hiConformalPixelTracks)

process.dedx_step = cms.Path(#process.doAlldEdXEstimators *
                             process.dedxPTHarm2 *
                             process.dedxPTTru40)

process.ana_step = cms.Path(#process.ana *
                            process.anapxl)

## Schedule
process.schedule = cms.Schedule(process.rereco_step,process.dedx_step,process.ana_step)
