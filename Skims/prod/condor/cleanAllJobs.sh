#!/bin/bash -
startdir=$PWD

jobTag=proc0628_trkAnaNoFilter
jobTagFilter=proc0628_trkAnaFilter
topDir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias
jobs="\
$topDir/MB-C10-A20RR-TRKANASKIM-v2_$jobTag \
$topDir/MB-C10-A20RR-TRKANASKIM-v3_$jobTag \
$topDir/MB-C10-M6RR-JMTskim-v0_$jobTag \
$topDir/MB-C10-M6RR-MBskim-v0_$jobTag \
$topDir/MB-C10-PR9-JMTskim-v0_$jobTag \
$topDir/MB-C10-PR9-MBskim-v0_$jobTag \
$topDir/MB-C10-PR9-TRKANASKIM-v3_$jobTag \
$topDir/MB-C10-PR9-TRKANASKIM-v3_$jobTagFilter \
"

topDir=/net/hibat0003/d00/scratch/frankma/data/JetMETTau
jobs=$jobs"\
$topDir/JMT-R10A-PR1-JMTskim-v0_$jobTag \
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
