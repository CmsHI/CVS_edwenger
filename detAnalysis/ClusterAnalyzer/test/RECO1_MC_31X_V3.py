# Auto generated configuration file
# using: 
# Revision: 1.123 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step2 -s RAW2DIGI,RECO,ALCA:MuAlCalIsolatedMu+RpcCalHLT,VALIDATION --relval 25000,100 --datatier GEN-SIM-RECO --eventcontent RECOSIM --conditions FrontierConditions_GlobalTag,MC_31X_V3::All --no_exec --python_filename RECO1_MC_31X_V3.py

import FWCore.ParameterSet.Config as cms

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( ['file:cmsData/QCD_Pt_3000_3500_cfi_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_noNoise_1.root'] );
#readFiles.extend( ['file:cmsData/QCD_Pt_3000_3500_cfi_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_defaultNoise_1.root'] );
#readFiles.extend( [
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/FE575692-CC78-DE11-8E27-001D09F2AF96.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/FC198C97-CC78-DE11-A7A4-001D09F241D2.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/F055C8A3-CC78-DE11-B539-000423D98BC4.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/D6C5D18A-CC78-DE11-A22D-001D09F29114.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/C2BDFDCA-CC78-DE11-AFC2-001D09F2447F.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/C08BBFB8-CE78-DE11-90DB-001D09F24664.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/ACB77B96-CC78-DE11-8C56-0019B9F72CE5.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/AC3183E7-CC78-DE11-A022-001D09F24024.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/A058F3C0-CC78-DE11-91E7-001D09F23A61.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/9EE73BA5-CC78-DE11-97F7-000423D8F63C.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/98818EB9-CC78-DE11-A4E1-001D09F24DDA.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/9691D28E-D278-DE11-BC29-000423D99394.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/94F7E9EE-E278-DE11-9B75-001D09F2516D.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/8AC868A1-CC78-DE11-B8AA-001D09F24664.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/867F88CE-D178-DE11-AF51-000423D9890C.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/84CA3C9A-CC78-DE11-B409-001D09F2532F.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/84C5E1A9-CC78-DE11-A283-0019B9F72F97.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/84B8D800-CD78-DE11-B2E0-000423D99E46.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/80A958A3-CC78-DE11-B4AF-000423D99F1E.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/7230EBA1-CC78-DE11-A023-001D09F251BD.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/7099D79E-CC78-DE11-ACBB-000423D98FBC.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/6E8C60C0-CC78-DE11-9258-001D09F2960F.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/62FBBAC6-CC78-DE11-9C88-001D09F251FE.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/60D47CC9-CC78-DE11-B203-001D09F23A07.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/5C501299-CC78-DE11-8C15-001D09F23A61.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/565584AA-CC78-DE11-ABA7-001D09F295FB.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/56120694-CC78-DE11-914B-0019B9F71A6B.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/50FD14A0-CC78-DE11-8F84-001D09F251FE.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/4891923D-D578-DE11-BBB5-000423D98868.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/400628A3-CC78-DE11-9424-000423D60FF6.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/3EAC289E-CD78-DE11-ADE0-001D09F24EAC.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/20D79C96-CC78-DE11-90F8-001D09F2915A.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/1E0A93BF-CC78-DE11-B57D-000423D98BC4.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/1C4946CA-CC78-DE11-AE0D-001D09F24EAC.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/1A09B692-CC78-DE11-AF45-001D09F248F8.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/1057C03A-D178-DE11-A339-000423D98EC4.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/0822DD24-E978-DE11-B43C-001D09F253C0.root',
#'/store/relval/CMSSW_3_1_2/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0007/02AAD3A6-D078-DE11-9C9A-000423D98EC8.root' ] );
secFiles.extend( [
               ] )

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/RawToDigi_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/AlCaRecoStreams_cff')
process.load('Configuration/StandardSequences/Validation_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('step2 nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('step2_DIGI2RAW.root')
#)
process.source = source

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.RECOSIMEventContent.outputCommands,
#    fileName = cms.untracked.string('step2_RAW2DIGI_RECO_ALCA_VALIDATION.root'),
    fileName = cms.untracked.string('cmsData/step2_RAW2DIGI_RECO.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RECO'),
        filterName = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
process.mix.playback = True
process.GlobalTag.globaltag = 'MC_31X_V3::All'

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.pathALCARECOHcalCalHOCosmics = cms.Path(process.seqALCARECOHcalCalHOCosmics)
process.pathALCARECOMuAlStandAloneCosmics = cms.Path(process.seqALCARECOMuAlStandAloneCosmics*process.ALCARECOMuAlStandAloneCosmicsDQM)
process.pathALCARECOTkAlZMuMu = cms.Path(process.seqALCARECOTkAlZMuMu*process.ALCARECOTkAlZMuMuDQM)
process.pathALCARECOTkAlCosmicsCTF0T = cms.Path(process.seqALCARECOTkAlCosmicsCTF0T*process.ALCARECOTkAlCosmicsCTF0TDQM)
process.pathALCARECOMuAlBeamHalo = cms.Path(process.seqALCARECOMuAlBeamHalo*process.ALCARECOMuAlBeamHaloDQM)
process.pathALCARECOTkAlCosmicsRS0THLT = cms.Path(process.seqALCARECOTkAlCosmicsRS0THLT*process.ALCARECOTkAlCosmicsRS0TDQM)
process.pathALCARECOTkAlCosmicsCTF = cms.Path(process.seqALCARECOTkAlCosmicsCTF*process.ALCARECOTkAlCosmicsCTFDQM)
process.pathALCARECOHcalCalIsoTrk = cms.Path(process.seqALCARECOHcalCalIsoTrk*process.ALCARECOHcalCalIsoTrackDQM)
process.pathALCARECOHcalCalHO = cms.Path(process.seqALCARECOHcalCalHO*process.ALCARECOHcalCalHODQM)
process.pathALCARECOTkAlCosmicsCTFHLT = cms.Path(process.seqALCARECOTkAlCosmicsCTFHLT*process.ALCARECOTkAlCosmicsCTFDQM)
process.pathALCARECOTkAlCosmicsRS0T = cms.Path(process.seqALCARECOTkAlCosmicsRS0T*process.ALCARECOTkAlCosmicsRS0TDQM)
process.pathALCARECOTkAlCosmicsCosmicTFHLT = cms.Path(process.seqALCARECOTkAlCosmicsCosmicTFHLT*process.ALCARECOTkAlCosmicsCosmicTFDQM)
process.pathALCARECOHcalCalMinBias = cms.Path(process.seqALCARECOHcalCalMinBias*process.ALCARECOHcalCalPhisymDQM)
process.pathALCARECOTkAlMuonIsolated = cms.Path(process.seqALCARECOTkAlMuonIsolated*process.ALCARECOTkAlMuonIsolatedDQM)
process.pathALCARECOTkAlUpsilonMuMu = cms.Path(process.seqALCARECOTkAlUpsilonMuMu*process.ALCARECOTkAlUpsilonMuMuDQM)
process.pathALCARECOHcalCalDijets = cms.Path(process.seqALCARECOHcalCalDijets*process.ALCARECOHcalCalDiJetsDQM)
process.pathALCARECOMuAlZMuMu = cms.Path(process.seqALCARECOMuAlZMuMu*process.ALCARECOMuAlZMuMuDQM)
process.pathALCARECOEcalCalPi0Calib = cms.Path(process.seqALCARECOEcalCalPi0Calib*process.ALCARECOEcalCalPi0CalibDQM)
process.pathALCARECOTkAlBeamHalo = cms.Path(process.seqALCARECOTkAlBeamHalo*process.ALCARECOTkAlBeamHaloDQM)
process.pathALCARECOSiPixelLorentzAngle = cms.Path(process.seqALCARECOSiPixelLorentzAngle)
process.pathALCARECOTkAlCosmicsCosmicTF0T = cms.Path(process.seqALCARECOTkAlCosmicsCosmicTF0T*process.ALCARECOTkAlCosmicsCosmicTF0TDQM)
process.pathALCARECOEcalCalElectron = cms.Path(process.seqALCARECOEcalCalElectron*process.ALCARECOEcalCalElectronCalibDQM)
process.pathALCARECOTkAlCosmicsCTF0THLT = cms.Path(process.seqALCARECOTkAlCosmicsCTF0THLT*process.ALCARECOTkAlCosmicsCTF0TDQM)
process.pathALCARECOMuAlCalIsolatedMu = cms.Path(process.seqALCARECOMuAlCalIsolatedMu*process.ALCARECOMuAlCalIsolatedMuDQM*process.ALCARECODTCalibrationDQM)
process.pathALCARECOSiStripCalZeroBias = cms.Path(process.seqALCARECOSiStripCalZeroBias*process.ALCARECOSiStripCalZeroBiasDQM)
process.pathALCARECOTkAlCosmicsRSHLT = cms.Path(process.seqALCARECOTkAlCosmicsRSHLT*process.ALCARECOTkAlCosmicsRSDQM)
process.pathALCARECOEcalCalEtaCalib = cms.Path(process.seqALCARECOEcalCalEtaCalib*process.ALCARECOEcalCalEtaCalibDQM)
process.pathALCARECOSiStripCalMinBias = cms.Path(process.seqALCARECOSiStripCalMinBias)
process.pathALCARECODQM = cms.Path(process.MEtoEDMConverter)
process.pathALCARECOTkAlLAS = cms.Path(process.seqALCARECOTkAlLAS*process.ALCARECOTkAlLASDQM)
process.pathALCARECOTkAlMinBias = cms.Path(process.seqALCARECOTkAlMinBias*process.ALCARECOTkAlMinBiasDQM)
process.pathALCARECOTkAlCosmicsRS = cms.Path(process.seqALCARECOTkAlCosmicsRS*process.ALCARECOTkAlCosmicsRSDQM)
process.pathALCARECORpcCalHLT = cms.Path(process.seqALCARECORpcCalHLT)
process.pathALCARECOHcalCalGammaJet = cms.Path(process.seqALCARECOHcalCalGammaJet)
process.pathALCARECOMuAlBeamHaloOverlaps = cms.Path(process.seqALCARECOMuAlBeamHaloOverlaps*process.ALCARECOMuAlBeamHaloOverlapsDQM)
process.pathALCARECOTkAlCosmicsCosmicTF0THLT = cms.Path(process.seqALCARECOTkAlCosmicsCosmicTF0THLT*process.ALCARECOTkAlCosmicsCosmicTF0TDQM)
process.pathALCARECOHcalCalNoise = cms.Path(process.seqALCARECOHcalCalNoise)
process.pathALCARECOMuAlOverlaps = cms.Path(process.seqALCARECOMuAlOverlaps*process.ALCARECOMuAlOverlapsDQM)
process.pathALCARECOTkAlCosmicsCosmicTF = cms.Path(process.seqALCARECOTkAlCosmicsCosmicTF*process.ALCARECOTkAlCosmicsCosmicTFDQM)
process.pathALCARECOEcalCalPhiSym = cms.Path(process.seqALCARECOEcalCalPhiSym*process.ALCARECOEcalCalPhisymDQM)
process.pathALCARECOMuAlGlobalCosmics = cms.Path(process.seqALCARECOMuAlGlobalCosmics*process.ALCARECOMuAlGlobalCosmicsDQM)
process.pathALCARECOTkAlJpsiMuMu = cms.Path(process.seqALCARECOTkAlJpsiMuMu*process.ALCARECOTkAlJpsiMuMuDQM)
process.validation_step = cms.Path(process.validation)
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.pathALCARECORpcCalHLT,process.pathALCARECOMuAlCalIsolatedMu,process.validation_step,process.endjob_step,process.out_step)
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.out_step)
