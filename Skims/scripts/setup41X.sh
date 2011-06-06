#!/bin/sh

cd $CMSSW_BASE/src

# skiming code
cvs co UserCode/edwenger/Skims

# analyzers
cvs co UserCode/edwenger/EvtSelAnalyzer
cvs co UserCode/edwenger/VertexAnalyzer
cvs co UserCode/edwenger/TrackSpectraAnalyzer
cvs co UserCode/edwenger/TrkEffAnalyzer
cvs co -r Y1JAna_0_1_4  -d Saved UserCode/SavedFMa/Saved

# update dijet ana
cvs co UserCode/ASYoon/MISC/src/DiJetAna.cc
cp UserCode/ASYoon/MISC/src/DiJetAna.cc Saved//DiJetAna/src/

# agglomerative vertex
#cvs co -r V00-00-07 UserCode/FerencSiklerVertexing
cvs co -r 1.8 UserCode/FerencSiklerVertexing  # EDFilter to EDProducer fixed

# vertex constraint for refitting
cvs co UserCode/ASYoon/PPTrackingTools/VertexConstraintProducer

# for GEN spectra production
#cvs co UserCode/ASYoon/MCGeneration
#mv UserCode/ASYoon/MCGeneration .
#cvs co Configuration/Generator/python/PythiaUEZ2Settings_cfi.py
#cvs co Configuration/Generator/python/PythiaUECW900ASettings_cfi.py
#cvs co Configuration/Generator/python/PythiaUED6TSettings_cfi.py
#cvs co Configuration/Generator/python/PythiaUEP0Settings_cfi.py
#cvs co Configuration/Generator/python/PythiaUEProQ20Settings_cfi.py
#cvs co Configuration/Generator/python/MinBias_7TeV_pythia8_cff.py


# cleanup
mv UserCode/edwenger .
mv UserCode/ASYoon/PPTrackingTools/ .
rm -r UserCode/ASYoon

# further clean up to keep it light
#rm edwenger/TrackSpectraAnalyzer/macros/ -r
#rm edwenger/TrkEffAnalyzer/macros/ -r

# FW update for 36X
sed -i 's/DEFINE_SEAL_MODULE();//' UserCode/FerencSiklerVertexing/plugins/module.cc
sed -i 's/DEFINE_ANOTHER_FWK_MODULE/DEFINE_FWK_MODULE/' UserCode/FerencSiklerVertexing/plugins/module.cc

# To convert to XML format
scram build -c