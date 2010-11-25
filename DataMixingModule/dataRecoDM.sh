#!/bin/sh

infile="file:DMRawRecoOnReco_DIGI2RAW.root"
outfile="hiRecoDM_RECO.root"

cmsDriver.py hiRecoDM -n 10 --scenario HeavyIons \
-s RAW2DIGI,L1Reco,RECO --processName TEST --data \
--datatier GEN-SIM-RECO --eventcontent RECO \
--customise SimGeneral.DataMixingModule.customiseMatchRecVertex,Configuration.GlobalRuns.reco_TLR_39X \
--cust_function customiseVtxPP,customiseCommonHI \
--geometry DB --filein $infile --fileout $outfile \
--conditions GR_R_39X_V3::All --no_exec
