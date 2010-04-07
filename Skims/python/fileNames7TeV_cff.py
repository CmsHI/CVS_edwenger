import FWCore.ParameterSet.Config as cms

PromptReco7TeV = cms.PSet(
    fileNames = cms.untracked.vstring(
      #132440 LS 140-375
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/0AA7C390-0F3C-DF11-BD65-000423D998BA.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/26C8DED9-0E3C-DF11-9D83-0030487CD7B4.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/6A3533F5-103C-DF11-B3AA-00304879BAB2.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/762824C3-0C3C-DF11-A4FD-0030487CD6D2.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/80471A6B-0E3C-DF11-8DCD-0030487C6A66.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/8C042B04-2D3C-DF11-939F-0030487CD178.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/8C51BAC6-1A3C-DF11-A0EE-000423D94A04.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/8CAE3014-133C-DF11-A05D-000423D174FE.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/90269E76-0D3C-DF11-A1A0-0030487CD840.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/9884BC11-0C3C-DF11-8F9C-000423D986C4.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/A860D55E-193C-DF11-BE29-0030487C60AE.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/B647CAD9-0E3C-DF11-886F-0030487CD716.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/D87D5469-2E3C-DF11-A470-000423D99896.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/E27821C3-0C3C-DF11-9BD9-0030487CD718.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/F427D642-173C-DF11-A909-0030487C60AE.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/F4C92A98-163C-DF11-9788-0030487C7392.root",
      #132471 LS 132-361
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/041DF205-193D-DF11-85B7-00304879FA4A.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/06FC0687-153D-DF11-8ECB-001D09F2462D.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/0832D4BE-193D-DF11-8366-001D09F2305C.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/0C2FD047-113D-DF11-B76C-0030487A18F2.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/2053984F-183D-DF11-9765-001D09F24489.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/222EB746-113D-DF11-922B-0030487A1884.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/243F2099-103D-DF11-8906-000423D9890C.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/269BCFBE-193D-DF11-BA90-001D09F276CF.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/2ED8CE9A-173D-DF11-B369-001D09F292D1.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/344A9604-193D-DF11-836F-00304879FA4C.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/5294DF9A-173D-DF11-86A4-001D09F28D4A.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/6451CB15-143D-DF11-BE00-0030487A3C92.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/74B27297-103D-DF11-9968-0030487C6090.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/74FB4F85-153D-DF11-B1C2-0030487C90C4.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/7C4AA364-133D-DF11-A764-001617E30D4A.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/82A54ABF-193D-DF11-B896-0019B9F730D2.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/A02097E3-0F3D-DF11-AFDE-0030487CAEAC.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/A2CCB5BB-103D-DF11-8821-0030487C5CFA.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/A8AC64BE-193D-DF11-B767-001D09F24E39.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/B4CC89EF-143D-DF11-8820-001D09F2514F.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/CE183D99-103D-DF11-8C68-00304879EE3E.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/CE36029B-173D-DF11-8743-001D09F24664.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/D67AE939-1D3D-DF11-AB0D-000423D98868.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/E65B765C-133D-DF11-9163-001617E30CC8.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/EAD8BF6F-213D-DF11-A988-001617E30E28.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/EE6BEB84-1C3D-DF11-9A95-0030487CD16E.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/EE6F0889-153D-DF11-963C-0030487CD7EA.root",
      "/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/FE37E14D-183D-DF11-8EF5-001D09F24DA8.root"
      ),
    lumisToProcess = cms.untracked.VLuminosityBlockRange(
      '132440:140-132440:375',   # beamspot movement between 123 and 140
      '132471:132-132471:361',   # pixel/tracker HV turned on at LS 132
      '132474:1-132474:max',     # lumi scan about 30-60 minutes into run
      #'132569:17-132569:max',   # ECAL and CSC timing scans
      #'132599:1-132599:max',    # pixel HV scan throughout
      '132601:300-132601:max',   # pixel HV scan finished LS 300, lumi scan after 6am sometime
      '132605:1-132605:max'      # lumi scan around 5 hours into run, then CSC timing scan 
    )
)
