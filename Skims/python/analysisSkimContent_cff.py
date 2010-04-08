import FWCore.ParameterSet.Config as cms

analysisSkimContent = cms.PSet(
    outputCommands = cms.untracked.vstring('drop *',
      # event
      'keep *_offlinePrimaryVertices_*_*',
      'keep *_pixelVertices_*_*',
      'keep *_pixel3Vertices_*_*',                                     
      'keep *_offlineBeamSpot_*_*',
      'keep *_TriggerResults_*_HLT',
      'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*RECO',
      # jet
      'keep *Jet*_*CaloJet*_*_*',
      'keep *_*JetID*_*_*',
      'keep *Jet*_*JetExtender*_*_*',
      'keep *_towerMaker_*_*',
      'keep *_hcalnoise_*_*',
      # tracks
      'keep recoTracks_generalTracks_*_*RECO',
      'keep recoTracks_refitTracks_*_*',
      'keep recoRecoChargedCandidates_allTracks_*_*'  ,                                   
      # mc (if present)
      'keep *_*GenJet*_*_*',
      'keep reco*_genParticles_*_*',
      # clean up unwanted collections
      'drop *_*BJetTags*_*_*',
      'drop *_kt*Jet*_*_*',
      'drop *_sisCone*Jet*_*_*',
      'drop *_sc*Jet*_*_*'                                     
      )
    )
