import FWCore.ParameterSet.Config as cms

highPurityTracks = cms.EDFilter("TrackSelector",
    src = cms.InputTag("generalTracks"),
    cut = cms.string('quality("highPurity")')
)

selectTracks = cms.EDFilter("TrackWithVertexSelector",
    # the track collection
    src = cms.InputTag('highPurityTracks'),
    # kinematic cuts  (pT in GeV)
    etaMin = cms.double(0.0),
    etaMax = cms.double(5.0),
    ptMin = cms.double(0.0),
    ptMax = cms.double(500.0),
    # impact parameter cut (in cm)
    d0Max = cms.double(10.),
    dzMax = cms.double(35.),
    # quality cuts (valid hits, normalized chi2)
    normalizedChi2 = cms.double(999999.),
    numberOfValidHits = cms.uint32(0),
    numberOfLostHits = cms.uint32(999), ## at most 999 lost hits
    numberOfValidPixelHits = cms.uint32(0), ## at least <n> hits in the pixels
    ptErrorCut = cms.double(999.), ## [pTError/pT]*max(1,normChi2) <= ptErrorCut
    # compatibility with a vertex ?
    useVtx = cms.bool(True),
    vertexTag = cms.InputTag('offlinePrimaryVertices'),
    nVertices = cms.uint32(2), ## how many vertices to look at before dropping the track
    vtxFallback = cms.bool(True), ## falback to beam spot if there are no vertices
    # uses vtx=(0,0,0) with deltaZeta=15.9, deltaRho = 0.2
    zetaVtx = cms.double(0.5),
    rhoVtx = cms.double(0.3), ## tags used by b-tagging folks
    # should _not_ be used for the TrackWithVertexRefSelector
    copyExtras = cms.untracked.bool(False), ## copies also extras and rechits on RECO
    copyTrajectories = cms.untracked.bool(False), # don't set this to true on AOD!
)

allTracks = cms.EDProducer("ConcreteChargedCandidateProducer",
    src = cms.InputTag("selectTracks"),
    particleType = cms.string('pi+')
)

goodTracks = cms.EDFilter("PtMinCandViewSelector",
    src = cms.InputTag("allTracks"),
    ptMin = cms.double(0.0)
)

chargedCandidates = cms.Sequence(highPurityTracks*selectTracks*allTracks*goodTracks)
