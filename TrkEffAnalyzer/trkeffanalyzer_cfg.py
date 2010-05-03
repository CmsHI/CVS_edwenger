# Auto generated configuration file
# using: 
# Revision: 1.168.2.1 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step2 -s RAW2DIGI,RECO --datatier GEN-SIM-RECO --eventcontent RECODEBUG --conditions auto:mc --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.MixingNoPileUp_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.168.2.1 $'),
    annotation = cms.untracked.string('step2 nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.options = cms.untracked.PSet(
    #wantSummary = cms.untracked.bool(True)
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#'/store/relval/CMSSW_3_5_7/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V26-v1/0013/7E0CC2E0-7F49-DF11-8626-003048678B1A.root'
'/store/relval/CMSSW_3_5_7/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V26-v1/0012/1CFFEFA0-5B49-DF11-B237-0018F3D09698.root'
)
)

# Track efficiency analyzer
process.ana = cms.EDAnalyzer('TrkEffAnalyzer',
         tracks = cms.untracked.InputTag('generalTracks'),
         label_tp_effic = cms.untracked.InputTag('mergedtruth','MergedTrackTruth'),
         label_tp_fake = cms.untracked.InputTag('mergedtruth','MergedTrackTruth'),                 
         associatormap = cms.untracked.InputTag('trackingParticleRecoTrackAsssociation')
)

# Output definition
process.RECODEBUGEventContent.outputCommands.extend(['keep *_trackingParticleRecoTrackAsssociation_*_*'])
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.RECODEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('output_trkeff.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RECO'),
        filterName = cms.untracked.string('')
    )
)

# Additional output definition
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('hists.root')
                                   )

# Other statements
process.GlobalTag.globaltag = 'MC_3XY_V26::All'

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.assoc_step = cms.Path(process.trackingParticleRecoTrackAsssociation)
process.ana_step = cms.Path(process.ana)
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.assoc_step,process.ana_step,process.endjob_step,process.out_step)
