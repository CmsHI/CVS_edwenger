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

jobTag=proc0902_looseSelectTracks
topDir=$scratch/data/MinimumBias
jobs="\
$topDir/MB-C10-M6RR-MBHfLooseskim-v0_$jobTag \
$topDir/MB-C10-M6RR-JMTHfLooseskim-v0_$jobTag \
"

# merge inFolder
inFolder="."
outFolder="mergeAll"
for i in $jobs; do
  #echo $i
  cmd="./mergeJob.sh ${i}/${inFolder} trkhists ${i}/${outFolder}"
  echo $cmd
  eval $cmd
done
