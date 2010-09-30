#!/bin/sh

infile="/store/express/Run2010B/ExpressPhysics/FEVT/Express-v2/000/146/417/10F981FF-5EC6-DF11-9657-0030486733B4.root"
outfile="data_reRECO.root"

cmsDriver.py reRecoData -n -1 \
-s RAW2DIGI,RECO --scenario HeavyIons \
--conditions FrontierConditions_GlobalTag,GR_R_39X_V0::All --data \
--datatier 'RECO' --eventcontent RECO \
--filein=$infile --fileout=$outfile --processName 'reRECO' \
--no_exec
