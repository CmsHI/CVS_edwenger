#!/bin/sh

infile="file:data_VR_RECO_inZSRAW.root"
outfile="data_ZS_RECO.root"

cmsDriver.py zsRecoData -n -1 \
-s RAW2DIGI,RECO  --scenario HeavyIons \
--conditions FrontierConditions_GlobalTag,GR10_E_V9::All --data --zs \
--datatier 'RECO' --eventcontent=RECO \
--filein=$infile --fileout=$outfile --processName 'zsRECO' \
--no_exec
