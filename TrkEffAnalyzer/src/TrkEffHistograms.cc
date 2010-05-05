#include "edwenger/TrkEffAnalyzer/interface/TrkEffHistograms.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TTree.h"
#include "TFile.h"
#include "TH2F.h"

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
    leafStr = "ids/I:etas/F:pts/F:hits/I:status/I:acc/I:nrec/I:ptr/F:dz/F:d0/F:pterr/F:d0err/F:dzerr/F:hitr/I:algo/I";
    trackTrees[0]->Branch("simTrackValues", &simTrackValues, leafStr.Data());
    
    trackTrees.push_back(new TTree("recTrackTree","recTrackTree"));
    leafStr = "charge/I:etar/F:ptr/F:phir/F:dz/F:d0/F:pterr/F:d0err/F:dzerr/F:hitr/I:algo/I:nsim/I:status/I:ids/I:parids/I:etas/F:pts/F";
    trackTrees[1]->Branch("recTrackValues", &recTrackValues, leafStr.Data());
    
  }
  
  if(fillHistograms) {

    // pt bins
    const double small = 1e-3;
    double pt;

    for(pt =   0; pt <   1-small; pt +=  0.05) ptBins.push_back(pt);
    for(pt =   1; pt <   2-small; pt +=  0.1 ) ptBins.push_back(pt);
    for(pt =   2; pt <   5-small; pt +=  0.2 ) ptBins.push_back(pt);
    for(pt =   5; pt <  10-small; pt +=  0.5 ) ptBins.push_back(pt);
    for(pt =  10; pt <  20-small; pt +=  1.0 ) ptBins.push_back(pt);
    for(pt =  20; pt <  50-small; pt +=  2.0 ) ptBins.push_back(pt);
    for(pt =  50; pt < 100-small; pt +=  5.0 ) ptBins.push_back(pt);
    for(pt = 100; pt < 200-small; pt += 10.0 ) ptBins.push_back(pt);
    for(pt = 200; pt < 500-small; pt += 20.0 ) ptBins.push_back(pt);

    // eta bins
    static float etaMin   = -3.0;
    static float etaMax   =  3.0;
    static float etaWidth =  0.1;

    for(double eta = etaMin; eta < etaMax + etaWidth/2; eta += etaWidth)
      etaBins.push_back(eta);

    // simulated
    hsim = new TH2F("hsim","Sim Tracks;#eta;p_{T} (GeV/c)",
		    etaBins.size()-1, &etaBins[0],
		    ptBins.size()-1, &ptBins[0]);

    // accepted
    hacc = new TH2F("hacc","Accepted Tracks;#eta;p_{T} (GeV/c)",
		    etaBins.size()-1, &etaBins[0],
		    ptBins.size()-1, &ptBins[0]);

    // efficiency
    heff = new TH2F("heff","Effic Rec Tracks;#eta;p_{T} (GeV/c)",
		    etaBins.size()-1, &etaBins[0],
		    ptBins.size()-1, &ptBins[0]);

    // multiply reconstructed
    hmul = new TH2F("hmul","Mult Rec Tracks;#eta;p_{T} (GeV/c)",
		    etaBins.size()-1, &etaBins[0],
		    ptBins.size()-1, &ptBins[0]);

    // reconstructed
    hrec = new TH2F("hrec","Rec Tracks;#eta;p_{T} (GeV/c)",
		    etaBins.size()-1, &etaBins[0],
		    ptBins.size()-1, &ptBins[0]);

    // fakes
    hfak = new TH2F("hfak","Fake Tracks;#eta;p_{T} (GeV/c)",
		    etaBins.size()-1, &etaBins[0],
		    ptBins.size()-1, &ptBins[0]);

  }

}

void 
TrkEffHistograms::fillSimHistograms(const SimTrack_t & s)
{

  if(fillNtuples && s.status>0){
    simTrackValues = s;
    trackTrees[0]->Fill();
  }

  if(fillHistograms) {
    hsim->Fill(s.etas, s.pts);
    if(s.acc)    hacc->Fill(s.etas, s.pts);
    if(s.nrec>0) heff->Fill(s.etas, s.pts);
    if(s.nrec>1) hmul->Fill(s.etas, s.pts);
  }

}

void 
TrkEffHistograms::fillRecHistograms(const RecTrack_t & r)
{

  if(fillNtuples){
    recTrackValues = r;
    trackTrees[1]->Fill();
  }

  if(fillHistograms) {
    hrec->Fill(r.etar, r.ptr);
    if(!r.nsim) hfak->Fill(r.etar, r.ptr);
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

  if(fillHistograms) {
    histoFile->cd();
    hsim->Write();
    hacc->Write();
    heff->Write();
    hmul->Write();
    hrec->Write();
    hfak->Write();
    histoFile->Close();
  }

}
