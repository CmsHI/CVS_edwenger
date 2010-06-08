#!/bin/bash -
if [ $# -lt 3 ]; then
  echo Usage
  echo "  $0 <sampleName> <ana_input_file> <tag>"
  exit
fi

sampleName=$1
effNtAnaInFile=$2
tag=$3
trkNtName=$4

# === First Get the HLT Turn-on ===
# use nt
effNtAnaOutDir=plots/$tag/ntHltEff
root -b -q anaHltJetEff_simple.C+\(\"$sampleName\",\"$effNtAnaInFile\",\"$effNtAnaOutDir\",false,\"trackAna/\"\)
if [ $? -ne 0 ]; then exit 1; fi

# use hist from nt
effHisAnaInFile=$effNtAnaOutDir/anahlt.root
effHisAnaOutDir=plots/$tag/HisHltEff
root -b -q anaHltJetEff_simple.C+\(\"$sampleName\",\"$effHisAnaInFile\",\"$effHisAnaOutDir\",true,\"\",1\)
if [ $? -ne 0 ]; then exit 1; fi

# === Second Plot the Triggered Spectra ===
trigSpecAnaInFile=$effHisAnaOutDir/anahlt.root
trigSpecAnaOutDir=plots/$tag/trigSpec
root -b -q anaTrigSpectra_simple.C+\(\"$sampleName\",\"$trigSpecAnaInFile\",\"$effNtAnaInFile\",\"$trigSpecAnaOutDir\",\"trackAna/\",\"$trkNtName\",0.99\)
if [ $? -ne 0 ]; then exit 1; fi

trigSpecPlotsInFile=$trigSpecAnaOutDir/anaspec.root
root -b -q finalTrigSpectra.C+\(\"$sampleName\",\"$trigSpecPlotsInFile\",\"$trigSpecAnaOutDir\"\)

