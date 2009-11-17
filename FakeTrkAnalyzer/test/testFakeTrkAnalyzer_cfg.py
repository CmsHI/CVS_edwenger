import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import os

process = cms.Process("TEST")

#global tags for conditions data:
#https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'MC_31X_V9::All'


##################################################################################

# setup 'standard'  options
options = VarParsing.VarParsing ('standard')

# setup any defaults you want
options.output = 'output_testFakes.root'
options.secondaryOutput = 'edmFile_testFakes.root'
options.files = [

       #'file:/d01/edwenger/Summer09/MinBias/ZeroBias/lowpTRECOSkim_1.root'

       #'file:/d01/y_alive/early_spectra/RECO/RECO_QCD_Pt_100_110_2200GeV.root'

       #'file:/d01/y_alive/early_spectra/RECO/RECO_QCD_Pt_200_210_2200GeV.root'

       #'file:/d01/y_alive/early_spectra/RECO/RECO_QCD_Pt_500_510_2200GeV.root'

       'file:/d01/y_alive/early_spectra/RECO/RECO_QCD_Pt_300_400_EVRT_TEST.root'
    
    ]
options.maxEvents = -1 

# get and parse the command line arguments
options.parseArguments()

##################################################################################

# Input Source
process.source = cms.Source('PoolSource',
	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
	fileNames = cms.untracked.vstring(options.files) 
)
							
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

# Track quality cuts
process.load("Validation.RecoTrack.cuts_cff")
process.cutsRecoTracks.quality = cms.vstring('highPurity')
#process.cutsRecoTracks.tip = cms.double(0.2)
#process.cutsRecoTracks.minHit = cms.int32(8)

# Track associator
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")
process.trackingParticleRecoTrackAsssociation.label_tr = cms.InputTag("cutsRecoTracks")

# Jet energy corrections
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff")

# Analyzer: fake tracks
process.load("edwenger.FakeTrkAnalyzer.faketrkanalyzer_cfi")
process.fakeTrkAnalyzer.ptMin=30.0
process.fakeTrkAnalyzer.tracks=cms.untracked.InputTag('cutsRecoTracks')
process.fakeTrkAnalyzer.jets=cms.untracked.InputTag('L2L3CorJetIC5Calo')

# output file service
process.TFileService = cms.Service( "TFileService", fileName = cms.string(options.output) )

process.ana = cms.Path(process.cutsRecoTracks
                       * process.L2L3CorJetIC5Calo
                       * process.trackingParticleRecoTrackAsssociation
                       * process.fakeTrkAnalyzer)
