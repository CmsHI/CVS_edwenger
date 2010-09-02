#!/bin/bash -
startdir=$PWD

jobTag=proc0823_1ChPrimEvtSel
scratch=/net/hidsk0001/d00/scratch/frankma
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

jobTag=
topDir=$scratch/data/MinimumBias
jobs="\
$topDir/MB-C10-May6RR-MB900GeVskim-v1 \
$topDir/MB-C10-May6RR-JMT900GeVskim-v1 \
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
