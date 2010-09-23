#!/bin/sh

infile="/store/express/Run2010B/ExpressPhysics/FEVT/Express-v2/000/146/417/10F981FF-5EC6-DF11-9657-0030486733B4.root"
outfile="data_VR_RECO.root"

cmsDriver.py vrRecoData -n -1 \
-s RAW2DIGI,RECO,ZSDIGI2RAW  --scenario HeavyIons \
--conditions FrontierConditions_GlobalTag,GR10_E_V9::All --data \
--datatier 'RECO','ZSRAW' --eventcontent RECO,ZSRAW \
--filein=$infile --fileout=$outfile --processName 'RERECO' \
--customise RecoLocalTracker/SiStripZeroSuppression/customiseCMN.py \
--cust_function customiseIteratedMedian \
--no_exec
