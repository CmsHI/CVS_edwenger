#!/bin/sh

cd $CMSSW_BASE/src

# skiming code
cvs co UserCode/edwenger/Skims

# analyzers
cvs co UserCode/edwenger/EvtSelAnalyzer
cvs co UserCode/edwenger/VertexAnalyzer
cvs co UserCode/edwenger/TrackSpectraAnalyzer
cvs co UserCode/edwenger/TrkEffAnalyzer
cvs co -d Saved UserCode/SavedFMa/Saved

# agglomerative vertex
cvs co -r V00-00-07 UserCode/FerencSiklerVertexing

# vertex constraint for refitting
cvs co UserCode/ASYoon/PPTrackingTools/VertexConstraintProducer

# cleanup
mv UserCode/edwenger .
mv UserCode/ASYoon/PPTrackingTools/ .
rm -r UserCode/ASYoon

# FW update for 36X
sed -i 's/DEFINE_SEAL_MODULE();//' UserCode/FerencSiklerVertexing/plugins/module.cc
sed -i 's/DEFINE_ANOTHER_FWK_MODULE/DEFINE_FWK_MODULE/' UserCode/FerencSiklerVertexing/plugins/module.cc
