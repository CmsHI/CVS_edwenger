#!/bin/bash -
tag=V0531_v7
./runAnaAll.sh $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0531/trkhists_trkAnaSkimAOD_9*.root' $tag

for i in `seq 8 9`; do
  stag=$tag/sub$i
  ./runAnaAll.sh $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0531/trkhists_trkAnaSkimAOD_'$i'*.root' $stag
done
