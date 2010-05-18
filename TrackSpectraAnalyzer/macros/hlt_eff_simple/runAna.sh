# use histo
inFile=../process_aod/outputs/trkhists_try9_100k.root
root -b -q anaHltJetEff_simple.C+\(\"$inFile\",\"plots/fromHist\",true\)

# use nt
inFile=dcache:///net/hisrv0001/home/edwenger/t2bat/MinimumBias/MB-C10-PR9-TRKANASKIM-v2/38a0275e6a2a62980cf9b1868edbd493/ROOTuple_HighPurity_*.root
root -b -q anaHltJetEff_simple.C+\(\"$inFile\",\"plots/fromNt\",false\)
