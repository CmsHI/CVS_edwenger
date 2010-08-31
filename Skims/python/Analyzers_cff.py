import FWCore.ParameterSet.Config as cms

# clone event selection analyzer to run after successive steps
from edwenger.EvtSelAnalyzer.evtselanalyzer_cfi import *
preTrgAna = evtselanalyzer.clone()
postTrgAna = evtselanalyzer.clone()
postEvtSelAna = evtselanalyzer.clone()
postVtxAna = evtselanalyzer.clone(includeSelTrk=cms.untracked.bool(True),
                                  vtxlabel=cms.untracked.InputTag("selectedVertex"))
postTrkVtxAna = evtselanalyzer.clone(includeSelTrk=cms.untracked.bool(True),
                                     vtxlabel=cms.untracked.InputTag("sortedGoodVertices"))

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
trackAna.vsrc = cms.untracked.InputTag("sortedGoodVertices")

preTrackAna = trackAna.clone(isGEN=cms.untracked.bool(True),
                             pureGENmode=cms.untracked.bool(True),
                             #pthatCut = cms.untracked.double(15.0), #needed for 0-15 dijet sample
                             nsdOnly=cms.untracked.bool(True))

refitTrackAna = trackAna.clone(src=cms.untracked.InputTag("refitTracks"))

trackAna_STD = trackAna.clone(src = cms.untracked.InputTag("selectTracks"),
                              applyEvtEffCorr=cms.untracked.bool(True),
                              #evtEffCorrType=cms.untracked.int32(0),  # STD
                              evtEffCorrType=cms.untracked.int32(2),   # AGR
                              ptMin=cms.untracked.double(0.5), # AGR
                              evtMultCut=cms.untracked.int32(3),
                              #evtSelEffv = cms.untracked.vdouble(30,0,9.0384e-05,0.00223004,0.0304791,0.217088,0.460085,0.673905,0.830166,0.92802,0.978678,1.00044,
                              #                                   1.00497,1.00382,1.00041,0.994874,0.99174,0.990136,0.989347,0.989595,0.989071,0.990078,0.990065,0.990801,
                              #                                   0.991051,0.991619,0.99187,0.992327,0.992703,0.99281,0.992914)) # NSD with STD
                              evtSelEffv = cms.untracked.vdouble(30,0.30361,0.765493,0.915144,0.968228,0.985074,0.990709,0.992796,0.993328,0.993417,0.993582,0.993913,0.993383,0.99379,0.993978,0.993733,0.993953,0.99387,0.993817,0.993735,0.993942,0.995149,0.994863,0.993719,0.993205,0.992893,0.993857,0.993767,0.99413,0.997728,0.995064)) # Inel with AGR Single HF EvtSel

looseTrackAna = trackAna.clone(src=cms.untracked.InputTag("looseSelectTracks"),
                               vsrc=cms.untracked.InputTag("selectedVertex"))

looseTrackAna_STD = looseTrackAna.clone(applyEvtEffCorr=cms.untracked.bool(True),
                                        #evtEffCorrType=cms.untracked.int32(0), # STD
                                        evtEffCorrType=cms.untracked.int32(2), # AGR
                                        ptMin=cms.untracked.double(0.5), # AGR
                                        evtMultCut=cms.untracked.int32(0),
                                        #evtMultCut=cms.untracked.int32(3),
                                        #evtSelEffv = cms.untracked.vdouble(30,0.0266389,0.352064,0.55773,0.709436,0.821248,0.89875,0.956152,0.995745,1.01817,1.02657,1.02549,1.01831,
                                        #                                   1.01144,1.00505,0.998037,0.994116,0.992235,0.991112,0.991103,0.990337,0.991335,0.991312,0.991832,
                                        #                                  0.992135,0.992577,0.992819,0.993234,0.993617,0.993562,0.993773))  # NSD with STD
                                        evtSelEffv = cms.untracked.vdouble(30,0.566018,0.920571,0.963983,0.981874,0.989162,0.992304,0.993871,0.994226,0.994387,0.99452,0.994759,0.994392,0.994667,0.995005,0.99485,0.994987,0.994639,0.994662,0.994711,0.994989,0.995553,0.995464,0.994314,0.993933,0.994078,0.994233,0.99439,0.994913,0.997981,0.995393)) # Inel with AGR Single HF EvtSel

                                        
# tracking efficiency analyzer
from edwenger.TrkEffAnalyzer.trkEffAnalyzer_cff import *
trkEffAnalyzer.tracks = cms.untracked.InputTag("selectTracks")
trkEffAnalyzer.vertices = cms.untracked.InputTag("sortedGoodVertices")
trkEffAnalyzer.fillNtuples = cms.bool(False)
trkEffAnalyzer.constPtBins = cms.bool(False)

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
