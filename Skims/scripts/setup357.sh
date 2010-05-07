#!/bin/sh

cd $CMSSW_BASE/src

# skiming code
cvs co UserCode/edwenger/Skims

# PF test
cvs co UserCode/edwenger/Misc

# agglomerative vertex
cvs co -r V00-00-07 UserCode/FerencSiklerVertexing

# wei's rootuple producers
cvs co UserCode/davidlw/ROOTupleAnalyzer

# fake and real selector, vertex constraint producer
cvs co UserCode/ASYoon/PPTrackingTools/FakeAndRealTrackSelector
cvs co UserCode/ASYoon/PPTrackingTools/VertexConstraintProducer

# MTV with HitPixelLayersTPSelector.cc
cvs co UserCode/CmsHi/TrackAnalysis

# Cleanup
mv UserCode/edwenger .
mv UserCode/davidlw .
mv UserCode/ASYoon/PPTrackingTools/ .
mv UserCode/CmsHi/ .