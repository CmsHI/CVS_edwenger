#!/bin/sh

dir=EventFilter/RawDataCollector
dir2=`pwd`

cd $CMSSW_BASE/src
addpkg $dir

cd $dir2
cp RawDataCollector*.cc SealModule.cc $CMSSW_BASE/src/$dir/src
cp RawDataCollector*.h $CMSSW_BASE/src/$dir/interface
cp rawDataCollector*_cfi.py $CMSSW_BASE/src/$dir/python
