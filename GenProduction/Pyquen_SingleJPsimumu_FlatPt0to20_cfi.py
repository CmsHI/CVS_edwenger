import FWCore.ParameterSet.Config as cms

from Configuration.Generator.PythiaUESettings_cfi import *
hiSignal = cms.EDProducer("Pythia6PtYDistGun",
    PGunParameters = cms.PSet(
        ParticleID = cms.vint32(443),
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
        JPsiToMuons = cms.vstring('BRAT(858) = 0 ',  # switch off',
                                  'BRAT(859) = 1 ',  # switch on',
                                  'BRAT(860) = 0 ',  # switch off',
                                  'MDME(858,1) = 0 ',  # switch off',
                                  'MDME(859,1) = 1 ',  # switch on',
                                  'MDME(860,1) = 0 ',  # switch off'
                                  ),
        parameterSets = cms.vstring(
            'pythiaUESettings',
            'JPsiToMuons'
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
    annotation = cms.untracked.string('Pythia Gun J/Psi -> mu mu')
    )

ProductionFilterSequence = cms.Sequence(hiSignal*mumugenfilter)
