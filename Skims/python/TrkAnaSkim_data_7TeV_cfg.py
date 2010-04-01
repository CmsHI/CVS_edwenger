import FWCore.ParameterSet.Config as cms
process = cms.Process("ANASKIM")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.GlobalTag.globaltag = 'GR10_P_V4A::All'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/E27821C3-0C3C-DF11-9BD9-0030487CD718.root',
    #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/B647CAD9-0E3C-DF11-886F-0030487CD716.root',
    #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/9884BC11-0C3C-DF11-8F9C-000423D986C4.root',
    #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/90269E76-0D3C-DF11-A1A0-0030487CD840.root',
    #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/80471A6B-0E3C-DF11-8DCD-0030487C6A66.root',
    #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/762824C3-0C3C-DF11-A4FD-0030487CD6D2.root',
    #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/4C8979D2-073C-DF11-B97B-000423D6AF24.root',
    #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/181C44F7-093C-DF11-A9CB-001D09F24FEC.root',
    '/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/A8AC64BE-193D-DF11-B767-001D09F24E39.root',
    '/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/B4CC89EF-143D-DF11-8820-001D09F2514F.root',
    '/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/CE183D99-103D-DF11-8C68-00304879EE3E.root',
    '/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/CE36029B-173D-DF11-8743-001D09F24664.root',
    '/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/D67AE939-1D3D-DF11-AB0D-000423D98868.root',
    '/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/E65B765C-133D-DF11-9163-001617E30CC8.root',
    '/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/EAD8BF6F-213D-DF11-A988-001617E30E28.root',
    '/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/EE6BEB84-1C3D-DF11-9A95-0030487CD16E.root',
    '/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/EE6F0889-153D-DF11-963C-0030487CD7EA.root',
    '/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/471/FE37E14D-183D-DF11-8EF5-001D09F24DA8.root'
    ),                        
    lumisToProcess = cms.untracked.VLuminosityBlockRange(
    '132440:140-132440:375','132741:132-132741:361')
)

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/UserCode/edwenger/Skims/python/TrkAnaSkim_data_7TeV_cfg.py,v $'),
    annotation = cms.untracked.string('BPTX_AND + BSC_OR + !BSCHALO')
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))


# =============== Extra Reco Steps =====================
process.load("edwenger.Skims.ExtraVertex_cff")       # agglomerative pixel vertexing
process.load("MitQcd.Production.BeamSpotExtra_cff")  # to introduce custom beamspot source



# =============== Final Filter Path =====================
process.load("edwenger.Skims.eventSelection_cff")
process.load("edwenger.Skims.hfCoincFilter_cff")
process.trkAnaSkim_step = cms.Path(process.physDeclFilter *
                                   process.minBiasBscFilter *
                                   process.hfCoincFilter *
                                   process.purityFractionFilter *
                                   process.offlineBeamSpot *
                                   process.extraVertex)


# =============== Output ================================
process.load("edwenger.Skims.analysisSkimContent_cff")
process.output = cms.OutputModule("PoolOutputModule",
    analysisSkimContent,
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('trkAnaSkim_step')),
    dataset = cms.untracked.PSet(
      dataTier = cms.untracked.string('AOD'),
      filterName = cms.untracked.string('TrkAnaFilter')),
    fileName = cms.untracked.string('trkAnaSkimAOD.root')
    )

process.outpath = cms.EndPath(process.output)

