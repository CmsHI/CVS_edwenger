#!/bin/sh

infile="/store/express/Run2010B/ExpressPhysics/FEVT/Express-v2/000/146/417/10F981FF-5EC6-DF11-9657-0030486733B4.root"
outfile="DMRawSimOnReco_DIGI2RAW.root"

cmsDriver.py DMRawSimOnReco -n 10 \
-s RAW2DIGI,DATAMIX,L1,DIGI2RAW \
--datamix DataOnData --data \
--conditions FrontierConditions_GlobalTag,MC_38Y_V12::All \
--eventcontent RAWDEBUG \
--customise SimGeneral.DataMixingModule.customiseMatchRecVertex,SimGeneral.DataMixingModule.DataMixer_DataConditions_3_8_X_data2010 \
--cust_function customiseSiStripRawSignal,customise \
--filein $infile --fileout $outfile --no_exec
