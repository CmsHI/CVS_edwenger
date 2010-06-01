#include <TROOT.h>                   // access to gROOT, entry point to ROOT system
#include <TStyle.h>                  // class to handle plot styles in ROOT
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TChain.h"
#include "TGraphAsymmErrors.h"
#include "TCut.h"
#include "TSystem.h"

#include "edwenger/TrackSpectraAnalyzer/macros/hlt_eff_simple/CPlot.h"           // helper class for plots
#include "edwenger/TrackSpectraAnalyzer/macros/hlt_eff_simple/HistoGroup.h"
#include "edwenger/TrackSpectraAnalyzer/macros/hlt_eff_simple/tgraphTools.C"

#include <map>
#include <cassert>
using namespace std;

void finalTrigSpectra(const char * inFileName = "plots/trigSpectra/anaspec.root",
    TString outdir="plots/trigSpectra")
{
  CPlot::sOutDir = outdir;
  Float_t histJetEtMax = 300;
  Int_t numPtBins=75;
  TFile * inFile = new TFile(inFileName);

  HistoGroup psTrigSpec("scTrigSpec");
  psTrigSpec.Add(inFile,"hSpecMBJet_scaled");
  psTrigSpec.Add(inFile,"hSpec6U_scaled");
  psTrigSpec.Add(inFile,"hSpec15U_scaled");
  psTrigSpec.Add(inFile,"hSpec30U_scaled");
  psTrigSpec.Add(inFile,"hSpec50U_scaled");
  psTrigSpec.Sum();

  // Find spectra ratio of combined triggered spectra
  // and all MB spectra
  TH1F * hSpecRatio = (TH1F*)psTrigSpec.hSum_->Clone("hSpecRatio");
  TH1F * hSpecMBHF_scaled = (TH1F*)inFile->FindObjectAny("hSpecMBHF_scaled");
  hSpecRatio->Divide(hSpecRatio);

  // Final plots
  // Spectra comparison
  TCanvas * cPSTrigSpec = new TCanvas("cPSTrigSpec","cPSTrigSpec",500,500);
  CPlot cpPSTrigSpec("finalPSTrigSpec","Jet triggered spectra","p_{T}^{trk} [GeV/c]","# evt");
  cpPSTrigSpec.SetLogy(1);
  cpPSTrigSpec.SetXRange(0,100);
  cpPSTrigSpec.AddHist1D(inFile,"hSpecMBHF_scaled","MB_HF","histE",kBlack,kFullCircle,9);
  cpPSTrigSpec.AddHist1D(psTrigSpec.hm_["hSpecMBJet_scaled"],"MB_HF leadingJet<34GeV","E",kViolet+2);
  cpPSTrigSpec.AddHist1D(psTrigSpec.hm_["hSpec6U_scaled"],"HLT_L1Jet6U, 34GeV<leadingJet<46GeV","E",kAzure+6);
  cpPSTrigSpec.AddHist1D(psTrigSpec.hm_["hSpec15U_scaled"],"HLT_Jet15U, 46GeV<leadingJet<74GeV","E",kGreen-3);
  cpPSTrigSpec.AddHist1D(psTrigSpec.hm_["hSpec30U_scaled"],"HLT_Jet30U, 74GeV<leadingJet<94GeV","E",kOrange-5);
  cpPSTrigSpec.AddHist1D(psTrigSpec.hm_["hSpec50U_scaled"],"HLT_Jet50U, 94GeV<leadingJet","E",kRed-2);
  cpPSTrigSpec.AddHist1D(psTrigSpec.hSum_,"Combine Triggers","hist",kRed,0);
  cpPSTrigSpec.SetLegendHeader("Calojets |#eta|<2.5");
  cpPSTrigSpec.SetLegend(0.315,0.62,0.86,0.92);
  cpPSTrigSpec.Draw(cPSTrigSpec,true,"gif");

  // Ratio Plot
  TCanvas * cMBTrigSpecRatio = new TCanvas("cMBTrigSpecRatio","cMBTrigSpecRatio",500,500);
  CPlot cpMBTrigSpecRatio("finalMBTrigSpecRatio","Jet triggered spectra","p_{T}^{trk} [GeV/c]","# evt");
  cpMBTrigSpecRatio.SetXRange(0,100);
  cpMBTrigSpecRatio.SetYRange(0,1.1);
  cpMBTrigSpecRatio.AddHist1D(hSpecRatio,"E",kViolet+2);
  //cpMBTrigSpecRatio.SetLegendHeader("Calojets |#eta|<2.5");
  //cpMBTrigSpecRatio.SetLegend(0.315,0.62,0.86,0.92);
  cpMBTrigSpecRatio.Draw(cMBTrigSpecRatio,true,"gif");
  
  // All done, save hists
  gSystem->mkdir(outdir,true);
  TFile * outf = new TFile(Form("%s/finalspec.root",outdir.Data()),"RECREATE");
  psTrigSpec.Save();
}
