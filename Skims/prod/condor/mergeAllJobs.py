import os

jobTag=""
scratch = "/net/hidsk0001/d00/scratch/frankma/data"
samples  = {
    "MinimumBias":"MB-R10A-Jun14RR-MBskim-v0"+jobTag,
    "JetMETTau":"JMT-R10A-Jun14RR-J15Uskim-v0"+jobTag
    }

inFolder="."
outFolder="mergeAll"
for PD,sample in samples.items():
  indir=scratch+"/"+PD+"/"+sample
  cmd="./mergeJob.sh " + indir+"/"+inFolder + " trkhists " + indir+"/"+outFolder
  print cmd
  os.system(cmd)
