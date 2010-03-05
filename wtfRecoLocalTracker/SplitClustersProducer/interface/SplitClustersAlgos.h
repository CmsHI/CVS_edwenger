#ifndef SplitClustersAlgos_h
#define SplitClustersAlgos_h

/** \class SplitClustersAlgos
 *  Implementation base for cluster splitting producer
 *  Provides functions that can be shared with analysis classes.
 *
 *  $Date: 2009/10/24 21:31:56 $
 *  $Revision: 1.2 $
 *  \author wtford
 */

#include <memory>

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

class SplitClustersAlgos {
  public:

  enum {
    byHits = 0,
    byTracks
  };

  /// Constructor
    SplitClustersAlgos() { }
  
  /// Destructor
    virtual ~SplitClustersAlgos(){ }
  
 protected:

//
//  Number of strips in the left sub-cluster of a splittable cluster
//
    uint8_t leftStripCount(const std::vector<uint8_t> amp,
			   const std::vector<PSimHit> associated,
			   const int);

    uint8_t leftStripCount(const std::vector<uint8_t> amp,
			   const std::vector<PSimHit> associated,
			   float& totalEnergy,
			   float& leftAmplitudeFraction,
			   const int);

};

#endif
