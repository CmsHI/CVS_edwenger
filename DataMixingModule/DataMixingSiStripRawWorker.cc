// File: DataMixingSiStripRawWorker.cc
// Description:  see DataMixingSiStripRawWorker.h
// Author:  Mike Hildreth, University of Notre Dame
//
//--------------------------------------------

#include <map>
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Framework/interface/ConstProductRegistry.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Provenance/interface/Provenance.h"
#include "DataFormats/Provenance/interface/BranchDescription.h"
//
//
#include "DataMixingSiStripRawWorker.h"

using namespace std;

namespace edm
{

  // Virtual constructor

  DataMixingSiStripRawWorker::DataMixingSiStripRawWorker() { sel_=0;}

  // Constructor 
  DataMixingSiStripRawWorker::DataMixingSiStripRawWorker(const edm::ParameterSet& ps) : 
							    label_(ps.getParameter<std::string>("Label"))

  {                                                         

    // get the subdetector names
    //    this->getSubdetectorNames();  //something like this may be useful to check what we are supposed to do...

    // create input selector
    if (label_.size()>0){
      sel_=new Selector( ModuleLabelSelector(label_));
    }
    else {
      sel_=new Selector( MatchAllSelector());
    }

    // declare the products to produce

    Sistripdigi_collectionSig_   = ps.getParameter<edm::InputTag>("SistripdigiCollectionSig");
    SistripLabelSig_   = ps.getParameter<edm::InputTag>("SistripLabelSig");

    SiStripRawPileInputTag_ = ps.getParameter<edm::InputTag>("SiStripRawPileInputTag");

    SiStripDigiCollectionDM_  = ps.getParameter<std::string>("SiStripDigiCollectionDM");

    // clear local storage for this event                                                                     
    SiHitStorage_.clear();

  }
	       

  // Virtual destructor needed.
  DataMixingSiStripRawWorker::~DataMixingSiStripRawWorker() { 
    delete sel_;
    sel_=0;
  }  



  void DataMixingSiStripRawWorker::addSiStripSignals(const edm::Event &e) { 
    // fill in maps of hits

    Handle< edm::DetSetVector<SiStripDigi> >  input;

    if( e.getByLabel(Sistripdigi_collectionSig_.label(),SistripLabelSig_.label(),input) ) {
      OneDetectorMap LocalMap;

      //loop on all detsets (detectorIDs) inside the input collection
      edm::DetSetVector<SiStripDigi>::const_iterator DSViter=input->begin();
      for (; DSViter!=input->end();DSViter++){

#ifdef DEBUG
	LogDebug("DataMixingSiStripRawWorker")  << "Processing DetID " << DSViter->id;
#endif

	LocalMap.clear();
	LocalMap.reserve((DSViter->data).size());
	LocalMap.insert(LocalMap.end(),(DSViter->data).begin(),(DSViter->data).end());	
	
	SiHitStorage_.insert( SiGlobalIndex::value_type( DSViter->id, LocalMap ) );
      }
 
    }
  } // end of addSiStripSignals



  void DataMixingSiStripRawWorker::addSiStripPileups(const int bcr, EventPrincipal *ep, unsigned int eventNr) {
    LogDebug("DataMixingSiStripRawWorker") <<"\n===============> adding pileups from event  "<<ep->id()<<" for bunchcrossing "<<bcr;

    boost::shared_ptr<Wrapper<edm::DetSetVector<SiStripRawDigi> >  const> inputPTR =
      getProductByTag<edm::DetSetVector<SiStripRawDigi> >(*ep, SiStripRawPileInputTag_ );

    if(!inputPTR) {
      std::cout << "no pointer to SiStripRawDigis from SiStripRawPileInputTag_" << std::endl;
      return;
    }
    
    const edm::DetSetVector<SiStripRawDigi>  *input = const_cast< edm::DetSetVector<SiStripRawDigi> * >(inputPTR->product());

    OneDetectorMap LocalMap;

    //loop on all detsets (detectorIDs) inside the input collection
    edm::DetSetVector<SiStripRawDigi>::const_iterator DSViter=input->begin();
    for (; DSViter!=input->end();DSViter++){

      // Make empty collection with this detID
      edm::DetSet<SiStripRawDigi> SSRD(DSViter->id); 

      // find local map (if it exists) for this detector ID
      SiGlobalIndex::const_iterator itest;
      itest = SiHitStorage_.find(DSViter->id);

      // if detID already has digis in existing map, add them to rawdigis
      if(itest!=SiHitStorage_.end()) {  

#ifdef DEBUG
	LogDebug("DataMixingSiStripRawWorker")  << "Pileups: Processing DetID " << DSViter->id;
#endif

	// get the map from storage
	LocalMap = itest->second;
	OneDetectorMap::const_iterator iLocal  = LocalMap.begin();

	// loop on all strips in rawdigi detset
	int currentstrip=0;
	edm::DetSet<SiStripRawDigi>::const_iterator iRawDigi = DSViter->begin();
	while( iRawDigi != DSViter->end() ) {

	  int ADCSum = iRawDigi->adc();

	  // if current strip exists in map, add ADC values
	  if(iLocal->strip() == currentstrip) {
	    ADCSum += iLocal->adc();
	    iLocal++;
	  }

	  // put ADC sum in DetSet and go to next strip
	  SSRD.push_back( SiStripRawDigi(ADCSum) );
	  iRawDigi++;
	  currentstrip++;

	}

	// copy combined digi+rawdigi into rawdigi DetSetVector
	vSiStripRawDigi.push_back(SSRD);

      // otherwise, just copy the rawdigis from the background event to the output
      } else {
	vSiStripRawDigi.push_back(*DSViter);
      }

    }

  }


 
  void DataMixingSiStripRawWorker::putSiStrip(edm::Event &e) {

    // make new raw digi collection
    std::auto_ptr< edm::DetSetVector<SiStripRawDigi> > MySiStripRawDigis(new edm::DetSetVector<SiStripRawDigi>(vSiStripRawDigi) );

    // put collection
    e.put( MySiStripRawDigis, SiStripDigiCollectionDM_ );

    // clear local storage for this event
    SiHitStorage_.clear();
  }

}
