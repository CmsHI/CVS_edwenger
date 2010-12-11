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

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/SiStripDetId/interface/SiStripDetId.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"

//
// class declaration
//

class DeDxHi : public edm::EDAnalyzer {
   public:
      explicit DeDxHi(const edm::ParameterSet&);
      ~DeDxHi();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

  edm::InputTag theTrackCollection;
  edm::InputTag thePxlTrkCollection;
  edm::InputTag theTrackDeDxMap;
  edm::InputTag thePxlTrkDeDxMap;

};

//
// constants, enums and typedefs
//

//dedx histograms
TH2F  *dedx_merged;
TH2F  *dedx_pixel;
TH2F  *dedx_selected;

//other declarations
TFile *file;

//
// static data member definitions
//

//
// constructors and destructor
//
DeDxHi::DeDxHi(const edm::ParameterSet& iConfig) : 
  theTrackCollection(iConfig.getParameter<edm::InputTag>("TrackCollection")),
  thePxlTrkCollection(iConfig.getParameter<edm::InputTag>("PxlTrkCollection")),
  theTrackDeDxMap(iConfig.getParameter<edm::InputTag>("TrackDeDxMap")),
  thePxlTrkDeDxMap(iConfig.getParameter<edm::InputTag>("PxlTrkDeDxMap"))

{
   //now do what ever initialization is needed
   file = new TFile("dEdx_HI_output.root", "recreate");
}


DeDxHi::~DeDxHi()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
   file->Close();
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
   * Track analisys
   */

   //=================================================================
   //get Low pT pixel tracks from the event
   //=================================================================
   edm::Handle<reco::TrackCollection> pixelTrackCollectionHandle;
   iEvent.getByLabel(thePxlTrkCollection, pixelTrackCollectionHandle);
   reco::TrackCollection pixelTrackCollection = *pixelTrackCollectionHandle.product();

   //=================================================================
   //get dEdx information for low pT pixel tracks
   //=================================================================
   Handle<edm::ValueMap<reco::DeDxData> > dEdxPixelTrackHandle;
   iEvent.getByLabel(thePxlTrkDeDxMap, dEdxPixelTrackHandle);
   const edm::ValueMap<reco::DeDxData> dEdxPixelTrack = *dEdxPixelTrackHandle.product();

   //=================================================================
   //get HI selected tracks from the event
   //=================================================================
   edm::Handle<reco::TrackCollection> selectedTrackCollectionHandle;
   iEvent.getByLabel(theTrackCollection, selectedTrackCollectionHandle);
   reco::TrackCollection selectedTrackCollection = *selectedTrackCollectionHandle.product();

   //=================================================================
   //get dEdx information for HI selected tracks
   //=================================================================
   Handle<edm::ValueMap<reco::DeDxData> > dEdxSelectedTrackHandle;
   iEvent.getByLabel(theTrackDeDxMap, dEdxSelectedTrackHandle);
   const edm::ValueMap<reco::DeDxData> dEdxSelectedTrack = *dEdxSelectedTrackHandle.product();

   //loop over the pixel tracks of the event
   for(unsigned int i=0; i<pixelTrackCollection.size(); i++)
   {
      //get ith track from the event
      reco::TrackRef pixelTrack     = reco::TrackRef( pixelTrackCollectionHandle, i );

      //fill the (dEdx,p) merged (pixel-selected) histogram
      if (pixelTrack->p() < .9)
         dedx_merged->Fill(pixelTrack->p(), dEdxPixelTrack[pixelTrack].dEdx());

     //only pixel tracks
      dedx_pixel->Fill(pixelTrack->p(), dEdxPixelTrack[pixelTrack].dEdx());
   }

   //loop over the selected tracks of the event
   for(unsigned int i = 0; i < selectedTrackCollection.size(); i++)
   {
      //get ith track from the event
      reco::TrackRef selectedTrack     = reco::TrackRef( selectedTrackCollectionHandle, i );

      //fill the (dEdx,p) merged (pixel-selected) histogram
      if (selectedTrack->p() > .9)
         dedx_merged->Fill(selectedTrack->p(), dEdxSelectedTrack[selectedTrack].dEdx());

      //only selected tracks
      dedx_selected->Fill(selectedTrack->p(), dEdxSelectedTrack[selectedTrack].dEdx());

   }
}


// ------------ method called once each job just before starting event loop  ------------
void 
DeDxHi::beginJob()
{
   //dedx histograms
   dedx_merged   = new TH2F("dedx vs p merged", "dedx vs p merged", 300, 0, 3, 300, 0, 45);
   dedx_pixel    = new TH2F("dedx vs p pixel tracks", "dedx vs p pixel tracks", 300, 0, 3, 300, 0, 45);
   dedx_selected = new TH2F("dedx vs p selected tracks", "dedx vs p selected tracks", 300, 0, 3, 300, 0, 45);
}
// ------------ method called once each job just after ending the event loop  ------------
void 
DeDxHi::endJob()
{
   file->cd();

   //dedx histograms
   dedx_merged->Write();
   dedx_pixel->Write();
   dedx_selected->Write();
}

//define this as a plug-in
DEFINE_FWK_MODULE(DeDxHi);
