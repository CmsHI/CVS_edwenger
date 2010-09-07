import os

jobTag=""
scratch = "/net/hidsk0001/d00/scratch/frankma/data"
samples  = {
    #"MB-R10A-Jun14RR-MBskim-v0"+jobTag:"MinimumBias",
    #"JMT-R10A-Jun14RR-J15Uskim-v0"+jobTag:"JetMETTau",
    "JMT-R10A-PR4-Jet50Uskim-v0"+jobTag:"JetMETTau",
    "JMT-R10A-PR4-Jet15Uskim-v0"+jobTag:"JetMETTau"
    }

inFolder="."
outFolder="mergeAll"
for sample,PD in samples.items():
  indir=scratch+"/"+PD+"/"+sample
  cmd="./mergeJob.sh " + indir+"/"+inFolder + " trkhists " + indir+"/"+outFolder
  print cmd
  os.system(cmd)
