import FWCore.ParameterSet.Config as cms

PromptReco7TeV = cms.PSet(
    
    fileNames = cms.untracked.vstring(
      "/store/data/Commissioning10/MinimumBias/RECO/Apr1ReReco-v2/0139/3ADE63D6-923E-DF11-B92A-001A92971BD8.root" #132440
      ),
    
    # latest good run/lumi lists and JSON files can be found here:
    # https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/

    # no longer explicitly setting allowed lumi block ranges
    # defer to JSON file for CRAB production
    
    #lumisToProcess = cms.untracked.VLuminosityBlockRange()
    
)
