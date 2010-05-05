//
// Original Author:  Edward Wenger
//         Created:  Thu Apr 29 14:31:47 CEST 2010
// $Id: TrkEffAnalyzer.cc,v 1.8 2010/05/05 10:35:01 edwenger Exp $
//

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "DataFormats/RecoCandidate/interface/TrackAssociation.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByHits.h"
#include "edwenger/TrkEffAnalyzer/interface/TrkEffAnalyzer.h"

//#define DEBUG

TrkEffAnalyzer::TrkEffAnalyzer(const edm::ParameterSet& iConfig)
:
  trackTags_(iConfig.getUntrackedParameter<edm::InputTag>("tracks")),
  label_tp_effic_(iConfig.getUntrackedParameter<edm::InputTag>("label_tp_effic")),
  label_tp_fake_(iConfig.getUntrackedParameter<edm::InputTag>("label_tp_fake")),
  associatorMap_(iConfig.getUntrackedParameter<edm::InputTag>("associatormap")),
  vtxTags_(iConfig.getUntrackedParameter<edm::InputTag>("vertices")),
  bsTags_(iConfig.getUntrackedParameter<edm::InputTag>("beamspot")),
  doAssociation_(iConfig.getUntrackedParameter<bool>("doAssociation",true))
{

  histograms = new TrkEffHistograms(iConfig);

}


// ------------ method called for each event  ------------
void
TrkEffAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  const TrackAssociatorByHits * theAssociatorByHits;

  // sim track collections

  edm::Handle<TrackingParticleCollection>  TPCollectionHeff ;
  iEvent.getByLabel(label_tp_effic_,TPCollectionHeff);
  const TrackingParticleCollection tPCeff = *(TPCollectionHeff.product());
  
  edm::Handle<TrackingParticleCollection>  TPCollectionHfake ;
  iEvent.getByLabel(label_tp_fake_,TPCollectionHfake);
  const TrackingParticleCollection tPCfake = *(TPCollectionHfake.product());

  // reco track, vertex, beamspot collections

  edm::Handle<edm::View<reco::Track> >  trackCollection;
  iEvent.getByLabel(trackTags_,trackCollection);

  iEvent.getByLabel(vtxTags_,vertexCollectionH);
  iEvent.getByLabel(bsTags_,beamSpotH);

  // association map

  reco::RecoToSimCollection recSimColl;
  reco::SimToRecoCollection simRecColl;

  if(doAssociation_){
     edm::ESHandle<TrackAssociatorBase> theAssociator;
     iSetup.get<TrackAssociatorRecord>().get("TrackAssociatorByHits",theAssociator);
     theAssociatorByHits = (const TrackAssociatorByHits*) theAssociator.product();

     simRecColl= theAssociatorByHits->associateSimToReco(trackCollection,TPCollectionHeff,&iEvent);
     recSimColl= theAssociatorByHits->associateRecoToSim(trackCollection,TPCollectionHfake,&iEvent);
  }else{
     edm::Handle<reco::SimToRecoCollection > simtorecoCollectionH;
     iEvent.getByLabel(associatorMap_,simtorecoCollectionH);
     simRecColl= *(simtorecoCollectionH.product());

     edm::Handle<reco::RecoToSimCollection > recotosimCollectionH;
     iEvent.getByLabel(associatorMap_,recotosimCollectionH);
     recSimColl= *(recotosimCollectionH.product());
  }


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

// ------------ method called at the beginning of each new run  ------------
void
TrkEffAnalyzer::beginRun(const edm::Run& r, const edm::EventSetup& es)
{

  // Get tracker geometry
  edm::ESHandle<TrackerGeometry> tracker;
  es.get<TrackerDigiGeometryRecord>().get(tracker);
  theTracker = tracker.product();

}

// ------------ method called once each job just after ending the event loop  ------------
void 
TrkEffAnalyzer::endJob()
{

  histograms->writeHistograms();

}

// ------------
SimTrack_t 
TrkEffAnalyzer::setSimTrack(TrackingParticle& tp, const reco::Track& mtr, size_t nrec)
{

  SimTrack_t s;
  s.ids = tp.pdgId();
  s.etas = tp.eta();
  s.pts = tp.pt();
  s.hits = tp.matchedHit();
  s.status = tp.status();
  std::pair<bool,bool> acc = isAccepted(tp);
  s.acc = acc.first || acc.second;

#ifdef DEBUG
  edm::LogTrace("TrkEffAnalyzer")  
    << "primary simtrack pt=" << s.pts 
    << " eta=" << s.etas
    << " hits=" << s.hits
    << " pdgId=" << s.ids;
#endif

  s.nrec = nrec;
  
  if(nrec > 0) {
    double dxy=0.0, dz=0.0;
    testVertex(*const_cast<reco::Track*>(&mtr),dxy,dz);
    s.d0 = dxy;
    s.dz = dz;
    s.pterr = mtr.ptError();
    s.d0err = mtr.d0Error();
    s.dzerr = mtr.dzError();
    s.hitr = mtr.numberOfValidHits();
    s.algo = mtr.algo();
  } else {
    s.dz = 0.0;
    s.d0 = 0.0;
    s.pterr = 0.0;
    s.d0err = 0.0;
    s.dzerr = 0.0;
    s.hitr = 0;
    s.algo = 0;
  }

  return s;

}

// ------------
RecTrack_t 
TrkEffAnalyzer::setRecTrack(reco::Track& tr, const TrackingParticle& tp, size_t nsim)
{

  RecTrack_t r;
  r.charge = tr.charge();
  r.etar = tr.eta();
  r.ptr = tr.pt();
  r.phir = tr.phi();

  double dxy=0.0, dz=0.0;
  if(!testVertex(tr,dxy,dz)) 
    std::cout << "used the beamspot as there is no vertex" << std::endl;
  r.d0 = dxy;
  r.dz = dz;
  
  r.pterr = tr.ptError();
  r.d0err = tr.d0Error(); // FIX ME (include vtx error in quadrature?)
  r.dzerr = tr.dzError(); // FIX ME 
  r.hitr = tr.numberOfValidHits();
  r.algo = tr.algo();

#ifdef DEBUG
  edm::LogTrace("TrkEffAnalyzer")  
    << "reco track pt=" << r.ptr
    << " eta=" << r.etar
    << " hits=" << r.hitr;
#endif

  r.nsim = nsim;

  if(nsim>0) {
    r.ids = tp.pdgId();
    r.parids = 0; // FIX ME (add parent id)
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

// ------------
bool
TrkEffAnalyzer::testVertex(reco::Track& tr, double &dxy, double &dz)
{

  const reco::VertexCollection *vtxs = vertexCollectionH.product();

  if(vtxs->size() == 0 || vtxs->begin()->isFake()) {
    dxy = tr.dxy(beamSpotH->position());
    dz  = 0.0;
    return false;
  } else {
    dxy = tr.dxy(vtxs->begin()->position()); // FIX ME (use most populated vertex?)
    dz  = tr.dz(vtxs->begin()->position());  // FIX ME (or closest vertex to track?)
    return true;
  }

}

// ------------
std::pair<bool,bool> 
TrkEffAnalyzer::isAccepted(TrackingParticle & tp)
{
  std::vector<bool> f(nLayers, false);

  const std::vector<PSimHit> & simHits = tp.trackPSimHit(DetId::Tracker);
  
  for(std::vector<PSimHit>::const_iterator simHit = simHits.begin();
                                      simHit!= simHits.end(); simHit++)
  {
    int id = getLayerId(*simHit);

    if(id != -1)
      f[id] = true;
  }

  bool canBeTriplet =
    ( (f[BPix1] && f[BPix2]     && f[BPix3]) ||
      (f[BPix1] && f[BPix2]     && f[FPix1_pos]) ||
      (f[BPix1] && f[BPix2]     && f[FPix1_neg]) ||
      (f[BPix1] && f[FPix1_pos] && f[FPix2_pos]) ||
      (f[BPix1] && f[FPix1_neg] && f[FPix2_neg]) );

  bool canBePair =
    ( (f[BPix1] && f[BPix2]) ||
      (f[BPix1] && f[BPix3]) ||
      (f[BPix2] && f[BPix3]) ||
      (f[BPix1] && f[FPix1_pos]) ||
      (f[BPix1] && f[FPix1_neg]) ||
      (f[BPix1] && f[FPix2_pos]) ||
      (f[BPix1] && f[FPix2_neg]) ||
      (f[BPix2] && f[FPix1_pos]) ||
      (f[BPix2] && f[FPix1_neg]) ||
      (f[BPix2] && f[FPix2_pos]) ||
      (f[BPix2] && f[FPix2_neg]) );

  return std::pair<bool,bool>(canBeTriplet, canBePair);
}

// ------------
int 
TrkEffAnalyzer::getLayerId(const PSimHit & simHit)
{
  unsigned int id = simHit.detUnitId();

  if(theTracker->idToDetUnit(id)->subDetector() ==
       GeomDetEnumerators::PixelBarrel)
  {
    PXBDetId pid(id);
    return pid.layer() - 1; // 0, 1, 2
  }

  if(theTracker->idToDetUnit(id)->subDetector() ==
       GeomDetEnumerators::PixelEndcap)
  {
    PXFDetId pid(id);
    return BPix3 + ((pid.side()-1) << 1) + pid.disk(); // 3 -
  }

  // strip
  return -1;
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrkEffAnalyzer);
