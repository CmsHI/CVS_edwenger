import FWCore.ParameterSet.Config as cms

from edwenger.Skims.patAna_cff import *   # PAT jet sequence
from edwenger.Skims.eventSelection_cff import *
from edwenger.Skims.Analyzers_cff import *

analysisSeq = cms.Sequence(patAnaSequence *
                           selectedVertex *
                           looseTrackAna *
                           primaryVertexFilter *
                           trackAna *
                           trkEffAnalyzer)

analysisMcSeq = cms.Sequence(patAnaSequence *
                           selectedVertex *
                           looseTrackAna_mc *
                           primaryVertexFilter *
                           trackAna_mc *
                           trkEffAnalyzer)
