// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <TH2.h>

//
// class declaration
//

class EvtSelAnalyzer : public edm::EDAnalyzer {
   public:
      explicit EvtSelAnalyzer(const edm::ParameterSet&);
      ~EvtSelAnalyzer(){};


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob();

      // ----------member data ---------------------------

      edm::InputTag gtlabel_;
      edm::InputTag towerslabel_;
      double hfEThreshold_;
      edm::InputTag trackslabel_;
      std::string qualityString_;
      edm::InputTag triglabel_;
      std::vector<std::string> trignames_;

      edm::Service<TFileService> f;
      TH1D *hL1TechBits;
      TH1D *hL1AlgoBits;
      TH2D *hHPFracNtrk;
      TH2D *hHfTowers;
      TH1D *hHLTPaths;

      // TO DO: add histogram of event multiplicity and isGEN flag
};
