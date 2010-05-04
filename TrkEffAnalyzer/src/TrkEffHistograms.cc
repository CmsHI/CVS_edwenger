#include "edwenger/TrkEffAnalyzer/interface/TrkEffHistograms.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"

#include <iostream>
#include <cmath>
using namespace std;

TrkEffHistograms::TrkEffHistograms(const edm::ParameterSet& pset)
{
  fillHistograms         = pset.getParameter<bool>("fillHistograms");
  fillNtuples            = pset.getParameter<bool>("fillNtuples");

  string histoFileLabel = pset.getParameter<string>("histoFile");

  if(fillHistograms)
    histoFile = new TFile(histoFileLabel.c_str(),"recreate");

  string ntupleFileLabel = pset.getParameter<string>("ntupleFile");

  if(fillNtuples)
    ntupleFile = new TFile(ntupleFileLabel.c_str(),"recreate");
}


TrkEffHistograms::~TrkEffHistograms()
{
}

void 
TrkEffHistograms::declareHistograms()
{
  if(fillNtuples) {
    
    TString leafStr;
    
    trackTrees.push_back(new TTree("simTrackTree","simTrackTree"));
    leafStr = "ids/I:etas/F:pts/F:hits/I:status/I:nrec/I";
    trackTrees[0]->Branch("simTrackValues", &simTrackValues, leafStr.Data());
    
    trackTrees.push_back(new TTree("recTrackTree","recTrackTree"));
    leafStr = "charge/I:etar/F:ptr/F:phir/F:dz/F:d0/F:pterr/F:d0err/F:dzerr/F:hitr/I:nsim/I:ids/I:parids/I:etas/F:pts/F";
    trackTrees[1]->Branch("recTrackValues", &recTrackValues, leafStr.Data());
    
  }
  
  if(fillHistograms) {

    htest = new TH1F("htest","htest",100,0.0,10.0);

  }

}

void 
TrkEffHistograms::fillSimHistograms(const SimTrack_t & s)
{

  if(fillNtuples && s.status>0){
    simTrackValues = s;
    trackTrees[0]->Fill();
  }

}

void 
TrkEffHistograms::fillRecHistograms(const RecTrack_t & r)
{

  if(fillNtuples){
    recTrackValues = r;
    trackTrees[1]->Fill();
  }

}

void 
TrkEffHistograms::writeHistograms()
{

  typedef vector<TTree *>::const_iterator TI;
  if(fillNtuples) {
    ntupleFile->cd();
    for(TI t = trackTrees.begin(); t!= trackTrees.end(); t++) (*t)->Write();
    ntupleFile->Close();
  }

}
