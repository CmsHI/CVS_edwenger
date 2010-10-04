import os

scratch="/net/hidsk0001/d00/scratch/frankma"
jobTag="proc1001"

# ================== 362 ===============
samples  = {
    "JM-R10A-PR4-Jet50Uskim-v0":"JetMETTau"
    }

inFolder="merge0"
outFolder="mergeAll"
for sample,PD in samples.items():
  indir=scratch+"/data/"+PD+"/"+sample+"_"+jobTag
  cmd="./mergeJob.sh " + indir+"/"+inFolder + " trkhists " + indir+"/"+outFolder
  print cmd
  os.system(cmd)
