import FWCore.ParameterSet.Config as cms

process = cms.Process("HFSKIM")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

# Global tags for data re-processing: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideFrontierConditions#Global_tags_for_2009_collisi_AN1
process.GlobalTag.globaltag = 'GR09_R_34X_V5::All'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/d101/edwenger/data/v4/trkAnaSkimAOD_1.root')
)

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.4 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/UserCode/edwenger/Skims/python/TrkAnaSkim_data_900GeV_cfg.py,v $'),
    annotation = cms.untracked.string('HF coincidence')
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

### Filter path ###
process.load("edwenger.Skims.hfCoincFilter_cff")
process.filter_step = cms.Path(process.hfCoincFilter)

### Output ###
process.output = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('filter_step')),
    dataset = cms.untracked.PSet(
      dataTier = cms.untracked.string('AOD'),
      filterName = cms.untracked.string('hfCoincFilter')),
    fileName = cms.untracked.string('hfSkim.root')
    )

process.outpath = cms.EndPath(process.output)
