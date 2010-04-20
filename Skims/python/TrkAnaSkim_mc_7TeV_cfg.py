import FWCore.ParameterSet.Config as cms
process = cms.Process("ANASKIM")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

# =============== 7 TeV Collisions =====================

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
    '/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_356ReReco-v1/0009/FEFC70B6-F53D-DF11-B57E-003048679150.root',
    '/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_356ReReco-v1/0009/FED8673E-F53D-DF11-9E58-0026189437EB.root'),
)

# =============== Other Statements =====================

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.GlobalTag.globaltag = 'START3X_V26A::All'

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.10 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/UserCode/edwenger/Skims/python/TrkAnaSkim_mc_7TeV_cfg.py,v $'),
    annotation = cms.untracked.string('BPTX_AND + BSC_OR + !BSCHALO')
)

# =============== Extra Reco Steps =====================
process.load("edwenger.Skims.ExtraVertex_cff")       # agglomerative pixel vertexing
#process.load("edwenger.Skims.BeamSpot7TeV_cff")      # custom beamspot db source
process.load("edwenger.Skims.TrackRefit_cff")        # refit constrained to primary Vertex

#================ Track Association =====================
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")
process.trackingParticleRecoTrackAsssociation.label_tr = cms.InputTag("generalTracks")
process.TrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')

#================ Fake and Real track producer ==========
process.load("PPTrackingTools.FakeAndRealTrackSelector.selectFakeAndReal_cff")

# =============== PAT ===========================
process.load("edwenger.Skims.patAna_cff")
# get the 7 TeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "Summer09_7TeV_ReReco332")

#================= MTV for eff/fake rate ================
process.load("Validation.RecoTrack.MultiTrackValidator_cff")
process.load("Validation.RecoTrack.cuts_cff")
process.load("CmsHi.TrackAnalysis.findableSimTracks_cfi")

# cuts
process.cutsTPEffic.minRapidity  = cms.double(-2.5)
process.cutsTPEffic.maxRapidity  = cms.double(2.5)

process.findableSimTracks.ptMin = 0.9
process.findableSimTracks.minHit = 8
process.findableSimTracks.minRapidity = cms.double(-2.5)
process.findableSimTracks.maxRapidity = cms.double(2.5)
process.findableSimTracks.tip = cms.double(30)
process.findableSimTracks.lip = cms.double(30)
#process.findableSimTracks.tpStatusBased = cms.bool(True)

process.cutsRecoTracks.quality = ['highPurity']
process.cutsRecoTracks.minHit   = cms.int32(8)
process.cutsRecoTracks.minRapidity  = cms.double(-2.5)
process.cutsRecoTracks.maxRapidity  = cms.double(2.5)

# mtv config
process.multiTrackValidator.outputFile = 'multitrackvalidator.root'
process.multiTrackValidator.associators = ['TrackAssociatorByHits']
process.multiTrackValidator.skipHistoFit=cms.untracked.bool(False)
#process.multiTrackValidator.label_tp_effic = cms.InputTag("cutsTPEff") # selection on mergedtruth
process.multiTrackValidator.label_tp_effic = cms.InputTag("findableSimTracks")
process.multiTrackValidator.label_tp_fake  = cms.InputTag("cutsTPFake")
process.multiTrackValidator.label = ['cutsRecoTracks']
process.multiTrackValidator.useLogPt=cms.untracked.bool(True)
process.multiTrackValidator.minpT = cms.double(0.1)
process.multiTrackValidator.maxpT = cms.double(3000.0)
process.multiTrackValidator.nintpT = cms.int32(40)
process.multiTrackValidator.UseAssociators = cms.bool(True)

process.mtv = cms.Sequence(
    process.findableSimTracks *
    #process.cutsTPEffic *
    process.cutsTPFake  *
    process.cutsRecoTracks *
    process.multiTrackValidator
)

# =============== Final Paths =====================
process.load("edwenger.Skims.eventSelection_cff")
process.load("edwenger.Skims.hfCoincFilter_cff")
process.eventFilter = cms.Sequence(process.minBiasBscFilterMC *
                                   process.hfCoincFilter *
                                   process.purityFractionFilter)

process.trkAnaSkim_step = cms.Path(process.eventFilter *
                                   #process.offlineBeamSpot *
                                   process.extraVertex *
                                   process.trackRefit *
                                   process.trackingParticleRecoTrackAsssociation*
                                   process.selectFakeAndReal *
                                   #process.selectFakeAndRealLoose *
                                   process.mtv)

process.pat_step = cms.Path(process.eventFilter * process.patAnaSequence)


# =============== Output ================================
process.load("edwenger.Skims.analysisSkimContent_cff")
process.output = cms.OutputModule("PoolOutputModule",
    process.analysisSkimContent,
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('trkAnaSkim_step')),
    dataset = cms.untracked.PSet(
      dataTier = cms.untracked.string('AOD'),
      filterName = cms.untracked.string('TrkAnaFilter')),
    fileName = cms.untracked.string('trkAnaSkimAODSIM.root')
    )

process.output_step = cms.EndPath(process.output)

process.schedule = cms.Schedule(process.trkAnaSkim_step,
				process.pat_step,
                                process.output_step)
