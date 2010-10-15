#!/bin/sh

inputfile="/store/data/Run2010B/HeavyIonTest/RAW/v1/000/146/421/E68F7D2E-68C6-DF11-A4A6-0030487D05B0.root"

python $CMSSW_RELEASE_BASE/src/Configuration/DataProcessing/test/RunPromptReco.py \
--scenario=HeavyIons --global-tag=GR10_P_V10::All --lfn=$inputfile
