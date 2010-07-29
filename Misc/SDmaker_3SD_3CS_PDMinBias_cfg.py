import FWCore.ParameterSet.Config as cms

process = cms.Process("makeSD")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('SD and central skims'),
    name = cms.untracked.string('$Source: /local/projects/CMSSW/rep/CMSSW/Configuration/Skimming/test/SDmaker_6SD_3CS_PDMinBias_1e28_reprocess370patch2_cfg.py,v $')
    )

process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.GeometryExtended_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('Configuration.EventContent.EventContentHeavyIons_cff')
process.GlobalTag.globaltag = "MC_37X_V4::All"

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_3_8_0/RelValHydjetQ_B0_2760GeV/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V7-v1/0005/DCAB6598-7B95-DF11-861B-0026189438F2.root'
        ),
    inputCommands = cms.untracked.vstring("keep *", "drop *_MEtoEDMConverter_*_*")                  
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

import HLTrigger.HLTfilters.hltHighLevelDev_cfi

### JetHI SD
process.JetHI = HLTrigger.HLTfilters.hltHighLevelDev_cfi.hltHighLevelDev.clone()
process.JetHI.HLTPaths = ["HLT_HIJet35U"]
process.JetHI.HLTPathsPrescales  = cms.vuint32(1)
process.JetHI.HLTOverallPrescale = cms.uint32(1)
process.JetHI.throw = False
process.JetHI.andOr = True
process.filterSdJetHI = cms.Path(process.JetHI)

### PhotonHI SD
process.PhotonHI = process.JetHI.clone()
process.PhotonHI.HLTPaths = ["HLT_Photon15"]
process.filterSdPhotonHI = cms.Path(process.PhotonHI)

### MuHI SD
process.MuHI = process.JetHI.clone()
process.MuHI.HLTPaths = ["HLT_L1DoubleMuOpen"]
process.filterSdMuHI = cms.Path(process.MuHI)

### JetHI AOD CS

### MuHI AOD CS

### Zmumu AOD CS



process.outputSdJetHI = cms.OutputModule("PoolOutputModule",
                                         SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('filterSdJetHI')),                               
                                         dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('RECO'),
    filterName = cms.untracked.string('SD_JetHI')),
                                         outputCommands = process.RECOEventContent.outputCommands,
                                         fileName = cms.untracked.string('SD_JetHI.root')
                                         )

process.outputSdPhotonHI = cms.OutputModule("PoolOutputModule",
                                            SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('filterSdPhotonHI')),                               
                                            dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('RECO'),
    filterName = cms.untracked.string('SD_PhotonHI')),
                                            outputCommands = process.RECOEventContent.outputCommands,
                                            fileName = cms.untracked.string('SD_PhotonHI.root')
                                            )

process.outputSdMuHI = cms.OutputModule("PoolOutputModule",
                                        SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('filterSdMuHI')),                               
                                        dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('RECO'),
    filterName = cms.untracked.string('SD_MuHI')),
                                        outputCommands = process.RECOEventContent.outputCommands,
                                        fileName = cms.untracked.string('SD_MuHI.root')
                                        )


process.this_is_the_end = cms.EndPath(
    process.outputSdJetHI      +
    process.outputSdPhotonHI   +
    process.outputSdMuHI
)
