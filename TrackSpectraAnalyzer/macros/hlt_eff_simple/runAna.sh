#!/bin/bash -
if [ $# -lt 1 ]; then
  echo Usage:
  echo "  $0 <tag>"
  exit 1
fi

tag=$1

# use histo
#inFile=../process_aod/outputs/trkhists_try9_100k.root
inFile=$scratch/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v2/all/trkhists_only_all.root
root -b -q anaHltJetEff_simple.C+\(\"$inFile\",\"plots/${tag}_fromHist\",true\)

# use nt
#inFile=dcache:///net/hisrv0001/home/edwenger/t2bat/MinimumBias/MB-C10-PR9-TRKANASKIM-v2/38a0275e6a2a62980cf9b1868edbd493/ROOTuple_HighPurity_*.root
inFile=$scratch/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v2/trkhists_trkAnaSkimAOD_*_1.root
root -b -q anaHltJetEff_simple.C+\(\"$inFile\",\"plots/${tag}_fromNt\",false\)

# use hist from nt
inFile=plots/${tag}_fromNt/anahlt.root
root -b -q anaHltJetEff_simple.C+\(\"$inFile\",\"plots/${tag}_histFromNt\",true,\"\",1\)
