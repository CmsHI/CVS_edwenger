#ifndef HiTrkEffAnalyzer_HiPFCandidateTrackAnalyzer
#define HiTrkEffAnalyzer_HiPFCandidateTrackAnalyzer

// system include files
#include <memory>
#include <string>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

//EAW
#include "TFile.h"
#include "TNtuple.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
//

/**\class PFCandidateTrackAnalyzer 
\brief produces IsolatedPFCandidates from PFCandidates

\author Colin Bernet
\date   february 2008
*/




class HiPFCandidateTrackAnalyzer : public edm::EDAnalyzer {
 public:

  explicit HiPFCandidateTrackAnalyzer(const edm::ParameterSet&);

  ~HiPFCandidateTrackAnalyzer();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

  virtual void beginRun(const edm::Run & r, const edm::EventSetup & c);

  virtual void beginJob();

 private:
  

  void printElementsInBlocks(const reco::PFCandidate& cand,
			     std::ostream& out=std::cout) const;


  
  /// PFCandidates in which we'll look for pile up particles 
  edm::InputTag   inputTagPFCandidates_;

  /// Vertices for track selection
  edm::InputTag   inputTagVertices_;

  /// SimTracks for track association
  edm::InputTag   inputTagSimTracks_;

  /// Tracks for track association
  edm::InputTag   inputTagTracks_;
  
  /// verbose ?
  bool   verbose_;

  /// contains tracking particles
  bool   hasSimInfo_;

  /// print the blocks associated to a given candidate ?
  bool   printBlocks_;

  double thePtMin_;

  bool applyTrkQCs_;
  double minHits_, maxPtErr_, maxD0_, maxDZ_, maxD0Norm_, maxDZNorm_;
  bool pixelSeedOnly_;

  // ntuple
  TNtuple *nt;

  // TH1F


  // TH2F
  TH2F *hTrkPtEcalEtSum, *hTrkPtHcalEtSum, *hTrkPtCaloEtSum;
  TH2F *hTrkPtEcalEtSum_fake, *hTrkPtHcalEtSum_fake, *hTrkPtCaloEtSum_fake;

  TH2F *hTrkPtEcalEtMax, *hTrkPtHcalEtMax, *hTrkPtCaloEtMax;
  TH2F *hTrkPtEcalEtMax_fake, *hTrkPtHcalEtMax_fake, *hTrkPtCaloEtMax_fake;


  // TH3F
  TH3F *hTrkPtEtaEcalEtSum, *hTrkPtEtaHcalEtSum, *hTrkPtEtaCaloEtSum;
  TH3F *hTrkPtEtaEcalEtSum_fake, *hTrkPtEtaHcalEtSum_fake, *hTrkPtEtaCaloEtSum_fake;

  TH3F *hTrkPtEtaEcalEtMax, *hTrkPtEtaHcalEtMax, *hTrkPtEtaCaloEtMax;
  TH3F *hTrkPtEtaEcalEtMax_fake, *hTrkPtEtaHcalEtMax_fake, *hTrkPtEtaCaloEtMax_fake;

  edm::Service<TFileService> fs;

  float cand_type;
  float cand_pt;
  float cand_eta;
  float max_trk;
  float sum_trk;
  float max_ecal;
  float sum_ecal;
  float max_hcal;
  float sum_hcal;
  float sum_calo;
  float max_calo;

  float max_nhits,max_relpterr,max_algo,max_nd0,max_ndz,max_fake;

  bool prodNtuple_;
  bool isData_;


  std::vector<double> etaBins, ptBins, cEtSumBins, nhitBins;

  //vector<pair<float,float>> elements; // type, pt 

};

#endif
