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

// ROOT includes
#include <TH2.h>

class HiVertexAnalyzer : public edm::EDAnalyzer {
   public:
      explicit HiVertexAnalyzer(const edm::ParameterSet&);
      ~HiVertexAnalyzer(){};

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

      edm::InputTag vtxlabel_;
      int nTracksBins_;

      edm::Service<TFileService> f;
      TH1D *hVtxSize;
      TH1D *hVtxTracks;
      TH1D *hVtxZ;
      TH2D *hMultVtxTracks;
      TH2D *hMultVtxZ;
      TH1D *hAllVtxZ;

};
