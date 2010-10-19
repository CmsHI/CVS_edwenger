import FWCore.ParameterSet.Config as cms
from SimCalorimetry.HcalSimProducers.hcalUnsuppressedDigis_cfi import hcalSimBlock


mixData = cms.EDProducer("DataMixingModule",
                   hcalSimBlock,
    input = cms.SecSource("PoolSource",
        nbPileupEvents = cms.PSet(
            averageNumber = cms.double(1.0)
        ),
        seed = cms.int32(1234567),
        type = cms.string('fixed'),
        sequential = cms.untracked.bool(False), 
        fileNames = cms.untracked.vstring(
            'file:DMPreProcess_RAW2DIGI.root'
        )
    ),
    # Mixing Module parameters
    Label = cms.string(''),
    maxBunch = cms.int32(0),
    bunchspace = cms.int32(25),
    minBunch = cms.int32(0),
    #                   
    mixProdStep1 = cms.bool(False),
    mixProdStep2 = cms.bool(False),
    IsThisFastSim = cms.string('NO'),  # kludge for fast simulation flag...
    # Use digis?               
    EcalMergeType = cms.string('Digis'),  # set to "Digis" to merge digis
    HcalMergeType = cms.string('Digis'),
    HcalDigiMerge = cms.string('NotFullProd'),
    #
    # Input Specifications:
    #
    SiStripRawDigiSource = cms.string("NONE"), # rawdigi+digi->rawdigi (specify 'SIGNAL' or 'PILEUP')
    SiStripRawInputTag = cms.InputTag("siStripDigis","VirginRaw"),               
    #                         
    SistripLabelSig = cms.InputTag("ZeroSuppressed"),
    SistripdigiCollectionSig = cms.InputTag("siStripDigis"),
                   #
    pixeldigiCollectionSig = cms.InputTag("siPixelDigis"),
    #
    SiStripPileInputTag = cms.InputTag("siStripDigis","ZeroSuppressed"),
                   #
    pixeldigiCollectionPile = cms.InputTag("siPixelDigis"),
                   #
    EBProducerSig = cms.InputTag("ecalRecHit"),
    EBrechitCollectionSig = cms.InputTag("EcalRecHitsEB"),                   
    EEProducerSig = cms.InputTag("ecalRecHit"),                   
    EErechitCollectionSig = cms.InputTag("EcalRecHitsEE"),
    ESProducerSig = cms.InputTag("ecalPreshowerRecHit"),
    ESrechitCollectionSig = cms.InputTag("EcalRecHitsES"),                   
                   #
    HBHEProducerSig = cms.InputTag("hbhereco"),
    HBHErechitCollectionSig = cms.InputTag(""),
    HOProducerSig = cms.InputTag("horeco"),                   
    HOrechitCollectionSig = cms.InputTag(""),
    HFProducerSig = cms.InputTag("hfreco"),
    HFrechitCollectionSig = cms.InputTag(""),                   
    ZDCrechitCollectionSig = cms.InputTag("zdcreco"),
    #
    EBPileRecHitInputTag = cms.InputTag("ecalRecHit", "EcalRecHitsEB"),
    EEPileRecHitInputTag = cms.InputTag("ecalRecHit", "EcalRecHitsEE"),
    ESPileRecHitInputTag = cms.InputTag("ecalPreshowerRecHit", "EcalRecHitsES"),                  
    #
    HBHEPileRecHitInputTag = cms.InputTag("hbhereco", ""),
    HOPileRecHitInputTag = cms.InputTag("horeco", ""),                   
    HFPileRecHitInputTag = cms.InputTag("hfreco", ""),
    ZDCPileRecHitInputTag = cms.InputTag("zdcreco",""),
    #
    # Calorimeter digis
    #
    EBdigiProducerSig = cms.InputTag("ecalDigis"),
    EEdigiProducerSig = cms.InputTag("ecalDigis"),
    ESdigiProducerSig = cms.InputTag("ecalPreshowerDigis"),
    EBdigiCollectionSig = cms.InputTag("ebDigis"),
    EEdigiCollectionSig = cms.InputTag("eeDigis"),
    ESdigiCollectionSig = cms.InputTag(""),
    HBHEdigiCollectionSig  = cms.InputTag("hcalDigis"),
    HOdigiCollectionSig    = cms.InputTag("hcalDigis"),
    HFdigiCollectionSig    = cms.InputTag("hcalDigis"),
    ZDCdigiCollectionSig   = cms.InputTag("hcalDigis"),          
    #
    EBPileInputTag = cms.InputTag("ecalDigis","ebDigis"),
    EEPileInputTag = cms.InputTag("ecalDigis","eeDigis"),
    ESPileInputTag = cms.InputTag("ecalPreshowerDigis",""),
    HBHEPileInputTag = cms.InputTag("hcalDigis"),                  
    HOPileInputTag   = cms.InputTag("hcalDigis"),                  
    HFPileInputTag   = cms.InputTag("hcalDigis"),                  
    ZDCPileInputTag  = cms.InputTag("hcalDigis"),          
    #  Signal
    CSCDigiTagSig = cms.InputTag("muonCSCDigis"),
    CSCwiredigiCollectionSig = cms.InputTag("MuonCSCWireDigi"),
    CSCstripdigiCollectionSig = cms.InputTag("MuonCSCStripDigi"),
    CSCCompdigiCollectionSig = cms.InputTag("MuonCSCComparatorDigi"),
    RPCDigiTagSig = cms.InputTag("muonRPCDigis"),                   
    RPCdigiCollectionSig = cms.InputTag("MuonRPCDigis"),
    DTDigiTagSig = cms.InputTag("muonDTDigis"),
    DTdigiCollectionSig = cms.InputTag("MuonDTDigis"),
                   #
    #  Pileup
                   #                   
    DTPileInputTag        = cms.InputTag("muonDTDigis"),
    RPCPileInputTag       = cms.InputTag("muonRPCDigis"),
    CSCWirePileInputTag   = cms.InputTag("muonCSCDigis","MuonCSCWireDigi"),
    CSCStripPileInputTag  = cms.InputTag("muonCSCDigis","MuonCSCStripDigi"),
    CSCCompPileInputTag   = cms.InputTag("muonCSCDigis","MuonCSCComparatorDigi"),
                   #
    #
    #  Outputs
    #
    SiStripDigiCollectionDM = cms.string('siStripDigisDM'),
    PixelDigiCollectionDM = cms.string('siPixelDigisDM'),                   
    EBRecHitCollectionDM = cms.string('EcalRecHitsEBDM'),
    EERecHitCollectionDM = cms.string('EcalRecHitsEEDM'),                   
    ESRecHitCollectionDM = cms.string('EcalRecHitsESDM'),
    HBHERecHitCollectionDM = cms.string('HBHERecHitCollectionDM'),
    HFRecHitCollectionDM = cms.string('HFRecHitCollectionDM'),
    HORecHitCollectionDM = cms.string('HORecHitCollectionDM'),                   
    ZDCRecHitCollectionDM = cms.string('ZDCRecHitCollectionDM'),
    DTDigiCollectionDM = cms.string('muonDTDigisDM'),
    CSCWireDigiCollectionDM = cms.string('MuonCSCWireDigisDM'),
    CSCStripDigiCollectionDM = cms.string('MuonCSCStripDigisDM'),
    CSCComparatorDigiCollectionDM = cms.string('MuonCSCComparatorDigisDM'),
    RPCDigiCollectionDM = cms.string('muonRPCDigisDM'),
    #
    #  Calorimeter Digis
    #               
    EBDigiCollectionDM   = cms.string('EBDigiCollectionDM'),
    EEDigiCollectionDM   = cms.string('EEDigiCollectionDM'),
    ESDigiCollectionDM   = cms.string(''),
    HBHEDigiCollectionDM = cms.string(''),
    HODigiCollectionDM   = cms.string(''),
    HFDigiCollectionDM   = cms.string(''),
    ZDCDigiCollectionDM  = cms.string('')
)

