#!/bin/bash -
#inputTopDir=/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/edwenger/MinimumBias/MB-C10-A20RR-TRKANASKIM-v2/86d28cd0599312fbc0b38fb077d9e1fc
#output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/MB-C10-A20RR-TRKANASKIM-v2_${jobTag}
#inputTopDir=/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/edwenger/MinimumBias/MB-C10-A20RR-TRKANASKIM-v3/86d28cd0599312fbc0b38fb077d9e1fc
#output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/MB-C10-A20RR-TRKANASKIM-v3_${jobTag}
#inputTopDir=/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/edwenger/MinimumBias/MB-C10-PR9-TRKANASKIM-v2/38a0275e6a2a62980cf9b1868edbd493
#output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v2_${jobTag}

jobTag=proc0531
inputTopDir=/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/edwenger/MinimumBias/MB-C10-PR9-TRKANASKIM-v3/ae98f896d123ace1f592d26e790fa90c
output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_${jobTag}
inputList=$output_dir/log/pub_files_aod.txt
tag=trkAnaSkimAOD
#prefix=dcache:
#prefix=file:
njobs=10 #00000

echo begin
#for i in `ls $inputTopDir | grep $tag | sort | head -n $njobs`
for i in `cat $inputList | sort | head -n $njobs`
do
  ifile=${i##*/}
  cmd="./condor64.sh runOne.sh $inputTopDir $ifile $output_dir -1"
  #cmd="./runOne.sh $inputTopDir $ifile $output_dir 10"
  echo $cmd
  #eval $cmd
done

