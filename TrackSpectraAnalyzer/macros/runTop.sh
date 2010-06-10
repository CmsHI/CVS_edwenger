#!/bin/bash -
recoTrkNtName="nt_jettrack"

# === run data ===
tag=proc0607
stag=$tag/all_data
./runAnaAll.sh "Data" $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0607/trkhists_trkAnaSkimAOD_*.root' $stag $recoTrkNtName
exit



# === Checks ===
# run sub ranges of files to check results as a function of lumi
for i in `seq 1 9`; do
  stag=$tag/sub$i
  ./runAnaAll.sh "Data" $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0607/trkhists_trkAnaSkimAOD_'$i'*.root' $stag $recoTrkNtName
done

# 2* treat separately
stag=$tag/sub2
./runAnaAll.sh "Data" $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0607/sub2/trkhists_trkAnaSkimAOD_*.root' $stag $recoTrkNtName
