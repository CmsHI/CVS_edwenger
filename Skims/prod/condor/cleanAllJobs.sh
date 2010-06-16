scratch=/net/hibat0003/d00/scratch/frankma/data/MinimumBias
startdir=$PWD
jobs="\
MB-C10-A20RR-TRKANASKIM-v2_proc0615_TrkCutDefault \
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
  cd $scratch/$i
  echo "  # of files:" `ls trkhists*.root | wc -l`
  cmd=../scripts/cleanJob.sh
  echo "  run: $cmd"
  eval $cmd
  cd $startdir
done
