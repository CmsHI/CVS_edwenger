#!/bin/sh

dir=`pwd`

cd $CMSSW_BASE/src
dir2=SimGeneral/DataMixingModule
cvs co -r V00-02-20 $dir2
cvs co Configuration/GenProduction
addpkg SimGeneral/MixingModule
cvs co -r V01-18-49 Configuration/StandardSequences/python/*DM_cff.py

# HIP monitoring
# cvs co -d TkDPG UserCode/giordano/TkDPG

cd $dir
cp BuildFile.xml Gen*.cc $CMSSW_BASE/src/SimGeneral/MixingModule/plugins
cp customise*.py $CMSSW_BASE/src/$dir2/python

