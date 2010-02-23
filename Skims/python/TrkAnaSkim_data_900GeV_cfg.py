# - References
#   * DPGAnalysis/Skims/python/
#   * https://twiki.cern.ch/twiki/bin/viewauth/CMS/TRKPromptFeedBack
#   * UserCode/SavedFMa/pp/jet/ana/MinBias2009Winter/skim

import FWCore.ParameterSet.Config as cms

process = cms.Process("ANASKIM")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
# activate the following lines to get some output
#process.MessageLogger.destinations = cms.untracked.vstring("cout")
#process.MessageLogger.cout = cms.untracked.PSet(threshold = cms.untracked.string("INFO"))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    "/store/data/BeamCommissioning09/MinimumBias/RECO/Feb9ReReco_v2/0025/F68A5ABB-E515-DF11-8A77-0026189437F0.root"
     ),
    eventsToProcess = cms.untracked.VEventRange(
    '123596:2-123596:max','123615:70-123615:max','123732:62-123732:109',
    '123815:8- 123815:max','123818:2-123818:42','123908:2-123908:12',
    '124008:1-124008:1','124009:1- 124009:68','124020:12-124020:94',
    '124022:66-124022:179','124023:38-124023:max','1240 24:2- 124024:83',
    '124025:5-124025:13','124027:24-124027:max','124030:2-124030:max'
     )
    )

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /local/projects/CMSSW/rep/UserCode/edwenger/Skims/python/TrkAnaSkim_data_900GeV_cfg.py,v $'),
    annotation = cms.untracked.string('BPTX_AND + BSC_OR + !BSCHALO')
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))


# ================ Physics Declared Bit ================
process.physDeclFilter = cms.EDFilter("PhysDecl",
    applyfilter = cms.untracked.bool(True)
    )


# ================ BPTX Coincidence =====================
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed
process.bptxAnd = hltLevel1GTSeed.clone(
    L1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('0')
    )

# ================ Single-side BSC, Beam Halo Veto ======
process.bscOr = hltLevel1GTSeed.clone(
    L1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('(34) AND NOT (36 OR 37 OR 38 OR 39)')
    )


# ================ Monster Event Rejection ==============
process.monsterVeto = cms.EDFilter("FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(True),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
    )


# ================ Vertex Filter ========================
#process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
#    vertexCollection = cms.InputTag('offlinePrimaryVertices'),
#    minimumNDOF = cms.uint32(4) ,
#    maxAbsZ = cms.double(15),	
#    maxd0 = cms.double(2)	
#    )

# =============== Final Filter Path =====================
process.trkAnaSkim_step = cms.Path(process.physDeclFilter *
                                   process.bptxAnd *
                                   process.bscOr *
                                   process.monsterVeto)


# =============== Output ================================
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *',
      # event
      'keep *_offlinePrimaryVertices_*_*',
      'keep *_pixelVertices_*_*',
      'keep *_offlineBeamSpot_*_*',
      'keep *_TriggerResults_*_HLT',
      'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*RECO',
      # jet
      'keep *Jet*_*CaloJet*_*_*',
      'keep *_*JetID*_*_*',
      'keep *Jet*_*JetExtender*_*_*',
      'keep *_towerMaker_*_*',
      'keep *_hcalnoise_*_*',
      # tracks
      'keep recoTracks_generalTracks_*_*RECO',
      # mc (if present)
      'keep *_*GenJet*_*_*',
      'keep reco*_genParticles_*_*',
      # clean up unwanted collections
      'drop *_*BJetTags*_*_*',
      'drop *_kt*Jet*_*_*',
      'drop *_sisCone*Jet*_*_*',
      'drop *_sc*Jet*_*_*'                                     
      ),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('trkAnaSkim_step')),
    dataset = cms.untracked.PSet(
      dataTier = cms.untracked.string('AOD'),
      filterName = cms.untracked.string('TrkAnaFilter')),
    fileName = cms.untracked.string('trkAnaSkimAOD.root')
    )

process.outpath = cms.EndPath(process.output)

