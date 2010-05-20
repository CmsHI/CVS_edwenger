#!/bin/bash -
inputTopDir=/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/edwenger/MinimumBias/MB-C10-PR9-TRKANASKIM-v2/38a0275e6a2a62980cf9b1868edbd493
jobTag=job2
tag=trkAnaSkimAOD
prefix=dcache:
output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v2_${jobTag}

for i in `find $inputTopDir | grep $tag | sort | sed 's;^;'$prefix';'`
do
   cmd="./condor64.sh runOne.sh $i $output_dir -1"
   #cmd="./runOne.sh $i $output_dir 10"
   echo $cmd
   eval $cmd
done

