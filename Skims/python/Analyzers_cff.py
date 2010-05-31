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
                                   evtSelEffv = cms.untracked.vdouble(8,0.0334129,0.357513,0.581081,0.759259,0.8,0.873984,0.921875,0.932584), #from 50K
                                   #evtSelEffv = cms.untracked.vdouble(8,0.0315315,0.324786,0.53719,0.765217,0.77305,0.921875,0.957447,0.891026),# from 5K 
                                   efit_para = cms.untracked.vdouble(-2.36755,1.40115,-0.394101,0.900227,0.99578,0,0),
                                   evtSelEffCut = cms.untracked.double(0.04))

trackAna_STD = trackAna.clone(src = cms.untracked.InputTag("selectTracks"),
                              applyEvtEffCorr=cms.untracked.bool(True),
                              evtEffCorrType=cms.untracked.int32(0),
                              efit_type=cms.untracked.int32(1),
                              evtSelEffv = cms.untracked.vdouble(12,0,0,0,0.037037,0.222642,0.439024,0.644531,0.76779,0.955102,0.962025,0.987952,0.981273), #from 50K
                              #evtSelEffv = cms.untracked.vdouble(12,0,0,0,0.0347826,0.255319,0.460938,0.673759,0.717949,0.9,0.898438,0.978723,0.985185),# from 5K
                              efit_para = cms.untracked.vdouble(0.105333,-0.894184,-134.324,-3.99298,0,0,0),
                              evtSelEffCut = cms.untracked.double(0.01))


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
