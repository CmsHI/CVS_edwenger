import FWCore.ParameterSet.Config as cms

from edwenger.EvtSelAnalyzer.evtselanalyzer_cfi import evtselanalyzer

preTriggerAna = evtselanalyzer.clone()
postTriggerAna = evtselanalyzer.clone()
postEvtSelAna = evtselanalyzer.clone()
