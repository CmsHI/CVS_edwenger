import FWCore.ParameterSet.Config as cms

# this is to enable or disable features as necessary for running over MC
def enableSIM(process):
    process.preTrgAna.isGEN=True
    process.postTrgAna.isGEN=True
    process.postEvtSelAna.isGEN=True
    process.postVtxAna.isGEN=True
    process.postTrkVtxAna.isGEN=True
    process.analysisSeq.replace(process.trackAna,process.trackAna_mc)
    process.analysisSeq.replace(process.looseTrackAna,process.looseTrackAna_mc)
    process.rootpleProducer.OnlyRECO=False
    process.eventFilter.remove(process.physDeclFilter) # always false in MC
    process.eventFilter.remove(process.bptxAnd)        # always false in MC
    process.trkEffAnalyzer.hasSimInfo=True
    return process


# this is for Summer 09 samples where the HLT has been re-run during Spring 10 production
def enableREDIGI(process):
    process.preTrgAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.postTrgAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.postEvtSelAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.postVtxAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.postTrkVtxAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.trackAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.looseTrackAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    return process


# this is for running of AOD skims that already have extra reco/pat objects
def enableAOD(process):
    process.extraReco.remove(process.extraVertex)
    process.analysisSeq.remove(process.patAnaSequence)
    return process
