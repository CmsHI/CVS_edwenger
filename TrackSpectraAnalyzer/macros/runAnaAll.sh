#!/bin/bash -
if [ $# -lt 2 ]; then
  echo Usage
  echo "  $0 <ana_input_file> <tag>"
  exit
fi

effNtAnaInFile=$1
tag=$2

# === First Get the HLT Turn-on ===
# use nt
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

