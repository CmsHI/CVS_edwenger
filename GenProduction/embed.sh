#!/bin/sh

sample="edwenger/GenProduction/Pyquen_DiJetEnrichedEM_Pt50_cfi.py"
infile="/store/relval/CMSSW_3_8_0/RelValHydjetQ_B0_2760GeV/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V7-v1/0005/DCAB6598-7B95-DF11-861B-0026189438F2.root"
outfile="dijetEM50_RAW.root"

cmsDriver.py $sample -n 1 \
-s GEN:hiSignal,SIM,DIGI,DIGI2RAW,HLT:HIon \
--conditions auto:mc --scenario HeavyIons --himix \
--datatier 'GEN-SIM-RAW-RECO' --eventcontent=FEVTDEBUGHLT \
--filein=$infile --fileout=$outfile --processName 'HISIGNAL' \
--customise edwenger/Misc/customiseCaloNZS.py \
--cust_function customiseCaloNZS \
--no_exec