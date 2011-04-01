// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "RecoJets/JetAlgorithms/interface/JetAlgoHelper.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h" // needed for TrackerDigiGeometryRecord

#include "DataFormats/Math/interface/deltaR.h"



// ROOT includes
#include "TNtuple.h"
#include "TH1.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"

// define track and jet spectra analyzer
class HiTrackSpectraAnalyzer : public edm::EDAnalyzer {
   public:
      explicit HiTrackSpectraAnalyzer(const edm::ParameterSet&);
      ~HiTrackSpectraAnalyzer(){};


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob(){} ;

      bool hitDeadPXF(const reco::Track&);

   // ----------member data ---------------------------

   TNtuple *nt_dndptdeta; 
   TNtuple *nt_gen_dndptdeta;
   TNtuple *nt_jet;
   TNtuple *nt_jettrack;

   TH1F *hNevt;

   TH1F *hPxlMultDist;

   TH1F *hRecMult_STD;
   TH1F *hRecMult_STD_corr;

   TH1F *hGenNevt;

   TH2F *hTrkPtEta;
   TH2F *hGenTrkPtEta;

   TH2F *hCentJetEt;
   
   TH3F *hTrkPtEtaJetEt;
   TH3F *hTrkPtEtaJetEtW;
   
   TH3F *hTrkPtEtaJetEt_vbin;
   TH3F *hTrkPtEtaJetEtW_vbin;

   TH3F *hTrkPtEtaJetEtW_mult1;
   TH3F *hTrkPtEtaJetEtW_mult2;
   TH3F *hTrkPtEtaJetEtW_mult3;
   
   TH3F *hGenTrkPtEtaJetEt;
   TH3F *hGenTrkPtEtaJetEtW;

   TH3F *hGenTrkPtEtaJetEt_vbin;
   TH3F *hGenTrkPtEtaJetEtW_vbin;

   std::vector<TH1F*> hNevt_Cent;
   std::vector<TH1F*> hJet0Pt_Cent;
   std::vector<TH3F*> hTrkPtEtaJetEt_Trig;
   std::vector<TH3F*> hTrkPtEtaJetEt_Cent;
   std::vector<TH3F*> hGenTrkPtEtaJetEt_Cent;


   //evt sel eff. correction
   TF1 *evtSelEff;

   // Jets
   TH1F *hNumJets;
   TH1F *hJet0Pt, *hJet0Eta;
   TH1F *hJet0Pt_jetCut, *hJet0Eta_jetCut;
   TH1F *hJet0Pt_occHand, *hJet0Eta_occHand;
   TH1F *hClosestJetdR_lead;
   TH1F *hClosestJetdR_slead;
   TH1F *hClosestJetInd;
   TH1F *hClosestJetEta_lead;
   TH1F *hClosestJetEta_slead;
   TH1F *hdNdPt_none_jet;
   TH1F *hdNdPt_lead_jet;
   TH1F *hdNdPt_slead_jet;
   TH1F *hdNdPt_none_jet_eta; //eta cut
   TH1F *hdNdPt_lead_jet_eta;
   TH1F *hdNdPt_slead_jet_eta;
   TH1F *hdNdEt_leadjet;
   TH1F *hdNdEt_sleadjet;
   TH1F *hNtrkNoneJet;
   TH1F *hNtrkLeadJet;
   TH1F *hNtrkSLeadJet;

   TH2F *hClosestJetdRdPt_lead;
   TH2F *hClosestJetdRdPt_slead;
   TH2F *hClosestJetdRdPt_lead_eta; // eta cut
   TH2F *hClosestJetdRdPt_slead_eta;

   std::vector<TH1F*> hJet0Pt_Trig;

   edm::Service<TFileService> fs;
   
   bool isGEN_, pureGENmode_;
   bool histOnly_, includeExtra_;
   bool applyEvtEffCorr_;
   bool setQualityBit_;
   bool triggerNeeded_;
   bool pixelMultMode_;
   bool closestJets_;
   bool trkAcceptedJet_;
   bool useSubLeadingJet_;
   bool fiducialCut_;

   int32_t evtEffCorrType_, evtMultCut_;

   double  etaMax_, ptMin_;
   double  pixelMult_;
   double  leadJetEt_, leadJetEta_, leadJetOccEt_, leadJetOccEta_;
   double  leadGJetEt_, leadGJetEta_;
   double  occHandle_, occGENHandle_;

   std::vector<bool> hltAccept_;
   std::vector<double> evtSelEffv_;

   edm::InputTag src_;
   edm::InputTag vsrc_;
   edm::InputTag jsrc_;
   edm::InputTag gsrc_;
   edm::InputTag gjsrc_;
   edm::InputTag src_evtCorr_;
   std::vector<std::string> hltNames_;
   edm::InputTag triglabel_;
   
   std::vector<int32_t> neededTrigSpectra_;
   std::vector<int32_t> neededCentBins_;
   std::vector<double> jetEtCuts_;
   std::vector<double> etaBins, ptBins, jetBins, centBins;

   CentralityProvider * centrality_;

   const TrackerGeometry * theTracker;
};
