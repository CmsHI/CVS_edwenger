#!/bin/sh

infile="/store/hidata/HIRun2010/HICorePhysics/RECO/PromptReco-v3/000/151/353/DC04F2F3-32F2-DF11-BD72-0030487C5CE2.root"
secfile="/store/hidata/HIRun2010/HICorePhysics/RAW/v1/000/151/353/00B95950-00F2-DF11-9485-001D09F252DA.root"

cmsDriver.py DMPreProcess -n 10 \
-s RAW2DIGI --eventcontent DATAMIXER \
--conditions FrontierConditions_GlobalTag,GR10_P_V12::All \
--filein $infile --secondfilein $secfile --data \
--customise SimGeneral.DataMixingModule.customiseMatchRecVertex \
--cust_function customiseDataMixerOutput \
--no_exec