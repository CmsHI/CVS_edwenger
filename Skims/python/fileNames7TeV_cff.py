import FWCore.ParameterSet.Config as cms

PromptReco7TeV = cms.PSet(
    fileNames = cms.untracked.vstring(
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/E27821C3-0C3C-DF11-9BD9-0030487CD718.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/B647CAD9-0E3C-DF11-886F-0030487CD716.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/9884BC11-0C3C-DF11-8F9C-000423D986C4.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/90269E76-0D3C-DF11-A1A0-0030487CD840.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/80471A6B-0E3C-DF11-8DCD-0030487C6A66.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/762824C3-0C3C-DF11-A4FD-0030487CD6D2.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/4C8979D2-073C-DF11-B97B-000423D6AF24.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/181C44F7-093C-DF11-A9CB-001D09F24FEC.root',
      '/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/A8AC64BE-193D-DF11-B767-001D09F24E39.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/B4CC89EF-143D-DF11-8820-001D09F2514F.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/CE183D99-103D-DF11-8C68-00304879EE3E.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/CE36029B-173D-DF11-8743-001D09F24664.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/D67AE939-1D3D-DF11-AB0D-000423D98868.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/E65B765C-133D-DF11-9163-001617E30CC8.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/EAD8BF6F-213D-DF11-A988-001617E30E28.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/EE6BEB84-1C3D-DF11-9A95-0030487CD16E.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/EE6F0889-153D-DF11-963C-0030487CD7EA.root',
      #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/FE37E14D-183D-DF11-8EF5-001D09F24DA8.root'
    ),
    lumisToProcess = cms.untracked.VLuminosityBlockRange(
      '132440:140-132440:375','132741:132-132741:361'
    )
)
