#!/bin/sh

cd $CMSSW_BASE/src

# skiming code
cvs co UserCode/edwenger/Skims
mv UserCode/edwenger .

# agglomerative vertex
cvs co UserCode/FerencSiklerVertexing

# track fitters recipe from Giuseppe
cvs co -r V04-00-02-01 TrackingTools/TrackFitters
cvs update -r CMSSW_3_5_6 TrackingTools/TrackFitters/BuildFile 
cvs update -r CMSSW_3_5_6 TrackingTools/TrackFitters/plugins/BuildFile  
