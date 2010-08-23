/** \file
 * Implementation of class RawDataCollectorModule
 *
 */

#include "EventFilter/RawDataCollector/interface/RawDataCollectorModule.h"
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h> 
#include <DataFormats/FEDRawData/interface/FEDRawData.h>
#include <DataFormats/FEDRawData/interface/FEDNumbering.h>

#include <DataFormats/Common/interface/Handle.h>
#include <FWCore/Framework/interface/Event.h>
#include "DataFormats/Provenance/interface/ProcessHistory.h" 
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>

using namespace edm;

RawDataCollectorModule::RawDataCollectorModule(const edm::ParameterSet& pset) {

  useCurrentProcessOnly_ = pset.getParameter<bool>("currentProcessOnly") ; 
  preferLaterProcess_ = pset.getParameter<bool>("preferLaterProcess");
  verbose_ = pset.getUntrackedParameter<bool>("verbose",true);

  produces<FEDRawDataCollection>();
}

RawDataCollectorModule::~RawDataCollectorModule(){

}


void RawDataCollectorModule::produce(Event & e, const EventSetup& c){

 /// Get Data from all FEDs
 std::vector< Handle<FEDRawDataCollection> > rawData;
 e.getManyByType(rawData);

 /// Sort so later processes come first
 if(!useCurrentProcessOnly_ && preferLaterProcess_)
   std::sort(rawData.begin(),rawData.end(),LargerProcessIndex);

 std::auto_ptr<FEDRawDataCollection> producedData(new FEDRawDataCollection);

 for (unsigned int i=0; i< rawData.size(); ++i ) { 

   const FEDRawDataCollection *rdc=rawData[i].product();

   if ( useCurrentProcessOnly_ &&
        ( rawData[i].provenance()->processName() != e.processHistory().rbegin()->processName() ) )
       continue ; // skip all raw collections not produced by the current process

   /// test provenance info
   //std::cout << "\nRAW collection #" << i+1 << std::endl;
   //std::cout << "branch name = " << rawData[i].provenance()->branchName() << std::endl;
   //std::cout << "process index = " << rawData[i].provenance()->productID().processIndex() << std::endl;

   for ( int j=0; j< FEDNumbering::MAXFEDID; ++j ) {
     const FEDRawData & fedData = rdc->FEDData(j);
     size_t size=fedData.size();

     if ( size > 0 ) {
       // this fed has data -- lets copy it
       FEDRawData & fedDataProd = producedData->FEDData(j);
       if ( fedDataProd.size() != 0 ) {
	 if(verbose_) {
	   std::cout << " More than one FEDRawDataCollection with data in FED ";
	   std::cout << j << " Skipping the 2nd\n";
	 }
	 continue;
       } 
       fedDataProd.resize(size);
       unsigned char *dataProd=fedDataProd.data();
       const unsigned char *data=fedData.data();
       for ( unsigned int k=0; k<size; ++k ) {
	 dataProd[k]=data[k];
       }
     }
   }
 }

 // Insert the new product in the event  
 e.put(producedData);  

}


