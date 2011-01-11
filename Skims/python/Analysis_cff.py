import FWCore.ParameterSet.Config as cms

from edwenger.Skims.patAna_cff import *   # PAT jet sequence
from edwenger.Skims.eventSelection_cff import *
from edwenger.Skims.Analyzers_cff import *

analysisSeq = cms.Sequence(patAnaSequence *
			   cleanedPatJets *
                           selectedVertex *
                           postExtraTrkVtxSel * # trk,vtx,jet 
                           (looseTrackAna + looseTrackAna_STD) *
                           loosetrkEffAnalyzer *
                           primaryVertexFilter *
			   djcalo *
                           (trackAna + trackAna_STD + refitTrackAna) *
                           trkEffAnalyzer)

analysisGEN = cms.Sequence(preTrackAna)
