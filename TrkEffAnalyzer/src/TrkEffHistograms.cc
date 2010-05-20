#include "edwenger/TrkEffAnalyzer/interface/TrkEffHistograms.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TTree.h"
#include "TFile.h"
#include "TH2F.h"
#include "TH3F.h"

#include <iostream>
#include <cmath>
using namespace std;

TrkEffHistograms::TrkEffHistograms(const edm::ParameterSet& pset)
{
  fillHistograms         = pset.getParameter<bool>("fillHistograms");
  fillNtuples            = pset.getParameter<bool>("fillNtuples");
  constPtBins            = pset.getParameter<bool>("constPtBins");
}


TrkEffHistograms::~TrkEffHistograms()
{
}

void 
TrkEffHistograms::declareHistograms()
{
  if(fillNtuples) {
    
    TString leafStr;
    
    trackTrees.push_back(f->make<TTree>("simTrackTree","simTrackTree"));
    leafStr = "ids/I:etas/F:pts/F:hits/I:status/I:acc/I:nrec/I:ptr/F:dz/F:d0/F:pterr/F:d0err/F:dzerr/F:hitr/I:algo/I";
    trackTrees[0]->Branch("simTrackValues", &simTrackValues, leafStr.Data());
    
    trackTrees.push_back(f->make<TTree>("recTrackTree","recTrackTree"));
    leafStr = "charge/I:etar/F:ptr/F:phir/F:dz/F:d0/F:pterr/F:d0err/F:dzerr/F:hitr/I:algo/I:nsim/I:status/I:ids/I:parids/I:etas/F:pts/F";
    trackTrees[1]->Branch("recTrackValues", &recTrackValues, leafStr.Data());
    
  }
  
  if(fillHistograms) {

    // pt bins
    if(!constPtBins){
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
    }else{
       static float ptMin   =  0.0;
       static float ptMax   =  200.0;
       static float ptWidth =  0.2;

       for(double pt = ptMin; pt < ptMax + ptWidth/2; pt += ptWidth)
	  ptBins.push_back(pt);
    }
    

    // eta bins
    static float etaMin   = -3.0;
    static float etaMax   =  3.0;
    static float etaWidth =  0.1;

    for(double eta = etaMin; eta < etaMax + etaWidth/2; eta += etaWidth)
      etaBins.push_back(eta);


    // jet et bins
    static float jetMin = 0.0;
    static float jetMax = 600; // good to be matched with ana 
    static float jetWidth = 20;

    for(double jet = jetMin; jet < jetMax + jetWidth/2; jet += jetWidth)
       jetBins.push_back(jet);


    // simulated
    hsim = f->make<TH2F>("hsim","Sim Tracks;#eta;p_{T} (GeV/c)",
		    etaBins.size()-1, &etaBins[0],
		    ptBins.size()-1, &ptBins[0]);

    // accepted
    hacc = f->make<TH2F>("hacc","Accepted Tracks;#eta;p_{T} (GeV/c)",
		    etaBins.size()-1, &etaBins[0],
		    ptBins.size()-1, &ptBins[0]);

    // efficiency
    heff = f->make<TH2F>("heff","Effic Rec Tracks;#eta;p_{T} (GeV/c)",
		    etaBins.size()-1, &etaBins[0],
		    ptBins.size()-1, &ptBins[0]);

    // multiply reconstructed
    hmul = f->make<TH2F>("hmul","Mult Rec Tracks;#eta;p_{T} (GeV/c)",
			 etaBins.size()-1, &etaBins[0],
			 ptBins.size()-1, &ptBins[0]);

    // reconstructed
    hrec = f->make<TH2F>("hrec","Rec Tracks;#eta;p_{T} (GeV/c)",
		    etaBins.size()-1, &etaBins[0],
		    ptBins.size()-1, &ptBins[0]);

    // fakes
    hfak = f->make<TH2F>("hfak","Fake Tracks;#eta;p_{T} (GeV/c)",
		    etaBins.size()-1, &etaBins[0],
		    ptBins.size()-1, &ptBins[0]);

    // simulated 3D 
    hsim3D = f->make<TH3F>("hsim3D","Sim Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
                      etaBins.size()-1, &etaBins[0],
                      ptBins.size()-1, &ptBins[0],
                      jetBins.size()-1, &jetBins[0]);

    // efficiency   3D 
    heff3D = f->make<TH3F>("heff3D","Effic Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
                      etaBins.size()-1, &etaBins[0],
                      ptBins.size()-1, &ptBins[0],
                      jetBins.size()-1, &jetBins[0]);

    // reconstructed 3D 
    hrec3D = f->make<TH3F>("hrec3D","Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
                      etaBins.size()-1, &etaBins[0],
                      ptBins.size()-1, &ptBins[0],
                      jetBins.size()-1, &jetBins[0]);

    // fakes 3D 
    hfak3D = f->make<TH3F>("hfak3D","Fake Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
                      etaBins.size()-1, &etaBins[0],
                      ptBins.size()-1, &ptBins[0],
                      jetBins.size()-1, &jetBins[0]);
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
    hsim3D->Fill(s.etas, s.pts, s.jetr);
    if(s.acc)    hacc->Fill(s.etas, s.pts);
    if(s.nrec>0) heff->Fill(s.etas, s.pts), heff3D->Fill(s.etas, s.pts, s.jetr);
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
    hrec3D->Fill(r.etar, r.ptr, r.jetr);
    if(!r.nsim) hfak->Fill(r.etar, r.ptr), hfak3D->Fill(r.etar, r.ptr, r.jetr);
  }

}

void 
TrkEffHistograms::writeHistograms()
{

}
