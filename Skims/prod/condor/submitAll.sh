#!/bin/bash -
jobTag=proc0605
# === A20RR v2 ===
# there is refit track
inputTopDir=/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/edwenger/MinimumBias/MB-C10-A20RR-TRKANASKIM-v2/86d28cd0599312fbc0b38fb077d9e1fc
output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/MB-C10-A20RR-TRKANASKIM-v2_${jobTag}
./submitJob.sh $inputTopDir $output_dir

# === A20RR v3 ===
# there is refit track
inputTopDir=/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/edwenger/MinimumBias/MB-C10-A20RR-TRKANASKIM-v3/86d28cd0599312fbc0b38fb077d9e1fc
output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/MB-C10-A20RR-TRKANASKIM-v3_${jobTag}
./submitJob.sh $inputTopDir $output_dir

# === PR9 v3 ===
# there is refit track
inputTopDir=/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/edwenger/MinimumBias/MB-C10-PR9-TRKANASKIM-v3/ae98f896d123ace1f592d26e790fa90c
output_dir=/net/hibat0003/d00/scratch/frankma/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3_${jobTag}
./submitJob.sh $inputTopDir $output_dir

