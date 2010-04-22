#include "RecoParticleFlow/Configuration/test/PFCandidateAnalyzer.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"

//EAW
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"


using namespace std;
using namespace edm;
using namespace reco;

PFCandidateAnalyzer::PFCandidateAnalyzer(const edm::ParameterSet& iConfig) {
  


  inputTagPFCandidates_ 
    = iConfig.getParameter<InputTag>("PFCandidates");

  verbose_ = 
    iConfig.getUntrackedParameter<bool>("verbose",false);

  printBlocks_ = 
    iConfig.getUntrackedParameter<bool>("printBlocks",false);



  LogDebug("PFCandidateAnalyzer")
    <<" input collection : "<<inputTagPFCandidates_ ;
   
}



PFCandidateAnalyzer::~PFCandidateAnalyzer() { }



void 
PFCandidateAnalyzer::beginRun(const edm::Run& run, 
			      const edm::EventSetup & es) { }


void 
PFCandidateAnalyzer::analyze(const Event& iEvent, 
			     const EventSetup& iSetup) {
  
  LogDebug("PFCandidateAnalyzer")<<"START event: "<<iEvent.id().event()
			 <<" in run "<<iEvent.id().run()<<endl;
  
  
  
  // get PFCandidates

  Handle<PFCandidateCollection> pfCandidates;
  iEvent.getByLabel(inputTagPFCandidates_, pfCandidates);

  
  for( unsigned i=0; i<pfCandidates->size(); i++ ) {
    
    const reco::PFCandidate& cand = (*pfCandidates)[i];
    
    //--------- EDIT: Apr 21, 2010 (EAW)
    if(cand.pt() < 5) continue;

    if( verbose_ ) {
      cout<<cand<<endl;
      if (printBlocks_) printElementsInBlocks(cand);
    }    

    //--------- EDIT: Apr 19, 2010 (EAW)
    //if(cand.pt() < 5) continue;
    cout << "\ncandidate pt=" << cand.pt() << endl;
    
    if(cand.particleId() != PFCandidate::h && //type1
       cand.particleId() != PFCandidate::e && //type2
       cand.particleId() != PFCandidate::mu   //type3
       ) continue;
    cout << "candidate type=" << cand.particleId() << endl;

    for(unsigned i=0; i<cand.elementsInBlocks().size(); i++) {
      PFBlockRef blockRef = cand.elementsInBlocks()[i].first;
     
      if(i) cout << "WARNING: more than one block in this candidate" << endl;

      const edm::OwnVector< reco::PFBlockElement >& elements = (*blockRef).elements();
      
      for(unsigned ie=0; ie<elements.size(); ie++) {

	PFBlockElement::Type type = elements[ie].type();
	if(type==PFBlockElement::TRACK) {
	  cout << "TRK:" << endl;
	  reco::TrackRef trackRef = elements[ie].trackRef();
	  cout << "pt=" << trackRef->pt() << endl;
	} 
	else if(type==PFBlockElement::ECAL) {
	  cout << "ECAL:" << endl;
	  PFClusterRef ecalRef = elements[ie].clusterRef();
	  cout << "energy=" << ecalRef->energy() << endl;
	} 
	else if(type==PFBlockElement::HCAL) {
	  cout << "HCAL:" << endl;
	  PFClusterRef hcalRef = elements[ie].clusterRef();
	  cout << "energy=" << hcalRef->energy() << endl;	  
	}

      } //end loop over elements
 
    } //end loop over blocks

    //---------
  }
    
  LogDebug("PFCandidateAnalyzer")<<"STOP event: "<<iEvent.id().event()
			 <<" in run "<<iEvent.id().run()<<endl;
}




void PFCandidateAnalyzer::printElementsInBlocks(const PFCandidate& cand,
						ostream& out) const {
  if(!out) return;

  PFBlockRef firstRef;

  assert(!cand.elementsInBlocks().empty() );
  for(unsigned i=0; i<cand.elementsInBlocks().size(); i++) {
    PFBlockRef blockRef = cand.elementsInBlocks()[i].first;

    if(blockRef.isNull()) {
      cerr<<"ERROR! no block ref!";
      continue;
    }

    if(!i) {
      out<<(*blockRef);
      firstRef = blockRef;
    }
    else if( blockRef!=firstRef) {
      cerr<<"WARNING! This PFCandidate is not made from a single block"<<endl;
    }
 
    out<<"\t"<<cand.elementsInBlocks()[i].second<<endl;
  }
}


DEFINE_FWK_MODULE(PFCandidateAnalyzer);
