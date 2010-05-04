//
// Original Author:  Edward Wenger
//         Created:  Thu Apr 29 14:31:47 CEST 2010
// $Id: TrkEffAnalyzer.cc,v 1.4 2010/05/04 08:36:24 edwenger Exp $
//

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "edwenger/TrkEffAnalyzer/interface/TrkEffHistograms.h"

// define track efficiency analyzer class
class TrkEffAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TrkEffAnalyzer(const edm::ParameterSet&);
      ~TrkEffAnalyzer(){};


   private:
      virtual void beginJob();
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob();
      
      SimTrack_t setSimTrack(TrackingParticle&, const reco::Track&, size_t);
      RecTrack_t setRecTrack(reco::Track&, const TrackingParticle&, size_t);

      // ----------member data ---------------------------
      edm::InputTag trackTags_; 
      edm::InputTag label_tp_effic_;
      edm::InputTag label_tp_fake_;
      edm::InputTag associatorMap_;
      edm::InputTag vtxTags_;

      TrkEffHistograms *histograms;
      edm::Service<TFileService> f;
};

