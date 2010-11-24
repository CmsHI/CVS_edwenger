#!/bin/sh

infile="file:SingleZmumu_MatchVertexDM_DIGI2RAW.root"
outfile="hiRecoDM_RECO.root"

cmsDriver.py hiRecoDM -n 10 --scenario HeavyIons \
-s RAW2DIGI,L1Reco,RECO --processName TEST \
--datatier GEN-SIM-RECO --eventcontent FEVTDEBUG \
--customise SimGeneral.DataMixingModule.customiseMatchRecVertex,SimGeneral.DataMixingModule.DataMixer_DataConditions_3_8_X_data2010,Configuration.GlobalRuns.reco_TLR_39X \
--cust_function customiseBeamRaw,customise,customiseCommonHI \
--geometry DB --filein $infile --fileout $outfile \
--conditions START39_V7HI::All --no_exec
