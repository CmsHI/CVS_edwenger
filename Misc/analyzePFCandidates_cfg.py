import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYSIS")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)


process.source = cms.Source (
    "PoolSource",    
    fileNames = cms.untracked.vstring(
        #'file:display.root'
        '/store/data/Commissioning10/MinimumBias/RECO/Apr1ReReco-v2/0139/3ADE63D6-923E-DF11-B92A-001A92971BD8.root'
      ),
    secondaryFileNames = cms.untracked.vstring(),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
    )


process.pfCandidateAnalyzer = cms.EDAnalyzer("PFCandidateAnalyzer",
    PFCandidates = cms.InputTag("particleFlow"),
    verbose = cms.untracked.bool(True), ## print candidate info
    printBlocks = cms.untracked.bool(True) ## print block/element info
)

#process.load("FastSimulation.Configuration.EventContent_cff")
process.aod = cms.OutputModule("PoolOutputModule",
    #process.AODSIMEventContent,
    fileName = cms.untracked.string('aod.root')
)

process.outpath = cms.EndPath(process.aod )


process.p = cms.Path(process.pfCandidateAnalyzer)


