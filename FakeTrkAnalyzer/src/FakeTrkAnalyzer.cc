// -*- C++ -*-
//
// Package:    FakeTrkAnalyzer
// Class:      FakeTrkAnalyzer
// 
/**\class FakeTrkAnalyzer FakeTrkAnalyzer.cc edwenger/FakeTrkAnalyzer/src/FakeTrkAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Edward Allen WENGER
//         Created:  Fri Nov  6 10:15:22 CET 2009
// $Id: FakeTrkAnalyzer.cc,v 1.1 2009/11/10 11:20:57 edwenger Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByHits.h"

#include "DataFormats/JetReco/interface/CaloJet.h"

#include "DataFormats/Math/interface/deltaR.h"


// root include files
#include "TFile.h"
#include "TNtuple.h"
#include "TH1.h"

using namespace std;
using namespace edm;

//
// class declaration
//

class FakeTrkAnalyzer : public edm::EDAnalyzer {
public:
  explicit FakeTrkAnalyzer(const edm::ParameterSet&);
  ~FakeTrkAnalyzer();
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  // ----------member data ---------------------------
  TNtuple *nt;
  Service<TFileService> f;

  InputTag trackTags_; //used to select what tracks to read from configuration file
  InputTag jetTags_;
  InputTag muonTags_;
  double thePtMin_;         //the minimum pt of tracks for filling jet and muon isolation info
  
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FakeTrkAnalyzer::FakeTrkAnalyzer(const edm::ParameterSet& iConfig)
  :
  trackTags_(iConfig.getUntrackedParameter<edm::InputTag>("tracks")),
  jetTags_(iConfig.getUntrackedParameter<edm::InputTag>("jets")),
  muonTags_(iConfig.getUntrackedParameter<edm::InputTag>("muons")),
  thePtMin_(iConfig.getUntrackedParameter<double>("ptMin"))
  
{
  //now do what ever initialization is needed
  
}


FakeTrkAnalyzer::~FakeTrkAnalyzer()
{
  
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
FakeTrkAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;
  
  // get track collection
  Handle<View<Track> > trackCollectionH;
  iEvent.getByLabel(trackTags_,trackCollectionH);
  const View<Track>  tC = *(trackCollectionH.product()); 

  // get association map
  Handle<reco::RecoToSimCollection > rectosimCollection;
  iEvent.getByLabel("trackingParticleRecoTrackAsssociation", rectosimCollection);
  const RecoToSimCollection p = *(rectosimCollection.product());

  // loop over tracks
  for(View<Track>::size_type i=0; i<tC.size(); ++i) {
    RefToBase<Track> track(trackCollectionH, i);
    
    // select high pt tracks
    double pt_trk = track->pt();
    if(pt_trk < thePtMin_)
      continue;
    cout << "The track pt = " << pt_trk << endl;
    
    // determine whether track can be matched to tracking truth
    bool isFake = false;
    std::vector<std::pair<TrackingParticleRef, double> > tp;
    if(p.find(track) != p.end()){
      tp = p[track];
      if (tp.size()!=0) {
	TrackingParticleRef tpr = tp.begin()->first;
	double associationQuality = tp.begin()->second;
	cout << "associated with quality = " << associationQuality << endl;
      }
      else 
	isFake=true;
    }
    
    // get calo jet collection
    Handle<CaloJetCollection> jets;
    iEvent.getByLabel( jetTags_, jets );
    
    double mindr = 999.9;
    double et_nj = 0.0;
    int counter = 0;
    
    for(CaloJetCollection::const_iterator itJet = jets->begin();
	itJet != jets->end();
	++itJet) {
      
      double et = itJet->et();
      double dr = deltaR(track->eta(),track->phi(),itJet->eta(),itJet->phi());
      if(et > pt_trk*0.7 && dr < mindr) {
	mindr = dr;
	et_nj = et;
      }
      
      if(counter<2)
	cout << "jet " << counter << ": \t et:" << et << endl;
      
      counter++;
      
    } // end loop over jets
    
    cout << "The nearest jet with et > " << pt_trk*0.7 << "GeV: \n\t dr= " << mindr << "\t et=" << et_nj << endl;
    
    nt->Fill(pt_trk,mindr,et_nj,isFake);
    
    
  } // end loop over tracks
  
}


// ------------ method called once each job just before starting event loop  ------------
void 
FakeTrkAnalyzer::beginJob()
{

  nt = f->make<TNtuple>("nt","Fake Track Testing","pt:dr:et:fake");


}

// ------------ method called once each job just after ending the event loop  ------------
void 
FakeTrkAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(FakeTrkAnalyzer);
