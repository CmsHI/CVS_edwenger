# cmsRun patAndTreesNoPF.py print files_clear files_load=files.txt

import FWCore.ParameterSet.VarParsing as VarParsing

ivars = VarParsing.VarParsing('standard')
ivars.register('initialEvent',mult=ivars.multiplicity.singleton,info="for testing")

ivars.files = "dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/krajczar/mix/MinBias_DijetUnquenched80_d20101125_Offset_1_RECO/Mixed_1_RECO_Mixed_1_DIGI_FA71D5A1-EAF0-DF11-9C4B-003048D2C0F0.root"

ivars.maxEvents = -1
ivars.initialEvent = 1
ivars.output = 'justTreesNoPF_offset1.root'

ivars.parseArguments()

import FWCore.ParameterSet.Config as cms


process = cms.Process('HIJETS')

#process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

#process.Timing = cms.Service("Timing")

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(ivars.files)
                            )

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(ivars.maxEvents)
                    )

#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START39_V7HI::All'  #39X

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')

#output file service
process.load("CommonTools.UtilAlgos.TFileService_cfi")
process.TFileService.fileName=cms.string(ivars.output)

#load centrality
from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFhits"),
    nonDefaultGlauberModel = cms.string("Hydjet_2760GeV"),
    centralitySrc = cms.InputTag("hiCentrality")
    )

process.makeCentralityTableTFile = cms.EDAnalyzer('CentralityTableProducer',
                                                  isMC = cms.untracked.bool(True),
                                                  makeDBFromTFile = cms.untracked.bool(False),
                                                  makeTFileFromDB = cms.untracked.bool(True)
                                                  )

process.inclusiveJetAnalyzer = cms.EDAnalyzer("InclusiveJetAnalyzer",
                                              jetTag = cms.InputTag("icPu5patJets"),
                                              isMC = cms.untracked.bool(True), 
                                              useCentrality = cms.untracked.bool(True)
                                              )


# --- Pat Stuff --

# is this just setting defaults that I don't use anyway?  What is DisableMonteCarloDeps doing?  
process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
from PhysicsTools.PatAlgos.tools.heavyIonTools import *
configureHeavyIons(process)

#from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJECSet( process, "Summer09_7TeV_ReReco332")

process.load("HeavyIonsAnalysis.Configuration.analysisProducers_cff")
process.hiExtra = cms.Sequence(
    process.allTracks +
    process.heavyIon
    )

# --- Gen stuff ---
# Note that we need to re-run gen jets b/c until AK-IC bug fix propagates to production
process.load('PhysicsTools.HepMCCandAlgos.HiGenParticles_cfi')

process.load('RecoJets.Configuration.GenJetParticles_cff')
process.load('RecoHI.HiJetAlgos.HiGenJets_cff')

#process.hiGenParticles.srcVector = cms.vstring('hiSignal')

process.hiGen = cms.Sequence(
#Careful when using embedded samples
    process.hiGenParticles +
    process.hiGenParticlesForJets *
    process.hiRecoGenJets +
    process.genPartons *
    process.hiPartons
    )

# Define Jet Algo parameters

process.load('RecoHI.HiJetAlgos.HiRecoJets_cff')

process.patJets.addGenPartonMatch   = True
process.patJets.addGenJetMatch      = True

process.load("RecoHI.HiJetAlgos.HiL1Corrector_cff")

process.patJets.embedCaloTowers = cms.bool(False)


### IC5 PU, Calo ###

process.icPu5corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("iterativeConePu5CaloJets"),
                                                    corrLevels = cms.PSet(L2Relative = cms.string("L2Relative_IC5Calo"),
                                                                          L3Absolute = cms.string("L3Absolute_IC5Calo"),
                                                                          L5Flavor = cms.string("none")))
process.icPu5clean = process.heavyIonCleanedGenJets.clone(src = cms.InputTag('iterativeCone5HiGenJets'))
process.icPu5match = process.patJetGenJetMatch.clone(src = cms.InputTag("iterativeConePu5CaloJets"),
                                                     matched = cms.InputTag("icPu5clean"))
process.icPu5parton = process.patJetPartonMatch.clone(src = cms.InputTag("iterativeConePu5CaloJets"))
process.icPu5patJets = process.patJets.clone(jetSource  = cms.InputTag("iterativeConePu5CaloJets"),
                                             genJetMatch = cms.InputTag("icPu5match"),
                                             genPartonMatch= cms.InputTag("icPu5parton"),
                                             jetCorrFactorsSource = cms.VInputTag(cms.InputTag("icPu5corr")))

process.icPu5patSequence = cms.Sequence(#process.iterativeConePu5CaloJets*
                                        process.icPu5corr*
                                        process.icPu5clean*
                                        process.icPu5match*
                                        process.icPu5parton*
                                        process.icPu5patJets)

##

process.runAllJets = cms.Sequence(process.icPu5patSequence)

# put it all together

process.path = cms.Path(
    #process.makeCentralityTableTFile*
    process.hiExtra*
    process.hiGen*
    process.runAllJets*
    process.inclusiveJetAnalyzer
    )

process.load("HeavyIonsAnalysis.Configuration.analysisEventContent_cff")
