//
// Original Author:  Edward Wenger
//         Created:  Thu Apr 29 14:31:47 CEST 2010
// $Id: TrkEffAnalyzer.cc,v 1.4 2010/05/04 08:36:24 edwenger Exp $
//

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "DataFormats/RecoCandidate/interface/TrackAssociation.h"
#include "edwenger/TrkEffAnalyzer/interface/TrkEffAnalyzer.h"

//#define DEBUG

TrkEffAnalyzer::TrkEffAnalyzer(const edm::ParameterSet& iConfig)
:
  trackTags_(iConfig.getUntrackedParameter<edm::InputTag>("tracks")),
  label_tp_effic_(iConfig.getUntrackedParameter<edm::InputTag>("label_tp_effic")),
  label_tp_fake_(iConfig.getUntrackedParameter<edm::InputTag>("label_tp_fake")),
  associatorMap_(iConfig.getUntrackedParameter<edm::InputTag>("associatormap")),
  vtxTags_(iConfig.getUntrackedParameter<edm::InputTag>("vertices"))

{

  histograms = new TrkEffHistograms(iConfig);

}


// ------------ method called for each event  ------------
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

  // reco track and vertex collections

  edm::Handle<edm::View<reco::Track> >  trackCollection;
  iEvent.getByLabel(trackTags_,trackCollection);

  edm::Handle<reco::VertexCollection> vertexCollection;
  iEvent.getByLabel(vtxTags_,vertexCollection);


  // SIM loop

  for(TrackingParticleCollection::size_type i=0; i<tPCeff.size(); i++) {

    TrackingParticleRef tpr(TPCollectionHeff, i);
    TrackingParticle* tp=const_cast<TrackingParticle*>(tpr.get());

    if(tp->status() < 0 || tp->charge()==0) continue; //only charged primaries

    std::vector<std::pair<edm::RefToBase<reco::Track>, double> > rt;
    const reco::Track* mtr=0;
    size_t nrec=0;

    if(simRecColl.find(tpr) != simRecColl.end()){
      rt = (std::vector<std::pair<edm::RefToBase<reco::Track>, double> >) simRecColl[tpr];
      nrec=rt.size();   
      if(nrec) mtr = rt.begin()->first.get();      
    }
     
    SimTrack_t s = setSimTrack(*tp, *mtr, nrec);
    histograms->fillSimHistograms(s);  

#ifdef DEBUG
    if(nrec) edm::LogTrace("TrkEffAnalyzer") 
      << "TrackingParticle #" << i << " with pt=" << tp->pt() 
      << " associated with quality:" << rt.begin()->second;
#endif
  }


  // RECO loop

  for(edm::View<reco::Track>::size_type i=0; i<trackCollection->size(); ++i){
    
    edm::RefToBase<reco::Track> track(trackCollection, i);
    reco::Track* tr=const_cast<reco::Track*>(track.get());
    
    std::vector<std::pair<TrackingParticleRef, double> > tp;
    const TrackingParticle *mtp=0;
    size_t nsim=0;

    if(recSimColl.find(track) != recSimColl.end()){
      tp = recSimColl[track];
      nsim=tp.size();
      if(nsim) mtp = tp.begin()->first.get();       
    }

    RecTrack_t r = setRecTrack(*tr, *mtp, nsim);
    histograms->fillRecHistograms(r); 

#ifdef DEBUG
    if(nsim) edm::LogTrace("TrkEffAnalyzer") 
      << "reco::Track #" << i << " with pt=" << track->pt() 
      << " associated with quality:" << tp.begin()->second;
#endif
  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
TrkEffAnalyzer::beginJob()
{

  histograms->declareHistograms();

}

// ------------ method called once each job just after ending the event loop  ------------
void 
TrkEffAnalyzer::endJob()
{

  histograms->writeHistograms();

}


SimTrack_t 
TrkEffAnalyzer::setSimTrack(TrackingParticle& tp, const reco::Track& mtr, size_t nrec)
{

  SimTrack_t s;
  s.ids = tp.pdgId();
  s.etas = tp.eta();
  s.pts = tp.pt();
  s.hits = tp.matchedHit();
  s.status = tp.status();

#ifdef DEBUG
  edm::LogTrace("TrkEffAnalyzer")  
    << "primary simtrack pt=" << s.pts 
    << " eta=" << s.etas
    << " hits=" << s.hits
    << " pdgId=" << s.ids;
#endif

  s.nrec = nrec;

  return s;

}


RecTrack_t 
TrkEffAnalyzer::setRecTrack(reco::Track& tr, const TrackingParticle& tp, size_t nsim)
{

  RecTrack_t r;
  r.charge = tr.charge();
  r.etar = tr.eta();
  r.ptr = tr.pt();
  r.phir = tr.phi();
  r.dz = tr.dz(); // FIX ME (USE VTX)
  r.d0 = tr.d0(); // FIX ME
  r.pterr = tr.ptError();
  r.d0err = tr.d0Error();
  r.dzerr = tr.dzError();
  r.hitr = tr.numberOfValidHits();

#ifdef DEBUG
  edm::LogTrace("TrkEffAnalyzer")  
    << "reco track pt=" << r.ptr
    << " eta=" << r.etar
    << " hits=" << r.hitr;
#endif

  r.nsim = nsim;

  if(nsim>0) {
    r.ids = tp.pdgId();
    r.parids = 0; // FIX ME
    r.etas = tp.eta();
    r.pts = tp.pt();
  } else {
    r.ids = 0;
    r.parids = 0;
    r.etas = 0.0;
    r.pts = 0.0;
  }
  
  return r;

}

//define this as a plug-in
DEFINE_FWK_MODULE(TrkEffAnalyzer);
