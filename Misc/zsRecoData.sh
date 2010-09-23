#!/bin/sh

infile="file:data_VR_RECO_inREPACKRAW.root"
outfile="data_ZS_RECO.root"

cmsDriver.py zsRecoData -n -1 \
-s RAW2DIGI:Configuration/StandardSequences/RawToDigi_Repacked_cff.RawToDigi,RECO \
--scenario HeavyIons \
--conditions FrontierConditions_GlobalTag,GR10_E_V9::All --data \
--datatier 'RECO' --eventcontent=RECO \
--filein=$infile --fileout=$outfile --processName 'zsRECO' \
--no_exec
