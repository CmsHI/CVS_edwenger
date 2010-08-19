import FWCore.ParameterSet.Config as cms

from DQMOffline.Configuration.DQMOffline_Certification_cff import *

DQMOfflineHeavyIons_Certification = cms.Sequence(daq_dqmoffline*dcs_dqmoffline*crt_dqmoffline)

# are the following still needed?
DQMOfflineHeavyIons_Certification.remove(jetMETDQMOfflineClient)
DQMOfflineHeavyIons_Certification.remove(dataCertificationJetMET)
