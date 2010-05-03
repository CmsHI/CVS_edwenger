// -*- C++ -*-
//
// Package:    TrkEffAnalyzer
// Class:      TrkEffAnalyzer
// 
/**\class TrkEffAnalyzer TrkEffAnalyzer.cc edwenger/TrkEffAnalyzer/src/TrkEffAnalyzer.cc

 Description: Generate ntuples for tracking efficiency and fake rate studies 
              from tracking particles, tracks, and association maps 

 Implementation: Simplified code based on MultiTrackValidator.cc

*/
//
// Original Author:  Edward Wenger
//         Created:  Thu Apr 29 14:31:47 CEST 2010
// $Id: TrkEffAnalyzer.cc,v 1.1 2010/05/03 08:55:02 edwenger Exp $
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
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "DataFormats/RecoCandidate/interface/TrackAssociation.h"

#include "TFile.h"  
#include "TNtuple.h"
#include "TH1.h"

//
// class declaration
//

class TrkEffAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TrkEffAnalyzer(const edm::ParameterSet&);
      ~TrkEffAnalyzer();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
      edm::InputTag trackTags_; 
      edm::InputTag label_tp_effic_;
      edm::InputTag label_tp_fake_;
      edm::InputTag associatorMap_;

      TNtuple *ntSim;
      TNtuple *ntReco;
      edm::Service<TFileService> f;
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
TrkEffAnalyzer::TrkEffAnalyzer(const edm::ParameterSet& iConfig)
:
  trackTags_(iConfig.getUntrackedParameter<edm::InputTag>("tracks")),
  label_tp_effic_(iConfig.getUntrackedParameter<edm::InputTag>("label_tp_effic")),
  label_tp_fake_(iConfig.getUntrackedParameter<edm::InputTag>("label_tp_fake")),
  associatorMap_(iConfig.getUntrackedParameter<edm::InputTag>("associatormap"))

{
   //now do whatever initialization is needed

}


TrkEffAnalyzer::~TrkEffAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TrkEffAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // association maps

  edm::Handle<reco::SimToRecoCollection > simtorecoCollectionH;
  iEvent.getByLabel(associatorMap_,simtorecoCollectionH);
  reco::SimToRecoCollection simRecColl= *(simtorecoCollectionH.product()); 
  
  edm::Handle<reco::RecoToSimCollection > recotosimCollectionH;
  iEvent.getByLabel(associatorMap_,recotosimCollectionH);
  reco::RecoToSimCollection recSimColl= *(recotosimCollectionH.product()); 

  // sim track collections

  edm::Handle<TrackingParticleCollection>  TPCollectionHeff ;
  iEvent.getByLabel(label_tp_effic_,TPCollectionHeff);
  const TrackingParticleCollection tPCeff = *(TPCollectionHeff.product());
  
  edm::Handle<TrackingParticleCollection>  TPCollectionHfake ;
  iEvent.getByLabel(label_tp_fake_,TPCollectionHfake);
  const TrackingParticleCollection tPCfake = *(TPCollectionHfake.product());

  // reco track collection

  edm::Handle<edm::View<reco::Track> >  trackCollection;
  iEvent.getByLabel(trackTags_,trackCollection);


  // SIM loop

  for(TrackingParticleCollection::size_type i=0; i<tPCeff.size(); i++) {

    TrackingParticleRef tpr(TPCollectionHeff, i);
    TrackingParticle* tp=const_cast<TrackingParticle*>(tpr.get());

    double pt=0.0, eta=0.0, phi=0.0, tip=0.0, lip=0.0;
    int pdgId=0, nsimhits=0, status=0, charge=0;
    double ptr=0.0, etar=0.0, phir=0.0, d0=0.0, dz=0.0, d0err=0.0, dzerr=0.0, pterr=0.0;
    int nrechits=0;

    pt=tp->pt(); eta=tp->eta(); phi=tp->phi(); charge=tp->charge();
    tip=sqrt(tp->vertex().perp2()); lip=tp->vertex().z();
    pdgId=tp->pdgId(); nsimhits=tp->matchedHit(); status=tp->status();

    if(status<0 || charge==0) continue; //only charged primaries

    std::cout << "primary simtrack pt = " << pt 
	      << " eta = " << eta
	      << " phi = " << phi
	      << " hits = " << nsimhits
	      << " pdgId = " << pdgId
	      << " status = " << status
	      << std::endl;

    const reco::Track* matchedTrackPointer=0;
    std::vector<std::pair<edm::RefToBase<reco::Track>, double> > rt;
    if(simRecColl.find(tpr) != simRecColl.end()){
      rt = (std::vector<std::pair<edm::RefToBase<reco::Track>, double> >) simRecColl[tpr];
      
      if (rt.size()==0) continue;
      matchedTrackPointer = rt.begin()->first.get();
      std::cout << "TrackingParticle #" << i 
		<< " with pt=" << sqrt(tp->momentum().perp2()) 
		<< " associated with quality:" << rt.begin()->second << std::endl;

      ptr=matchedTrackPointer->pt(); 
      etar=matchedTrackPointer->eta(); 
      phir=matchedTrackPointer->phi();      
      
    }
  
    ntSim->Fill(pt,eta,phi,ptr,etar,phir);
    
  }
  

  // RECO loop

  for(edm::View<reco::Track>::size_type i=0; i<trackCollection->size(); ++i){
    edm::RefToBase<reco::Track> track(trackCollection, i);

    double pt=0.0, eta=0.0, phi=0.0;
    double ptr=0.0, etar=0.0, phir=0.0;

    //std::cout << "reco track pt = " << track->pt() << std::endl;
    ptr=track->pt(); etar=track->eta(); phir=track->phi();
 
    std::vector<std::pair<TrackingParticleRef, double> > tp;
    if(recSimColl.find(track) != recSimColl.end()){
      tp = recSimColl[track];
      if (tp.size()==0) continue;

      //std::cout << "reco::Track #" << i << " with pt=" << track->pt() 
      // 	  << " associated with quality:" << tp.begin()->second << std::endl;
      
      TrackingParticleRef tpr = tp.begin()->first;
      pt=tpr->pt(); eta=tpr->eta(); phi=tpr->phi();
      
    }
    
    ntReco->Fill(ptr,etar,phir,pt,eta,phi);

  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
TrkEffAnalyzer::beginJob()
{
  ntSim = f->make<TNtuple>("ntSim","SimToReco","pt:eta:phi:ptr:etar:phir");
  ntReco = f->make<TNtuple>("ntReco","RecoToSim","ptr:etar:phir:pt:eta:phi");
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TrkEffAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrkEffAnalyzer);
