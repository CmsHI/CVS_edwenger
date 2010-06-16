import FWCore.ParameterSet.Config as cms
process = cms.Process("ANALYSIS")

process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery=100
process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(True))

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'GR_R_35X_V7A::All' #357
process.GlobalTag.globaltag ='GR_R_35X_V8B::All' #358p3

#from edwenger.TrkEffAnalyzer.mb_c10_m6rr_files_cfi import *
#from edwenger.TrkEffAnalyzer.jmt_r10a_pr1_files_cfi import *
process.source = cms.Source (
    "PoolSource",    
    #fileNames = readFiles,
    fileNames = cms.untracked.vstring(
    '/store/data/Commissioning10/MinimumBias/RECO/May6thReReco-v1/0116/A2EC56ED-3A5C-DF11-AE04-002618943832.root',
    '/store/data/Commissioning10/MinimumBias/RECO/May6thReReco-v1/0110/966DE744-A959-DF11-949F-001A92971B90.root',
    '/store/data/Commissioning10/MinimumBias/RECO/May6thReReco-v1/0110/6CCAB9F0-AC59-DF11-A2F7-002618943948.root'
    ),
    lumisToProcess = cms.untracked.VLuminosityBlockRange('132440:157-132440:378'),
    secondaryFileNames = cms.untracked.vstring(),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
)

process.clones = cms.EDFilter("PtMinCandViewCloneSelector",
    src=cms.InputTag("particleFlow",""),
    ptMin=cms.double(5.0),
    filter=cms.bool(True)
)

process.pfCandidateAnalyzer = cms.EDAnalyzer("PFCandidateAnalyzer",
    PFCandidates = cms.InputTag("particleFlow",""),
    Vertices = cms.InputTag("sortedGoodVertices"),                                         
    verbose = cms.untracked.bool(False), ## print candidate info
    printBlocks = cms.untracked.bool(False), ## print block/element info
    ptMin = cms.untracked.double(5.0)                                         
)


process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string("pftuple.root"))

process.aod = cms.OutputModule("PoolOutputModule",
    outputCommands=cms.untracked.vstring('drop *',
                                         'keep recoPFClusters_*CAL_*_*',
                                         'keep recoPFBlocks_*_*_*', 
                                         'keep recoPFCandidates_*__*',
                                         'keep recoTracks_generalTracks_*_*'),
    fileName = cms.untracked.string('aod.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('filter'))
)

process.load("edwenger.Skims.EventFilter_cff")
process.load("edwenger.Skims.ExtraReco_cff")
process.eventFilter *= process.primaryVertexFilter
process.eventFilter *= process.clones
process.filter = cms.Path(process.eventFilter)
process.p = cms.Path(process.eventFilter
                     *process.goodVertices
                     *process.sortedGoodVertices
                     *process.pfCandidateAnalyzer)
#process.outpath = cms.EndPath(process.aod)
process.schedule = cms.Schedule(process.filter,
                                process.p)
                                #process.outpath)


