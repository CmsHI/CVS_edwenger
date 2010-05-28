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

trackAna_STD_1pxl = trackAna.clone(applyEvtEffCorr=cms.untracked.bool(True),
                              evtEffCorrType=cms.untracked.int32(0),
                              efit_para = cms.untracked.vdouble(-8.06924e-02,-7.17044e-01,-2.74749e-01,4.39522e-01,4.43058e-01,1.97233e-01,1.16675e+00))
trackAna_STD_4trk = trackAna.clone(applyEvtEffCorr=cms.untracked.bool(True),
                              evtEffCorrType=cms.untracked.int32(0),
                              efit_para = cms.untracked.vdouble(-1.81752e+00,1.02653e+00,-2.64738e-02,4.23789e+00,4.27101e+00,8.43603e-03,2.05349e+00))
trackAna_SPEC_1pxl = trackAna.clone(applyEvtEffCorr=cms.untracked.bool(True),
                               evtEffCorrType=cms.untracked.int32(1),
                               efit_para = cms.untracked.vdouble(-7.54462e-01,1.58465e+00,-4.34577e+01,1.07743e+00,1.08147e+00,4.27893e-01,1.05561e+00))
trackAna_SPEC_4trk = trackAna.clone(applyEvtEffCorr=cms.untracked.bool(True),
                               evtEffCorrType=cms.untracked.int32(1),
                               efit_para = cms.untracked.vdouble(-5.21944e-01,5.67420e-01,-9.96122e-01,6.65958e-01,6.71037e-01,1.64208e-01,1.59302e+00))

preTrackAna = trackAna.clone(pureGENmode=cms.untracked.bool(True))
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
