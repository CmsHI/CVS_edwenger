#!/bin/bash -
./create_custom_python_cfg.sh

scratch=/net/hidsk0001/d00/scratch/frankma
jobTag=proc0902_looseSelectTracks

# ================== 900GeV ===============
job=MB-C10-M6RR-MBHfLooseskim-v0
inputList=pub_list/$job.txt
output_dir=$scratch/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

job=MB-C10-M6RR-JMTHfLooseskim-v0
inputList=pub_list/$job.txt
output_dir=$scratch/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

exit

# ===================== 358p3 =====================
job=MB-C10-M6RR-MBskim-v0
inputList=pub_list/$job.txt
output_dir=$scratch/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

job=MB-C10-M6RR-JMTskim-v0
inputList=pub_list/$job.txt
output_dir=$scratch/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

job=MB-C10-PR9-JMTskim-v0
inputList=pub_list/$job.txt
output_dir=$scratch/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

job=MB-C10-PR9-MBskim-v0
inputList=pub_list/$job.txt
output_dir=$scratch/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

job=JMT-R10A-PR1-JMTskim-v0
inputList=pub_list/$job.txt
output_dir=$scratch/data/JetMETTau/${job}_${jobTag}
./submitJob.sh $inputList $output_dir
