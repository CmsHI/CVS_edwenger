#!/bin/bash -
./create_custom_python_cfg.sh

jobTag=proc0615
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

