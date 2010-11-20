#!/bin/sh

cmsDriver.py DMPreProcess -n 10 \
-s RAW2DIGI --eventcontent DATAMIXER \
--conditions FrontierConditions_GlobalTag,GR10_P_V12::All --data \
--dbsquery "find file,file.parent where run=151353 and lumi=100 and dataset like *HICorePhysics*/RECO" \
--customise SimGeneral.DataMixingModule.customiseMatchRecVertex \
--cust_function customiseDataMixerOutput \
--no_exec