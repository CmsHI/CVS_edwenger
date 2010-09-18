#!/bin/sh

infile="file:dijet80_VR_RECO_inZSRAW.root"
outfile="dijet80_ZS_RECO.root"

cmsDriver.py zsReco -n 1 \
-s RAW2DIGI,RECO  --scenario HeavyIons --himix \
--conditions auto:mc \
--datatier 'GEN-SIM-RECO' --eventcontent=RECOSIM \
--filein=$infile --fileout=$outfile --processName 'zsRECO' \
--customise edwenger/Misc/customiseStripDB.py \
--cust_function customiseStripDB \
--no_exec
