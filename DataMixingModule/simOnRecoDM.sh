#!/bin/sh

infile="file:DMPreProcess_RAW2DIGI.root"

cmsDriver.py Configuration/GenProduction/SingleZmumu_FlatPt0to20_cfi -n 1 \
-s GEN:hiSignal,SIM,DIGI,DATAMIX,L1,DIGI2RAW \
--conditions FrontierConditions_GlobalTag,START38_V8::All \
--eventcontent FEVTDEBUG \
--customise SimGeneral/DataMixingModule/DataMixer_DataConditions_3_8_X_data2010.py \
--filein $infile --no_exec
