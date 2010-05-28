import FWCore.ParameterSet.Config as cms

from edwenger.Skims.patAna_cff import *   # PAT jet sequence
from edwenger.Skims.eventSelection_cff import *
from edwenger.Skims.Analyzers_cff import *

analysisSeq = cms.Sequence(patAnaSequence *
                           selectedVertex *
                           looseTrackAna *
                           primaryVertexFilter *
                           (trackAna + trackAna_STD_1pxl + trackAna_STD_4trk + trackAna_SPEC_1pxl + trackAna_SPEC_4trk + refitTrackAna) *
                           trkEffAnalyzer)
