// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

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

#include "DataFormats/PatCandidates/interface/Jet.h"

// ROOT includes
#include "TNtuple.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"

// define track and jet spectra analyzer
class TrackSpectraAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TrackSpectraAnalyzer(const edm::ParameterSet&);
      ~TrackSpectraAnalyzer(){};


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob(){} ;

   // ----------member data ---------------------------
   
   TNtuple *nt_dndptdeta; 
   TNtuple *nt_gen_dndptdeta;
   TNtuple *nt_jet;
   TNtuple *nt_jettrack;

   TH1F *hNevt;
   //TH1F *hGenNevt;

   TH1F *hTrkPtMB;

   TH2F *hTrkPtEta;
   TH2F *hGenTrkPtEta;
   
   TH3F *hTrkPtEtaJetEt;
   TH3F *hTrkPtEtaJetEtW;

   TH3F *hGenTrkPtEtaJetEt;
   TH3F *hGenTrkPtEtaJetEtW;


   // Jets
   TH1F *hNumJets;
   TH1F *hJet0Pt;
   TH1F *hJet0Pt_HltMB;
   TH1F *hJet0Pt_HltJet6U;
   TH1F *hJet0Pt_HltJet15U;
   TH1F *hJet0Pt_HltJet30U;
   TH1F *hJet0Pt_HltJet50U;

   edm::Service<TFileService> fs;
   
   bool doOutput_;
   bool isGEN_, doJet_;
   bool histOnly_;
   double  etaMax_;
   edm::InputTag src_;
   edm::InputTag vsrc_;
   edm::InputTag jsrc_;
   std::vector<std::string> hltNames_;
   edm::InputTag triglabel_;

};
