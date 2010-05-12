#!/bin/sh

cd $CMSSW_BASE/src

# skiming code
cvs co UserCode/edwenger/Skims

# analyzers
cvs co UserCode/edwenger/EvtSelAnalyzer
cvs co UserCode/edwenger/VertexAnalyzer
cvs co UserCode/edwenger/TrackSpectraAnalyzer
cvs co UserCode/edwenger/TrkEffAnalyzer

# agglomerative vertex
cvs co -r V00-00-07 UserCode/FerencSiklerVertexing

# wei's rootuple producers
cvs co UserCode/davidlw/ROOTupleAnalyzer

# hacked AnalyticalTrackSelector
addpkg RecoTracker/FinalTrackSelectors
cvs co UserCode/edwenger/Misc
mv UserCode/edwenger/Misc/AnalyticalTrackSelector.cc RecoTracker/FinalTrackSelectors/src
mv UserCode/edwenger/Misc/AnalyticalTrackSelector.h RecoTracker/FinalTrackSelectors/interface
rm -r UserCode/edwenger/Misc

mv UserCode/edwenger .
mv UserCode/davidlw .
