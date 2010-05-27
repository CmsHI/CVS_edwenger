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
trackAna_STD = trackAna.clone(applyEvtEffCorr=cms.untracked.bool(True),
                              evtEffCorrType=cms.untracked.int32(0),
                              efit_para = cms.untracked.vdouble(2.31590e+00,4.58625e+00,-5.37943e+01,
                                                                3.80259e-02,3.83653e-02,3.48486e+00,5.72379e-01))
trackAna_SPEC = trackAna.clone(applyEvtEffCorr=cms.untracked.bool(True),
                               evtEffCorrType=cms.untracked.int32(1),
                               efit_para = cms.untracked.vdouble(2.31590e+00,7.58625e+00,-5.37943e+01,
                                                                 3.80259e-02,3.83653e-02,3.48486e+00,6.72379e-01))
trackAna_AGR = trackAna.clone(applyEvtEffCorr=cms.untracked.bool(True),
                              evtEffCorrType=cms.untracked.int32(2),
                              efit_para = cms.untracked.vdouble(1.31590e+00,4.58625e+00,-5.37943e+01,
                                                                3.80259e-02,3.83653e-02,3.48486e+00,3.72379e-01))

preTrackAna = trackAna.clone(pureGENmode=cms.untracked.bool(False))
refitTrackAna = trackAna.clone(src=cms.untracked.InputTag("refitTracks"))
looseTrackAna = trackAna.clone(src=cms.untracked.InputTag("looseSelectTracks"))


# tracking efficiency analyzer
from edwenger.TrkEffAnalyzer.trkEffAnalyzer_cff import *
trkEffAnalyzer.tracks = cms.untracked.InputTag("selectTracks")
trkEffAnalyzer.fillNtuples = cms.bool(False)
trkEffAnalyzer.constPtBins = cms.bool(True)
trackingParticleRecoTrackAsssociation.label_tr = cms.InputTag("selectTracks")

# sequences
preTrgTest = cms.Sequence(preTrgAna)
postTrgTest = cms.Sequence(postTrgAna)
postEvtSelTest = cms.Sequence(postEvtSelAna * preVtxSel)
postVtxTest = cms.Sequence(postVtxAna * postVtxSel)
preTrkVtxTest = cms.Sequence(preTrkVtxSel)
postTrkVtxTest = cms.Sequence(postTrkVtxAna * postTrkVtxSel)
