#inDir=/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/edwenger/MinimumBias/MB-C10-PR9-TRKANASKIM-v2/38a0275e6a2a62980cf9b1868edbd493
inDir=./
tag=trkAnaSkimAOD

for file in `find $inDir | grep $tag | sort`; do
  base=`echo $file | sed 's/_1\.root$//'`
  #echo base: $base
  for i in `seq 1 5`; do
    next=${base}_$((i+1)).root
    if [ -f $next ]; then
      #echo next: $next
      this=${base}_${i}.root
      echo rm $this
    fi
  done
done
