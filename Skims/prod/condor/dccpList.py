import os

dcache  = "/pnfs/cmsaf.mit.edu/t2bat/cms"
scratch = "/net/hidsk0001/d00/scratch/frankma/data/MinimumBias"
sample  = "MB-C10-May6RR-JMT900GeVskim-v1"

filelist = []

for line in open ('pub_list/'+ sample + '.txt', 'r'):
    filelist.append(line.rstrip('\n').replace('trkAnaSkimAOD','trkhists'))

for file in filelist:
    subfile = file.split('/')
    #print 'preparing to copy ' + subfile[-1] + ' from dcache'
    print 'preparing to copy ' + file + ' from dcache'
    if not os.path.isfile(sample + "/" + subfile[-1]):
        os.system('dccp ' + dcache + file + ' ' + scratch+"/"+sample)
    else:
        print subfile[-1] + " already exists in '" + scratch+"/"+sample + "' directory"

