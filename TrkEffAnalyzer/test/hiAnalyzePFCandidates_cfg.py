import FWCore.ParameterSet.Config as cms
process = cms.Process("ANALYSIS")

# Services
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration/StandardSequences/ReconstructionHeavyIons_cff')
process.load("RecoHI.Configuration.Reconstruction_hiPF_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'MC_37Y_V5::All'

process.MessageLogger.cerr.FwkReport.reportEvery=100
process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(True))

process.Timing = cms.Service("Timing")

#Input source
process.source = cms.Source (
    "PoolSource",    
    fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_3_7_0/RelValHydjetQ_B0_2760GeV/GEN-SIM-RECO/MC_37Y_V4-v1/0026/88958F00-8F69-DF11-846A-00261894383C.root'
    ),
    secondaryFileNames = cms.untracked.vstring(),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Filter on PF candidate pt
process.clones = cms.EDFilter("PtMinCandViewCloneSelector",
    src=cms.InputTag("particleFlow",""),
    ptMin=cms.double(15.0),
    filter=cms.bool(True)
)

# Filter on reco::Track pt
process.trkfilter = cms.EDFilter("PtMinTrackSelector",
                                 src = cms.InputTag("hiSelectedTracks"),
                                 ptMin = cms.double(15.0),
                                 filter = cms.bool(True),
                                 )

process.rechits = cms.Sequence(process.siPixelRecHits * process.siStripMatchedRecHits)
process.pfreco = cms.Sequence(process.rechits * process.heavyIonTracking * process.HiParticleFlowReco)
process.filter_seq = cms.Sequence(process.trkfilter * process.pfreco * process.clones)

# track association
from SimTracker.TrackAssociation.TrackAssociatorByHits_cfi import *
from SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi import *
TrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')

# PF candidate analyzer
process.pfCandidateAnalyzer = cms.EDAnalyzer("PFCandidateAnalyzer",
    PFCandidates = cms.InputTag("particleFlow",""),
    Vertices = cms.InputTag("hiSelectedVertex"),                                         
    verbose = cms.untracked.bool(False), ## print candidate info
    printBlocks = cms.untracked.bool(False), ## print block/element info
    ptMin = cms.untracked.double(15.0), ## of PF candidate
    SimTracks = cms.InputTag("mergedtruth","MergedTrackTruth"),
    Tracks = cms.InputTag("hiSelectedTracks"),                                         
    hasSimInfo = cms.untracked.bool(False),
    minHits = cms.untracked.double(5),
    maxPtErr = cms.untracked.double(0.05),
    maxD0 = cms.untracked.double(0.2),                                         
    maxDZ = cms.untracked.double(0.2),
    maxD0Norm = cms.untracked.double(3.0),
    maxDZNorm = cms.untracked.double(3.0),
    pixelSeedOnly = cms.untracked.bool(True)                                         
)

# edm output
process.aod = cms.OutputModule("PoolOutputModule",
    outputCommands=cms.untracked.vstring('drop *',
                                         'keep recoPFClusters_*CAL_*_*',
                                         'keep recoPFBlocks_*_*_*', 
                                         'keep recoPFCandidates_*__*',
                                         'keep recoTracks_generalTracks_*_*'),
    fileName = cms.untracked.string('aod.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('filter'))
)

# ntuple output
process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string("pftuple.root"))

# Paths

process.filter = cms.Path(process.filter_seq)
process.p = cms.Path(process.filter_seq
                     *process.pfCandidateAnalyzer)
#process.outpath = cms.EndPath(process.aod)

# Schedule
process.schedule = cms.Schedule(process.filter,
                                process.p,
                                #process.outpath
                                )


def customiseMC(process):
    process.TFileService.fileName="pftupleMC.root"     # new ntuple name
    process.source.fileNames= [
        #'/store/relval/CMSSW_3_7_0/RelValHydjetQ_B0_2760GeV/GEN-SIM-RECO/MC_37Y_V4-v1/0026/88958F00-8F69-DF11-846A-00261894383C.root'
        '/store/relval/CMSSW_3_7_0/RelValPyquen_DiJet_pt80to120_2760GeV/GEN-SIM-RECO/MC_37Y_V4-v1/0026/E61F5D5C-726A-DF11-B4BE-002618943807.root'
        #'file:../../../RecoHI/Configuration/test/hydjetMB_PFRECO.root'
        ]
        
    #process.pfCandidateAnalyzer.hasSimInfo=cms.untracked.bool(True)
    
    return process

def customiseLooseCuts(process):
    process.pfCandidateAnalyzer.minHits=0
    process.pfCandidateAnalyzer.maxPtErr=1.0
    process.pfCandidateAnalyzer.maxD0=10.0
    process.pfCandidateAnalyzer.maxDZ=10.0
    process.pfCandidateAnalyzer.maxD0Norm=100.0
    process.pfCandidateAnalyzer.maxDZNorm=100.0
    process.pfCandidateAnalyzer.pixelSeedOnly=False

    return process


process = customiseMC(process)
process = customiseLooseCuts(process)
