def enableSIM(process):
    process.preTrgAna.isGEN=True
    process.postTrgAna.isGEN=True
    process.postEvtSelAna.isGEN=True
    process.postVtxAna.isGEN=True
    process.postTrkVtxAna.isGEN=True
    process.trackAna.isGEN=True
    process.rootpleProducer.OnlyRECO=False
    
    return process


def enableREDIGI(process):
    process.preTrgAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.postTrgAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.postEvtSelAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.postVtxAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.postTrkVtxAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.trackAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')
    process.looseTrackAna.triglabel=cms.untracked.InputTag('TriggerResults','','REDIGI')

    return process
