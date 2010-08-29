#ifndef RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPPEDESTALSSUBTRACTOR_H
#define RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPPEDESTALSSUBTRACTOR_H

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"
#include "CondFormats/SiStripObjects/interface/SiStripPedestals.h"
#include <vector>

class SiStripPedestalsSubtractor {

  friend class SiStripRawProcessingFactory;

 public:
  
  void subtract(const edm::DetSet<SiStripRawDigi>&, std::vector<int16_t>&);
  void subtract(const uint32_t&, const uint16_t &, std::vector<int16_t>&);
  void init(const edm::EventSetup& );
  
 private:
  
  SiStripPedestalsSubtractor() : peds_cache_id(0), addConstant_(0), minTruncStrips_(0), maxTries_(0) {};
  SiStripPedestalsSubtractor(int ac, int mts, int mt) : peds_cache_id(0), addConstant_(ac), minTruncStrips_(mts), maxTries_(mt) {}; 
  edm::ESHandle<SiStripPedestals> pedestalsHandle;
  std::vector<int> pedestals;
  uint32_t peds_cache_id;
  int addConstant_;
  int minTruncStrips_;
  int maxTries_;
  
  template <class input_t> void subtract_(const uint32_t&,const uint16_t&, const input_t&, std::vector<int16_t>&);
  const int16_t& eval(const int16_t& in) { return in;}
  const uint16_t& eval(const SiStripRawDigi& in) { return in.adc();}

};
#endif
