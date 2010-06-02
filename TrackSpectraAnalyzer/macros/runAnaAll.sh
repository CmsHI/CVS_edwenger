#!/bin/bash -
tag=V0531_v5

# === First Get the HLT Turn-on ===
# use nt
effNtAnaInFile=$scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0531/trkhists_trkAnaSkimAOD_9*.root'
effNtAnaOutDir=plots/$tag/ntHltEff
root -b -q anaHltJetEff_simple.C+\(\"$effNtAnaInFile\",\"$effNtAnaOutDir\",false,\"trackAna/\"\)

# use hist from nt
effHisAnaInFile=$effNtAnaOutDir/anahlt.root
effHisAnaOutDir=plots/$tag/HisHltEff
root -b -q anaHltJetEff_simple.C+\(\"$effHisAnaInFile\",\"$effHisAnaOutDir\",true,\"\",1\)

# === Second Plot the Triggered Spectra ===
trigSpecAnaInFile=$effHisAnaOutDir/anahlt.root
trigSpecAnaOutDir=plots/$tag/trigSpec
root -b -q anaTrigSpectra_simple.C+\(\"$trigSpecAnaInFile\",\"$effNtAnaInFile\",\"$trigSpecAnaOutDir\",\"trackAna/\"\)

trigSpecPlotsInFile=$trigSpecAnaOutDir/anaspec.root
root -b -q finalTrigSpectra.C+\(\"$trigSpecPlotsInFile\",\"$trigSpecAnaOutDir\"\)

