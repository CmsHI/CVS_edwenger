import FWCore.ParameterSet.Config as cms

PromptReco7TeV = cms.PSet(
    
    fileNames = cms.untracked.vstring(
      "/store/data/Commissioning10/MinimumBias/RECO/Apr1ReReco-v2/0139/3ADE63D6-923E-DF11-B92A-001A92971BD8.root" #132440
      ),
    
    # from april 15th signoff in run registry
    # NEW RUNS added wrt previous JSON:  [132646- 132716]
    # OLD RUNS REVISION (Runs <= 132606):
    #  - Re-inserted as GOOD LS Ranges with HF LUMI SCANS (132442, 132476, 132477).
    #  - Re-inserted as GOOD LS Ranges with Pix Bias SCAN (132601, 132599).
    #  - RPC Runs 132471, 132473, 132474, 132476, 132477, 132478 become BAD for RPC (RPC Endcap- BAD)
    #  - Run 132569 set BAD for ECAL
    
    lumisToProcess = cms.untracked.VLuminosityBlockRange(
      '132440:85-132440:138',
      '132440:141-132440:401',
      '132596:382-132596:383',
      '132596:447-132596:453',
      '132598:80-132598:82',
      '132598:174-132598:188',
      '132599:1-132599:379',
      '132599:381-132599:538',
      '132601:1-132601:207',
      '132601:209-132601:259',
      '132601:261-132601:1131',
      '132602:1-132602:83',
      '132605:1-132605:444',
      '132605:446-132605:622',
      '132605:624-132605:829',
      '132605:831-132605:968',
      '132606:1-132606:37',
      '132656:1-132656:140',
      '132658:1-132658:177',
      '132659:1-132659:84',
      '132661:1-132661:130',
      '132662:1-132662:130',
      '132662:132-132662:217',
      '132716:220-132716:591',
      '132716:593-132716:640',
    )
    
)
