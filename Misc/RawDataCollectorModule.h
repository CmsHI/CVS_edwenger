#ifndef RawDataCollectorModule_H
#define RawDataCollectorModule_H


/** \class RawDataCollectorModule
 *
 */

#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/EDProducer.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h> 
#include <DataFormats/Common/interface/Handle.h>

class RawDataCollectorModule: public edm::EDProducer {
public:
    
    ///Constructor
    RawDataCollectorModule(const edm::ParameterSet& pset);
    
    ///Destructor
    virtual ~RawDataCollectorModule();
    
    void produce(edm::Event & e, const edm::EventSetup& c); 
          
private:

    static bool LargerProcessIndex(edm::Handle<FEDRawDataCollection> lhs, 
				   edm::Handle<FEDRawDataCollection> rhs) {
      return (lhs.provenance()->productID().processIndex() > 
	      rhs.provenance()->productID().processIndex());
    };

    bool useCurrentProcessOnly_ ; 
    bool preferLaterProcess_;
    bool verbose_ ;

};

#endif
