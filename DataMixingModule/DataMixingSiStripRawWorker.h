#ifndef DataMixingSiStripRawWorker_h
#define SimDataMixingSiStripRawWorker_h

/** \class DataMixingSiStripRawWorker
 *
 * DataMixingModule is the EDProducer subclass 
 * that overlays rawdata events on top of MC,
 * using real data for pileup simulation
 * This class takes care of the SiStrip information
 *
 * \author Mike Hildreth, University of Notre Dame
 *
 * \version   1st Version October 2007
 *
 ************************************************************/

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Selector.h"

#include "DataFormats/Provenance/interface/ProductID.h"
#include "DataFormats/Common/interface/Handle.h"
//Data Formats
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"

#include <map>
#include <vector>
#include <string>


namespace edm
{
  class DataMixingSiStripRawWorker
    {
    public:

      DataMixingSiStripRawWorker();

     /** standard constructor*/
      explicit DataMixingSiStripRawWorker(const edm::ParameterSet& ps);

      /**Default destructor*/
      virtual ~DataMixingSiStripRawWorker();

      void putSiStrip(edm::Event &e) ;
      void addSiStripSignals(const edm::Event &e); 
      void addSiStripPileups(const int bcr, edm::EventPrincipal*,unsigned int EventId);


    private:
      // data specifiers

      edm::InputTag Sistripdigi_collectionSig_ ; // primary name given to collection of SiStrip digis
      edm::InputTag SistripLabelSig_ ;           // secondary name given to collection of SiStrip digis
      edm::InputTag SiStripRawPileInputTag_ ;    // InputTag for raw pileup strips
      std::string SiStripDigiCollectionDM_  ;    // secondary name to be given to new SiStrip raw digis

      // 

      typedef std::vector<SiStripDigi> OneDetectorMap;   // maps by strip ID for later combination - can have duplicate strips
      typedef std::map<uint32_t, OneDetectorMap> SiGlobalIndex; // map to all data for each detector ID

      SiGlobalIndex SiHitStorage_;

      // collection of RawDigis to put back in the event
      std::vector< edm::DetSet<SiStripRawDigi> > vSiStripRawDigi;


      //      unsigned int eventId_; //=0 for signal, from 1-n for pileup events

      Selector * sel_;
      std::string label_;

    };
}//edm

#endif
