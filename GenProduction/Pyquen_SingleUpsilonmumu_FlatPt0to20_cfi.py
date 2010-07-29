import FWCore.ParameterSet.Config as cms

from Configuration.Generator.PythiaUESettings_cfi import *
hiSignal = cms.EDProducer("Pythia6PtYDistGun",
    PGunParameters = cms.PSet(
        ParticleID = cms.vint32(553),
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
        UpsilonToMuons = cms.vstring('BRAT(1034) = 0 ',  # switch off',
                                     'BRAT(1035) = 1 ',  # switch on',
                                     'BRAT(1036) = 0 ',  # switch off',
                                     'BRAT(1037) = 0 ',  # switch off',
                                     'BRAT(1038) = 0 ',  # switch off',
                                     'BRAT(1039) = 0 ',  # switch off',
                                     'BRAT(1040) = 0 ',  # switch off',
                                     'BRAT(1041) = 0 ',  # switch off',
                                     'BRAT(1042) = 0 ',  # switch off',
                                     'MDME(1034,1) = 0 ',  # switch off',
                                     'MDME(1035,1) = 1 ',  # switch on',
                                     'MDME(1036,1) = 0 ',  # switch off',
                                     'MDME(1037,1) = 0 ',  # switch off',
                                     'MDME(1038,1) = 0 ',  # switch off',
                                     'MDME(1039,1) = 0 ',  # switch off',
                                     'MDME(1040,1) = 0 ',  # switch off',
                                     'MDME(1041,1) = 0 ',  # switch off',
                                     'MDME(1042,1) = 0 ',  # switch off'
                                     ),
        parameterSets = cms.vstring(
            'pythiaUESettings',
            'UpsilonToMuons'
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
    annotation = cms.untracked.string('Pythia Gun Y -> mu mu')
    )

ProductionFilterSequence = cms.Sequence(hiSignal*mumugenfilter)
