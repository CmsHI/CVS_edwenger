#!/bin/sh

# possible input files for testing
infileMB="/store/relval/CMSSW_3_9_0_pre7/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RECO/MC_39Y_V2-v1/0045/E0991CAE-B5D3-DF11-A19E-0026189438E4.root"
infileB0="/store/relval/CMSSW_3_9_0_pre7/RelValHydjetQ_B0_2760GeV/GEN-SIM-RECO/MC_39Y_V2-v1/0045/8699918A-B4D3-DF11-8C61-002618943972.root"
infileJet80="/store/relval/CMSSW_3_9_0_pre6/RelValPyquen_DiJet_pt80to120_2760GeV/GEN-SIM-RECO/MC_39Y_V1-v1/0049/4CCBCCF6-40D5-DF11-94A0-00248C0BE014.root"
infilePhoton20="/store/relval/CMSSW_3_9_0_pre6/RelValPyquen_GammaJet_pt20_2760GeV/GEN-SIM-RECO/MC_39Y_V1-v1/0049/C27074AA-42D5-DF11-8007-0026189438B4.root"
infileZmmee10="/store/relval/CMSSW_3_9_0_pre6/RelValPyquen_ZeemumuJets_pt10_2760GeV/GEN-SIM-RECO/MC_39Y_V1-v1/0049/96086977-42D5-DF11-9206-0026189438B4.root"

# cmsDriver command for Central Skims
cmsDriver.py centralSkims -s SKIM:DiJet+Photon+ZMM \
--conditions auto:mc --scenario HeavyIons \
--filein=$infileMB --no_exec
