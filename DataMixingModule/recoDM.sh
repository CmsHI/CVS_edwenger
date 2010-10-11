#!/bin/sh

#infile="file:SinglePiPt100_cfi_GEN_SIM_DIGI_DATAMIX_L1_DIGI2RAW.root"
infile="file:SingleZmumu_FlatPt0to20_cfi_GEN_SIM_DIGI_DATAMIX_L1_DIGI2RAW.root"

cmsDriver.py step2 -n 1 --scenario HeavyIons \
-s RAW2DIGI,L1Reco,RECO --processName TEST \
--datatier GEN-SIM-RECO --eventcontent FEVTDEBUG \
--customise SimGeneral/DataMixingModule/DataMixer_DataConditions_3_8_X_data2010.py \
--geometry DB --filein $infile \
--conditions FrontierConditions_GlobalTag,START38_V8::All --no_exec
