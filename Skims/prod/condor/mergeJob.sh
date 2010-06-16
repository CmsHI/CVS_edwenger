scratch=/net/hibat0003/d00/scratch/frankma/data/MinimumBias
job=MB-C10-A20RR-TRKANASKIM-v2_proc0615_TrkCutV3

ct=0
cmd=
N=100
for job in `ls $scratch/$job/trkhists*.root | head -n $N`; do
  #echo $job
  job=${job##*/}
  #echo $ct
  subi=`expr $ct % 10`
  if [ $subi -eq 0 ]; then
    if [ -n "$cmd" ]; then
      echo $cmd
    fi
    cmd="mergeFiles.sh outdir sub$ct.root"
  else
    #echo $ct: $job
    cmd="$cmd $job"
  fi
  ct=$((ct+1))
done
