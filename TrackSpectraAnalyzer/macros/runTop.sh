#!/bin/bash -
tag=V0531_v8

# run all
#stag=$tag/all
stag=$tag/sub2good
./runAnaAll.sh $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0531/sub2good/trkhists_trkAnaSkimAOD_*.root' $stag
exit

# run sub ranges of files to check results as a function of lumi
for i in `seq 1 9`; do
  stag=$tag/sub$i
  ./runAnaAll.sh $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0531/trkhists_trkAnaSkimAOD_'$i'*.root' $stag
done
