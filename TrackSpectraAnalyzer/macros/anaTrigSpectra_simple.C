#include <TROOT.h>                   // access to gROOT, entry point to ROOT system
#include <TStyle.h>                  // class to handle plot styles in ROOT
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TChain.h"
#include "TGraphAsymmErrors.h"

#include "edwenger/TrackSpectraAnalyzer/macros/hlt_eff_simple/CPlot.h"           // helper class for plots
#include "edwenger/TrackSpectraAnalyzer/macros/hlt_eff_simple/HistoGroup.h"
#include "edwenger/TrackSpectraAnalyzer/macros/hlt_eff_simple/tgraphTools.C"

#include <map>
#include <cassert>
using namespace std;

Double_t GetEffJetPt(TGraph *eff, Double_t minEff)
{
  assert(eff);
  cout << "Begin: " << eff->GetName() << endl;
  Double_t minpt = 0;
  for(Int_t ig = 0; ig<eff->GetN(); ++ig){
    Double_t x,y;
    eff->GetPoint(ig,x,y);
    //cout << "Point " << ig << " x,y: " << x << "," << y << endl;
    if(y>minEff){
      minpt = x;
      cout<<"jet pt with eff > "<<minEff<<" = "<<minpt<< " GeV" << endl;
      break;
    }
  }
  return  minpt;
}

void anaTrigSpectra_simple(const char * inFileName = "../anasimplehlt/plots/V0531_histFromNt/anahlt.root",
    const char * ntFiles = "/net/hibat0003/d00/scratch/frankma/data/MinimumBias/MB-C10-PR9-TRKANASKIM-v3/trkhists_9*.root",
    TString outdir="plots/trigSpectra",
    TString histDir = "trackAna/")
{
  CPlot::sOutDir = outdir;
  Float_t histJetEtMax = 300;
  Int_t numPtBins=75;
  TFile * inFile = new TFile(inFileName);

  TGraphAsymmErrors *gHltEff_HltJet6U, *gHltEff_HltJet15U, *gHltEff_HltJet30U, *gHltEff_HltJet50U;
  inFile->GetObject("gHltEff_HltJet6U",gHltEff_HltJet6U);
  inFile->GetObject("gHltEff_HltJet15U",gHltEff_HltJet15U);
  inFile->GetObject("gHltEff_HltJet30U",gHltEff_HltJet30U);
  inFile->GetObject("gHltEff_HltJet50U",gHltEff_HltJet50U);
  map<TString, TGraphAsymmErrors* > gAEs;
  gAEs["gHltEff_HltJet6U"] = gHltEff_HltJet6U;
  gAEs["gHltEff_HltJet15U"] = gHltEff_HltJet15U;
  gAEs["gHltEff_HltJet30U"] = gHltEff_HltJet30U;
  gAEs["gHltEff_HltJet50U"] = gHltEff_HltJet50U;
  map<TString, TGraphAsymmErrors*>::iterator ig;
  for (ig=gAEs.begin(); ig != gAEs.end(); ++ig) {
    assert(ig->second);
    cout << ig->first << " " << ig->second->GetName() << endl;
    clearXErrorBar(ig->second);
  }
  // check
  TCanvas * cHltEff = new TCanvas("cHltEff","cHltEff",500,500);
  CPlot cpHltEff("HltEff","Hlt Eff","p_{T}^{corr jet} [GeV/c]","#epsilon_{HLT}");
  cpHltEff.SetXRange(0,histJetEtMax);
  cpHltEff.SetYRange(0,1.1);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet6U"],"HLT_L1Jet6U","pz",kAzure+6);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet15U"],"HLT_Jet15U","pz",kGreen-3);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet30U"],"HLT_Jet30U","pz",kOrange-5);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet50U"],"HLT_Jet50U","pz",kRed-2);
  cpHltEff.SetLegendHeader("Calojets |#eta|<2.5");
  cpHltEff.SetLegend(0.58,0.21,0.91,0.50);
  cpHltEff.Draw(cHltEff,false);

  // Find Efficiency > 99% jet et
  map<TString, Double_t> effJetPt;
  for (ig=gAEs.begin(); ig != gAEs.end(); ++ig) {
    effJetPt[ig->first] = GetEffJetPt(ig->second,0.99);
  }


  // ================== Triggered Spectra Ana ==================
  HistoGroup trigSpec("trigSepc");
  TChain * nt_jettrk = new TChain(histDir+"nt_jettrack","ntuple: jets and tracks");
  nt_jettrk->Add(ntFiles);
  cout << "Ana: " << histDir << "nt_jettrack: " << nt_jettrk->GetEntries() << " tracks" << endl;;
}
