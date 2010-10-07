# Auto generated configuration file
# using: 
# Revision: 1.222.2.2 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: DMPreProcess -s RAW2DIGI -n 10 --eventcontent DATAMIXER --conditions FrontierConditions_GlobalTag,GR_R_38X_V7::All --filein yourDATAFile --data --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RAW2DIGI')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.222.2.2 $'),
    annotation = cms.untracked.string('DMPreProcess nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.options = cms.untracked.PSet(

)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/express/Run2010B/ExpressPhysics/FEVT/Express-v2/000/146/417/10F981FF-5EC6-DF11-9657-0030486733B4.root')
)

# Output definition

process.DATAMIXERoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.DATAMIXEREventContent.outputCommands,
    fileName = cms.untracked.string('DMPreProcess_RAW2DIGI.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)

# Additional output definition for RECO vertex and VR strip digis
process.DATAMIXERoutput.outputCommands.extend(['keep *_offlinePrimaryVertices_*_*',
                                               'keep *_siStripDigis_VirginRaw_*'])

# Other statements
process.GlobalTag.globaltag = 'GR_R_38X_V7::All'

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.Path(process.endOfProcess)
process.DATAMIXERoutput_step = cms.EndPath(process.DATAMIXERoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.endjob_step,process.DATAMIXERoutput_step)
