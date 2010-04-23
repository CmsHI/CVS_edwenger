#include "RecoParticleFlow/Configuration/test/PFCandidateAnalyzer.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"

//EAW
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
//


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

  thePtMin_ = 
    iConfig.getUntrackedParameter<double>("ptMin",3.0);


  LogDebug("PFCandidateAnalyzer")
    <<" input collection : "<<inputTagPFCandidates_ ;



   
}



PFCandidateAnalyzer::~PFCandidateAnalyzer() { }



void 
PFCandidateAnalyzer::beginRun(const edm::Run& run, 
			      const edm::EventSetup & es) {}

void 
PFCandidateAnalyzer::beginJob() {

  nt = f->make<TNtuple>("nt","PF Testing","type:pt:tkptmax:tkptsum:eetmax:eetsum:hetmax:hetsum");

}


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
    
    cand_type = cand.particleId();
    cand_pt = cand.pt();

    //--------- EDIT: Apr 21, 2010 (EAW)
    if(cand_pt < thePtMin_) continue;
    
    if(!(cand_type == PFCandidate::h ||     //type1
	 cand_type == PFCandidate::e ||     //type2
	 cand_type == PFCandidate::mu ||    //type3
	 cand_type == PFCandidate::gamma || //type4
	 cand_type == PFCandidate::h0)      //type5
       ) continue;

    if( verbose_ ) {
      cout<<cand<<endl;
      if (printBlocks_) printElementsInBlocks(cand);
    }   

    for(unsigned i=0; i<cand.elementsInBlocks().size(); i++) {

      PFBlockRef blockRef = cand.elementsInBlocks()[i].first;
     
      if(i) {
	cout << "WARNING: more than one block in this candidate" << endl; 
	continue;
      }

      const edm::OwnVector< reco::PFBlockElement >& elements = (*blockRef).elements();
      
      max_trk=0.0, sum_trk=0.0, max_ecal=0.0, sum_ecal=0.0, max_hcal=0.0, sum_hcal=0.0;

      for(unsigned ie=0; ie<elements.size(); ie++) {

	PFBlockElement::Type type = elements[ie].type();
	if(type==PFBlockElement::TRACK) {
	  cout << "TRK:";
	  reco::TrackRef trackRef = elements[ie].trackRef();
	  double trkpt = trackRef->pt();
	  cout << "pt=" << trkpt << endl;
	  sum_trk+=trkpt;
	  if(trkpt>max_trk) max_trk=trkpt;
	} 
	else if(type==PFBlockElement::ECAL) {
	  cout << "ECAL:";
	  PFClusterRef ecalRef = elements[ie].clusterRef();
	  double eet = ecalRef->energy()/cosh(ecalRef->eta());
	  cout << "et=" << eet << endl;
	  sum_ecal+=eet;
	  if(eet>max_ecal) max_ecal=eet;
	} 
	else if(type==PFBlockElement::HCAL) {
	  cout << "HCAL:";
	  PFClusterRef hcalRef = elements[ie].clusterRef();
	  double het = hcalRef->energy()/cosh(hcalRef->eta());
	  cout << "et=" << het << endl;
	  sum_hcal+=het;
	  if(het>max_hcal) max_hcal=het;
	}

      } //end loop over elements
 
    } //end loop over blocks

    nt->Fill(cand_type,cand_pt,max_trk,sum_trk,max_ecal,sum_ecal,max_hcal,sum_hcal);

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
