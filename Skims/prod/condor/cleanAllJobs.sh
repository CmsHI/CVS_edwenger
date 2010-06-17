#!/bin/bash -
startdir=$PWD

topDir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias
jobs="\
$topDir/MB-C10-A20RR-TRKANASKIM-v2_proc0615_nt \
$topDir/MB-C10-A20RR-TRKANASKIM-v3_proc0615_nt \
$topDir/MB-C10-M6RR-JMTskim-v0_proc0615_nt \
$topDir/MB-C10-M6RR-MBskim-v0_proc0615_nt \
$topDir/MB-C10-PR9-JMTskim-v0_proc0615_nt \
$topDir/MB-C10-PR9-MBskim-v0_proc0615_nt \
$topDir/MB-C10-PR9-TRKANASKIM-v3_proc0615_nt \
"

topDir=/net/hibat0003/d00/scratch/frankma/data/JetMETTau
jobs=$jobs"\
$topDir/JMT-R10A-PR1-JMTskim-v0_proc0615_nt \
"

for i in $jobs; do
  echo $i
  cd $i
  echo "  # of files:" `ls trkhists*.root | wc -l`
  cmd=../../scripts/cleanJob.sh
  echo "  run: $cmd"
  eval $cmd
  cd $startdir
done
