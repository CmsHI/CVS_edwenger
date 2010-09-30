#!/bin/sh

infile="file:data_vrRECO_inREPACKRAW.root"
outfile="data_zsRECO.root"

cmsDriver.py zsRecoData -n -1 \
-s RAW2DIGI:RawToDigi_withVR,RECO --scenario HeavyIons --repacked \
--conditions FrontierConditions_GlobalTag,GR_R_39X_V0::All --data \
--datatier 'RECO' --eventcontent=RECO \
--filein=$infile --fileout=$outfile --processName 'zsRECO' \
--customise RecoLocalTracker/SiStripZeroSuppression/customiseCMN.py \
--cust_function customiseMergeCollections \
--no_exec
