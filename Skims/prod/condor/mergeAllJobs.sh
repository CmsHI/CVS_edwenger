topDir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias
jobTag=proc0625
jobs="\
$topDir/MB-C10-A20RR-TRKANASKIM-v2_$jobTag \
$topDir/MB-C10-A20RR-TRKANASKIM-v3_$jobTag \
$topDir/MB-C10-M6RR-JMTskim-v0_$jobTag \
$topDir/MB-C10-M6RR-MBskim-v0_$jobTag \
$topDir/MB-C10-PR9-JMTskim-v0_$jobTag \
$topDir/MB-C10-PR9-MBskim-v0_$jobTag \
$topDir/MB-C10-PR9-TRKANASKIM-v3_$jobTag \
"

topDir=/net/hibat0003/d00/scratch/frankma/data/JetMETTau
jobs=$jobs"\
$topDir/JMT-R10A-PR1-JMTskim-v0_$jobTag \
"

# merge level
#level=
level="/merge"
for i in $jobs; do
  #echo $i
  cmd="./mergeJob.sh ${i}${level} trkhists"
  echo $cmd
  eval $cmd
done
