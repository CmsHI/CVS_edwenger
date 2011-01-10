import os

dcache  = "/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/frankma"
scratch = "/net/hidsk0001/d00/scratch/frankma/data"
#inputs_dir = '/net/hisrv0001/home/frankma/inputs_data'
samples  = {
    "hcpr-spectra-v2-all":"HCPR",
    "hcpr-spectra-v2-0to5":"HCPR",
    "hcpr-spectra-v2-5to10":"HCPR",
    "hcpr-spectra-v2-10to30":"HCPR",
    "hcpr-spectra-v2-30to50":"HCPR",
    "hcpr-spectra-v2-50to100":"HCPR",
    }

for sample,PD in samples.items():
  print "===== sample: ",PD,sample," =====\n"
  indir=dcache+"/"+PD+"/"+sample
  outdir=scratch+"/"+PD+"/"+sample
  os.system('mkdir -p '+outdir)

  for file in os.listdir(indir):
    if "root" not in file:
      continue
    infile=indir+"/"+file
    if not os.path.isfile(outdir + "/" + file):
      cmd = 'dccp ' + infile + ' ' + outdir
      print cmd
      #os.system(cmd)
    else:
      print file + " already exists in '" + outdir + "' directory"

