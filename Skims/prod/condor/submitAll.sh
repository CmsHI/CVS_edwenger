#!/bin/bash -
./create_custom_python_cfg.sh

jobTag=proc0615_nt

# ===================== 357 =====================
# === A20RR v2 ===
# there is refit track
inputList=pub_list/MB-C10-A20RR-TRKANASKIM-v2.txt
output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/MB-C10-A20RR-TRKANASKIM-v2_${jobTag}
./submitJob.sh $inputList $output_dir

# === A20RR v3 ===
# there is refit track
inputList=pub_list/MB-C10-A20RR-TRKANASKIM-v3.txt
output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/MB-C10-A20RR-TRKANASKIM-v3_${jobTag}
./submitJob.sh $inputList $output_dir

# === PR9 v3 ===
# there is refit track
inputList=pub_list/MB-C10-PR9-TRKANASKIM-v3.txt
output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_${jobTag}
./submitJob.sh $inputList $output_dir

# ===================== 358p3 =====================
job=MB-C10-M6RR-MBskim-v0
inputList=pub_list/$job.txt
output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

job=MB-C10-M6RR-JMTskim-v0
inputList=pub_list/$job.txt
output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

job=MB-C10-PR9-JMTskim-v0
inputList=pub_list/$job.txt
output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

job=MB-C10-PR9-MBskim-v0
inputList=pub_list/$job.txt
output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/${job}_${jobTag}
./submitJob.sh $inputList $output_dir

job=JMT-R10A-PR1-JMTskim-v0
inputList=pub_list/$job.txt
output_dir=/net/hibat0003/d00/scratch/frankma/data/JetMETTau/${job}_${jobTag}
./submitJob.sh $inputList $output_dir
