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
                              ptMin=cms.cms.untracked.double(0.5), # AGR
                              evtMultCut=cms.untracked.int32(3),
                              #evtSelEffv = cms.untracked.vdouble(30,0,9.0384e-05,0.00223004,0.0304791,0.217088,0.460085,0.673905,0.830166,0.92802,0.978678,1.00044,
                              #                                   1.00497,1.00382,1.00041,0.994874,0.99174,0.990136,0.989347,0.989595,0.989071,0.990078,0.990065,0.990801,
                              #                                   0.991051,0.991619,0.99187,0.992327,0.992703,0.99281,0.992914)) # NSD with STD
                              evtSelEffv = cms.untracked.vdouble(30,0.26409,0.70228,0.874059,0.947856,0.975991,0.986444,0.990663,0.99215,0.992782,0.99322,0.993505,0.993298,
                                                                 0.993563,0.99395,0.993616,0.993901,0.993922,0.993727,0.993611,0.99405,0.995097,0.994775,0.993825,0.993407,0.992965,
                                                                 0.993638,0.993563,0.994132,0.996707,0.994432)) # Inel with AGR

looseTrackAna = trackAna.clone(src=cms.untracked.InputTag("looseSelectTracks"),
                               vsrc=cms.untracked.InputTag("selectedVertex"))

looseTrackAna_STD = looseTrackAna.clone(applyEvtEffCorr=cms.untracked.bool(True),
                                        #evtEffCorrType=cms.untracked.int32(0), # STD
                                        evtEffCorrType=cms.untracked.int32(2), # AGR
                                        ptMin=cms.cms.untracked.double(0.5), # AGR
                                        evtMultCut=cms.untracked.int32(0),
                                        #evtMultCut=cms.untracked.int32(3),
                                        #evtSelEffv = cms.untracked.vdouble(30,0.0266389,0.352064,0.55773,0.709436,0.821248,0.89875,0.956152,0.995745,1.01817,1.02657,1.02549,1.01831,
                                        #                                   1.01144,1.00505,0.998037,0.994116,0.992235,0.991112,0.991103,0.990337,0.991335,0.991312,0.991832,
                                        #                                  0.992135,0.992577,0.992819,0.993234,0.993617,0.993562,0.993773))  # NSD with STD
                                        evtSelEffv = cms.untracked.vdouble(30,0.420342,0.811176,0.910886,0.958826,0.979494,0.987923,0.991724,0.993038,0.993752,0.994141,0.994386,0.994304,
                                                                           0.994509,0.994915,0.994774,0.994909,0.994645,0.994573,0.994587,0.995036,0.995477,0.995492,0.994386,0.994089,
                                                                           0.994168,0.993992,0.994148,0.994865,0.996942,0.994742)) # Inel with AGR

                                        
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
