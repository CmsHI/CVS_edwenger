#ifndef TrkEffAnalyzer_TrkEffHistograms
#define TrkEffAnalyzer_TrkEffHistograms

#include <vector>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

namespace edm { class ParameterSet ; }
class TFile;
class TTree;
class TH2F;

// define matched track structs

typedef struct
{
  int ids;
  float etas;
  float pts;
  int hits;
  int status;
  int acc;
  int nrec;
  float ptr;
  float dz;
  float d0;
  float pterr;
  float d0err;
  float dzerr;
  int hitr;
  int algo;
} SimTrack_t;

typedef struct
{
  int charge;
  float etar;
  float ptr;
  float phir;
  float dz;
  float d0;
  float pterr;
  float d0err;
  float dzerr;
  int hitr; 
  int algo;
  int nsim;
  int status;
  int ids;
  int parids;
  float etas;
  float pts;
} RecTrack_t;

// define class to hold histograms and track trees

class TrkEffHistograms
{

 public:
  TrkEffHistograms(const edm::ParameterSet& pset);
  ~TrkEffHistograms();

  void declareHistograms();
  void fillSimHistograms(const SimTrack_t & s);
  void fillRecHistograms(const RecTrack_t & r);
  void writeHistograms();

 private: 
   std::vector<TTree*> trackTrees;
   SimTrack_t simTrackValues;
   RecTrack_t recTrackValues;
   bool fillHistograms;
   bool fillNtuples;
   edm::Service<TFileService> f;

   std::vector<double> etaBins, ptBins;

   // SimTrack
   TH2F* hsim;
   TH2F* hacc;
   TH2F* heff;
   TH2F* hmul;

   // RecTrack
   TH2F* hrec;
   TH2F* hfak;

};

#endif