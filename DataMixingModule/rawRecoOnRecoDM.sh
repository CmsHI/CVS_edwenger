#!/bin/sh

infile="/store/data/Run2010B/Photon/RAW-RECO/v2/000/146/644/B8A9A7E7-11CB-DF11-8BAF-1CC1DE04FF98.root"
outfile="DMRawRecoOnReco_DIGI2RAW.root"

cmsDriver.py DMRawRecoOnReco -n 10 \
-s RAW2DIGI,DATAMIX,L1,DIGI2RAW,HLT:HIon \
--datamix DataOnData --data --processName MIX \
--conditions GR_R_38X_V15::All \
--eventcontent FEVT \
--customise SimGeneral.DataMixingModule.customiseMatchRecVertex \
--cust_function customiseSiStripRawDigi \
--filein $infile --fileout $outfile --no_exec
