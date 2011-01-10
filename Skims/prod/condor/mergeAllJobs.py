import os

scratch="/net/hidsk0001/d00/scratch/frankma/data"
#jobTag="proc1022_final_v2"

# ================== 362 ===============
samples  = {
    #"MB-C10-Jun14RR-900GeV-MBskim-v1":"MinimumBias",
    #"MB-C10-Jun14RR-900GeV-JMTskim-v1":"JetMETTau",
    #"JM-R10A-PR4-Jet50Uskim-v0":"JetMETTau",
    #"JM-R10A-PR4-Jet15Uskim-v0":"JetMETTau"
    "hcpr-spectra-v2-all":"HCPR",
    "hcpr-spectra-v2-0to5":"HCPR",
    "hcpr-spectra-v2-5to10":"HCPR",
    "hcpr-spectra-v2-10to30":"HCPR",
    "hcpr-spectra-v2-30to50":"HCPR",
    "hcpr-spectra-v2-50to100":"HCPR"
    }

inFolder="."
outFolder="mergeAll"
for sample,PD in samples.items():
  #indir=scratch+"/"+PD+"/"+sample+"_"+jobTag
  indir=scratch+"/"+PD+"/"+sample
  cmd="./mergeJob.sh " + indir+"/"+inFolder + " trkhists " + indir+"/"+outFolder
  print cmd
  #os.system(cmd)
