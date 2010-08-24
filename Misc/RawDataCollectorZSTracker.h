#ifndef RawDataCollectorZSTracker_H
#define RawDataCollectorZSTracker_H


/** \class RawDataCollectorZSTracker
 *
 */

#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/EDProducer.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h> 
#include <DataFormats/Common/interface/Handle.h>

class RawDataCollectorZSTracker: public edm::EDProducer {
public:
    
    ///Constructor
    RawDataCollectorZSTracker(const edm::ParameterSet& pset);
    
    ///Destructor
    virtual ~RawDataCollectorZSTracker();
    
    void produce(edm::Event & e, const edm::EventSetup& c); 
          
private:

    static bool LargerProcessIndex(edm::Handle<FEDRawDataCollection> lhs, 
				   edm::Handle<FEDRawDataCollection> rhs) {
      return (lhs.provenance()->productID().processIndex() > 
	      rhs.provenance()->productID().processIndex());
    };

    bool useCurrentProcessOnly_ ; 
    bool preferLaterProcess_;
    int  verbose_ ;

};

#endif
