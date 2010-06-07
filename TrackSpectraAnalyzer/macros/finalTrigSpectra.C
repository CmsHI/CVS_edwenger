#include <TROOT.h>                   // access to gROOT, entry point to ROOT system
#include <TStyle.h>                  // class to handle plot styles in ROOT
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TChain.h"
#include "TGraphAsymmErrors.h"
#include "TCut.h"
#include "TSystem.h"

#include "edwenger/TrackSpectraAnalyzer/macros/hlt_eff_simple/CPlot.h"           // helper class for plots
#include "edwenger/TrackSpectraAnalyzer/macros/hlt_eff_simple/HisGroup.h"
#include "edwenger/TrackSpectraAnalyzer/macros/hlt_eff_simple/tgraphTools.C"

#include <map>
#include <cassert>
using namespace std;

void finalTrigSpectra(const char * inFileName = "plots/trigSpectra/proc0531_v3/anaspec.root",
    TString outdir="plots/trigSpectra/proc0531_v3")
{
  CPlot::sOutDir = outdir;
  Float_t histJetEtMax = 300;
  Int_t numPtBins=75;
  TFile * inFile = new TFile(inFileName);
  TH1::SetDefaultSumw2();

  HisGroup psTrigSpec("scTrigSpec");
  psTrigSpec.Add(inFile,"hSpecMBJet_scaled");
  psTrigSpec.Add(inFile,"hSpec6U_scaled");
  psTrigSpec.Add(inFile,"hSpec15U_scaled");
  psTrigSpec.Add(inFile,"hSpec30U_scaled");
  psTrigSpec.Add(inFile,"hSpec50U_scaled");
  psTrigSpec.Sum();

  // Find spectra ratio of combined triggered spectra
  // and all MB spectra
  TH1D * hSpecRatio = (TH1D*)psTrigSpec.hSum_->Clone("hSpecRatio");
  TH1D * hSpecMBHF_scaled = (TH1D*)inFile->FindObjectAny("hSpecMBHF_scaled");
  hSpecRatio->Divide(hSpecMBHF_scaled);

  // Final plots
  // Spectra comparison
  TCanvas * cPSTrigSpec = new TCanvas("cPSTrigSpec","cPSTrigSpec",510,640);
  CPlot cpPSTrigSpec("finalPSTrigSpec","Jet triggered spectra","p_{T}^{trk} [GeV/c]","# evt");
  cpPSTrigSpec.SetLogy(1);
  cpPSTrigSpec.SetXRange(0,70);
  cpPSTrigSpec.AddHist1D(inFile,"hSpecMBHF_scaled","MB_HF","E",kBlack,kFullCircle);
  cpPSTrigSpec.AddHist1D(psTrigSpec.hm_["hSpecMBJet_scaled"],"MB_HF, E_{T}^{jet1} < 30GeV","E",kViolet+2);
  cpPSTrigSpec.AddHist1D(psTrigSpec.hm_["hSpec6U_scaled"],"HLT_L1Jet6U, E_{T}^{jet1} = 30-42GeV","E",kAzure+6);
  cpPSTrigSpec.AddHist1D(psTrigSpec.hm_["hSpec15U_scaled"],"HLT_Jet15U, E_{T}^{jet1} = 42-78GeV","E",kGreen-3);
  cpPSTrigSpec.AddHist1D(psTrigSpec.hm_["hSpec30U_scaled"],"HLT_Jet30U, E_{T}^{jet1} = 78-94GeV","E",kOrange-5);
  cpPSTrigSpec.AddHist1D(psTrigSpec.hm_["hSpec50U_scaled"],"HLT_Jet50U, E_{T}^{jet1} > 94GeV","E",kRed-2);
  cpPSTrigSpec.AddHist1D(psTrigSpec.hSum_,"Triggered Classes Combined","hist",kRed,0);
  cpPSTrigSpec.SetLegendHeader("Ak5 Calojets");
  cpPSTrigSpec.SetLegend(0.373,0.577,0.917,0.925);
  cpPSTrigSpec.Draw(cPSTrigSpec,true,"all");

  // Ratio Plot
  TCanvas * cMBTrigSpecRatio = new TCanvas("cMBTrigSpecRatio","cMBTrigSpecRatio",500,500);
  CPlot cpMBTrigSpecRatio("finalMBTrigSpecRatio","Jet triggered spectra","p_{T}^{trk} [GeV/c]","# evt");
  cpMBTrigSpecRatio.SetXRange(0,70);
  cpMBTrigSpecRatio.SetYRange(0,1.1);
  cpMBTrigSpecRatio.AddHist1D(hSpecRatio,"#frac{Triggered Classes Combined}{MB_HF}","E",kViolet+2);
  cpMBTrigSpecRatio.SetLegend(0.18,0.18,0.72,0.36);
  cpMBTrigSpecRatio.Draw(cMBTrigSpecRatio,true,"gif");
  
  // All done, save hists
  gSystem->mkdir(outdir,true);
  TFile * outf = new TFile(Form("%s/finalspec.root",outdir.Data()),"RECREATE");
  psTrigSpec.Save();
}
