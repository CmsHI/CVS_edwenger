#!/bin/sh

dir=RecoLocalTracker/SiStripZeroSuppression
dir2=`pwd`

cd $CMSSW_BASE/src
cvs co -r HIStudies_branch_v0 $dir
cd $dir2
cp *CMNSubtractor.h $CMSSW_BASE/src/$dir/interface
cp *CMNSubtractor.cc SiStripRawProcessingFactory.cc $CMSSW_BASE/src/$dir/src
cp customiseCMN.py DefaultAlgorithms_cff.py $CMSSW_BASE/src/$dir/python
