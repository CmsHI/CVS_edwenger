#!/bin/sh

infile="/store/express/Run2010B/ExpressPhysics/FEVT/Express-v2/000/146/417/10F981FF-5EC6-DF11-9657-0030486733B4.root"

cmsDriver.py DMPreProcess -n 10 \
-s RAW2DIGI --eventcontent DATAMIXER \
--conditions FrontierConditions_GlobalTag,GR_R_38X_V7::All \
--filein $infile --data \
--customise SimGeneral.DataMixingModule.customiseMatchRecVertex \
--cust_function customiseDataMixerOutput \
--no_exec