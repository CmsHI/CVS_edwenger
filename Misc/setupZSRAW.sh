#!/bin/sh

dir=EventFilter/RawDataCollector
dir2=`pwd`

cd $CMSSW_BASE/src
addpkg $dir

cd $dir2
cp RawDataCollectorZSTracker.cc SealModule.cc $CMSSW_BASE/src/$dir/src
cp RawDataCollectorZSTracker.h $CMSSW_BASE/src/$dir/interface
cp rawDataCollectorZSTracker_cfi.py $CMSSW_BASE/src/$dir/python
