import os
os.system("./create_custom_python_cfg.sh")

scratch="/net/hidsk0001/d00/scratch/frankma"
jobTag="proc1001"

# ================== 362 ===============
samples  = {
    "JM-R10A-PR4-Jet50Uskim-v0":"JetMETTau"
    }

for sample,PD in samples.items():
  inputList="pub_list/"+sample+".txt"
  outputDir=scratch+"/data/"+PD+"/"+sample+"_"+jobTag
  cmd="./submitJob.sh "+inputList+" "+outputDir
  print cmd
