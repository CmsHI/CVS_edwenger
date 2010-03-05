#include "detAnalysis/ClusterAnalyzer/interface/ClusterAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
#include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/ProjectedSiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "DataFormats/TrackingRecHit/interface/InvalidTrackingRecHit.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"

// For pixel
// For Pixel RecHits
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

// For SimHits
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"

#include "Geometry/TrackerGeometryBuilder/interface/GluedGeomDet.h"
#include "AnalysisDataFormats/TrackInfo/src/TrackInfo.cc"


// For Strip RecHits
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/SiStripCluster/interface/SiStripClusterCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/Common/interface/OwnVector.h"

#include "AnalysisDataFormats/SiStripClusterInfo/interface/SiStripClusterInfo.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"

#include "sstream"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH2I.h"
#include "TH3S.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TPostScript.h"

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
  
ClusterAnalyzer::ClusterAnalyzer(edm::ParameterSet const& conf): 
 conf_(conf),
 theCMNSubtractionMode(conf.getUntrackedParameter<std::string>( "CMNSubtractionMode" ,"Median")), 
 theOutputFilename(conf.getUntrackedParameter<std::string>( "outputFilename", "out.root")),
 theTrackSourceLabel(conf.getParameter<edm::InputTag>( "trackSourceLabel" )), 
 theTrackTrackInfoAssocLabel(conf.getParameter<edm::InputTag>( "trackTrackInfoAssocLabel" )),
 theClusterSourceLabel(conf.getParameter<edm::InputTag>( "clusterSourceLabel" )),
 theModulesToBeExcluded(conf.getParameter< std::vector<uint32_t> >("modulesToBeExcluded")),
 //daqMonInterface_(edm::Service<DQMStore>().operator->()), 
 show_mechanical_structure_view(true), 
 reset_each_run(false),
 secondmindistance(conf.getParameter<double>("SecondMinDistanceRecHitSimHit"))
//  ,SplitClustersProducer(conf)
//  ,splitter_(conf)
{
 totalNumberOfClustersOnTrack=0;
 countOn=0;
}

ClusterAnalyzer::~ClusterAnalyzer(){ }

void ClusterAnalyzer::beginJob( ) {

 outputfile = new TFile(theOutputFilename.c_str(), "recreate");
 hTrackPt = new TH1F("TrackPt","",1000,0,1000);

 hTIBLayer = new TH1F("TIBLayer","",100,0,10);
 hTOBLayer = new TH1F("TOBLayer","",100,0,10);
 hTECSide = new TH1F("TECSide","",100,0,10);
 hTECWheel = new TH1F("TECWheel","",100,0,10);
 hTECRing = new TH1F("TECRing","",100,0,10);
 hTIDSide = new TH1F("TIDSide","",100,0,10);
 hTIDWheel = new TH1F("TIDWheel","",100,0,10);
 hTIDRing = new TH1F("TIDRing","",100,0,10);

 hNumberOfSimHitsPerRecHit = new TH1F("NumberOfSimHitsPerRecHit","",100,0,100);
 hNumberOfSimHitsPerRecHitFromTracks = new TH1F("NumberOfSimHitsPerRecHitFromTracks","",100,0,100);
 hNumberOfSimTracksPerMergedRecHit = new TH1F("NumberOfSimTracksPerMergedRecHit","",100,0,100);
 hNumberOfSimTracksPerMergedRecHitFromTracks = new TH1F("NumberOfSimTracksPerMergedRecHitFromTracks","",100,0,100);

 hMergedFractionTotal = new TH1F("MergedFractionTotal","",100,0,1);
 hMergedFractionAssociating = new TH1F("MergedFractionAssociating","",100,0,1);
 hNumeratorMergedFractionTotal = new TH1F("NumeratorMergedFractionTotal","",3000,0,3000);
 hNumeratorMergedFractionAssociating = new TH1F("NumeratorMergedFractionAssociating","",3000,0,3000);
 hDenominatorMergedFractionTotal = new TH1F("DenominatorMergedFractionTotal","",3000,0,3000);
 hDenominatorMergedFractionAssociating = new TH1F("DenominatorMergedFractionAssociating","",3000,0,3000);
 // TOB histos definition
 for(int i=0; i<6; i++){
  TString tobT = "MergedFractionTotalTOB";
  TString tobM = "MergedFractionAssociatingTOB";
  TString tobTr = "MergedFractionFromTracksTOB";
  tobT += i+1;tobM += i+1;
  tobTr += i+1;
  hMergedFractionTotalTOB[i] = new TH1F(tobT,"",100,0,1);
  hMergedFractionAssociatingTOB[i] = new TH1F(tobM,"",100,0,1);
  hMergedFractionFromTracksTOB[i] = new TH1F(tobTr,"",100,0,1);
 }
 for(int i=0; i<6; i++){
  TString tobNT = "NumeratorMergedFractionTotalTOB";
  TString tobNM = "NumeratorMergedFractionAssociatingTOB";
  TString tobNTr = "NumeratorMergedFractionFromTracksTOB";
  tobNT += i+1;tobNM += i+1;
  tobNTr += i+1;
  hNumeratorMergedFractionTotalTOB[i] = new TH1F(tobNT,"",3000,0,3000);
  hNumeratorMergedFractionAssociatingTOB[i] = new TH1F(tobNM,"",3000,0,3000);
  hNumeratorMergedFractionFromTracksTOB[i] = new TH1F(tobNTr,"",3000,0,3000);
 }
 for(int i=0; i<6; i++){
  TString tobDT = "DenominatorMergedFractionTotalTOB";
  TString tobDM = "DenominatorMergedFractionAssociatingTOB";
  TString tobDTr = "DenominatorMergedFractionFromTracksTOB";
  tobDT += i+1;tobDM += i+1;
  tobDTr += i+1;
  hDenominatorMergedFractionTotalTOB[i] = new TH1F(tobDT,"",3000,0,3000);
  hDenominatorMergedFractionAssociatingTOB[i] = new TH1F(tobDM,"",3000,0,3000);
  hDenominatorMergedFractionFromTracksTOB[i] = new TH1F(tobDTr,"",3000,0,3000);
 }
 // TIB histos definition
 for(int i=0; i<4; i++){
  TString tibT = "MergedFractionTotalTIB";
  TString tibM = "MergedFractionAssociatingTIB";
  TString tibTr = "MergedFractionFromTracksTIB";
  tibT += i+1;tibM += i+1;
  tibTr += i+1;
  hMergedFractionTotalTIB[i] = new TH1F(tibT,"",100,0,1);
  hMergedFractionAssociatingTIB[i] = new TH1F(tibM,"",100,0,1);
  hMergedFractionFromTracksTIB[i] = new TH1F(tibTr,"",100,0,1);
 }
 for(int i=0; i<4; i++){
  TString tibNT = "NumeratorMergedFractionTotalTIB";
  TString tibNM = "NumeratorMergedFractionAssociatingTIB";
  TString tibNTr = "NumeratorMergedFractionFromTracksTIB";
  tibNT += i+1;tibNM += i+1;
  tibNTr += i+1;
  hNumeratorMergedFractionTotalTIB[i] = new TH1F(tibNT,"",3000,0,3000);
  hNumeratorMergedFractionAssociatingTIB[i] = new TH1F(tibNM,"",3000,0,3000);
  hNumeratorMergedFractionFromTracksTIB[i] = new TH1F(tibNTr,"",3000,0,3000);
 }
 for(int i=0; i<4; i++){
  TString tibDT = "DenominatorMergedFractionTotalTIB";
  TString tibDM = "DenominatorMergedFractionAssociatingTIB";
  TString tibDTr = "DenominatorMergedFractionFromTracksTIB";
  tibDT += i+1;tibDM += i+1;
  tibDTr += i+1;
  hDenominatorMergedFractionTotalTIB[i] = new TH1F(tibDT,"",3000,0,3000);
  hDenominatorMergedFractionAssociatingTIB[i] = new TH1F(tibDM,"",3000,0,3000);
  hDenominatorMergedFractionFromTracksTIB[i] = new TH1F(tibDTr,"",3000,0,3000);
 }
 // TID histos definition
 for(int i=0; i<3; i++){
  TString tidT = "MergedFractionTotalTID";
  TString tidM = "MergedFractionAssociatingTID";
  TString tidTr = "MergedFractionFromTracksTID";
  tidT += i+1;tidM += i+1;
  tidTr += i+1;
  hMergedFractionTotalTID[i] = new TH1F(tidT,"",100,0,1);
  hMergedFractionAssociatingTID[i] = new TH1F(tidM,"",100,0,1);
  hMergedFractionFromTracksTID[i] = new TH1F(tidTr,"",100,0,1);
 }
 for(int i=0; i<3; i++){
  TString tidNT = "NumeratorMergedFractionTotalTID";
  TString tidNM = "NumeratorMergedFractionAssociatingTID";
  TString tidNTr = "NumeratorMergedFractionFromTracksTID";
  tidNT += i+1;tidNM += i+1;
  tidNTr += i+1;
  hNumeratorMergedFractionTotalTID[i] = new TH1F(tidNT,"",3000,0,3000);
  hNumeratorMergedFractionAssociatingTID[i] = new TH1F(tidNM,"",3000,0,3000);
  hNumeratorMergedFractionFromTracksTID[i] = new TH1F(tidNTr,"",3000,0,3000);
 }
 for(int i=0; i<3; i++){
  TString tidDT = "DenominatorMergedFractionTotalTID";
  TString tidDM = "DenominatorMergedFractionAssociatingTID";
  TString tidDTr = "DenominatorMergedFractionFromTracksTID";
  tidDT += i+1;tidDM += i+1;
  tidDTr += i+1;
  hDenominatorMergedFractionTotalTID[i] = new TH1F(tidDT,"",3000,0,3000);
  hDenominatorMergedFractionAssociatingTID[i] = new TH1F(tidDM,"",3000,0,3000);
  hDenominatorMergedFractionFromTracksTID[i] = new TH1F(tidDTr,"",3000,0,3000);
 }
 // TEC histos definition
 for(int i=0; i<9; i++){
  TString tidT = "MergedFractionTotalTEC";
  TString tidM = "MergedFractionAssociatingTEC";
  TString tidTr = "MergedFractionFromTracksTEC";
  tidT += i+1;tidM += i+1;
  tidTr += i+1;
  hMergedFractionTotalTEC[i] = new TH1F(tidT,"",100,0,1);
  hMergedFractionAssociatingTEC[i] = new TH1F(tidM,"",100,0,1);
  hMergedFractionFromTracksTEC[i] = new TH1F(tidTr,"",100,0,1);
 }
 for(int i=0; i<9; i++){
  TString tecNT = "NumeratorMergedFractionTotalTEC";
  TString tecNM = "NumeratorMergedFractionAssociatingTEC";
  TString tecNTr = "NumeratorMergedFractionFromTracksTEC";
  tecNT += i+1;tecNM += i+1;
  tecNTr += i+1;
  hNumeratorMergedFractionTotalTEC[i] = new TH1F(tecNT,"",3000,0,3000);
  hNumeratorMergedFractionAssociatingTEC[i] = new TH1F(tecNM,"",3000,0,3000);
  hNumeratorMergedFractionFromTracksTEC[i] = new TH1F(tecNTr,"",3000,0,3000);
 }
 for(int i=0; i<9; i++){
  TString tecDT = "DenominatorMergedFractionTotalTEC";
  TString tecDM = "DenominatorMergedFractionAssociatingTEC";
  TString tecDTr = "DenominatorMergedFractionFromTracksTEC";
  tecDT += i+1;tecDM += i+1;
  tecDTr += i+1;
  hDenominatorMergedFractionTotalTEC[i] = new TH1F(tecDT,"",3000,0,3000);
  hDenominatorMergedFractionAssociatingTEC[i] = new TH1F(tecDM,"",3000,0,3000);
  hDenominatorMergedFractionFromTracksTEC[i] = new TH1F(tecDTr,"",3000,0,3000);
 }
 hClusterWidthVSNumberOfSimHitsPerRecHit = new TH2I("ClusterWidthVSNumberOfSimHitsPerRecHit","",100, 0, 100, 100, 0, 100);
 hClustersToBeSplit = new TH1F("ClustersToBeSplit","",100,0,1000);
 hClustersToBeSplitCharge = new TH1F("ClustersToBeSplitCharge","",100,0,1000);
 hClusterToBeSplitWidth = new TH1F("ClusterToBeSplitWidth","",100,0,100);
 hClustersToBeSplitChargeVSEnergyLossTotal = new TH2I("ClustersToBeSplitChargeVSEnergyLossTotal","",100, 0, 1000, 100, 0, 0.03);
 hClustersToBeSplitFromTracks = new TH1F("ClustersToBeSplitFromTracks","",100,0,1000);
 hClustersToBeSplitChargeFromTracks = new TH1F("ClustersToBeSplitChargeFromTracks","",100,0,1000);
 hClusterToBeSplitWidthFromTracks = new TH1F("ClusterToBeSplitWidthFromTracks","",100,0,100);
 hClustersToBeSplitChargeVSEnergyLossTotalFromTracks = new TH2I("ClustersToBeSplitChargeVSEnergyLossTotalFromTracks","",100, 0, 1000, 100, 0, 0.03);
 hLeftChargeFraction = new TH1F("LeftEnergyFraction","",100,0,1);

 hCluterDeltaPositionsP2P1 = new TH1F("CluterDeltaPositionsP2P1","",100,0,1);
 hCluterDeltaPositionsP2P0 = new TH1F("CluterDeltaPositionsP2P0","",100,0,1);
 hCluterDeltaPositionsP2P1FromTracks = new TH1F("CluterDeltaPositionsP2P1FromTracks","",100,0,1);
 hCluterDeltaPositionsP2P0FromTracks = new TH1F("CluterDeltaPositionsP2P0FromTracks","",100,0,1);

}

void ClusterAnalyzer::endJob(void){

 outputfile->WriteTObject(hTrackPt);
 outputfile->WriteTObject(hTOBLayer);
 outputfile->WriteTObject(hTIBLayer);
 outputfile->WriteTObject(hTIDSide);
 outputfile->WriteTObject(hTIDWheel);
 outputfile->WriteTObject(hTIDRing);
 outputfile->WriteTObject(hTECSide);
 outputfile->WriteTObject(hTECWheel);
 outputfile->WriteTObject(hTECRing);

 outputfile->WriteTObject(hNumberOfSimHitsPerRecHit);
 outputfile->WriteTObject(hNumberOfSimHitsPerRecHitFromTracks);
 outputfile->WriteTObject(hNumberOfSimTracksPerMergedRecHit);
 outputfile->WriteTObject(hNumberOfSimTracksPerMergedRecHitFromTracks);

 outputfile->WriteTObject(hMergedFractionTotal);
 outputfile->WriteTObject(hMergedFractionAssociating);
 outputfile->WriteTObject(hNumeratorMergedFractionTotal);
 outputfile->WriteTObject(hNumeratorMergedFractionAssociating);
 outputfile->WriteTObject(hDenominatorMergedFractionTotal);
 outputfile->WriteTObject(hDenominatorMergedFractionAssociating);

 for(int i=0; i<6; i++){
  outputfile->WriteTObject(hMergedFractionTotalTOB[i]);
  outputfile->WriteTObject(hMergedFractionAssociatingTOB[i]);
  outputfile->WriteTObject(hMergedFractionFromTracksTOB[i]);
 }
 for(int i=0; i<6; i++){
  outputfile->WriteTObject(hNumeratorMergedFractionTotalTOB[i]);
  outputfile->WriteTObject(hNumeratorMergedFractionAssociatingTOB[i]);
  outputfile->WriteTObject(hNumeratorMergedFractionFromTracksTOB[i]);
 }
 for(int i=0; i<6; i++){
  outputfile->WriteTObject(hDenominatorMergedFractionTotalTOB[i]);
  outputfile->WriteTObject(hDenominatorMergedFractionAssociatingTOB[i]);
  outputfile->WriteTObject(hDenominatorMergedFractionFromTracksTOB[i]);
 }

 for(int i=0; i<9; i++){
  outputfile->WriteTObject(hMergedFractionTotalTEC[i]);
  outputfile->WriteTObject(hMergedFractionAssociatingTEC[i]);
  outputfile->WriteTObject(hMergedFractionFromTracksTEC[i]);
 }
 for(int i=0; i<9; i++){
  outputfile->WriteTObject(hNumeratorMergedFractionTotalTEC[i]);
  outputfile->WriteTObject(hNumeratorMergedFractionAssociatingTEC[i]);
  outputfile->WriteTObject(hNumeratorMergedFractionFromTracksTEC[i]);
 }
 for(int i=0; i<9; i++){
  outputfile->WriteTObject(hDenominatorMergedFractionTotalTEC[i]);
  outputfile->WriteTObject(hDenominatorMergedFractionAssociatingTEC[i]);
  outputfile->WriteTObject(hDenominatorMergedFractionFromTracksTEC[i]);
 }

 for(int i=0; i<4; i++){
  outputfile->WriteTObject(hMergedFractionTotalTIB[i]);
  outputfile->WriteTObject(hMergedFractionAssociatingTIB[i]);
  outputfile->WriteTObject(hMergedFractionFromTracksTIB[i]);
 }
 for(int i=0; i<9; i++){
  outputfile->WriteTObject(hNumeratorMergedFractionTotalTIB[i]);
  outputfile->WriteTObject(hNumeratorMergedFractionAssociatingTIB[i]);
  outputfile->WriteTObject(hNumeratorMergedFractionFromTracksTIB[i]);
 }
 for(int i=0; i<9; i++){
  outputfile->WriteTObject(hDenominatorMergedFractionTotalTIB[i]);
  outputfile->WriteTObject(hDenominatorMergedFractionAssociatingTIB[i]);
  outputfile->WriteTObject(hDenominatorMergedFractionFromTracksTIB[i]);
 }

 for(int i=0; i<3; i++){
  outputfile->WriteTObject(hMergedFractionTotalTID[i]);
  outputfile->WriteTObject(hMergedFractionAssociatingTID[i]);
  outputfile->WriteTObject(hMergedFractionFromTracksTID[i]);
 }
 for(int i=0; i<3; i++){
  outputfile->WriteTObject(hNumeratorMergedFractionTotalTID[i]);
  outputfile->WriteTObject(hNumeratorMergedFractionAssociatingTID[i]);
  outputfile->WriteTObject(hNumeratorMergedFractionFromTracksTID[i]);
 }
 for(int i=0; i<3; i++){
  outputfile->WriteTObject(hDenominatorMergedFractionTotalTID[i]);
  outputfile->WriteTObject(hDenominatorMergedFractionAssociatingTID[i]);
  outputfile->WriteTObject(hDenominatorMergedFractionFromTracksTID[i]);
 }
 outputfile->WriteTObject(hClusterWidthVSNumberOfSimHitsPerRecHit);
 outputfile->WriteTObject(hClustersToBeSplit);
 outputfile->WriteTObject(hClustersToBeSplitCharge);
 outputfile->WriteTObject(hClusterToBeSplitWidth);
 outputfile->WriteTObject(hClustersToBeSplitChargeVSEnergyLossTotal);
 outputfile->WriteTObject(hClustersToBeSplitFromTracks);
 outputfile->WriteTObject(hClustersToBeSplitChargeFromTracks);
 outputfile->WriteTObject(hClusterToBeSplitWidthFromTracks);
 outputfile->WriteTObject(hClustersToBeSplitChargeVSEnergyLossTotalFromTracks);
 outputfile->WriteTObject(hCluterDeltaPositionsP2P1);
 outputfile->WriteTObject(hCluterDeltaPositionsP2P0);
 outputfile->WriteTObject(hCluterDeltaPositionsP2P1FromTracks);
 outputfile->WriteTObject(hCluterDeltaPositionsP2P0FromTracks);
 // outputfile->Write();
 outputfile->Close();
}

void ClusterAnalyzer::beginRun(const edm::Run& run, const edm::EventSetup& es){

 es.get<TrackerDigiGeometryRecord>().get( trackerGeom_ );
 es.get<SiStripDetCablingRcd>().get(stripDetCabling_);

 es.get<TrackAssociatorRecord>().get("TrackAssociatorByHits",trackAssociator);

}
void ClusterAnalyzer::endRun(const edm::Run&, const edm::EventSetup&){}

void ClusterAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& es) {
 using namespace edm;
 runNb   = e.id().run();
 eventNb = e.id().event();

 /////////////////////////////////
 // SiStripCluster collection  ///
 /////////////////////////////////
   /* wtf
 edm::Handle< edmNew::DetSetVector<SiStripCluster> > clusterHandle;
 e.getByLabel("siStripClusters", clusterHandle);
 // cout <<"clusterHandle size : "<<clusterHandle->size()<<endl;
 */

 /////////////////////////
 // SimHits collections //
 /////////////////////////
 edm::Handle<PSimHitContainer> TOBHitsLowTof;
 edm::Handle<PSimHitContainer> TOBHitsHighTof;
 edm::Handle<PSimHitContainer> TECHitsLowTof;
 edm::Handle<PSimHitContainer> TECHitsHighTof;
 edm::Handle<PSimHitContainer> TIBHitsLowTof;
 edm::Handle<PSimHitContainer> TIBHitsHighTof;
 edm::Handle<PSimHitContainer> TIDHitsLowTof;
 edm::Handle<PSimHitContainer> TIDHitsHighTof;
 e.getByLabel("g4SimHits","TrackerHitsTOBLowTof", TOBHitsLowTof);
 e.getByLabel("g4SimHits","TrackerHitsTOBHighTof", TOBHitsHighTof);
 e.getByLabel("g4SimHits","TrackerHitsTECLowTof", TECHitsLowTof);
 e.getByLabel("g4SimHits","TrackerHitsTECHighTof", TECHitsHighTof);
 e.getByLabel("g4SimHits","TrackerHitsTIBLowTof", TIBHitsLowTof);
 e.getByLabel("g4SimHits","TrackerHitsTIBHighTof", TIBHitsHighTof);
 e.getByLabel("g4SimHits","TrackerHitsTIDLowTof", TIDHitsLowTof);
 e.getByLabel("g4SimHits","TrackerHitsTIDHighTof", TIDHitsHighTof);
 vector<PSimHit> theTrackerHits;
 theTrackerHits.insert(theTrackerHits.end(), TOBHitsLowTof->begin(), TOBHitsLowTof->end());
 theTrackerHits.insert(theTrackerHits.end(), TOBHitsHighTof->begin(), TOBHitsHighTof->end());
 theTrackerHits.insert(theTrackerHits.end(), TECHitsLowTof->begin(), TECHitsLowTof->end());
 theTrackerHits.insert(theTrackerHits.end(), TECHitsHighTof->begin(), TECHitsHighTof->end());
 theTrackerHits.insert(theTrackerHits.end(), TIBHitsLowTof->begin(), TIBHitsLowTof->end());
 theTrackerHits.insert(theTrackerHits.end(), TIBHitsHighTof->begin(), TIBHitsHighTof->end());
 theTrackerHits.insert(theTrackerHits.end(), TIDHitsLowTof->begin(), TIDHitsLowTof->end());
 theTrackerHits.insert(theTrackerHits.end(), TIDHitsHighTof->begin(), TIDHitsHighTof->end());

 /////////////////////////
 // RecHits collections //
 /////////////////////////
//  edm::Handle<SiStripMatchedRecHit2DCollection> rechitsmatched;
 edm::Handle<SiStripRecHit2DCollection> rechitsrphi;
 edm::Handle<SiStripRecHit2DCollection> rechitsstereo;
//  edm::Handle<SiPixelRecHitCollection> pixelrechits;
//  e.getByLabel("siStripMatchedRecHits","matchedRecHit", rechitsmatched);
 e.getByLabel("siStripMatchedRecHits","rphiRecHit", rechitsrphi);
 e.getByLabel("siStripMatchedRecHits","stereoRecHit", rechitsstereo);
//  e.getByLabel("siPixelRecHits",pixelrechits);

 ///////////////////
 // SimTracks //////
 ///////////////////
   /* wtf
 edm::Handle<TrackingParticleCollection> trackingParticleCollectionHandle;
 e.getByLabel("trackingtruthprod",trackingParticleCollectionHandle);
   */

 ////////////////
 // RecTracks ///
 ////////////////
 edm::Handle<reco::TrackCollection > trackCollectionHandle;
 e.getByLabel(theTrackSourceLabel, trackCollectionHandle);
 const reco::TrackCollection* myTracks = trackCollectionHandle.product();
//  cout<<"RecTracks size : "<< myTracks->size() <<endl; 

 ///////////////////////////////////////////////
 // Construct the Associators once for all  ////
 ///////////////////////////////////////////////
 //TrackerHitAssociator hitAssociator(e,conf_);
 hitAssociator = new TrackerHitAssociator::TrackerHitAssociator(e);
 // RecoToSimCollection recSimColl=trackAssociator->associateRecoToSim(trackCollectionHandle, trackingParticleCollectionHandle, &e);

 /////////////////////////////////////////
 // PART A of the Analyzer:             //
 // All RecHits are taken into account  //
 /////////////////////////////////////////

 int clusters_to_be_split=0;
 int rechitscounter=0;
 int rechitscounterTID[3];
 int rechitscounterTIB[4];
 int rechitscounterTEC[9];
 int rechitscounterTOB[6];
 float merged = 0;
 float mergedTID[3];
 float mergedTIB[4];
 float mergedTEC[9];
 float mergedTOB[6];
 float associatingcounter = 0;
 float associatingcounterTID[3];
 float associatingcounterTIB[4];
 float associatingcounterTEC[9];
 float associatingcounterTOB[6];
 for(int i=0; i<4; i++){ rechitscounterTIB[i] = 0; mergedTIB[i] = 0; associatingcounterTIB[i] = 0; }
 for(int i=0; i<3; i++){ rechitscounterTID[i] = 0; mergedTID[i] = 0; associatingcounterTID[i] = 0; }
 for(int i=0; i<9; i++){ rechitscounterTEC[i] = 0; mergedTEC[i] = 0; associatingcounterTEC[i] = 0; }
 for(int i=0; i<6; i++){ rechitscounterTOB[i] = 0; mergedTOB[i] = 0; associatingcounterTOB[i] = 0; }
 unsigned int iSubDet = 0;
 int layer = 0;
 int side = 0;
 int wheel = 0;
 int ring = 0;

 ////////////////////////////////////////////////
 // LOOPS on rphi and stereo RecHits follow    //
 ////////////////////////////////////////////////

 // LOOP on SiStripRecHits
 const SiStripRecHit2DCollection::DataContainer& rechitsrphiColl = rechitsrphi->data();
 for(SiStripRecHit2DCollection::DataContainer::const_iterator rechitsrphiIter = rechitsrphiColl.begin(); rechitsrphiIter != rechitsrphiColl.end(); ++rechitsrphiIter)
 {
  const SiStripRecHit2D *aHit = &*(rechitsrphiIter);
  StripSubdetector detid=(StripSubdetector)aHit->geographicalId();
  iSubDet = StripSubdetector(detid).subdetId();
  if (iSubDet == StripSubdetector::TID || iSubDet == StripSubdetector::TIB || iSubDet == StripSubdetector::TEC || iSubDet == StripSubdetector::TOB){

   //Get the layer or wheel number 
   if (iSubDet == StripSubdetector::TIB){ TIBDetId* tibId = new TIBDetId(detid); layer = tibId->layer(); }
   if (iSubDet == StripSubdetector::TID){ TIDDetId* tidId = new TIDDetId(detid); side = tidId->side(); wheel = tidId->wheel(); ring = tidId->ring(); }
   if (iSubDet == StripSubdetector::TOB){ TOBDetId* tobId = new TOBDetId(detid); layer = tobId->layer(); }
   if (iSubDet == StripSubdetector::TEC){ TECDetId* tecId = new TECDetId(detid); side = tecId->side(); wheel = tecId->wheel(); ring = tecId->ring(); }

   //Get the RecHit
   SiStripRecHit2D const rechit=*aHit;
   //Get the Cluster which has produced the RecHit
   const SiStripCluster* clust = 0; 
   if(aHit->cluster().isNonnull())
   { clust=&(*aHit->cluster());
   }else if(aHit->cluster_regional().isNonnull())
   { clust=&(*aHit->cluster_regional());
   }else{ edm::LogError("TrackerHitAssociator")<<"no cluster reference attached"; }

   //Counting total number of RecHits
   rechitscounter++;
   if(iSubDet == StripSubdetector::TID) rechitscounterTID[wheel-1] += 1;
   if(iSubDet == StripSubdetector::TIB) rechitscounterTIB[layer-1] += 1;
   if(iSubDet == StripSubdetector::TEC) rechitscounterTEC[wheel-1] += 1;
   if(iSubDet == StripSubdetector::TOB) rechitscounterTOB[layer-1] += 1;

   //Associate the RecHit to the SimHits
   associatedA.clear();
   associatedA = hitAssociator->associateHit(rechit);


   //Evaluate clusters to be split quantities

   std::vector<uint8_t> amp=clust->amplitudes();
   float totalEnergy = 0, leftAmplitudeFraction = 0;
   uint8_t stripCounter = 0;
   int charge = 0;
   if(associatedA.size()==2 && amp.size()>1){   
    for (size_t i=0;i<amp.size();++i) charge += amp[i];
    stripCounter = leftStripCount(amp, associatedA, totalEnergy, leftAmplitudeFraction, SplitClustersAlgos::byHits);

    if(stripCounter > 0 && stripCounter < amp.size()){
     std::vector<uint16_t> tmp1, tmp2;
     for(size_t j=0;j<size_t(stripCounter);++j) tmp1.push_back(amp[j]);
     for(size_t k=stripCounter;k<amp.size();++k) tmp2.push_back(amp[k]);
     int sumx1 = 0;
     int suma1 = 0;
     size_t asize1 = tmp1.size();
     for (size_t i=0;i<asize1;++i) {
      sumx1 += (clust->firstStrip()+i)*(tmp1[i]);
      suma1 += tmp1[i];
     }
     float barycenter1 = sumx1 / static_cast<float>(suma1) + 0.5;
     int sumx2 = 0;
     int suma2 = 0;
     size_t asize2 = tmp2.size();
     for (size_t i=0;i<asize2;++i) {
      sumx2 += (clust->firstStrip()+stripCounter+i)*(tmp2[i]);
      suma2 += tmp2[i];
     }
     float barycenter2 = sumx2 / static_cast<float>(suma2) + 0.5;
     hCluterDeltaPositionsP2P1->Fill(barycenter2 - barycenter1);
     hCluterDeltaPositionsP2P0->Fill(barycenter2 - clust->barycenter());
    }
   } // if two simHits and >1 strip

   //Evaluate some associating quantity
   hNumberOfSimHitsPerRecHit->Fill(associatedA.size());
   hClusterWidthVSNumberOfSimHitsPerRecHit->Fill(clust->amplitudes().size(),associatedA.size());

   //Counting number of associating RecHits
   if(!associatedA.empty()){
    associatingcounter += 1;
    if(iSubDet == StripSubdetector::TID) associatingcounterTID[wheel-1] += 1;
    if(iSubDet == StripSubdetector::TIB) associatingcounterTIB[layer-1] += 1;
    if(iSubDet == StripSubdetector::TEC) associatingcounterTEC[wheel-1] += 1;
    if(iSubDet == StripSubdetector::TOB) associatingcounterTOB[layer-1] += 1;
    //Counting number of merged RecHits
    if(associatedA.size()>1){
     if(associatedA.size()==2 && amp.size()>1 && stripCounter >0 && stripCounter < amp.size()){
      clusters_to_be_split++;
      hClustersToBeSplitCharge->Fill(charge);
      hClusterToBeSplitWidth->Fill(amp.size());
      hClustersToBeSplitChargeVSEnergyLossTotal->Fill(charge, totalEnergy);
      hLeftChargeFraction->Fill(leftAmplitudeFraction);
     }
     merged += 1;
     if(iSubDet == StripSubdetector::TID) mergedTID[wheel-1] += 1;
     if(iSubDet == StripSubdetector::TIB) mergedTIB[layer-1] += 1;
     if(iSubDet == StripSubdetector::TEC) mergedTEC[wheel-1] += 1;
     if(iSubDet == StripSubdetector::TOB) mergedTOB[layer-1] += 1;
    }
    //Counting number of associating SimTracks
    int num_simTrack_associated = 0;
    unsigned int testTrackId = 0;
    for(vector<PSimHit>::const_iterator m=associatedA.begin(); m<associatedA.end(); m++){
     /*
     cout << " (*m):" << (*m);
     cout << " (*m).energyLoss():" << (*m).energyLoss() << endl;
     cout << " (*m).processType():" << (*m).processType();
     cout << " (*m).particleType():" << (*m).particleType();
     cout << " (*m).trackId():" << (*m).trackId();
     */
     if(m == associatedA.begin()){ testTrackId = (*m).trackId(); num_simTrack_associated = 1; }
     if(testTrackId != (*m).trackId()){ testTrackId = (*m).trackId(); num_simTrack_associated++; }
    }
    //cout << "num_simTrack_associated:" << num_simTrack_associated << endl;
    if(associatedA.size()>1) hNumberOfSimTracksPerMergedRecHit->Fill(num_simTrack_associated);
   }
  } // if TIB or TID or TEC or TOB
 } // loop on rechitsrphi

 const SiStripRecHit2DCollection::DataContainer& rechitsstereoColl = rechitsstereo->data();
 for(SiStripRecHit2DCollection::DataContainer::const_iterator rechitsstereoIter = rechitsstereoColl.begin(); rechitsstereoIter != rechitsstereoColl.end(); ++rechitsstereoIter){
  const SiStripRecHit2D *aHit = &*(rechitsstereoIter);
  StripSubdetector detid=(StripSubdetector)aHit->geographicalId();
  iSubDet = StripSubdetector(detid).subdetId();
  if (iSubDet == StripSubdetector::TID || iSubDet == StripSubdetector::TIB || iSubDet == StripSubdetector::TEC || iSubDet == StripSubdetector::TOB){

   //Get the layer or wheel number 
   if (iSubDet == StripSubdetector::TIB){ TIBDetId* tibId = new TIBDetId(detid); layer = tibId->layer(); }
   if (iSubDet == StripSubdetector::TID){ TIDDetId* tidId = new TIDDetId(detid); side = tidId->side(); wheel = tidId->wheel(); ring = tidId->ring(); }
   if (iSubDet == StripSubdetector::TOB){ TOBDetId* tobId = new TOBDetId(detid); layer = tobId->layer(); }
   if (iSubDet == StripSubdetector::TEC){ TECDetId* tecId = new TECDetId(detid); side = tecId->side(); wheel = tecId->wheel(); ring = tecId->ring(); }

   //Get the RecHit
   SiStripRecHit2D const rechit=*rechitsstereoIter;
   //Get the Cluster which has produced the RecHit
   const SiStripCluster* clust = 0; 
   if(aHit->cluster().isNonnull())
   { clust=&(*aHit->cluster());
   }else if(aHit->cluster_regional().isNonnull())
   { clust=&(*aHit->cluster_regional());
   }else{ edm::LogError("TrackerHitAssociator")<<"no cluster reference attached"; }

   //Counting total number of RecHits
   rechitscounter++;
   if(iSubDet == StripSubdetector::TID) rechitscounterTID[wheel-1] += 1;
   if(iSubDet == StripSubdetector::TIB) rechitscounterTIB[layer-1] += 1;
   if(iSubDet == StripSubdetector::TEC) rechitscounterTEC[wheel-1] += 1;
   if(iSubDet == StripSubdetector::TOB) rechitscounterTOB[layer-1] += 1;

   //Associate the RecHit to the SimHits
   associatedA.clear();
   associatedA = hitAssociator->associateHit(rechit);

   //Evaluate clusters to be split quantities
   std::vector<uint8_t> amp=clust->amplitudes();
   float totalEnergy = 0, leftAmplitudeFraction = 0;
   uint8_t stripCounter = 0;
   int charge = 0;
   if(associatedA.size()==2 && amp.size()>1){   
    for (size_t i=0;i<amp.size();++i) charge += amp[i];
    stripCounter = leftStripCount(amp, associatedA, totalEnergy, leftAmplitudeFraction, SplitClustersAlgos::byHits);

    if(stripCounter > 0 && stripCounter < amp.size()){
     std::vector<uint16_t> tmp1, tmp2;
     for(size_t j=0;j<size_t(stripCounter);++j) tmp1.push_back(amp[j]);
     for(size_t k=stripCounter;k<amp.size();++k) tmp2.push_back(amp[k]);
     int sumx1 = 0;
     int suma1 = 0;
     size_t asize1 = tmp1.size();
     for (size_t i=0;i<asize1;++i) {
      sumx1 += (clust->firstStrip()+i)*(tmp1[i]);
      suma1 += tmp1[i];
     }
     float barycenter1 = sumx1 / static_cast<float>(suma1) + 0.5;
     int sumx2 = 0;
     int suma2 = 0;
     size_t asize2 = tmp2.size();
     for (size_t i=0;i<asize2;++i) {
      sumx2 += (clust->firstStrip()+stripCounter+i)*(tmp2[i]);
      suma2 += tmp2[i];
     }
     float barycenter2 = sumx2 / static_cast<float>(suma2) + 0.5;
     hCluterDeltaPositionsP2P1->Fill(barycenter2 - barycenter1);
     hCluterDeltaPositionsP2P0->Fill(barycenter2 - clust->barycenter());
    }
   }

   // Evaluate some associating quantity
   hNumberOfSimHitsPerRecHit->Fill(associatedA.size());
   hClusterWidthVSNumberOfSimHitsPerRecHit->Fill(clust->amplitudes().size(),associatedA.size());

   //Counting number of associating RecHits
   if(!associatedA.empty()){
    associatingcounter += 1;
    if(iSubDet == StripSubdetector::TID) associatingcounterTID[wheel-1] += 1;
    if(iSubDet == StripSubdetector::TIB) associatingcounterTIB[layer-1] += 1;
    if(iSubDet == StripSubdetector::TEC) associatingcounterTEC[wheel-1] += 1;
    if(iSubDet == StripSubdetector::TOB) associatingcounterTOB[layer-1] += 1;
    //Counting number of merged RecHits
    if(associatedA.size()>1){
     if(associatedA.size()==2 && amp.size()>1 && stripCounter >0 && stripCounter < amp.size()){
      clusters_to_be_split++;
      hClustersToBeSplitCharge->Fill(charge);
      hClusterToBeSplitWidth->Fill(amp.size());
      hClustersToBeSplitChargeVSEnergyLossTotal->Fill(charge, totalEnergy);
      hLeftChargeFraction->Fill(leftAmplitudeFraction);
     }
     merged += 1;
     if(iSubDet == StripSubdetector::TID) mergedTID[wheel-1] += 1;
     if(iSubDet == StripSubdetector::TIB) mergedTIB[layer-1] += 1;
     if(iSubDet == StripSubdetector::TEC) mergedTEC[wheel-1] += 1;
     if(iSubDet == StripSubdetector::TOB) mergedTOB[layer-1] += 1;
    }
    //Counting number of associating SimTracks
    int num_simTrack_associated = 0;
    unsigned int testTrackId = 0;
    for(vector<PSimHit>::const_iterator m=associatedA.begin(); m<associatedA.end(); m++){
     if(m == associatedA.begin()){ testTrackId = (*m).trackId(); num_simTrack_associated = 1; }
     if(testTrackId != (*m).trackId()){ testTrackId = (*m).trackId(); num_simTrack_associated++; }
    }
    if(associatedA.size()>1) hNumberOfSimTracksPerMergedRecHit->Fill(num_simTrack_associated);
   }
  } // if TIB or TID or TEC or TOB
 } // loop on rechitsstereo

 //Compute the merged fractions
 hMergedFractionTotal->Fill(merged/rechitscounter);
 hMergedFractionAssociating->Fill(merged/associatingcounter);
 hNumeratorMergedFractionTotal->Fill(merged);
 hNumeratorMergedFractionAssociating->Fill(merged);
 hDenominatorMergedFractionTotal->Fill(rechitscounter);
 hDenominatorMergedFractionAssociating->Fill(associatingcounter);

 for(int i=0; i<4; i++){
  hMergedFractionTotalTIB[i]->Fill(mergedTIB[i]/rechitscounterTIB[i]);
  hMergedFractionAssociatingTIB[i]->Fill(mergedTIB[i]/associatingcounterTIB[i]);
  hNumeratorMergedFractionTotalTIB[i]->Fill(mergedTIB[i]);
  hNumeratorMergedFractionAssociatingTIB[i]->Fill(mergedTIB[i]);
  hDenominatorMergedFractionTotalTIB[i]->Fill(rechitscounterTIB[i]);
  hDenominatorMergedFractionAssociatingTIB[i]->Fill(associatingcounterTIB[i]);
 }
 for(int i=0; i<3; i++){
  hMergedFractionTotalTID[i]->Fill(mergedTID[i]/rechitscounterTID[i]);
  hMergedFractionAssociatingTID[i]->Fill(mergedTID[i]/associatingcounterTID[i]);
  hNumeratorMergedFractionTotalTID[i]->Fill(mergedTID[i]);
  hNumeratorMergedFractionAssociatingTID[i]->Fill(mergedTID[i]);
  hDenominatorMergedFractionTotalTID[i]->Fill(rechitscounterTID[i]);
  hDenominatorMergedFractionAssociatingTID[i]->Fill(associatingcounterTID[i]);
 }
 for(int i=0; i<6; i++){
  hMergedFractionTotalTOB[i]->Fill(mergedTOB[i]/rechitscounterTOB[i]);
  hMergedFractionAssociatingTOB[i]->Fill(mergedTOB[i]/associatingcounterTOB[i]);
  hNumeratorMergedFractionTotalTOB[i]->Fill(mergedTOB[i]);
  hNumeratorMergedFractionAssociatingTOB[i]->Fill(mergedTOB[i]);
  hDenominatorMergedFractionTotalTOB[i]->Fill(rechitscounterTOB[i]);
  hDenominatorMergedFractionAssociatingTOB[i]->Fill(associatingcounterTOB[i]);
 }
 for(int i=0; i<9; i++){
  hMergedFractionTotalTEC[i]->Fill(mergedTEC[i]/rechitscounterTEC[i]);
  hMergedFractionAssociatingTEC[i]->Fill(mergedTEC[i]/associatingcounterTEC[i]);
  hNumeratorMergedFractionTotalTEC[i]->Fill(mergedTEC[i]);
  hNumeratorMergedFractionAssociatingTEC[i]->Fill(mergedTEC[i]);
  hDenominatorMergedFractionTotalTEC[i]->Fill(rechitscounterTEC[i]);
  hDenominatorMergedFractionAssociatingTEC[i]->Fill(associatingcounterTEC[i]);
 }

 hClustersToBeSplit->Fill(clusters_to_be_split);

 ////////////////////////////////////////////////////////////////////////////////
 // PART B of the Analyzer:                                                    //
 // Only RecHits associated to the tracks are taken into account               //
 ////////////////////////////////////////////////////////////////////////////////

 int RecHitTIB[4];
 float RecHitMergedTIB[4];
 int RecHitTID[3];
 float RecHitMergedTID[3];
 int RecHitTOB[6];
 float RecHitMergedTOB[6];
 int RecHitTEC[9];
 float RecHitMergedTEC[9];
 for(int i=0; i<4; i++){ RecHitTIB[i] = 0; RecHitMergedTIB[i] = 0; }
 for(int i=0; i<3; i++){ RecHitTID[i] = 0; RecHitMergedTID[i] = 0; }
 for(int i=0; i<6; i++){ RecHitTOB[i] = 0; RecHitMergedTOB[i] = 0; }
 for(int i=0; i<9; i++){ RecHitTEC[i] = 0; RecHitMergedTEC[i] = 0; }
 int iTrack=0;

 ////////////////////////////////////////////////////////////////////////////////
 // LOOP over tracks                                                           //
  ////////////////////////////////////////////////////////////////////////////////

 int clusters_to_be_split_from_tracks = 0;

 std::vector<DetId> gIdVector;
 gIdVector.reserve(100000);
 std::vector<uint16_t> fSVector;
 fSVector.reserve(100000);

 std::vector<DetId> gIdVectorMono;
 gIdVectorMono.reserve(100000);
 std::vector<uint16_t> fSVectorMono;
 fSVectorMono.reserve(100000);

 std::vector<DetId> gIdVectorStereo;
 gIdVectorStereo.reserve(100000);
 std::vector<uint16_t> fSVectorStereo;
 fSVectorStereo.reserve(100000);

 reco::TrackCollection::const_iterator tciter;
 for ( tciter=myTracks->begin(); tciter!=myTracks->end(); tciter++) {
//    phi = tciter->momentum().phi() / math_pi*180.0;
//    eta = tciter->momentum().eta();
  hTrackPt->Fill(tciter->pt());
  iTrack++;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Loop over the recHits associated to the track in order to access corresponding cluster information
  //  (Example:  RecoTracker/FinalTrackSelectors/src/TrackerTrackHitFilter.cc
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  for ( trackingRecHit_iterator thititer = tciter->recHitsBegin(); thititer != tciter->recHitsEnd(); thititer++) {
   const TrackingRecHit* theRecHit = thititer->get(); // thititer is an iterator on edm::Ref to rechit

   const SiStripRecHit2D*        sistripsimplehit  = dynamic_cast<const SiStripRecHit2D*>(theRecHit);
   const SiStripMatchedRecHit2D* sistripmatchedhit = dynamic_cast<const SiStripMatchedRecHit2D*>(theRecHit);
//      const SiPixelRecHit* pixelmatchedhit = dynamic_cast<const SiPixelRecHit*>(theRecHit);
   DetId idActual = (theRecHit)->geographicalId();
   pair<unsigned int,int> SubDetAndLayer = FindDetLayer(idActual);
   unsigned int iSubDet = SubDetAndLayer.first;
   int iSubDetLayer = SubDetAndLayer.second;
   iSubDetLayer--;  // it is 1,2,3,4,... so I subtract 1 to use it in arrays
 //Associating
   associatedT.clear();
   associatedT = hitAssociator->associateHit(*theRecHit);
   if(iSubDet==StripSubdetector::TIB || iSubDet==StripSubdetector::TID || iSubDet==StripSubdetector::TOB || iSubDet==StripSubdetector::TEC){
     hNumberOfSimHitsPerRecHitFromTracks->Fill(associatedT.size());
   }

   /*
   if(iSubDet==StripSubdetector::TIB)cout << "TIB Layer: " << iSubDetLayer;
   if(iSubDet==StripSubdetector::TOB)cout << "TOB Layer: " << iSubDetLayer;
   if(iSubDet==StripSubdetector::TEC)cout << "TEC Layer: " << iSubDetLayer;
   if(iSubDet==StripSubdetector::TID)cout << "TID Layer: " << iSubDetLayer;
   if(iSubDet==PixelSubdetector::PixelBarrel)cout << "PIB Layer: " << iSubDetLayer;
   if(iSubDet==PixelSubdetector::PixelEndcap)cout << "PIE Layer: " << iSubDetLayer;

   if(sistripsimplehit){
     cout << "; Simple RecHit; ";
   }else if(sistripmatchedhit){
     cout << "; Matched RecHit; ";
   }else if(sistripprojectedhit){
     cout << "; Projected RecHit; ";
   }else if(sipixelhit){
     cout << "; Pixel RecHit; ";
   }else if(invalid){
     cout << "; Invalid RecHit; ";
   }else{
     cout << "; Unknown RecHit; ";
   }

   for(vector<PSimHit>::const_iterator m=associatedT.begin(); m<associatedT.end(); m++){
    cout << "Track Id : " << (*m).trackId() << "; ";
   }
   cout << endl;
   */

   //Avoiding pixel RecHits and double counting of tracker RecHits
   bool read_rechit = 0, pixel_proj_invalid_unknown_hit =0;
   if(sistripsimplehit){
    for(size_t k=0;k<gIdVector.size() && read_rechit == 0;++k){
     DetId gIdActual = sistripsimplehit->cluster()->geographicalId();
     uint16_t fSActual = sistripsimplehit->cluster()->firstStrip();
     if(gIdVector[k] == gIdActual && fSVector[k] == fSActual) read_rechit = 1;
    }
    gIdVector.push_back(sistripsimplehit->cluster()->geographicalId());
    fSVector.push_back(sistripsimplehit->cluster()->firstStrip());
   }else if(sistripmatchedhit){
    for(size_t k=0;k<gIdVectorMono.size() && read_rechit == 0;++k){
     DetId gIdActualMono = sistripmatchedhit->monoHit()->cluster()->geographicalId();
     uint16_t fSActualMono = sistripmatchedhit->monoHit()->cluster()->firstStrip();
     DetId gIdActualStereo = sistripmatchedhit->stereoHit()->cluster()->geographicalId();
     uint16_t fSActualStereo = sistripmatchedhit->stereoHit()->cluster()->firstStrip();
     if(gIdVectorMono[k] == gIdActualMono && fSVectorMono[k] == fSActualMono && gIdVectorStereo[k] == gIdActualStereo && fSVectorStereo[k] == fSActualStereo) read_rechit = 1;
    }
    gIdVectorMono.push_back(sistripmatchedhit->monoHit()->cluster()->geographicalId());
    fSVectorMono.push_back(sistripmatchedhit->monoHit()->cluster()->firstStrip());
    gIdVectorStereo.push_back(sistripmatchedhit->stereoHit()->cluster()->geographicalId());
    fSVectorStereo.push_back(sistripmatchedhit->stereoHit()->cluster()->firstStrip());
   }else{
    pixel_proj_invalid_unknown_hit = 1;
    //cout << "Pixel RecHit!" << endl;
   }

   // Plots of clusters to be split from Tracks
   if(sistripsimplehit && read_rechit == 0){
    std::vector<uint8_t> amp=sistripsimplehit->cluster()->amplitudes();
    float totalEnergy = 0, leftAmplitudeFraction = 0;
    uint8_t stripCounter = 0;
    int charge = 0;
    if(associatedT.size()==2 && amp.size()>1){
     for (size_t i=0;i<amp.size();++i) charge += amp[i];
     stripCounter = leftStripCount(amp, associatedT, totalEnergy, leftAmplitudeFraction, SplitClustersAlgos::byHits);

     if(stripCounter>0 && stripCounter<amp.size()){
      std::vector<uint16_t> tmp1, tmp2;
      for(size_t j=0;j<size_t(stripCounter);++j) tmp1.push_back(amp[j]);
      for(size_t k=stripCounter;k<amp.size();++k) tmp2.push_back(amp[k]);
      int sumx1 = 0;
      int suma1 = 0;
      size_t asize1 = tmp1.size();
      for (size_t i=0;i<asize1;++i) {
       sumx1 += (sistripsimplehit->cluster()->firstStrip()+i)*(tmp1[i]);
       suma1 += tmp1[i];
      }
      float barycenter1 = sumx1 / static_cast<float>(suma1) + 0.5;
      int sumx2 = 0;
      int suma2 = 0;
      size_t asize2 = tmp2.size();
      for (size_t i=0;i<asize2;++i) {
       sumx2 += (sistripsimplehit->cluster()->firstStrip()+stripCounter+i)*(tmp2[i]);
       suma2 += tmp2[i];
      }
      float barycenter2 = sumx2 / static_cast<float>(suma2) + 0.5;
      hCluterDeltaPositionsP2P1FromTracks->Fill(barycenter2 - barycenter1);
      hCluterDeltaPositionsP2P0FromTracks->Fill(barycenter2 - sistripsimplehit->cluster()->barycenter());
     }
     if(associatedT.size()==2 && amp.size()>1 && stripCounter>0 && stripCounter<amp.size()){
      clusters_to_be_split_from_tracks++;
      hClustersToBeSplitChargeFromTracks->Fill(charge);
      hClusterToBeSplitWidthFromTracks->Fill(amp.size());
      hClustersToBeSplitChargeVSEnergyLossTotalFromTracks->Fill(charge, totalEnergy);
     }
    }
   }else if(sistripmatchedhit && read_rechit ==0){//FIX ME: To understand if to each RecHit (Mono and Stereo) is associated a SimHit
    float totalEnergy, leftAmplitudeFraction;
    std::vector<uint8_t> ampMono=sistripmatchedhit->monoHit()->cluster()->amplitudes();
    uint8_t stripCounterMono = 0;
    int chargeMono = 0;
    std::vector<uint8_t> ampStereo=sistripmatchedhit->stereoHit()->cluster()->amplitudes();
    uint8_t stripCounterStereo = 0;
    int chargeStereo = 0;
    if(associatedT.size()==2 && (ampMono.size()>1 || ampStereo.size()>1) ){
     for (size_t i=0;i<ampMono.size();++i) chargeMono += ampMono[i];
     for (size_t i=0;i<ampStereo.size();++i) chargeStereo += ampStereo[i];
     stripCounterMono = leftStripCount(ampMono, associatedT, totalEnergy, leftAmplitudeFraction, SplitClustersAlgos::byHits);
     stripCounterStereo = leftStripCount(ampStereo, associatedT, totalEnergy, leftAmplitudeFraction, SplitClustersAlgos::byHits);

     if(stripCounterMono>0 && stripCounterMono<ampMono.size()){
      std::vector<uint16_t> tmp1, tmp2;
      for(size_t j=0;j<size_t(stripCounterMono);++j) tmp1.push_back(ampMono[j]);
      for(size_t k=stripCounterMono;k<ampMono.size();++k) tmp2.push_back(ampMono[k]);
      int sumx1 = 0;
      int suma1 = 0;
      size_t asize1 = tmp1.size();
      for (size_t i=0;i<asize1;++i) {
       sumx1 += (sistripmatchedhit->monoHit()->cluster()->firstStrip()+i)*(tmp1[i]);
       suma1 += tmp1[i];
      }
      float barycenter1 = sumx1 / static_cast<float>(suma1) + 0.5;
      int sumx2 = 0;
      int suma2 = 0;
      size_t asize2 = tmp2.size();
      for (size_t i=0;i<asize2;++i) {
       sumx2 += (sistripmatchedhit->monoHit()->cluster()->firstStrip()+stripCounterMono+i)*(tmp2[i]);
       suma2 += tmp2[i];
      }
      float barycenter2 = sumx2 / static_cast<float>(suma2) + 0.5;
      hCluterDeltaPositionsP2P1FromTracks->Fill(barycenter2 - barycenter1);
      hCluterDeltaPositionsP2P0FromTracks->Fill(barycenter2 - sistripmatchedhit->monoHit()->cluster()->barycenter());
     }
     if(stripCounterStereo>0 && stripCounterStereo<ampStereo.size()){
      std::vector<uint16_t> tmp1, tmp2;
      for(size_t j=0;j<size_t(stripCounterStereo);++j) tmp1.push_back(ampStereo[j]);
      for(size_t k=stripCounterStereo;k<ampStereo.size();++k) tmp2.push_back(ampStereo[k]);
      int sumx1 = 0;
      int suma1 = 0;
      size_t asize1 = tmp1.size();
      for (size_t i=0;i<asize1;++i) {
       sumx1 += (sistripmatchedhit->stereoHit()->cluster()->firstStrip()+i)*(tmp1[i]);
       suma1 += tmp1[i];
      }
      float barycenter1 = sumx1 / static_cast<float>(suma1) + 0.5;
      int sumx2 = 0;
      int suma2 = 0;
      size_t asize2 = tmp2.size();
      for (size_t i=0;i<asize2;++i) {
       sumx2 += (sistripmatchedhit->stereoHit()->cluster()->firstStrip()+stripCounterStereo+i)*(tmp2[i]);
       suma2 += tmp2[i];
      }
      float barycenter2 = sumx2 / static_cast<float>(suma2) + 0.5;
      hCluterDeltaPositionsP2P1FromTracks->Fill(barycenter2 - barycenter1);
      hCluterDeltaPositionsP2P0FromTracks->Fill(barycenter2 - sistripmatchedhit->stereoHit()->cluster()->barycenter());
     }
     if(associatedT.size()==2 && ampMono.size()>1 && stripCounterMono>0 && stripCounterMono<ampMono.size()){
      hClustersToBeSplitChargeFromTracks->Fill(chargeMono);
      hClusterToBeSplitWidthFromTracks->Fill(ampMono.size());
      hClustersToBeSplitChargeVSEnergyLossTotalFromTracks->Fill(chargeMono, totalEnergy);
     }
     if(associatedT.size()==2 && ampStereo.size()>1 && stripCounterStereo>0 && stripCounterStereo<ampStereo.size()){
      clusters_to_be_split_from_tracks++;
      hClustersToBeSplitChargeFromTracks->Fill(chargeStereo);
      hClusterToBeSplitWidthFromTracks->Fill(ampStereo.size());
      hClustersToBeSplitChargeVSEnergyLossTotalFromTracks->Fill(chargeStereo, totalEnergy);
     }
    }
   }else{
   }

   if(pixel_proj_invalid_unknown_hit == 0 && read_rechit == 0){
    //Plot on the number of SimTracks per RecHit
    if(!associatedT.empty()){
     int num_simTrack_associated = 0;
     unsigned int testTrackId = 0;
     for(vector<PSimHit>::const_iterator m=associatedT.begin(); m<associatedT.end(); m++){
      if(m == associatedT.begin()){ testTrackId = (*m).trackId(); num_simTrack_associated = 1; }
      if(testTrackId != (*m).trackId()){ testTrackId = (*m).trackId(); num_simTrack_associated++; }
     }
     if(associatedT.size()>1) hNumberOfSimTracksPerMergedRecHitFromTracks->Fill(num_simTrack_associated);
    }

    //Merged fraction from Tracks together with other plots about clusters
    if(iSubDet==StripSubdetector::TIB){
     for(int i=0; i<4; i++){
      if(iSubDetLayer==i){
	if(!sistripmatchedhit){
        RecHitTIB[i]+=2;
       }else{
        RecHitTIB[i]++;
       }
       if(associatedT.size()>1){
        if(!sistripmatchedhit){
         RecHitMergedTIB[i]+=2;
        }else{
         RecHitMergedTIB[i]++;
        }
       }
      }
     }
    }

    if(iSubDet==StripSubdetector::TID){
     for(int i=0; i<3; i++){
      if(iSubDetLayer==i){
       if(!sistripmatchedhit){
        RecHitTID[i]+=2;
       }else{
        RecHitTID[i]++;
       }
       if(associatedT.size()>1){
        if(!sistripmatchedhit){
         RecHitMergedTID[i]+=2;
        }else{
         RecHitMergedTID[i]++;
        }
       }
      }
     }
    }

    if(iSubDet==StripSubdetector::TOB){
     for(int i=0; i<6; i++){
      if(iSubDetLayer==i){
       if(!sistripmatchedhit){
        RecHitTOB[i]+=2;
       }else{
        RecHitTOB[i]++;
       }
       if(associatedT.size()>1){
        if(!sistripmatchedhit){
         RecHitMergedTOB[i]+=2;
        }else{
         RecHitMergedTOB[i]++;
        }
       }
      }
     }
    }

    if(iSubDet==StripSubdetector::TEC){
     for(int i=0; i<9; i++){
      if(iSubDetLayer==i){
       if(!sistripmatchedhit){
        RecHitTEC[i]+=2;
       }else{
        RecHitTEC[i]++;
       }
       if(associatedT.size()>1){
        if(!sistripmatchedhit){
         RecHitMergedTEC[i]+=2;
        }else{
         RecHitMergedTEC[i]++;
        }
       }
      }
     }
    }
   } // END if to avoid pixel and double counting hits from tracks
  } // STOP loop over trackInfos/RecHits
 } // STOP loop over tracks

 // Fill histos merged fraction for TIB layers and TID disks
 double rTIB[4];
 double rTID[3];
 double rTOB[6];
 double rTEC[9];
 for(int i=0; i<4; i++){ rTIB[i] = 0; }
 for(int i=0; i<3; i++){ rTID[i] = 0; }
 for(int i=0; i<6; i++){ rTOB[i] = 0; }
 for(int i=0; i<9; i++){ rTEC[i] = 0; }

 if(RecHitTIB[0]>0) rTIB[0]=double(RecHitMergedTIB[0])/RecHitTIB[0];
 if(RecHitTIB[1]>0) rTIB[1]=double(RecHitMergedTIB[1])/RecHitTIB[1];
 if(RecHitTIB[2]>0) rTIB[2]=double(RecHitMergedTIB[2])/RecHitTIB[2];
 if(RecHitTIB[3]>0) rTIB[3]=double(RecHitMergedTIB[3])/RecHitTIB[3];

 if(RecHitTID[0]>0) rTID[0]=double(RecHitMergedTID[0])/RecHitTID[0];
 if(RecHitTID[1]>0) rTID[1]=double(RecHitMergedTID[1])/RecHitTID[1];
 if(RecHitTID[2]>0) rTID[2]=double(RecHitMergedTID[2])/RecHitTID[2];

 if(RecHitTOB[0]>0) rTOB[0]=double(RecHitMergedTOB[0])/RecHitTOB[0];
 if(RecHitTOB[1]>0) rTOB[1]=double(RecHitMergedTOB[1])/RecHitTOB[1];
 if(RecHitTOB[2]>0) rTOB[2]=double(RecHitMergedTOB[2])/RecHitTOB[2];
 if(RecHitTOB[3]>0) rTOB[3]=double(RecHitMergedTOB[3])/RecHitTOB[3];
 if(RecHitTOB[4]>0) rTOB[4]=double(RecHitMergedTOB[4])/RecHitTOB[4];
 if(RecHitTOB[5]>0) rTOB[5]=double(RecHitMergedTOB[5])/RecHitTOB[5];

 if(RecHitTEC[0]>0) rTEC[0]=double(RecHitMergedTEC[0])/RecHitTEC[0];
 if(RecHitTEC[1]>0) rTEC[1]=double(RecHitMergedTEC[1])/RecHitTEC[1];
 if(RecHitTEC[2]>0) rTEC[2]=double(RecHitMergedTEC[2])/RecHitTEC[2];
 if(RecHitTEC[3]>0) rTEC[3]=double(RecHitMergedTEC[3])/RecHitTEC[3];
 if(RecHitTEC[4]>0) rTEC[4]=double(RecHitMergedTEC[4])/RecHitTEC[4];
 if(RecHitTEC[5]>0) rTEC[5]=double(RecHitMergedTEC[5])/RecHitTEC[5];
 if(RecHitTEC[6]>0) rTEC[6]=double(RecHitMergedTEC[6])/RecHitTEC[6];
 if(RecHitTEC[7]>0) rTEC[7]=double(RecHitMergedTEC[7])/RecHitTEC[7];
 if(RecHitTEC[8]>0) rTEC[8]=double(RecHitMergedTEC[8])/RecHitTEC[8];

 hMergedFractionFromTracksTIB[0]->Fill(rTIB[0]);
 hMergedFractionFromTracksTIB[1]->Fill(rTIB[1]);
 hMergedFractionFromTracksTIB[2]->Fill(rTIB[2]);
 hMergedFractionFromTracksTIB[3]->Fill(rTIB[3]);

 hMergedFractionFromTracksTID[0]->Fill(rTID[0]);
 hMergedFractionFromTracksTID[1]->Fill(rTID[1]);
 hMergedFractionFromTracksTID[2]->Fill(rTID[2]);

 hMergedFractionFromTracksTOB[0]->Fill(rTOB[0]);
 hMergedFractionFromTracksTOB[1]->Fill(rTOB[1]);
 hMergedFractionFromTracksTOB[2]->Fill(rTOB[2]);
 hMergedFractionFromTracksTOB[3]->Fill(rTOB[3]);
 hMergedFractionFromTracksTOB[4]->Fill(rTOB[4]);
 hMergedFractionFromTracksTOB[5]->Fill(rTOB[5]);

 hMergedFractionFromTracksTEC[0]->Fill(rTEC[0]);
 hMergedFractionFromTracksTEC[1]->Fill(rTEC[1]);
 hMergedFractionFromTracksTEC[2]->Fill(rTEC[2]);
 hMergedFractionFromTracksTEC[3]->Fill(rTEC[3]);
 hMergedFractionFromTracksTEC[4]->Fill(rTEC[4]);
 hMergedFractionFromTracksTEC[5]->Fill(rTEC[5]);
 hMergedFractionFromTracksTEC[6]->Fill(rTEC[6]);
 hMergedFractionFromTracksTEC[7]->Fill(rTEC[7]);
 hMergedFractionFromTracksTEC[8]->Fill(rTEC[8]);


 hNumeratorMergedFractionFromTracksTIB[0]->Fill(RecHitMergedTIB[0]);
 hNumeratorMergedFractionFromTracksTIB[1]->Fill(RecHitMergedTIB[1]);
 hNumeratorMergedFractionFromTracksTIB[2]->Fill(RecHitMergedTIB[2]);
 hNumeratorMergedFractionFromTracksTIB[3]->Fill(RecHitMergedTIB[3]);

 hNumeratorMergedFractionFromTracksTID[0]->Fill(RecHitMergedTID[0]);
 hNumeratorMergedFractionFromTracksTID[1]->Fill(RecHitMergedTID[1]);
 hNumeratorMergedFractionFromTracksTID[2]->Fill(RecHitMergedTID[2]);

 hNumeratorMergedFractionFromTracksTOB[0]->Fill(RecHitMergedTOB[0]);
 hNumeratorMergedFractionFromTracksTOB[1]->Fill(RecHitMergedTOB[1]);
 hNumeratorMergedFractionFromTracksTOB[2]->Fill(RecHitMergedTOB[2]);
 hNumeratorMergedFractionFromTracksTOB[3]->Fill(RecHitMergedTOB[3]);
 hNumeratorMergedFractionFromTracksTOB[4]->Fill(RecHitMergedTOB[4]);
 hNumeratorMergedFractionFromTracksTOB[5]->Fill(RecHitMergedTOB[5]);

 hNumeratorMergedFractionFromTracksTEC[0]->Fill(RecHitMergedTEC[0]);
 hNumeratorMergedFractionFromTracksTEC[1]->Fill(RecHitMergedTEC[1]);
 hNumeratorMergedFractionFromTracksTEC[2]->Fill(RecHitMergedTEC[2]);
 hNumeratorMergedFractionFromTracksTEC[3]->Fill(RecHitMergedTEC[3]);
 hNumeratorMergedFractionFromTracksTEC[4]->Fill(RecHitMergedTEC[4]);
 hNumeratorMergedFractionFromTracksTEC[5]->Fill(RecHitMergedTEC[5]);
 hNumeratorMergedFractionFromTracksTEC[6]->Fill(RecHitMergedTEC[6]);
 hNumeratorMergedFractionFromTracksTEC[7]->Fill(RecHitMergedTEC[7]);
 hNumeratorMergedFractionFromTracksTEC[8]->Fill(RecHitMergedTEC[8]);


 hDenominatorMergedFractionFromTracksTIB[0]->Fill(RecHitTIB[0]);
 hDenominatorMergedFractionFromTracksTIB[1]->Fill(RecHitTIB[1]);
 hDenominatorMergedFractionFromTracksTIB[2]->Fill(RecHitTIB[2]);
 hDenominatorMergedFractionFromTracksTIB[3]->Fill(RecHitTIB[3]);

 hDenominatorMergedFractionFromTracksTID[0]->Fill(RecHitTID[0]);
 hDenominatorMergedFractionFromTracksTID[1]->Fill(RecHitTID[1]);
 hDenominatorMergedFractionFromTracksTID[2]->Fill(RecHitTID[2]);

 hDenominatorMergedFractionFromTracksTOB[0]->Fill(RecHitTOB[0]);
 hDenominatorMergedFractionFromTracksTOB[1]->Fill(RecHitTOB[1]);
 hDenominatorMergedFractionFromTracksTOB[2]->Fill(RecHitTOB[2]);
 hDenominatorMergedFractionFromTracksTOB[3]->Fill(RecHitTOB[3]);
 hDenominatorMergedFractionFromTracksTOB[4]->Fill(RecHitTOB[4]);
 hDenominatorMergedFractionFromTracksTOB[5]->Fill(RecHitTOB[5]);

 hDenominatorMergedFractionFromTracksTEC[0]->Fill(RecHitTEC[0]);
 hDenominatorMergedFractionFromTracksTEC[1]->Fill(RecHitTEC[1]);
 hDenominatorMergedFractionFromTracksTEC[2]->Fill(RecHitTEC[2]);
 hDenominatorMergedFractionFromTracksTEC[3]->Fill(RecHitTEC[3]);
 hDenominatorMergedFractionFromTracksTEC[4]->Fill(RecHitTEC[4]);
 hDenominatorMergedFractionFromTracksTEC[5]->Fill(RecHitTEC[5]);
 hDenominatorMergedFractionFromTracksTEC[6]->Fill(RecHitTEC[6]);
 hDenominatorMergedFractionFromTracksTEC[7]->Fill(RecHitTEC[7]);
 hDenominatorMergedFractionFromTracksTEC[8]->Fill(RecHitTEC[8]);
 hClustersToBeSplitFromTracks->Fill(clusters_to_be_split_from_tracks);

 ////////////////////////////////////////
 // PART C of the Analyzer:            //
 // Loop on SiPixelRecHits             //
 ////////////////////////////////////////

 /*
 const SiPixelRecHitCollection::DataContainer& pixelrechitsColl = pixelrechits->data();
 for (SiPixelRecHitCollection::DataContainer::const_iterator pixelrechitsIter = pixelrechitsColl.begin();
  pixelrechitsIter != pixelrechitsColl.end(); ++pixelrechitsIter) {
  SiPixelRecHit const pixrechit=*pixelrechitsIter;
  associatedP.clear();
  associatedP = hitAssociator->associateHit(*pixelrechitsIter);
  //cout << " # of PSimHit associated to a SiPixelRecHit: " << associatedP.size() << endl;
 }
 */

 //////////////////////////////////////////////////////
 // PART D of the Analyzer:                          //
 // Loop on the SimHits in the Tracker               //
 //////////////////////////////////////////////////////

 int SimHits=0;
 for (std::vector<PSimHit>::iterator isim = theTrackerHits.begin(); isim != theTrackerHits.end(); ++isim){
  DetId tkId(isim->detUnitId());
  if(tkId.subdetId()== StripSubdetector::TIB){
   TIBDetId tibId(tkId);
   SimHits++;
  }else if(tkId.subdetId()==StripSubdetector::TID){
   TIDDetId tidId(tkId);
   SimHits++;
  }else if(tkId.subdetId()==StripSubdetector::TOB){
   TOBDetId tobId(tkId);
   SimHits++;
  }else if(tkId.subdetId()==StripSubdetector::TEC){
   TECDetId tecId(tkId);
   SimHits++;
  }
 }




 delete hitAssociator;
}


//bool ClusterAnalyzer::fillClusterHistos(SiStripClusterInfo* clusterInfo_, const uint32_t& detId_,
//						   TString flag_, const LocalVector clusterLocalVector_) {
// if (clusterInfo_==0) return false;
// const edm::ParameterSet theClusterConditions = conf_.getParameter<edm::ParameterSet>("ClusterConditions");
// if ( theClusterConditions.getParameter<bool>("On")
//  &&
//  (
//  double(clusterInfo_->charge())/ clusterInfo_->noise() < theClusterConditions.getParameter<double>("minStoN")
//   ||
//   double(clusterInfo_->charge())/clusterInfo_->noise() > theClusterConditions.getParameter<double>("maxStoN") 
//  ||
//  double(clusterInfo_->width()) < theClusterConditions.getParameter<double>("minWidth") 
//  ||
//   double(clusterInfo_->width()) > theClusterConditions.getParameter<double>("maxWidth") 
//   )
// )
// return false;
    
// totalNumberOfClustersOnTrack++;
// hClusterPosition                      ->Fill( static_cast<float>( clusterInfo_->position())                      ,1.);
// hClusterWidth                         ->Fill( static_cast<float>( clusterInfo_->width())                         ,1.);
// hClusterCharge                        ->Fill( static_cast<float>( clusterInfo_->charge() )                       ,1.);
// hClusterMaxPosition                   ->Fill( static_cast<float>( clusterInfo_->maxPos())                        ,1.);
// hClusterMaxCharge                     ->Fill( static_cast<float>( clusterInfo_->maxCharge() )                    ,1.);
// hClusterChargeLeft                    ->Fill( static_cast<float>( clusterInfo_->chargeL() )                      ,1.);
// hClusterChargeRight                   ->Fill( static_cast<float>( clusterInfo_->chargeR() )                      ,1.);
// hClusterNoise                         ->Fill( static_cast<float>( clusterInfo_->noise() )                        ,1.);
// return true;
//} 


// void ClusterAnalyzer::getClusterInfoFromRecHit(const SiStripRecHit2D* trackerRecHit_, 
// 					          LocalVector      clusterLocalVector_,
// 					          reco::TrackRef             trackRef_,
// 					          const edm::EventSetup&            es){
  
//  // method that allows to get the cluster associated to the recHit and 
//  // that adds/stores the pointer to it in a vector of SiStripCluster pointers for each track in the event
//  // *** check hit validity
  
//  if(!trackerRecHit_->isValid()){
//   return;  
//  }
//  const uint32_t& recHitDetId = trackerRecHit_->geographicalId().rawId();
//  if (find(theModulesToBeExcluded.begin(),theModulesToBeExcluded.end(),recHitDetId)!=theModulesToBeExcluded.end()){
//   return;
//  }
//  // now get the cluster from RecHit and produce clusterInfo on the fly
//  if ( trackerRecHit_ != NULL ){
//    //  SiStripClusterInfo clusterInfo = SiStripClusterInfo(*(trackerRecHit_->cluster()));
//   // additional cross check for safety although track should have been excluded before getting here
//   const edm::ParameterSet theTrackThresholds = conf_.getParameter<edm::ParameterSet>("TrackThresholds");
//   if( trackRef_->normalizedChi2() < theTrackThresholds.getParameter<double>("maxChi2") && 
//       trackRef_->recHitsSize() > theTrackThresholds.getParameter<double>("minRecHit") ){
//     //   if ( fillClusterHistos( & clusterInfo, recHitDetId ,"_onTrack", clusterLocalVector_ ) ) {
//     //countOn++;
//     //} // fill histos for clusters satisfying cluster conditions
//   } //additional cross check  
//  } // non-NULL recHit  
// } 


//
//This method returns the iSubDet
//
pair<unsigned int,int> ClusterAnalyzer::FindDetLayer(DetId& id)
{
 unsigned int iSubDet = StripSubdetector(id).subdetId();
 int LayerToReturn=0;
 if (iSubDet == PixelSubdetector::PixelBarrel){
  PXBDetId* pibId = new PXBDetId(id);
  int layer = pibId->layer();
  LayerToReturn=layer;
 }
 if (iSubDet == PixelSubdetector::PixelEndcap){
  PXFDetId* pieId = new PXFDetId(id);
  int disk = pieId->disk();
  LayerToReturn=disk;
 }
 if (iSubDet == StripSubdetector::TIB){
  TIBDetId* tibId = new TIBDetId(id);
  int layer = tibId->layer();
  LayerToReturn=layer;
  hTIBLayer->Fill(layer);
 }
 if (iSubDet == StripSubdetector::TID){
  TIDDetId* tidId = new TIDDetId(id);
  int side = tidId->side();
  hTIDSide->Fill(side);
  int wheel = tidId->wheel();
  LayerToReturn=wheel;
  hTIDWheel->Fill(wheel);
  int ring = tidId->ring();
  hTIDRing->Fill(ring);
 }
 if (iSubDet == StripSubdetector::TOB){
  TOBDetId* tibId = new TOBDetId(id);
  int layer = tibId->layer();
  LayerToReturn=layer;
  hTOBLayer->Fill(layer);
 }
 if (iSubDet == StripSubdetector::TEC){
  TECDetId* tidId = new TECDetId(id);
  int side = tidId->side();
  hTECSide->Fill(side);
  int wheel = tidId->wheel();
  LayerToReturn=wheel;
  hTECWheel->Fill(wheel);
  int ring = tidId->ring();
  hTECRing->Fill(ring);
 }
 pair<unsigned int,int> p(iSubDet,LayerToReturn);
 return p; 
}

DEFINE_SEAL_MODULE();
DEFINE_FWK_MODULE (ClusterAnalyzer);
