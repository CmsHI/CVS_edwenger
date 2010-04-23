#!/bin/sh

cd $CMSSW_BASE/src

# skiming code
cvs co UserCode/edwenger/Skims
cvs co UserCode/edwener/Misc
mv UserCode/edwenger .

# agglomerative vertex
cvs co -r V00-00-07 UserCode/FerencSiklerVertexing

# track fitters recipe from Giuseppe
#cvs co -r CMSSW_3_5_7 TrackingTools/TrackFitters 

# wei's rootuple producers
cvs co UserCode/davidlw/ROOTupleAnalyzer
mv UserCode/davidlw .

# fake and real selector
cvs co UserCode/ASYoon/PPTrackingTools/FakeAndRealTrackSelector
mv UserCode/ASYoon/PPTrackingTools/ .

# to run MTV with HitPixelLayersTPSelector.cc
cvs co UserCode/CmsHi/TrackAnalysis
mv UserCode/CmsHi/ .

