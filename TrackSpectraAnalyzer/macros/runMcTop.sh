#!/bin/bash -
recoTrkNtName="nt_jettrack"
genTrkNtName="nt_gen_dndptdeta"

# === run mc ===
tag=mcproc0607_v3
# run mc reco
stag=$tag/all_mcrecotrk
./runAnaAll.sh "PythiaD6T" $scratch'/mc/s10-STARTV26A_356/proc0607/trkhistsMC_*_1.root' $stag $recoTrkNtName

# run mc gen
stag=$tag/all_mcgentrk
./runAnaAll.sh "PythiaD6T" $scratch'/mc/s10-STARTV26A_356/proc0607/trkhistsMC_*_1.root' $stag $genTrkNtName
