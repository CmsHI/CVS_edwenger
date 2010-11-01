import os

scratch="/net/hidsk0001/d00/scratch/frankma"
jobTag="proc1022_final_v2"

# ================== 362 ===============
samples  = {
    "MB-C10-Jun14RR-900GeV-MBskim-v1":"MinimumBias",
    "MB-C10-Jun14RR-900GeV-JMTskim-v1":"JetMETTau",
    "JM-R10A-PR4-Jet50Uskim-v0":"JetMETTau",
    "JM-R10A-PR4-Jet15Uskim-v0":"JetMETTau"
    }

inFolder="merge0"
outFolder="mergeAll"
for sample,PD in samples.items():
  indir=scratch+"/data/"+PD+"/"+sample+"_"+jobTag
  if "900GeV" in sample:
    indir=scratch+"/data/"+PD+"/"+sample
  cmd="./mergeJob.sh " + indir+"/"+inFolder + " trkhists " + indir+"/"+outFolder
  print cmd
  os.system(cmd)
