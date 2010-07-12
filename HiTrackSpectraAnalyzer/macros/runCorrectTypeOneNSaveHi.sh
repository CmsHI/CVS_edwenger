

inputDir=/home/sungho/plots/JulyEx/root_files/
outputDir=./rootOutput/

if [ -d $outputDir ]
    then
    echo "$outputDir exists"
else
    mkdir $outputDir
    echo "$outputDir is created!"
fi


#spectraFile=HiTrkHistMC_july08_mdataJuly09
spectraFile=HiTrkHistMC_july08_hydjet_small_v2
trkCorrFile1=HiTrkHistMC_july08_hydjet_small_v2
trkCorrFile2=HiTrkHistMC_july08_hydjet_small_v2
trkCorrFile3=HiTrkHistMC_july08_hydjet_small_v2
trkCorrFile4=HiTrkHistMC_july08_hydjet_small_v2
trkCorrFile5=HiTrkHistMC_july08_hydjet_small_v2


#anaDir=hitrackAna
#corDir=hitrkEffAnalyzer
anaDir=hitrackAna
corDir=trkEffAnalyzer

isGEN=false
varBin=true

ijet=0
fjet=2000

ieta=0
feta=2.4
drawFig=true
saveFile=true

root -l CorrectTypeOneNSaveHi.C+\(\"$inputDir\",\"$outputDir\",\"$spectraFile\",\"$trkCorrFile1\",\"$trkCorrFile2\",\"$trkCorrFile3\",\"$trkCorrFile4\",\"$trkCorrFile5\",\"$anaDir\",\"$corDir\",$isGEN\,$varBin\,$ijet\,$fjet\,$ieta\,$feta\,$drawFig\,$saveFile\)