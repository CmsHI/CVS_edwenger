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
      bool isGEN_;
      bool includeSelTrk_;
      double etaMaxSpec_;
      double ptMin_;

      edm::Service<TFileService> f;
      TH1D *hL1TechBits;
      TH1D *hL1AlgoBits;
      TH2D *hHPFracNtrk;
      TH2D *hHfTowers;
      TH1D *hHLTPaths;
      
      TH1D *hRecMult;
      TH1D *hRecMult_STD;
      TH1D *hRecMult_SPEC;
      TH1D *hRecMult_AGR;
      
      TH1D *hRecMult_sel;
      TH1D *hRecMult_STD_sel;
      TH1D *hRecMult_SPEC_sel;
      TH1D *hRecMult_AGR_sel;

      TH1D *hGenMultInel;
      TH1D *hGenMultNSD;
      TH1D *hGenMultSD;
      TH1D *hGenMultDD;
      TH1D *hGenMultND;

      TH1D *hGenMultInel_STD;
      TH1D *hGenMultNSD_STD;
      TH1D *hGenMultSD_STD;
      TH1D *hGenMultDD_STD;
      TH1D *hGenMultND_STD;

      TH1D *hGenMultInel_SPEC;
      TH1D *hGenMultNSD_SPEC;
      TH1D *hGenMultSD_SPEC;
      TH1D *hGenMultDD_SPEC;
      TH1D *hGenMultND_SPEC;

      TH1D *hGenMultInel_AGR;
      TH1D *hGenMultNSD_AGR;
      TH1D *hGenMultSD_AGR;
      TH1D *hGenMultDD_AGR;
      TH1D *hGenMultND_AGR;

      TH1D *hGenRecMultNSD;
      TH1D *hGenRecMultNSD_STD;
      TH1D *hGenRecMultNSD_SPEC;
      TH1D *hGenRecMultNSD_AGR;


};
