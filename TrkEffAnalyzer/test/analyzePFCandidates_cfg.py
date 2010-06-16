import FWCore.ParameterSet.Config as cms
process = cms.Process("ANALYSIS")

process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery=100
process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(True))

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#from edwenger.TrkEffAnalyzer.mb_c10_m6rr_files_cfi import *
#from edwenger.TrkEffAnalyzer.jmt_r10a_pr1_files_cfi import *
process.source = cms.Source (
    "PoolSource",    
    #fileNames = readFiles,
    fileNames = cms.untracked.vstring('/store/data/Commissioning10/MinimumBias/RECO/Apr20ReReco-v1/0164/D237203A-DC4C-DF11-BBF5-0018F3D095FC.root'),
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

process.filter = cms.Path(process.clones)
process.p = cms.Path(process.clones*process.pfCandidateAnalyzer)
#process.outpath = cms.EndPath(process.aod)
process.schedule = cms.Schedule(process.filter,
                                process.p)
                                #process.outpath)


