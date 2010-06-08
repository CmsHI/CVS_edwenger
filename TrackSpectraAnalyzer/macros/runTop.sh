#!/bin/bash -
recoTrkNtName="nt_jettrack"
genTrkNtName="nt_gen_dndptdeta"

# === run data ===
tag=proc0607
stag=$tag/all_data
#./runAnaAll.sh "Data" $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0607/trkhists_trkAnaSkimAOD_*.root' $stag $recoTrkNtName

# === run mc ===
tag=mcproc0607_v2
# run mc reco
stag=$tag/all_mcrecotrk
./runAnaAll.sh "PythiaD6T" 'dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/frankma/mc/s10-STARTV26A_356/proc0607/trkhistsMC_*_1.root' $stag $recoTrkNtName

# run mc gen
stag=$tag/all_mcgentrk
./runAnaAll.sh "PythiaD6T" 'dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/frankma/mc/s10-STARTV26A_356/proc0607/trkhistsMC_*_1.root' $stag $genTrkNtName
exit



# run sub ranges of files to check results as a function of lumi
for i in `seq 1 9`; do
  stag=$tag/sub$i
  ./runAnaAll.sh "Data" $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0607/trkhists_trkAnaSkimAOD_'$i'*.root' $stag $recoTrkNtName
done

# 2* treat separately
stag=$tag/sub2
./runAnaAll.sh "Data" $scratch'/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_proc0607/sub2/trkhists_trkAnaSkimAOD_*.root' $stag $recoTrkNtName
