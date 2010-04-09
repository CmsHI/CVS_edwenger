import FWCore.ParameterSet.Config as cms

PromptReco7TeV = cms.PSet(
    fileNames = cms.untracked.vstring(
      #132440
      "/store/data/Commissioning10/MinimumBias/RECO/Apr1ReReco-v2/0139/3ADE63D6-923E-DF11-B92A-001A92971BD8.root"
      ),
    lumisToProcess = cms.untracked.VLuminosityBlockRange(
      '132440:123-132440:max',   # 
      '132471:132-132471:max',   # pixel/tracker HV turned on at LS 132
      '132474:1-132474:64',      # lumi scan in x (77-92) and in y (115-132).
      '132474:93-132474:108',    # after scan in x ?
      '132474:133-132474:152',   # after scan in y ?
      #'132569:17-132569:max',   # ECAL and CSC timing scans
      #'132599:1-132599:max',    # pixel HV scan throughout
      '132601:301-132601:max',   # pixel HV scan finished LS 300, lumi scan after 6am sometime
      '132605:1-132605:max',     # lumi scan around 5 hours into run, then CSC timing scan
      #'132656:1-132656:max',    # between runs with tracker scans and runs with HF scans (?)
      '132716:1-132716:max'      # lumi scan starting at LS 552
    )
)
