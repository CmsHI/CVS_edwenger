#!/bin/sh

dir=`pwd`

cd $CMSSW_BASE/src
dir2=SimGeneral/DataMixingModule
addpkg $dir2
cvs co Configuration/GenProduction
cvs co -r V05-02-11 SimGeneral/MixingModule
addpkg Configuration/StandardSequences

# HIP monitoring
# cvs co -d TkDPG UserCode/giordano/TkDPG

cd $dir
cp *.h *.cc $CMSSW_BASE/src/$dir2/plugins
cp customise*.py *cfi.py $CMSSW_BASE/src/$dir2/python
cp *cff.py $CMSSW_BASE/src/Configuration/StandardSequences/python
cp *.sh $CMSSW_BASE/src/$dir2/test

