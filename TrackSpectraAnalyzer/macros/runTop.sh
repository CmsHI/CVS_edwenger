#!/bin/bash -
tag=mcproc0607

# run all
stag=$tag/sub7
#./runAnaAll.sh $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0607/trkhists_trkAnaSkimAOD_*.root' $stag
./runAnaAll.sh 'dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/frankma/mc/s10-STARTV26A_356/proc0607/trkhistsMC_*7*_1.root' $stag
exit

# run sub ranges of files to check results as a function of lumi
for i in `seq 1 9`; do
  stag=$tag/sub$i
  ./runAnaAll.sh $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0607/trkhists_trkAnaSkimAOD_'$i'*.root' $stag
done

# 2* treat separately
stag=$tag/sub2
./runAnaAll.sh $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0607/sub2/trkhists_trkAnaSkimAOD_*.root' $stag
