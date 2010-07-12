#!/bin/bash -
startdir=$PWD

jobTag=proc0710_trkAnaNoFilter
scratch=/net/hibat0007/d00/scratch/frankma
topDir=$scratch/data/MinimumBias
jobs="\
$topDir/MB-C10-M6RR-JMTskim-v0_$jobTag \
$topDir/MB-C10-M6RR-MBskim-v0_$jobTag \
$topDir/MB-C10-PR9-JMTskim-v0_$jobTag \
$topDir/MB-C10-PR9-MBskim-v0_$jobTag \
"

topDir=$scratch/data/JetMETTau
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
