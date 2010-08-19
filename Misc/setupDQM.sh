#!/bin/sh

dir=`pwd`

cd $CMSSW_BASE/src
addpkg DQMOffline/Configuration
addpkg DQM/HcalMonitorModule
addpkg DQM/SiStripMonitorClient
addpkg DQM/SiPixelCommon
addpkg DQM/SiPixelMonitorClient
addpkg Configuration/StandardSequences

cd $dir
cp DQMOfflineHeavyIons*.py $CMSSW_BASE/src/DQMOffline/Configuration/python
cp hcal_dqm_source_fileT0_HeavyIons_cff.py $CMSSW_BASE/src/DQM/HcalMonitorModule/python
cp SiStrip*Config*Tier0_HeavyIons_cff.py $CMSSW_BASE/src/DQM/SiStripMonitorClient/python
cp SiPixelOfflineDQM_HeavyIons_*_cff.py $CMSSW_BASE/src/DQM/SiPixelCommon/python
cp sipixel_tier0_qualitytest_heavyions.xml $CMSSW_BASE/src/DQM/SiPixelMonitorClient/test
cp sistrip_qualitytest_config_tier0_heavyions.xml $CMSSW_BASE/src/DQM/SiStripMonitorClient/data
cp Harvesting*cff.py $CMSSW_BASE/src/Configuration/StandardSequences/python