#!/bin/sh

dir=RecoLocalTracker/SiStripZeroSuppression

cvs co -r HIStudies_branch_v0 $dir
cp *CMNSubtractor.h $CMSSW_BASE/$dir/interface
cp *CMNSubtractor.cc SiStripRawProcessingFactory.cc $CMSSW_BASE/$dir/src
cp customiseCMN.py DefaultAlgorithms_cff.py $CMSSW_BASE/$dir/python
