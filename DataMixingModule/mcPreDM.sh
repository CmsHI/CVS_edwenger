#!/bin/sh

infile="/store/relval/CMSSW_3_8_5/RelValHydjetQ_B0_2760GeV/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V12-v1/0040/F69036A1-29D2-DF11-AD67-002618943845.root"

cmsDriver.py DMPreProcess -n 10 \
-s RAW2DIGI --eventcontent DATAMIXER \
--conditions FrontierConditions_GlobalTag,MC_38Y_V12::All \
--filein $infile --mc \
--customise SimGeneral.DataMixingModule.customiseMatchRecVertex,SimGeneral.DataMixingModule.DataMixer_DataConditions_3_8_X_data2010 \
--cust_function customiseDataMixerOutput,customise \
--no_exec