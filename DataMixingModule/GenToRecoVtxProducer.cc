#ifndef HI_GenToRecoVtxProducer_H
#define HI_GenToRecoVtxProducer_H
/*
*   $Date: 2010/10/11 11:40:10 $
*   $Revision: 1.4 $
*/
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/Provenance/interface/Provenance.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"


using namespace edm;
using namespace std;


class GenToRecoVtxProducer : public edm::EDProducer
{
   public:
      
   // ctor & dtor
   explicit GenToRecoVtxProducer( const edm::ParameterSet& );
   virtual ~GenToRecoVtxProducer();
      
   virtual void produce( edm::Event&, const edm::EventSetup& );
      
   virtual reco::Vertex* getVertex(edm::Event&);
   
   private :

   edm::InputTag            signalLabel;

};

GenToRecoVtxProducer::GenToRecoVtxProducer( const ParameterSet& pset ) 
	: signalLabel(pset.getParameter<edm::InputTag>("signalLabel"))
	  
{   
   produces<reco::VertexCollection>();
}

GenToRecoVtxProducer::~GenToRecoVtxProducer() 
{

}

reco::Vertex* GenToRecoVtxProducer::getVertex( Event& evt){

  Handle<HepMCProduct> input;
  evt.getByLabel(signalLabel,input);

  const HepMC::GenEvent* inev = input->GetEvent();
  HepMC::GenVertex* genvtx = inev->signal_process_vertex();
  if(!genvtx){
    cout<<"No Signal Process Vertex!"<<endl;
    HepMC::GenEvent::particle_const_iterator pt=inev->particles_begin();
    HepMC::GenEvent::particle_const_iterator ptend=inev->particles_end();
    while(!genvtx || ( genvtx->particles_in_size() == 1 && pt != ptend ) ){
      if(!genvtx) cout<<"No Gen Vertex!"<<endl;
      if(pt == ptend) cout<<"End reached!"<<endl;
      genvtx = (*pt)->production_vertex();
      ++pt;
    }
  }
  double aX,aY,aZ;

  aX = genvtx->position().x()/10.; // mm -> cm
  aY = genvtx->position().y()/10.;
  aZ = genvtx->position().z()/10.;

  cout << "Matching to vertex position: (" << aX << "," << aY << "," << aZ << ")" << endl;


  reco::Vertex::Error err;
  err(0,0)=pow(0.01,2);
  err(1,1)=pow(0.01,2);
  err(2,2)=pow(0.01,2);

  reco::Vertex *fVertex = new reco::Vertex(reco::Vertex::Point(aX,aY,aZ),err, 0, 1, 1);
  
  return fVertex;

}

void GenToRecoVtxProducer::produce( Event& evt, const EventSetup& )
{
   
   
  // New vertex collection
  std::auto_ptr<reco::VertexCollection> newVertexCollection(new reco::VertexCollection);

  newVertexCollection->push_back(*getVertex(evt));

  // put new vertex collection into event
  evt.put(newVertexCollection);

}

DEFINE_FWK_MODULE(GenToRecoVtxProducer);

#endif
