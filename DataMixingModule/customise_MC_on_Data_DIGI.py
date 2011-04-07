def customise_MC_on_Data_DIGI(process):

    # because 'ConfigBuilder.py' doesn't use 'loadDefaultOrSpecifiedCFF' in 'prepare_DIGI'.
    # otherwise we would just specify it in the cmsDriver step like 'GEN,SIM,DIGI:DigiDM_cff'
    print "Loading 'DigiDM_cff', so as not to add detector noise twice..."
    process.load("Configuration.StandardSequences.DigiDM_cff")

    # keep only the reco::vertex from the background event
    # and the GEN,SIM,DIGI content produced in the current process
    for outmod in process.outputModules_():
        print "Setting %s content to include only objects produced in current process..." % outmod
        getattr(process,outmod).outputCommands = cms.untracked.vstring('drop *','keep *_*_*_'+process.name_())
        print "Extending %s content to include 'hiSelectedVertex' from background event..." % outmod
        getattr(process,outmod).outputCommands.extend(['keep *_hiSelectedVertex_*_*'])

    # if we are using one of the himix configurations, the label needs to be 'generator' for datamixing
    if 'hiSignal' in process.filters_():
        print "Replacing 'hiSignal' label of embedded MC signal with 'generator'..."
        process.generator = process.hiSignal.clone(embeddingMode=cms.bool(False))
        process.ProductionFilterSequence.replace(process.hiSignal,process.generator)

    # match the generated signal to the background reconstructed vertex position
    print "Matching the generated signal to the reco::vertex position of the background event.."
    process.load("SimGeneral.MixingModule.MatchRecVtx_cfi")
    process.matchRecVtx.heavyIonLabel = "hiSelectedVertex"
    process.pgen.replace(process.VertexSmearing,process.matchRecVtx)

    return process
