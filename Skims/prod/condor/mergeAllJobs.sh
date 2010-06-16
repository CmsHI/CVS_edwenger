topDir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias
#jobs="\
#MB-C10-A20RR-TRKANASKIM-v2_proc0615_TrkCutDefault \
#"
jobs="\
MB-C10-A20RR-TRKANASKIM-v2_proc0615_TrkCutV1 \
MB-C10-A20RR-TRKANASKIM-v2_proc0615_TrkCutV2 \
MB-C10-A20RR-TRKANASKIM-v2_proc0615_TrkCutV3 \
MB-C10-A20RR-TRKANASKIM-v3_proc0615_TrkCutDefault \
MB-C10-A20RR-TRKANASKIM-v3_proc0615_TrkCutV1 \
MB-C10-A20RR-TRKANASKIM-v3_proc0615_TrkCutV2 \
MB-C10-A20RR-TRKANASKIM-v3_proc0615_TrkCutV3 \
MB-C10-PR9-TRKANASKIM-v3_proc0615_TrkCutDefault \
MB-C10-PR9-TRKANASKIM-v3_proc0615_TrkCutV1 \
MB-C10-PR9-TRKANASKIM-v3_proc0615_TrkCutV2 \
MB-C10-PR9-TRKANASKIM-v3_proc0615_TrkCutV3 \
"

for i in $jobs; do
  echo $i
  cmd="./mergeJob.sh $topDir $i"
  #echo $cmd
  eval $cmd
done
