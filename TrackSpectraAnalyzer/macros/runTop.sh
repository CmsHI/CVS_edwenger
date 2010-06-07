#!/bin/bash -
tag=V0607_v1

# run all
stag=$tag/all
./runAnaAll.sh $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0607/trkhists_trkAnaSkimAOD_*.root' $stag

# run sub ranges of files to check results as a function of lumi
for i in `seq 1 9`; do
  stag=$tag/sub$i
  ./runAnaAll.sh $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0607/trkhists_trkAnaSkimAOD_'$i'*.root' $stag
done

# 2* treat separately
stag=$tag/sub2
./runAnaAll.sh $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0607/sub2/trkhists_trkAnaSkimAOD_*.root' $stag
