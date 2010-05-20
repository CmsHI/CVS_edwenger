#!/bin/bash -
if [ $# -eq 0 -o $# -lt 3 ]; then
   echo Usage
   echo "   $0 <input_file.root> <output_dir> <nperjob>"
   exit 1
fi

# input parameters
cfg=custom_TrkAnaSkim_data_7TeV_cfg.py
inputFile=$1
outputDir=$2
nperjob=$3
swenv=/net/hisrv0001/home/frankma/work/job/ana/trkAnaCMSSW_3_5_7/env.sh

# check
if [ ! -e $cfg ]; then
   echo cfg: $cfg does not exist!
   exit 1
fi

# setup job parameters
inputFileName=${inputFile##*/}
echo $inputFileName
outputFileName=trkhists_$inputFileName
echo $outputFileName
outputFile=$outputDir/$outputFileName
echo $outputFile
logDir=$outputDir/log
echo $logDir
logFile=$logDir/${outputFileName%.root}.txt


# setup env
. /net/hisrv0001/home/frankma/UserCode/SavedFMa/bin/cgate/basic/osg_cmssw_set_basic.sh
. $swenv

mkdir -p $logDir
cmd="cmsRun $cfg maxEvents=$nperjob files=$inputFile output=$outputFile >& $logFile"

# review
which cmsRun
echo $cmd

# run!
eval $cmd
