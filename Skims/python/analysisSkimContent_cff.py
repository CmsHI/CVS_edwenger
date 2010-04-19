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
      'keep *_towerMaker_*_*',
      'keep *_selectedPatJets*_*_*',
      # tracks
      'keep recoTracks_generalTracks_*_*RECO',
      'keep recoTracks_refitTracks_*_*',
      'keep recoRecoChargedCandidates_allTracks_*_*'  ,                                   
      'keep recoTracks_refitTracks_*_*',
      'keep recoTracks_selectFake*_*_*',
      'keep recoTracks_selectReal*_*_*',
      #'keep *_trackingParticleRecoTrackAsssociation_*_*',
      # mc (if present)
      'keep *_*GenJet*_*_*',
      'keep reco*_genParticles_*_*',
      'keep GenEventInfoProduct_*_*_*',
      # clean up unwanted collections
      'drop *_*BJetTags*_*_*',
      'drop *_kt*Jet*_*_*',
      'drop *_sisCone*Jet*_*_*',
      'drop *_sc*Jet*_*_*'                                     
      )
    )
