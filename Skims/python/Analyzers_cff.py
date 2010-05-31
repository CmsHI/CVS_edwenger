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

preTrackAna = trackAna.clone(isGEN=cms.untracked.bool(True),
                             pureGENmode=cms.untracked.bool(True),
                             nsdOnly=cms.untracked.bool(True))

refitTrackAna = trackAna.clone(src=cms.untracked.InputTag("refitTracks"))
looseTrackAna = trackAna.clone(src=cms.untracked.InputTag("looseSelectTracks"))

looseTrackAna_STD = trackAna.clone(src=cms.untracked.InputTag("looseSelectTracks"),
                                   applyEvtEffCorr=cms.untracked.bool(True),
                                   evtEffCorrType=cms.untracked.int32(0),
                                   efit_type=cms.untracked.int32(0),
                                   evtSelEffv = cms.untracked.vdouble(16,0.0193237,0.219512,0.336957,0.524272,0.486726,0.62963,0.671756,
                                                                      0.767241,0.80315,0.84127,0.843137,0.912281,0.954128,0.963303,0.925926,0.975207),
                                   #evtSelEffv = cms.untracked.vdouble(16,0.0232558,0.213483,0.375723,0.470588,0.547739,0.641304,
                                   #                                  0.718085,0.731579,0.786585,0.835616,0.88125,0.951389,0.94,0.948148,0.97931,0.969925),
                                   efit_para = cms.untracked.vdouble(-2.36755,1.40115,-0.394101,0.900227,0.99578,0,0))

trackAna_STD = trackAna.clone(src = cms.untracked.InputTag("selectTracks"),
                              applyEvtEffCorr=cms.untracked.bool(True),
                              evtEffCorrType=cms.untracked.int32(0),
                              efit_type=cms.untracked.int32(1),
                              evtSelEffv = cms.untracked.vdouble(16,0.0193237,0.219512,0.336957,0.524272,0.486726,0.62963,0.671756,
                                                                 0.767241,0.80315,0.84127,0.843137,0.912281,0.954128,0.963303,0.925926,0.975207),
                              #evtSelEffv = cms.untracked.vdouble(16,0.0232558,0.213483,0.375723,0.470588,0.547739,0.641304,
                              #0.718085,0.731579,0.786585,0.835616,0.88125,0.951389,0.94,0.948148,0.97931,0.969925),
                              efit_para = cms.untracked.vdouble(0.105333,-0.894184,-134.324,-3.99298,0,0,0))


# tracking efficiency analyzer
from edwenger.TrkEffAnalyzer.trkEffAnalyzer_cff import *
trkEffAnalyzer.tracks = cms.untracked.InputTag("selectTracks")
trkEffAnalyzer.fillNtuples = cms.bool(False)
trkEffAnalyzer.constPtBins = cms.bool(True)
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
