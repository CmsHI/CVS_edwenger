import FWCore.ParameterSet.Config as cms

from DQMServices.Components.DQMDaqInfo_cfi import *

from DQMOffline.Configuration.DQMOffline_DAQ_cff import *
from DQMOffline.Configuration.DQMOffline_DCS_cff import *
from DQMOffline.Configuration.DQMOffline_CRT_cff import *

DQMOffline_Certification = cms.Sequence(daq_dqmoffline*dcs_dqmoffline*crt_dqmoffline)

DQMOffline_Certification_HI = cms.Sequence(DQMOffline_Certification)
# have the following been fixed yet?
DQMOffline_Certification_HI.remove(jetMETDQMOfflineClient)
DQMOffline_Certification_HI.remove(dataCertificationJetMET)
