#!/bin/sh

infile="file:dijet80_REDIGI_VR_noHIP.root"
outfile="dijet80_VR_RECO.root"

cmsDriver.py vrReco -n 1 \
-s RAW2DIGI,RECO  --scenario HeavyIons --himix \
--conditions auto:mc \
--datatier 'RECO' --eventcontent=RECO \
--filein=$infile --fileout=$outfile --processName 'RECO' \
--customise edwenger/Misc/customiseCMN.py \
--cust_function customisePercentile \
--no_exec
