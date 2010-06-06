import FWCore.ParameterSet.Config as cms

# clone event selection analyzer to run after successive steps
from edwenger.EvtSelAnalyzer.evtselanalyzer_cfi import *
preTrgAna = evtselanalyzer.clone()
postTrgAna = evtselanalyzer.clone()
postEvtSelAna = evtselanalyzer.clone()
postVtxAna = evtselanalyzer.clone(includeSelTrk=cms.untracked.bool(True))
postTrkVtxAna = evtselanalyzer.clone(includeSelTrk=cms.untracked.bool(True))

# clone vertex analyzer to run with various inputs
from edwenger.VertexAnalyzer.vertexanalyzer_cfi import *
preVtxSel = vertexanalyzer.clone(
    vtxlabel = cms.untracked.InputTag("pixel3Vertices"))
postVtxSel = vertexanalyzer.clone(
    vtxlabel = cms.untracked.InputTag("selectedVertex"))
preTrkVtxSel = vertexanalyzer.clone(
    vtxlabel = cms.untracked.InputTag("offlinePrimaryVertices"))
postTrkVtxSel = vertexanalyzer.clone(
    vtxlabel = cms.untracked.InputTag("sortedGoodVertices"))

# track and jet spectra analyzer
from edwenger.TrackSpectraAnalyzer.trackspectraanalyzer_cfi import *
trackAna.src = cms.untracked.InputTag("selectTracks")
trackAna.jsrc = cms.untracked.InputTag("selectedPatJets")
trackAna.lowPtStudyHist = cms.untracked.bool(True)

preTrackAna = trackAna.clone(isGEN=cms.untracked.bool(True),
                             pureGENmode=cms.untracked.bool(True),
                             #pthatCut = cms.untracked.double(15.0), #needed for 0-15 dijet sample
                             nsdOnly=cms.untracked.bool(True))

refitTrackAna = trackAna.clone(src=cms.untracked.InputTag("refitTracks"))
looseTrackAna = trackAna.clone(src=cms.untracked.InputTag("looseSelectTracks"))

looseTrackAna_STD = trackAna.clone(src=cms.untracked.InputTag("looseSelectTracks"),
                                   applyEvtEffCorr=cms.untracked.bool(True),
                                   evtEffCorrType=cms.untracked.int32(0),
                                   evtMultCut=cms.untracked.int32(0),
                                   evtSelEffv = cms.untracked.vdouble(20,0.0266389,0.352064,0.55773,0.709436,0.821248,0.89875,0.956152,0.995745,1.01817,1.02657,
                                                                      1.02549,1.01831,1.01144,1.00505,0.998037,0.994116,0.992235,0.991112,0.991103,0.990337))

trackAna_STD = trackAna.clone(src = cms.untracked.InputTag("selectTracks"),
                              applyEvtEffCorr=cms.untracked.bool(True),
                              evtEffCorrType=cms.untracked.int32(0),
                              evtMultCut=cms.untracked.int32(3),
                              evtSelEffv = cms.untracked.vdouble(20,0,9.0384e-05,0.00223004,0.0304791,0.217088,0.460085,0.673905,0.830166,0.92802,0.978678,
                                                                 1.00044,1.00497,1.00382,1.00041,0.994874,0.99174,0.990136,0.989347,0.989595,0.989071))


# tracking efficiency analyzer
from edwenger.TrkEffAnalyzer.trkEffAnalyzer_cff import *
trkEffAnalyzer.tracks = cms.untracked.InputTag("selectTracks")
trkEffAnalyzer.fillNtuples = cms.bool(False)
trkEffAnalyzer.constPtBins = cms.bool(True)
trkEffAnalyzer.lowPtMode   = cms.bool(False)
loosetrkEffAnalyzer = trkEffAnalyzer.clone(tracks=cms.untracked.InputTag("looseSelectTracks"),
                                           vertices = cms.untracked.InputTag('selectedVertex',''),
                                           )

trackingParticleRecoTrackAsssociation.label_tr = cms.InputTag("selectTracks")

# sequences
preTrgTest = cms.Sequence(preTrgAna)
postTrgTest = cms.Sequence(postTrgAna)
postEvtSelTest = cms.Sequence(postEvtSelAna * preVtxSel)
postVtxTest = cms.Sequence(postVtxAna * postVtxSel)
preTrkVtxTest = cms.Sequence(preTrkVtxSel)
postTrkVtxTest = cms.Sequence(postTrkVtxAna * postTrkVtxSel)
