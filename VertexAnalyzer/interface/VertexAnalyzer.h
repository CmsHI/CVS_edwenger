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

// for jet,track,vertices correlation 
#include "RecoJets/JetAlgorithms/interface/JetAlgoHelper.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"



// ROOT includes
#include <TH2.h>

class VertexAnalyzer : public edm::EDAnalyzer {
   public:
      explicit VertexAnalyzer(const edm::ParameterSet&);
      ~VertexAnalyzer(){};

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      typedef math::XYZPoint Point;

      // ----------member data ---------------------------

      edm::InputTag vtxlabel_;
      edm::InputTag trklabel_;
      edm::InputTag jetlabel_;

      int nTracksBins_;
      bool jetTrkVerticesCorr_;

      edm::Service<TFileService> f;
      TH1D *hVtxSize;
      TH1D *hVtxTracks;
      TH1D *hVtxZ;
      TH2D *hMultVtxTracks;
      TH2D *hMultVtxZ;
      TH1D *hAllVtxZ;
      
      TH1F *hLeadingTrkPVdZ;
      TH1F *hSLeadingTrkPVdZ;
      TH1F *hSSLeadingTrkPVdZ;
      TH1F *hLeadingTrkSVdZ;
      TH1F *hSLeadingTrkSVdZ;
      TH1F *hSSLeadingTrkSVdZ;

      TH1F *hTrkPtFromPV;
      TH1F *hTrkPtFromSV;

      TH2F *hVtxZTrkEta;

};
