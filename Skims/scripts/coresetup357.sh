#!/bin/sh

cd $CMSSW_BASE/src

# skiming code
cvs co UserCode/edwenger/Skims

# agglomerative vertex
cvs co -r V00-00-07 UserCode/FerencSiklerVertexing

# wei's rootuple producers
cvs co UserCode/davidlw/ROOTupleAnalyzer

mv UserCode/edwenger .
mv UserCode/davidlw .
