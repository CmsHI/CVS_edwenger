import FWCore.ParameterSet.Config as cms

# this is to enable or disable features as necessary for running over MC
def enableSIM(process):
    process.preTrgAna.isGEN=True
    process.postTrgAna.isGEN=True
    process.postEvtSelAna.isGEN=True
    process.postVtxAna.isGEN=True
    process.postTrkVtxAna.isGEN=True
    process.trackAna.isGEN=True
    process.looseTrackAna.isGEN=True
    process.rootpleProducer.OnlyRECO=False
    process.eventFilter.remove(process.physDeclFilter) # always false in MC
    process.eventFilter.remove(process.bptxAnd)        # always false in MC
    process.trkEffAnalyzer.hasSimInfo=True
    process.loosetrkEffAnalyzer.hasSimInfo=True
    process.extraReco *= process.trackingParticleRecoTrackAsssociation
    process.preTrgTest *= process.preTrackAna
    process.postVtxAna.vtxWeight=cms.untracked.bool(True)
    process.postTrkVtxAna.vtxWeight=cms.untracked.bool(True)
    return process

def enableSIMnoTP(process):
    process.preTrgAna.isGEN=True
    process.postTrgAna.isGEN=True
    process.postEvtSelAna.isGEN=True
    process.postVtxAna.isGEN=True
    process.postTrkVtxAna.isGEN=True
    process.trackAna.isGEN=True
    process.looseTrackAna.isGEN=True
    process.rootpleProducer.OnlyRECO=False
    process.eventFilter.remove(process.physDeclFilter) # always false in MC
    process.eventFilter.remove(process.bptxAnd)        # always false in MC
    process.trkEffAnalyzer.hasSimInfo=False
    process.loosetrkEffAnalyzer.hasSimInfo=False
    #process.extraReco *= process.trackingParticleRecoTrackAsssociation
    process.preTrgTest *= process.preTrackAna
    process.postVtxAna.vtxWeight=cms.untracked.bool(True)
    process.postTrkVtxAna.vtxWeight=cms.untracked.bool(True)
    return process
                                                                 

def removeTPAssociation(process):
    #process.trkEffAnalyzer.hasSimInfo=False #overwriteen above
    #process.loosetrkEffAnalyzer.hasSimInfo=False #overwritten above
    process.extraReco.remove(process.trackRefit)
    return process

# this is for Summer 09 samples where the HLT has been re-run during Spring 10 production
def enableREDIGI(process):
    process.preTrgAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.postTrgAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.postEvtSelAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.postVtxAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.postTrkVtxAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.preTrackAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.trackAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.looseTrackAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.trackAna_STD.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.looseTrackAna_STD.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.refitTrackAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.eventFilter.remove(process.hltMinBias)     # 
    return process


def enableREDIGI2(process):
    process.preTrgAna.triglabel=cms.untracked.InputTag('TriggerResults','','ANASKIM')
    process.postTrgAna.triglabel=cms.untracked.InputTag('TriggerResults','','ANASKIM')
    process.postEvtSelAna.triglabel=cms.untracked.InputTag('TriggerResults','','ANASKIM')
    process.postVtxAna.triglabel=cms.untracked.InputTag('TriggerResults','','ANASKIM')
    process.postTrkVtxAna.triglabel=cms.untracked.InputTag('TriggerResults','','ANASKIM')
    process.preTrackAna.triglabel=cms.untracked.InputTag('TriggerResults','','ANASKIM')
    process.trackAna.triglabel=cms.untracked.InputTag('TriggerResults','','ANASKIM')
    process.looseTrackAna.triglabel=cms.untracked.InputTag('TriggerResults','','ANASKIM')
    process.trackAna_STD.triglabel=cms.untracked.InputTag('TriggerResults','','ANASKIM')
    process.looseTrackAna_STD.triglabel=cms.untracked.InputTag('TriggerResults','','ANASKIM')
    process.refitTrackAna.triglabel=cms.untracked.InputTag('TriggerResults','','ANASKIM')
    process.eventFilter.remove(process.hltMinBias)     #
    return process

def enableREDIGI3(process):
    process.preTrgAna.triglabel=cms.untracked.InputTag('TriggerResults','','RECO')
    process.postTrgAna.triglabel=cms.untracked.InputTag('TriggerResults','','RECO')
    process.postEvtSelAna.triglabel=cms.untracked.InputTag('TriggerResults','','RECO')
    process.postVtxAna.triglabel=cms.untracked.InputTag('TriggerResults','','RECO')
    process.postTrkVtxAna.triglabel=cms.untracked.InputTag('TriggerResults','','RECO')
    process.preTrackAna.triglabel=cms.untracked.InputTag('TriggerResults','','RECO')
    process.trackAna.triglabel=cms.untracked.InputTag('TriggerResults','','RECO')
    process.looseTrackAna.triglabel=cms.untracked.InputTag('TriggerResults','','RECO')
    process.trackAna_STD.triglabel=cms.untracked.InputTag('TriggerResults','','RECO')
    process.looseTrackAna_STD.triglabel=cms.untracked.InputTag('TriggerResults','','RECO')
    process.refitTrackAna.triglabel=cms.untracked.InputTag('TriggerResults','','RECO')
    process.eventFilter.remove(process.hltMinBias)     #
    return process


# this is for running of AOD skims that already have extra reco/pat objects
def enableAOD(process):
    process.extraReco.remove(process.extraVertex)
    process.analysisSeq.remove(process.patAnaSequence)
    process.extraReco.remove(process.trackRefit)
    #process.analysisSeq.remove(process.refitTrackAna)
    return process


### this is for selecting jet events instead of prescaled MB
def enableHLTJet(process,hltname='HLT_Jet15U'):
    process.minBiasBscFilter.remove(process.bscOrBptxOr)
    process.minBiasBscFilter.remove(process.hltMinBias)
    process.hltJets.HLTPaths = [hltname]
    process.minBiasBscFilter = cms.Sequence(process.hltJets*process.minBiasBscFilter)
    # check
    print "Skim jet HLT path: ", process.hltJets.HLTPaths
    return process

### this is for make a single hit HF event selection for the AGR (1charged trk) evt selection
def enableEitherHFEvtSel(process):
    process.eventFilter.replace(process.hfCoincFilter,process.hfEitherOfTwoFilter)
    return process

### this is for GEN analyzer for Pt_hat: 0 ~ 15
def enableMinPtHatCut(process):
    process.preTrackAna.pthatCut=cms.untracked.double(15.0)
    return process

def enableMinPtHatCutAuto(process,genTypePtHatRange):
    if genTypePtHatRange.find('0_to_5') >= 0:
        print "Min pT_hat cut enable"
        process.preTrackAna.pthatCut=cms.untracked.double(5.0)
    if genTypePtHatRange.find('0_to_10') >= 0:
        print "Min pT_hat cut enable"
        process.preTrackAna.pthatCut=cms.untracked.double(10.0)
    if genTypePtHatRange.find('0_to_15') >= 0:
        print "Min pT_hat cut enable"
        process.preTrackAna.pthatCut=cms.untracked.double(15.0)
    if genTypePtHatRange.find('0_to_20') >= 0:
        print "Min pT_hat cut enable"
        process.preTrackAna.pthatCut=cms.untracked.double(20.0)
    return process

### this is for taking the correct event selection efficiency
def getEvtSelEff(evtSelType):
    eff = {
      # NSD with STD
      'STD_NSD_TrkVtx':[30,0,9.0384e-05,0.00223004,0.0304791,0.217088,0.460085,0.673905,0.830166,0.92802,0.978678,1.00044,1.00497,1.00382,1.00041,0.994874,0.99174,0.990136,0.989347,0.989595,0.989071,0.990078,0.990065,0.990801,0.991051,0.991619,0.99187,0.992327,0.992703,0.99281,0.992914],
      'STD_NSD_PixVtx':[30,0.0266389,0.352064,0.55773,0.709436,0.821248,0.89875,0.956152,0.995745,1.01817,1.02657,1.02549,1.01831,1.01144,1.00505,0.998037,0.994116,0.992235,0.991112,0.991103,0.990337,0.991335,0.991312,0.991832,0.992135,0.992577,0.992819,0.993234,0.993617,0.993562,0.993773],
      # Inel with AGR Single HF EvtSel
      'AGR_Inel_EitherHF_TrkVtx':[30,0.30361,0.765493,0.915144,0.968228,0.985074,0.990709,0.992796,0.993328,0.993417,0.993582,0.993913,0.993383,0.99379,0.993978,0.993733,0.993953,0.99387,0.993817,0.993735,0.993942,0.995149,0.994863,0.993719,0.993205,0.992893,0.993857,0.993767,0.99413,0.997728,0.995064],
      'AGR_Inel_EitherHF_PixVtx':[30,0.566018,0.920571,0.963983,0.981874,0.989162,0.992304,0.993871,0.994226,0.994387,0.99452,0.994759,0.994392,0.994667,0.995005,0.99485,0.994987,0.994639,0.994662,0.994711,0.994989,0.995553,0.995464,0.994314,0.993933,0.994078,0.994233,0.99439,0.994913,0.997981,0.995393],
      # 900GeV NSD with STD
      'STD_NSD_900GeV_TrkVtx': [42,0,9.60646e-05,0.00262191,0.0337782,0.228173,0.463418,0.661808,0.798992,0.88422,0.929808,0.954356,0.964662,0.968794,0.970072,0.970321,0.969551,0.970247,0.971157,0.972393,0.97493,0.974667,0.976915,0.977959,0.979166,0.979742,0.98169,0.982051,0.982586,0.983429,0.984924,0.984952,0.98544,0.98525,0.986517,0.986371,0.986602,0.988167,0.988759,0.989259,0.98922,0.989398,0.9895],
      'STD_NSD_900GeV_PixVtx': [30,0.051323,0.459641,0.654303,0.779912,0.862804,0.918859,0.955648,0.976691,0.987668,0.992159,0.993694,0.991707,0.989005,0.987004,0.985711,0.984101,0.984,0.984631,0.985698,0.987195,0.987634,0.989591,0.990561,0.991783,0.992561,0.993581,0.993937,0.994328,0.994914,0.995531]
	}
    return eff[evtSelType]

def updateEvtSelEff(ana,evtSelType):
  # Set the eff sel mode
  if evtSelType.find('STD') >= 0:
    ana.ptMin = 0.0
    ana.evtEffCorrType = 0
  if evtSelType.find('AGR') >= 0:
    ana.ptMin = 0.5
    ana.evtEffCorrType = 2
  # Set the eff sel eff numbers
  ana.evtSelEffv = getEvtSelEff(evtSelType)
