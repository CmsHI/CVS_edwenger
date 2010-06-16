#!/bin/bash -
if [ $# -lt 3 ]; then
  echo "Usage:"
  echo "$0 <outdir> <outfile> <file1.root file2.root ...>"
  exit 1
fi

outdir=$1
shift
outfile=$1
shift
infiles=$@

echo "outdir: $outdir"
echo "outfile: $outfile"
#echo "infiles: $infiles"

mkdir -p $outdir/log
logfile=${outfile%.root}.txt
mergecmd="echo hadd -T $outdir/$outfile $infiles"
runcmd="(time $mergecmd >& $outdir/log/$logfile) 2>> $outdir/log/$logfile"
#echo $runcmd
eval $runcmd
