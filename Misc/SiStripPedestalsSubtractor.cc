#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripPedestalsSubtractor.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/DataRecord/interface/SiStripPedestalsRcd.h"
#include "FWCore/Utilities/interface/Exception.h"

void SiStripPedestalsSubtractor::init(const edm::EventSetup& es){
  uint32_t p_cache_id = es.get<SiStripPedestalsRcd>().cacheIdentifier();
  if(p_cache_id != peds_cache_id) {
    es.get<SiStripPedestalsRcd>().get(pedestalsHandle);
    peds_cache_id = p_cache_id;
  }
}

void SiStripPedestalsSubtractor::subtract(const uint32_t& id, const uint16_t& firstStrip, std::vector<int16_t>& digis) {subtract_(id, firstStrip, digis, digis);}
void SiStripPedestalsSubtractor::subtract(const edm::DetSet<SiStripRawDigi>& input, std::vector<int16_t>& output) {subtract_(input.id, 0, input, output);}

template <class input_t>
inline
void SiStripPedestalsSubtractor::
subtract_(const uint32_t& id, const uint16_t& firstStrip, const input_t& input, std::vector<int16_t>& output) {
  try {

    pedestals.resize(firstStrip + input.size());
    SiStripPedestals::Range pedestalsRange = pedestalsHandle->getRange(id);
    pedestalsHandle->allPeds(pedestals, pedestalsRange);

    typename input_t::const_iterator inDigi = input.begin();
    std::vector<int>::const_iterator ped = pedestals.begin() + firstStrip;  
    std::vector<int16_t>::iterator   outDigi = output.begin();

    int ntries=0;
    while(1) {

      int inZeros=0, outZeros=0, nDigis=0;
      
      while( inDigi != input.end() ) {
	
	if( eval(*inDigi)  == 0 )  {
	  *outDigi = 0;
	  inZeros++;
	} else {
	  *outDigi = std::max(0,  ( *ped > 895 )        //FED bottoms out at 0
			      ? eval(*inDigi) - *ped + 1024 + ntries*addConstant_
			      : eval(*inDigi) - *ped        + ntries*addConstant_ );
	}	

	if( eval(*outDigi) == 0 )  outZeros++;
	nDigis++;
	
	++inDigi; 
	++ped; 
	++outDigi;
      }
      

      if(!outZeros && !inZeros) {
	if(ntries) std::cout << "  --> Hooray :) \n" << std::endl;
	break;
      } else {
	std::cout << "There were " << inZeros << " (" << outZeros 
		  << ") out of " << nDigis 
		  << " strips with ADC=0 before (after) pedestal subtraction"
		  << std::endl;

	if(outZeros-inZeros < minTruncStrips_) {
	  std::cout << "  --> Nothing more to do here.\n" << std::endl;
	  break;
	} else {

	  if(++ntries < maxTries_) {
	    std::cout << "  --> Add " << ntries*addConstant_ 
		      << " and try again..." << std::endl;
	    inDigi = input.begin();
	    ped = pedestals.begin() + firstStrip;  
	    outDigi = output.begin();
	  } else {
	    std::cout << "  --> I give up :( \n" << std::endl;
	    break;
	  } 

	} //end else 'outZeros-inZeros > 64'
	  
      } //end else 'outZeros'
      
    } //end while loop
    
    
  } catch(cms::Exception& e){
    edm::LogError("SiStripPedestalsSubtractor")  
      << "[SiStripPedestalsSubtractor::subtract] DetId " << id << " propagating error from SiStripPedestal" << e.what();
    output.clear();
  }

}
