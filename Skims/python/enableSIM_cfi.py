def enableSIM(process):
    process.preTrgAna.isGEN=True
    process.postTrgAna.isGEN=True
    process.postEvtSelAna.isGEN=True
    process.postVtxAna.isGEN=True
    process.postTrkVtxAna.isGEN=True
    process.trackAna.isGEN=True
    process.rootpleProducer.OnlyRECO=False
    
    return process
