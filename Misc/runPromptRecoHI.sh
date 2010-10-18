#!/bin/sh

inputfile="/store/data/Run2010B/HeavyIonTest/RAW/v1/000/146/421/E68F7D2E-68C6-DF11-A4A6-0030487D05B0.root"

# using 39x reprocessing tag until i see a prompt tag for 390

python $CMSSW_RELEASE_BASE/src/Configuration/DataProcessing/test/RunPromptReco.py \
--scenario=HeavyIons --reco --alcareco --global-tag=GR_R_39X_V1::All --lfn=$inputfile
