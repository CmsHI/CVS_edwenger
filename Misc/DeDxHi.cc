// -*- C++ -*-
//
// Package:    DeDxHi
// Class:      DeDxHi
// 
/**\class DeDxHi DeDxHi.cc dedxhi/DeDxHi/src/DeDxHi.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Caio Laganá
//         Created:  Wed Oct 27 11:47:14 BRST 2010
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/ProjectedSiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit1D.h"

#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/TrackReco/interface/DeDxData.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include <TH2.h>
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/SiStripDetId/interface/SiStripDetId.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// PID tracks
typedef struct
{
  int charge;
  float eta;
  float p;
  float pt;
  float phi;
  float dz;
  float d0;
  float pterr;
  float d0err;
  float dzerr;
  float chi2;
  int hit; 
  float dedx;
} PidTrack;

//
// class declaration
//

class DeDxHi : public edm::EDAnalyzer {
   public:
      explicit DeDxHi(const edm::ParameterSet&);
      ~DeDxHi(){};


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob(){} ;

      // ----------member data ---------------------------

  edm::InputTag theTrackCollection;
  edm::InputTag theTrackDeDxMap;

  edm::Service<TFileService> f;

  //dedx histograms
  TH2D  *dedx_selected;
  TTree *track_tree;

  PidTrack pidTrackValues;


};

//
// constructors and destructor
//
DeDxHi::DeDxHi(const edm::ParameterSet& iConfig) : 
  theTrackCollection(iConfig.getParameter<edm::InputTag>("TrackCollection")),
  theTrackDeDxMap(iConfig.getParameter<edm::InputTag>("TrackDeDxMap"))

{

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
DeDxHi::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   /*
   * Track analysis
   */

   //=================================================================
   //get tracks from the event
   //=================================================================
   edm::Handle<reco::TrackCollection> selectedTrackCollectionHandle;
   iEvent.getByLabel(theTrackCollection, selectedTrackCollectionHandle);
   reco::TrackCollection selectedTrackCollection = *selectedTrackCollectionHandle.product();

   //=================================================================
   //get dEdx information for tracks
   //=================================================================
   Handle<edm::ValueMap<reco::DeDxData> > dEdxSelectedTrackHandle;
   iEvent.getByLabel(theTrackDeDxMap, dEdxSelectedTrackHandle);
   const edm::ValueMap<reco::DeDxData> dEdxSelectedTrack = *dEdxSelectedTrackHandle.product();


   //loop over the selected tracks of the event
   for(unsigned int i = 0; i < selectedTrackCollection.size(); i++)
   {
     //get ith track from the event
     reco::TrackRef tr     = reco::TrackRef( selectedTrackCollectionHandle, i );
     
      //only selected tracks
     dedx_selected->Fill(tr->p(), dEdxSelectedTrack[tr].dEdx());

     // fill tree
     PidTrack pid;
     pid.charge = tr->charge();
     pid.eta = tr->eta();
     pid.p = tr->p();
     pid.pt = tr->pt();
     pid.phi = tr->phi();
     pid.dz = tr->dz();
     pid.d0 = tr->dxy();
     pid.pterr = tr->ptError();
     pid.d0err = tr->d0Error();
     pid.dzerr = tr->dzError();
     pid.chi2 = tr->normalizedChi2();
     pid.hit = tr->numberOfValidHits();
     pid.dedx = dEdxSelectedTrack[tr].dEdx();

     pidTrackValues = pid;
     track_tree->Fill();
     
   }
}


// ------------ method called once each job just before starting event loop  ------------
void 
DeDxHi::beginJob()
{
   //dedx histograms
   dedx_selected = f->make<TH2D>("dedx_selected", ";p (GeV/c); dE/dx (MeV/cm)", 600, 0, 3, 600, 0, 45);

   //track tree
   track_tree = f->make<TTree>("track_tree","track_tree");
   TString leafStr = "charge/I:eta/F:p/F:pt/F:phi/F:dz/F:d0/F:pterr/F:d0err/F:dzerr/F:chi2/F:hit/I:dedx/F";
   track_tree->Branch("pidTrackValues", &pidTrackValues, leafStr.Data());

}


//define this as a plug-in
DEFINE_FWK_MODULE(DeDxHi);
