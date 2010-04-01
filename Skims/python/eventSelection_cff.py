import FWCore.ParameterSet.Config as cms

# ================ Physics Declared Bit ================
physDeclFilter = cms.EDFilter("PhysDecl",
    applyfilter = cms.untracked.bool(True)
    )


# ================ L1 Filters =====================
from L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff import *
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed
from L1Trigger.Skimmer.l1Filter_cfi import l1Filter

bptxAnd = hltLevel1GTSeed.clone(
    L1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('0')
    )

bscOr = hltLevel1GTSeed.clone(
    L1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('34 OR 40 OR 41')
    )

bscNoHalo = hltLevel1GTSeed.clone(
    L1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('NOT (36 OR 37 OR 38 OR 39)')
    )

bscOrBptxOr = l1Filter.clone(
    algorithms = ["L1_BscMinBiasOR_BptxPlusORMinus"]
    )


# ================ HLT Filters =====================
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel

hltJets = hltHighLevel.clone(
    HLTPaths = cms.vstring('HLT_L1Jet6U','HLT_Jet15U','HLT_Jet30U'),
    andOr = cms.bool(True)
    )


# ================ Monster Event Rejection ==============
purityFractionFilter = cms.EDFilter("FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(True),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
    )


# ================ Vertex Filter ========================
primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
    vertexCollection = cms.InputTag('offlinePrimaryVertices'),
    minimumNDOF = cms.uint32(4),
    maxAbsZ = cms.double(15),	
    maxd0 = cms.double(2)	
    )

minBiasBscFilter = cms.Sequence(bscOrBptxOr * bptxAnd * bscOr * bscNoHalo)
