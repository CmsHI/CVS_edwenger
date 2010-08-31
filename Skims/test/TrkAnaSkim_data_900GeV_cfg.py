import FWCore.ParameterSet.Config as cms
process = cms.Process("ANASKIM")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

# =============== 7 TeV Collision Data =====================

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      # Run 134725
      '/store/data/Commissioning10/MinimumBias/RECO/SD_JetMETTau-Jun14thSkim_v1/0129/7E781D15-4380-DF11-902E-001A928116B2.root',
      '/store/data/Commissioning10/MinimumBias/RECO/SD_JetMETTau-Jun14thSkim_v1/0129/3A420F2A-4380-DF11-9B3E-0018F3D096F0.root',
      '/store/data/Commissioning10/MinimumBias/RECO/SD_JetMETTau-Jun14thSkim_v1/0129/103BCE28-4380-DF11-BE6B-00261894395F.root',
      '/store/data/Commissioning10/MinimumBias/RECO/SD_JetMETTau-Jun14thSkim_v1/0107/B44DBE51-437E-DF11-B273-002618FDA204.root',
      '/store/data/Commissioning10/MinimumBias/RECO/SD_JetMETTau-Jun14thSkim_v1/0107/A8D2D54D-437E-DF11-8712-0030486792DE.root'
      ),
    lumisToProcess = cms.untracked.VLuminosityBlockRange(
      '134725:231-134725:700')
    )

# =============== Other Statements =====================

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.GlobalTag.globaltag = 'GR_R_36X_V12A::All'

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.4 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/UserCode/edwenger/Skims/test/TrkAnaSkim_data_900GeV_cfg.py,v $'),
    annotation = cms.untracked.string('BPTX_AND + BSC_OR + !BSCHALO')
)

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string('trkhists.root')
                                   )
process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',
    ignoreTotal=cms.untracked.int32(0),
    oncePerEventMode = cms.untracked.bool(False)
    )


# =============== Import Sequences =====================

process.load("edwenger.Skims.EventFilter_cff")
process.load("edwenger.Skims.ExtraReco_cff")
process.load("edwenger.Skims.Analysis_cff")

#from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJECSet(process, "900GeV") # get the 900 GeV jet corrections, for the moment use default 7TeV spring10 JEC

from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All']) # turn off MC matching for data

from edwenger.Skims.customise_cfi import *
process.trackAna_STD.evtSelEffv = getEvtSelEff("STD_NSD_900GeV_TrkVtx")
process.looseTrackAna_STD.evtSelEffv = getEvtSelEff("STD_NSD_900GeV_PixVtx")
#enableHLTJet15U(process)
#process = enableAOD(process)

# =============== Final Paths =====================

process.eventFilter_step = cms.Path(process.eventFilter)
process.extraReco_step   = cms.Path(process.eventFilter * process.extraReco)
process.ana_step         = cms.Path(process.eventFilter * process.analysisSeq)

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

process.schedule = cms.Schedule(
    process.eventFilter_step,
    process.extraReco_step,
    process.ana_step,
    process.output_step
    )
