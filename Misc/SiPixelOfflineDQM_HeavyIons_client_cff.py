import FWCore.ParameterSet.Config as cms

# import p+p sequence
from DQM.SiPixelCommon.SiPixelOfflineDQM_client_cff import *

# clone and modify modules 
sipixelQTesterHI = sipixelQTester.clone(
    qtList = cms.untracked.FileInPath('DQM/SiPixelMonitorClient/test/sipixel_tier0_qualitytest_heavyions.xml')
    )

# define new HI sequence
PixelOfflineDQMClientWithDataCertificationHI = cms.Sequence(sipixelQTesterHI+
                                                            sipixelEDAClient+
                                                            sipixelDaqInfo+
                                                            sipixelDcsInfo+
                                                            sipixelCertification)
