#!/bin/sh

infile="/store/relval/CMSSW_3_8_0/RelValPyquen_DiJet_pt80to120_2760GeV/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V7-v1/0008/FA2B0C51-C198-DF11-AB5A-0026189438F9.root"
outfile="dijet80_REDIGI_VR.root"

cmsDriver.py reDigi -n 1 \
-s DIGI,DIGI2RAW  --scenario HeavyIons --himix \
--conditions auto:mc \
--datatier 'GEN-SIM-RAW-RECO' --eventcontent=FEVTDEBUGHLT \
--filein=$infile --fileout=$outfile --processName 'reDIGI' \
--customise edwenger/Misc/customiseNZS.py \
--cust_function customiseStripAll \
--no_exec
