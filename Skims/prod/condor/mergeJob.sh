script=/net/hisrv0001/home/frankma/work/HI/jet/sw/trkAnaCMSSW_3_5_7/src/edwenger/Skims/prod/condor/mergeFiles.sh
topDir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias
job=MB-C10-A20RR-TRKANASKIM-v2_proc0615_TrkCutV3

outdir=$topDir/$job/merge
ct=0
cmd=
N=100000000
nPerMerge=20

for job in `ls $topDir/$job/trkhists*.root | head -n $N`; do
  subi=`expr $ct % $nPerMerge`
  if [ $subi -eq 0 ]; then
    if [ -n "$cmd" ]; then
      eval $cmd
    fi
    cmd="$script $outdir trkhists_sub$ct.root"
  else
    #echo $ct: $job
    cmd="$cmd $job"
  fi
  ct=$((ct+1))
done
