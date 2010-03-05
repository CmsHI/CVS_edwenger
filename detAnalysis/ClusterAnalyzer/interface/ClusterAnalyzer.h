#ifndef ClusterAnalyzer_h
#define ClusterAnalyzer_h

#include "FWCore/Utilities/interface/Exception.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h" 

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"


#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"  
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetType.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"

//needed for the geometry:
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"


//Data Formats
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
#include "SimDataFormats/TrackerDigiSimLink/interface/StripDigiSimLink.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"


//Services
#include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"

//SimTrack
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"

//TrackAssociator 
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByHits.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"

// Trajectories
/* #include "TrackingTools/PatternTools/interface/Trajectory.h" */
/* #include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h" */

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TObjArray.h"
#include "TRandom.h"
#include "THashList.h"

#include "vector"
#include <memory>
#include <string>
#include <iostream>

// For the splitter function
#include "wtfRecoLocalTracker/SplitClustersProducer/interface/SplitClustersAlgos.h"


class DQMStore;


class ClusterAnalyzer : public edm::EDAnalyzer, public SplitClustersAlgos
{

 public:
  
  explicit ClusterAnalyzer(const edm::ParameterSet& conf);
  ~ClusterAnalyzer();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginJob() ;
  virtual void endJob() ;
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);
  virtual void endRun(const edm::Run&, const edm::EventSetup&);

  std::vector<PSimHit> associatedA;
  std::vector<PSimHit> associatedP;
  std::vector<PSimHit> associatedT;
 
 private: 

  // void getClusterInfoFromRecHit(const SiStripRecHit2D* trackerRecHit_, 
  //                               LocalVector       clusterLocalVector_,
  //                               reco::TrackRef              trackRef_,
  //                               const edm::EventSetup&            es);

  //  bool fillClusterHistos(SiStripClusterInfo*       clusterInfo_,
  //			 const uint32_t&                  detid_,
  //			 TString                          flag ,
  //			 const LocalVector clusterLocalVector_);

  std::pair<unsigned int,int> FindDetLayer(DetId&);

  edm::ParameterSet conf_;

  std::string theCMNSubtractionMode;
  std::string theOutputFilename;
  edm::InputTag theTrackSourceLabel;
  edm::InputTag theTrackTrackInfoAssocLabel;
  edm::InputTag theClusterSourceLabel;

  std::vector<uint32_t> theModulesToBeExcluded;
 
  DQMStore* daqMonInterface_;
  bool show_mechanical_structure_view;
  bool reset_each_run;

  unsigned long long cacheID_;
  
  int runNb;
  int eventNb;
  
  edm::ESHandle<TrackerGeometry> trackerGeom_;   
  edm::ESHandle<SiStripDetCabling> stripDetCabling_;
  edm::ESHandle<TrackAssociatorBase> trackAssociator;
  
  LocalVector clusterLocalVector_;

  int totalNumberOfClustersOnTrack;
  int countOn;
  double secondmindistance;
  
  TrackerHitAssociator * hitAssociator;

  TH1F* hTrackPt;
  TH1F* hTIBLayer;
  TH1F* hTOBLayer;
  TH1F* hTIDSide;
  TH1F* hTIDWheel;
  TH1F* hTIDRing;
  TH1F* hTECSide;
  TH1F* hTECWheel;
  TH1F* hTECRing;

  TH1F* hMergedFractionFromTracksTOB[6];
  TH1F* hMergedFractionFromTracksTIB[4];
  TH1F* hMergedFractionFromTracksTID[3];
  TH1F* hMergedFractionFromTracksTEC[9];
  TH1F* hNumeratorMergedFractionFromTracksTOB[6];
  TH1F* hNumeratorMergedFractionFromTracksTIB[4];
  TH1F* hNumeratorMergedFractionFromTracksTID[3];
  TH1F* hNumeratorMergedFractionFromTracksTEC[9];
  TH1F* hDenominatorMergedFractionFromTracksTOB[6];
  TH1F* hDenominatorMergedFractionFromTracksTIB[4];
  TH1F* hDenominatorMergedFractionFromTracksTID[3];
  TH1F* hDenominatorMergedFractionFromTracksTEC[9];

  TH1F* hNumberOfSimHitsPerRecHit;
  TH1F* hNumberOfSimHitsPerRecHitFromTracks;
  TH1F* hNumberOfSimTracksPerMergedRecHit;
  TH1F* hNumberOfSimTracksPerMergedRecHitFromTracks;
  TFile *outputfile;  
  
  TH1F* hMergedFractionTotal;
  TH1F* hMergedFractionAssociating;
  TH1F* hMergedFractionTotalTOB[6];
  TH1F* hMergedFractionAssociatingTOB[6];
  TH1F* hMergedFractionTotalTIB[4];
  TH1F* hMergedFractionAssociatingTIB[4];
  TH1F* hMergedFractionTotalTID[3];
  TH1F* hMergedFractionAssociatingTID[3];  
  TH1F* hMergedFractionTotalTEC[9];
  TH1F* hMergedFractionAssociatingTEC[9];  
  TH1F* hNumeratorMergedFractionTotal;
  TH1F* hNumeratorMergedFractionAssociating;
  TH1F* hNumeratorMergedFractionTotalTOB[6];
  TH1F* hNumeratorMergedFractionAssociatingTOB[6];
  TH1F* hNumeratorMergedFractionTotalTIB[4];
  TH1F* hNumeratorMergedFractionAssociatingTIB[4];
  TH1F* hNumeratorMergedFractionTotalTID[3];
  TH1F* hNumeratorMergedFractionAssociatingTID[3];  
  TH1F* hNumeratorMergedFractionTotalTEC[9];
  TH1F* hNumeratorMergedFractionAssociatingTEC[9];  
  TH1F* hDenominatorMergedFractionTotal;
  TH1F* hDenominatorMergedFractionAssociating;
  TH1F* hDenominatorMergedFractionTotalTOB[6];
  TH1F* hDenominatorMergedFractionAssociatingTOB[6];
  TH1F* hDenominatorMergedFractionTotalTIB[4];
  TH1F* hDenominatorMergedFractionAssociatingTIB[4];
  TH1F* hDenominatorMergedFractionTotalTID[3];
  TH1F* hDenominatorMergedFractionAssociatingTID[3];  
  TH1F* hDenominatorMergedFractionTotalTEC[9];
  TH1F* hDenominatorMergedFractionAssociatingTEC[9];  

  TH2I* hClusterWidthVSNumberOfSimHitsPerRecHit;

  TH1F *hClustersToBeSplit, *hClustersToBeSplitCharge, *hClusterToBeSplitWidth;
  TH2I *hClustersToBeSplitChargeVSEnergyLossTotal;

  TH1F *hClustersToBeSplitFromTracks, *hClustersToBeSplitChargeFromTracks, *hClusterToBeSplitWidthFromTracks;
  TH2I *hClustersToBeSplitChargeVSEnergyLossTotalFromTracks;

  TH1F *hLeftChargeFraction;

  TH1F *hCluterDeltaPositionsP2P1;
  TH1F *hCluterDeltaPositionsP2P0;
  TH1F *hCluterDeltaPositionsP2P1FromTracks;
  TH1F *hCluterDeltaPositionsP2P0FromTracks;

};

#endif
