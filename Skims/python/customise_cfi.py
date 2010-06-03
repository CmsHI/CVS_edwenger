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
    return process


# this is for running of AOD skims that already have extra reco/pat objects
def enableAOD(process):
    process.extraReco.remove(process.extraVertex)
    process.analysisSeq.remove(process.patAnaSequence)
    process.extraReco.remove(process.trackRefit)
    #process.analysisSeq.remove(process.refitTrackAna)
    return process


### this is for selecting jet events instead of prescaled MB
def enableHLTJet15U(process):
    process.eventFilter.remove(process.bscOrBptxOr)
    process.hltJets.HLTPaths = ['HLT_Jet15U']
    process.eventFilter *= process.hltJets
    return process
