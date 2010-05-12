import FWCore.ParameterSet.Config as cms
process = cms.Process("ANASKIM")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

# =============== 7 TeV Collisions =====================

process.load("edwenger.Skims.fileNames7TeV_cff")
process.source = cms.Source("PoolSource",process.PromptReco7TeV)


# =============== Other Statements =====================

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.GlobalTag.globaltag = 'GR_R_35X_V7A::All'

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.3 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/UserCode/edwenger/Skims/test/TrkAnaSkim_data_7TeV_cfg.py,v $'),
    annotation = cms.untracked.string('BPTX_AND + BSC_OR + !BSCHALO')
)

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string('ROOTuple_HighPurity.root')
                                   )

# =============== Event Filter =====================

process.load("edwenger.Skims.eventSelection_cff")
process.load("edwenger.Skims.hfCoincFilter_cff")
process.load("edwenger.Skims.evtSelAnalyzer_cff")

process.eventFilter = cms.Sequence(process.preTrgTest *
                                   process.minBiasBscFilter *  ## BSC OR, BPTX AND, !BSCHALO
                                   process.postTrgTest *
                                   process.hfCoincFilter *     ## E=3 GeV calotower threshold
                                   process.purityFractionFilter)

# =============== Extra Reco Steps =====================

process.load("edwenger.Skims.ChargedCandidates_cff") 
process.load("edwenger.Skims.ExtraVertex_cff")       
#process.load("edwenger.Skims.TrackRefit_cff")      
process.load("edwenger.Skims.RootpleProducer_cfi")   

process.extraVtx = cms.Sequence(process.extraVertex *          ## agglomerative pixel vertexing
                                process.postEvtSelTest *
                                process.selectedVertex *       ## most-populated (filters)
                                process.postVtxTest)

process.extraReco = cms.Sequence(process.chargedCandidates *   ## selected tracks -> charged candidates
                                 #process.trackRefit *         ## refit constrained to PV
                                 process.preTrkVtxTest *
                                 process.primaryVertexFilter * ## non-fake, ndof>4, abs(z)<15
                                 process.postTrkVtxTest *
                                 process.rootpleProducer)      ## make wei's rootples


# =============== PAT ===========================

process.load("edwenger.Skims.patAna_cff")

# get the 7 TeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "Summer09_7TeV_ReReco332")

# turn off MC matching for data
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'])


# =============== Final Paths =====================

process.eventFilter_step = cms.Path(process.eventFilter)

process.extraReco_step   = cms.Path(process.eventFilter *
                                    process.extraVtx *
                                    process.extraReco)

process.pat_step         = cms.Path(process.eventFilter *
                                    process.patAnaSequence)

process.ana_step         = cms.Path(process.eventFilter *
                                    process.selectedVertex *
                                    process.primaryVertexFilter *
                                    process.trackAna)

# =============== Output ================================

process.load("edwenger.Skims.analysisSkimContent_cff")
process.output = cms.OutputModule("PoolOutputModule",
    process.analysisSkimContent,
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('eventFilter_step')),
    dataset = cms.untracked.PSet(
      dataTier = cms.untracked.string('AOD'),
      filterName = cms.untracked.string('TrkAnaFilter')),
    fileName = cms.untracked.string('trkAnaSkimAOD.root')
    )

process.output_step = cms.EndPath(process.output)

# =============== Schedule =====================

process.schedule = cms.Schedule(process.eventFilter_step,
                                process.extraReco_step,
				process.pat_step,
                                process.ana_step,
                                process.output_step)
