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
process.GlobalTag.globaltag = 'GR_R_35X_V6::All'

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.14 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/UserCode/edwenger/Skims/python/TrkAnaSkim_data_7TeV_cfg.py,v $'),
    annotation = cms.untracked.string('BPTX_AND + BSC_OR + !BSCHALO')
)

# =============== Event Filter =====================

process.load("edwenger.Skims.eventSelection_cff")
process.load("edwenger.Skims.hfCoincFilter_cff")

process.eventFilter = cms.Sequence(process.minBiasBscFilter *
                                   process.hfCoincFilter *
                                   process.purityFractionFilter)

# =============== Extra Reco Steps =====================

#process.load("edwenger.Skims.BeamSpot7TeV_cff")     # custom beamspot db source
process.load("edwenger.Skims.ChargedCandidates_cff") # make charged candidates from selected tracks
process.load("edwenger.Skims.ExtraVertex_cff")       # agglomerative pixel vertexing
process.load("edwenger.Skims.TrackRefit_cff")        # refit constrained to primary vertex
process.load("edwenger.Skims.RootpleProducer_cfi")   # make wei's rootples

# rootuple output file
process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string('ROOTuple_HighPurity.root')
                                   )


process.extraReco = cms.Sequence(#process.offlineBeamspot *
                                 process.chargedCandidates *
                                 process.extraVertex *
                                 process.trackRefit *
                                 process.primaryVertexFilter *
                                 process.rootpleProducer)

# =============== Final Filter Paths =====================

process.eventFilter_step = cms.Path(process.eventFilter)
process.extraReco_step = cms.Path(process.eventFilter * process.extraReco)

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

process.schedule = cms.Schedule(process.eventFilter_step,
                                process.extraReco_step,
                                process.output_step)
