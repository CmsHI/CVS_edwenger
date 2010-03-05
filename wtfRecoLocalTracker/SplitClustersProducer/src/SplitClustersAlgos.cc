//
//  Implementation base for cluster splitting producer
//  Provides functions that can be shared with analysis classes.
//

// this class header
#include "wtfRecoLocalTracker/SplitClustersProducer/interface/SplitClustersAlgos.h"

#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"

using namespace std;

//
//  Number of strips in the left sub-cluster of a splittable cluster
//
uint8_t SplitClustersAlgos::leftStripCount(const std::vector<uint8_t> amp,
					   const std::vector<PSimHit> associated, const int splitBy = SplitClustersAlgos::byHits)
{
  float totalEnergy, leftAmplitudeFraction;
  return (leftStripCount(amp, associated, totalEnergy, leftAmplitudeFraction, splitBy));
}

uint8_t SplitClustersAlgos::leftStripCount(const std::vector<uint8_t> amp,
					   const std::vector<PSimHit> associated,
					   float& totalEnergy,
					   float& leftAmplitudeFraction,
					   const int splitBy = SplitClustersAlgos::byHits)
{
  /*
  From SimG4Core/Physics/src/ProcessTypeEnumerator.cc
  ProcessTypeEnumerator::ProcessTypeEnumerator(){
  mapProcesses["Undefined"] = 0;
  mapProcesses["Unknown"] = 1;
  mapProcesses["Primary"] = 2;
  mapProcesses["Hadronic"] = 3;
  mapProcesses["Decay"] = 4;
  mapProcesses["Compton"] = 5;
  mapProcesses["Annihilation"] = 6;
  mapProcesses["EIoni"] = 7;
  mapProcesses["HIoni"] = 8;
  mapProcesses["MuIoni"] = 9;
  mapProcesses["Photon"] = 10;
  mapProcesses["MuPairProd"] = 11;
  mapProcesses["Conversions"] = 12;
  mapProcesses["EBrem"] = 13;
  mapProcesses["SynchrotronRadiation"] = 14;
  mapProcesses["MuBrem"] = 15;
  mapProcesses["MuNucl"] = 16;
  */

  float leftEnergyFraction=0;
//   bool debug = true;
  bool debug = false;

  if (splitBy == SplitClustersAlgos::byHits) {
    //Calculate the energy fraction of 1 of the 2 SimHits associated
    //  We have 2 hits
    float leftEnergy = 0, middlex = 0, thisx = 0;
    totalEnergy = 0;
    std::vector<PSimHit>::const_iterator firstsimhit = associated.begin();
    middlex = firstsimhit->localPosition().x();
    leftEnergy = firstsimhit->energyLoss();
    if (debug) cout << "PSimHit trackID, Eloss, process";
    for(std::vector<PSimHit>::const_iterator simhitsIter=firstsimhit; simhitsIter<associated.end(); simhitsIter++){
      if (debug) cout << " " << simhitsIter->trackId() << ", " << simhitsIter->energyLoss()
		      << ", " << simhitsIter->processType() << "; ";
      totalEnergy += simhitsIter->energyLoss();
      if (middlex > (thisx = simhitsIter->localPosition().x())) {
        middlex = thisx;
        leftEnergy = simhitsIter->energyLoss();
      }
    }
    leftEnergyFraction = leftEnergy / totalEnergy;

  } else {
    // We have 2 tracks, perhaps more than one hit per track (from interactions)
    unsigned firstTrackNo = 0;
    float xpos[2] = {0,0}, energy[2] = {0,0}, thisEnergy;
    if (debug) cout << "PSimHit trackID, Eloss, process, frac";
    for(std::vector<PSimHit>::const_iterator simhitsIter=associated.begin(); simhitsIter<associated.end(); simhitsIter++){
      if (debug) cout << " " << simhitsIter->trackId() << ", " << simhitsIter->energyLoss()
		      << ", " << simhitsIter->processType() << "; ";
      thisEnergy = simhitsIter->energyLoss();
      if (firstTrackNo == 0) firstTrackNo = simhitsIter->trackId();
      if (simhitsIter->trackId() == firstTrackNo) {
        xpos[0] += thisEnergy*simhitsIter->localPosition().x();
        energy[0] += thisEnergy;
      } else {
        xpos[1] += thisEnergy*simhitsIter->localPosition().x();
        energy[1] += thisEnergy;
      }
    }
    if (energy[0] != 0) xpos[0] /= energy[0];
    if (energy[1] != 0) xpos[1] /= energy[1];
    if (xpos[0] < xpos[1]) leftEnergyFraction = energy[0] / (energy[0]+energy[1]);
    else leftEnergyFraction = energy[1] / (energy[0]+energy[1]);
  }
  if (debug) cout << " lfrac= " << leftEnergyFraction << endl;

  //Find # of strips from the left which in total have charge fraction ~ energy fraction
  float totalAmplitude =0, leftAmplitude =0;
  leftAmplitudeFraction = 0;
  uint8_t stripCounter = 0;
  for(size_t j=0; j < size_t(amp.size()); ++j) totalAmplitude += float(amp[j]);
  for(size_t j=0; j<amp.size(); ++j){
    leftAmplitude += float(amp[j]);
    leftAmplitudeFraction = leftAmplitude/totalAmplitude;
    if (leftAmplitudeFraction > leftEnergyFraction) {
      if (float(amp[j]) >= 2.0*totalAmplitude*(leftAmplitudeFraction-leftEnergyFraction)) stripCounter++;
      break;
    }
    stripCounter++;
  }
  return(stripCounter);
}
