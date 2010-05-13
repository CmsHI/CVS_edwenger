#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "edwenger/TrkEffAnalyzer/interface/HackedAnalyticalTrackSelector.h"


using reco::modules::HackedAnalyticalTrackSelector;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(HackedAnalyticalTrackSelector);
