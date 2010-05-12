import FWCore.ParameterSet.Config as cms
process = cms.Process("ANASKIM")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

# =============== 7 TeV Collisions =====================

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
    '/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_356ReReco-v1/0009/FEFC70B6-F53D-DF11-B57E-003048679150.root',
    '/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_356ReReco-v1/0009/FED8673E-F53D-DF11-9E58-0026189437EB.root')
)

# =============== Other Statements =====================

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.GlobalTag.globaltag = 'START3X_V26A::All'

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.5 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/UserCode/edwenger/Skims/test/TrkAnaSkim_mc_7TeV_cfg.py,v $'),
    annotation = cms.untracked.string('BPTX_AND + BSC_OR + !BSCHALO')
)

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string('ROOTupleMC_HighPurity.root')
                                   )

# =============== Event Filter =====================

process.load("edwenger.Skims.eventSelection_cff")
process.load("edwenger.Skims.hfCoincFilter_cff")
process.load("edwenger.Skims.evtSelAnalyzer_cff")

process.eventFilter = cms.Sequence(process.preTrgTest *
                                   process.minBiasBscFilterMC *  ## BSC OR, !BSCHALO
                                   process.postTrgTest *
                                   process.hfCoincFilter *       ## E=3 GeV calotower threshold
                                   process.purityFractionFilter)

# =============== Extra Reco Steps =====================

process.load("edwenger.Skims.ChargedCandidates_cff") 
process.load("edwenger.Skims.ExtraVertex_cff")
#process.load("edwenger.Skims.TrackRefit_cff")
process.load("edwenger.Skims.RootpleProducer_cfi")
process.load("davidlw.ROOTupleAnalyzer.AnalysisParticles_cfi")

process.extraVtx = cms.Sequence(process.extraVertex *          ## agglomerative pixel vertexing
                                process.postEvtSelTest *
                                process.selectedVertex *       ## most-populated (filters)
                                process.postVtxTest)

process.extraReco = cms.Sequence(process.chargedCandidates *   ## selected tracks -> charged candidates
                                 #process.trackRefit *         ## refit constrained to PV
                                 process.preTrkVtxTest *
                                 process.primaryVertexFilter * ## non-fake, ndof>4, abs(z)<15
                                 process.postTrkVtxTest *
                                 process.AnalysisParticles *  ## selected gen particles
                                 process.rootpleProducer)     ## make wei's rootples

# =============== PAT ===========================

process.load("edwenger.Skims.patAna_cff")
# get the 7 TeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "Summer09_7TeV_ReReco332")

#================ Track Association =============

#process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
#process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")
#process.trackingParticleRecoTrackAsssociation.label_tr = cms.InputTag("generalTracks")
#process.TrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')

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
                                    process.trackAna *
                                    #process.trackingParticleRecoTrackAsssociation *
                                    process.trkEffAnalyzer)

# =============== Output ================================
process.load("edwenger.Skims.analysisSkimContent_cff")
process.output = cms.OutputModule("PoolOutputModule",
    process.analysisSkimContent,
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('eventFilter_step')),
    dataset = cms.untracked.PSet(
      dataTier = cms.untracked.string('AODSIM'),
      filterName = cms.untracked.string('TrkAnaFilter')),
    fileName = cms.untracked.string('trkAnaSkimAODSIM.root')
    )

process.output_step = cms.EndPath(process.output)

# =============== Schedule =====================

from edwenger.Skims.enableSIM_cfi import *
process = enableSIM(process) #activate isGEN in analyzers

process.schedule = cms.Schedule(process.eventFilter_step,
                                process.extraReco_step,
				process.pat_step,
                                process.ana_step,
                                process.output_step)
