#!/bin/sh

infile="file:dijet80_REDIGI_VR_noHIP.root"
outfile="dijet80_VR_RECO.root"

cmsDriver.py vrReco -n 1 \
-s RAW2DIGI,RECO,ZSDIGI2RAW  --scenario HeavyIons --himix \
--conditions auto:mc \
--datatier 'GEN-SIM-RECO','GEN-SIM-ZSRAW' --eventcontent RECOSIM,ZSRAWSIM \
--filein=$infile --fileout=$outfile --processName 'RECO' \
--customise edwenger/Misc/customiseCMN.py \
--cust_function usePercentileAlgo \
--no_exec
