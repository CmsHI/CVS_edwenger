import FWCore.ParameterSet.Config as cms

from Configuration.Generator.PythiaUESettings_cfi import *
hiSignal = cms.EDProducer("Pythia6PtYDistGun",
    PGunParameters = cms.PSet(
        ParticleID = cms.vint32(23),
        kinematicsFile = cms.FileInPath('HeavyIonsAnalysis/Configuration/data/jpsipbpbFlat.root'),
        PtBinning = cms.int32(100000),
        YBinning = cms.int32(500),
        MaxPt = cms.double(20.0),
        MinPt = cms.double(0.0),
        MaxY = cms.double(5.0),
        MinY = cms.double(-5.0),
        MinPhi = cms.double(-3.1415926535897932384),
        MaxPhi = cms.double(3.1415926535897932384)
    ),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        ZToMuons = cms.vstring("MDME(174,1)=0",          # !Z decay into d dbar,
                               "MDME(175,1)=0",          # !Z decay into u ubar,
                               "MDME(176,1)=0",          # !Z decay into s sbar,
                               "MDME(177,1)=0",          # !Z decay into c cbar,
                               "MDME(178,1)=0",          # !Z decay into b bbar,
                               "MDME(179,1)=0",          # !Z decay into t tbar,
                               "MDME(182,1)=0",          # !Z decay into e- e+,
                               "MDME(183,1)=0",          # !Z decay into nu_e nu_ebar,
                               "MDME(184,1)=1",          # !Z decay into mu- mu+,
                               "MDME(185,1)=0",          # !Z decay into nu_mu nu_mubar,
                               "MDME(186,1)=0",          # !Z decay into tau- tau+,
                               "MDME(187,1)=0"           # !Z decay into nu_tau nu_taubar
                               ),
        parameterSets = cms.vstring(
            'pythiaUESettings',
            'ZToMuons'
        )
    )
)

mumugenfilter = cms.EDFilter("MCParticlePairFilter",
    Status = cms.untracked.vint32(1, 1),
    MinPt = cms.untracked.vdouble(2.5, 2.5),
    MaxEta = cms.untracked.vdouble(2.5, 2.5),
    MinEta = cms.untracked.vdouble(-2.5, -2.5),
    ParticleCharge = cms.untracked.int32(-1),
    ParticleID1 = cms.untracked.vint32(13),
    ParticleID2 = cms.untracked.vint32(13)
)

configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/UserCode/edwenger/GenProduction/Pyquen_Zmumu_cfi.py,v $'),
    annotation = cms.untracked.string('Pythia Gun Z -> mu mu')
    )

ProductionFilterSequence = cms.Sequence(hiSignal*mumugenfilter)
