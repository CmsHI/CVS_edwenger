#!/bin/sh

infile="file:dijet80_ZS_RAW.root"
outfile="dijet80_ZS_RECO.root"

cmsDriver.py zsReco -n 1 \
-s RAW2DIGI,RECO  --scenario HeavyIons --himix \
--conditions auto:mc \
--datatier 'RECO' --eventcontent=RECO \
--filein=$infile --fileout=$outfile --processName 'zsRECO' \
--customise edwenger/Misc/customiseStripDB.py \
--cust_function customiseStripDB \
--no_exec
