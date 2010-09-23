#!/bin/sh

dir=`pwd`

cd $CMSSW_BASE/src

cvs co -r V14-00-11 DQM/HcalMonitorModule
cvs co -r V03-04-12 DQM/SiStripMonitorClient
cvs co -r V03-02-08 DQM/SiPixelCommon
cvs co -r V03-12-08 DQM/SiPixelMonitorClient
cvs co -r V03-02-13 DQM/TrackingMonitor

addpkg DQMOffline/Configuration
addpkg Configuration/StandardSequences
addpkg Configuration/PyReleaseValidation

cd $dir
cp DQMOfflineHeavyIons*.py $CMSSW_BASE/src/DQMOffline/Configuration/python
cp Harvesting*cff.py $CMSSW_BASE/src/Configuration/StandardSequences/python
cp ConfigBuilder.py $CMSSW_BASE/src/Configuration/PyReleaseValidation/python