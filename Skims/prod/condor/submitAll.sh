#!/bin/bash -
./create_custom_python_cfg.sh

scratch=/net/hibat0007/d00/scratch/frankma
jobTag=proc0710_trkAnaNoFilter

# ===================== 358p3 =====================
job=MB-C10-M6RR-MBskim-v0
inputList=pub_list/$job.txt
output_dir=$scrath/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

job=MB-C10-M6RR-JMTskim-v0
inputList=pub_list/$job.txt
output_dir=$scrath/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

job=MB-C10-PR9-JMTskim-v0
inputList=pub_list/$job.txt
output_dir=$scrath/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

job=MB-C10-PR9-MBskim-v0
inputList=pub_list/$job.txt
output_dir=$scrath/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

job=JMT-R10A-PR1-JMTskim-v0
inputList=pub_list/$job.txt
output_dir=$scrath/data/JetMETTau/${job}_${jobTag}
./submitJob.sh $inputList $output_dir
