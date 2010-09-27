#!/bin/sh

infile="file:data_VR_RECO_inREPACKRAW.root"
outfile="data_ZS_RECO.root"

cmsDriver.py zsRecoData -n -1 \
-s RAW2DIGI,RECO --scenario HeavyIons --repacked \
--conditions FrontierConditions_GlobalTag,GR_R_39X_V0::All --data \
--datatier 'RECO' --eventcontent=RECO \
--filein=$infile --fileout=$outfile --processName 'zsRECO' \
--no_exec
