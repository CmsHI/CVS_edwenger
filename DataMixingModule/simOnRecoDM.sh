#!/bin/sh

infile="file:DMPreProcess_RAW2DIGI.root"
outfile="SingleZmumu_MatchVertexDM_DIGI2RAW.root"

cmsDriver.py Configuration/GenProduction/SingleZmumu_FlatPt0to20_cfi -n 10 \
-s GEN,SIM,DIGI,DATAMIX,L1,DIGI2RAW \
--conditions FrontierConditions_GlobalTag,START38_V8::All \
--eventcontent RAWDEBUG \
--customise SimGeneral.DataMixingModule.customiseMatchRecVertex,SimGeneral.DataMixingModule.customiseMatchRecVertex,SimGeneral.DataMixingModule.DataMixer_DataConditions_3_8_X_data2010 \
--cust_function customiseMatchRecVertex,customiseSiStripRawDigi,customise \
--filein $infile --fileout $outfile --no_exec
