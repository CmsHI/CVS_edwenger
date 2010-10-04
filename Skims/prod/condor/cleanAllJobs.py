import os

scratch="/net/hidsk0001/d00/scratch/frankma"
jobTag="proc1001"

# ================== 362 ===============
samples  = {
    "JM-R10A-PR4-Jet50Uskim-v0":"JetMETTau"
    }

for sample,PD in samples.items():
  outputDir=scratch+"/data/"+PD+"/"+sample+"_"+jobTag
  print outputDir
  os.chdir(outputDir)
  os.system('echo "  # of files:" `ls trkhists*.root | wc -l`')
  cmd="../../scripts/cleanJob.sh"
  print " run: ",cmd
  os.system(cmd)

