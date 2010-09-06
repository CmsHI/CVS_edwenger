import os

dcache  = "/pnfs/cmsaf.mit.edu/t2bat/cms"
scratch = "/net/hidsk0001/d00/scratch/frankma/data"
samples  = {
    "MinimumBias":"MB-R10A-Jun14RR-MBskim-v0",
    "JetMETTau":"JMT-R10A-Jun14RR-J15Uskim-v0"
    }

for PD,sample in samples.items():
  outdir=scratch+"/"+PD+"/"+sample
  os.system('mkdir -p '+outdir)

  filelist = []
  for line in open ('pub_list/'+ sample + '.txt', 'r'):
    filelist.append(line.rstrip('\n').replace('trkAnaSkimAOD','trkhists'))

  for file in filelist:
    infile=dcache+file
    subfile = file.split('/')
    #print 'preparing to copy ' + subfile[-1] + ' from dcache'
    if not os.path.isfile(outdir + "/" + subfile[-1]):
      cmd = 'dccp ' + infile + ' ' + outdir
      print cmd
      os.system(cmd)
    else:
      print subfile[-1] + " already exists in '" + outdir + "' directory"

