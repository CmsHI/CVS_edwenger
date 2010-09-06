#!/bin/sh

dir=`pwd`

cd $CMSSW_BASE/src
dir2=RecoTracker/DebugTools
addpkg $dir2

cd $dir
cp BuildFile.xml $CMSSW_BASE/src/$dir2
cp Ckf*.h $CMSSW_BASE/src/$dir2/interface
cp Ckf*.cc Modules.cc $CMSSW_BASE/src/$dir2/plugins
cp ckfDebugTracking_cfg.py $CMSSW_BASE/src/$dir2/test