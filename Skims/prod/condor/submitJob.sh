#!/bin/bash -
if [ $# -lt 2 ]; then
  echo Usage:
  echo "  $0 <input_dir> <output_dir>"
  exit 1
fi

inputTopDir=$1
output_dir=$2

#inputList=$output_dir/log/pub_files_aod.txt
tag=trkAnaSkimAOD
#prefix=dcache:
#prefix=file:
njobs=1000000

echo begin
for i in `ls $inputTopDir | grep $tag | sort | head -n $njobs`
#for i in `cat $inputList | sort | head -n $njobs`
do
  ifile=${i##*/}
  cmd="./condor64.sh runOne.sh $inputTopDir $ifile $output_dir -1"
  #cmd="./runOne.sh $inputTopDir $ifile $output_dir 10"
  echo $cmd
  eval $cmd
done

