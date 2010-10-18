#!/bin/sh

dir=`pwd`

cd $CMSSW_BASE/src
dir2=SimGeneral/DataMixingModule
addpkg $dir2
cvs co Configuration/GenProduction
cvs co SimGeneral/MixingModule

cd $dir
cp *.h *.cc $CMSSW_BASE/src/$dir2/plugins
cp customise*.py *cfi.py $CMSSW_BASE/src/$dir2/python
cp *.sh $CMSSW_BASE/src/$dir2/test