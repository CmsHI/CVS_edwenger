#include <TROOT.h>                   // access to gROOT, entry point to ROOT system
#include <TStyle.h>                  // class to handle plot styles in ROOT
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TGraphAsymmErrors.h"

#include "edwenger/TrackSpectraAnalyzer/macros/hlt_eff_simple/CPlot.h"           // helper class for plots
#include "edwenger/TrackSpectraAnalyzer/macros/hlt_eff_simple/tgraphTools.C"

#include <map>
using namespace std;

void anaHltJetEff_simple(const char * inFileName = "../process_aod/outputs/trkhists_try9_100k.root",
    TString outdir="plots")
{
  CPlot::sOutDir = outdir;
  TFile * inFile = new TFile(inFileName);


  map<TString, TH1F* > hs1D;
  // === Checks ===
  // Plot Jet Pt distributions from various triggers
  TH1F *hJet0Pt,*hJet0Pt_HltMB,*hJet0Pt_HltJet6U,*hJet0Pt_HltJet15U,*hJet0Pt_HltJet30U,*hJet0Pt_HltJet50U;
  hs1D["hJet0Pt"] = hJet0Pt;
  hs1D["hJet0Pt_HltMB"] = hJet0Pt_HltMB;
  hs1D["hJet0Pt_HltJet6U"] = hJet0Pt_HltJet6U;
  hs1D["hJet0Pt_HltJet15U"] = hJet0Pt_HltJet15U;
  hs1D["hJet0Pt_HltJet30U"] = hJet0Pt_HltJet30U;
  hs1D["hJet0Pt_HltJet50U"] = hJet0Pt_HltJet50U;
  inFile->GetObject("trackAna/hJet0Pt",hs1D["hJet0Pt"]);
  inFile->GetObject("trackAna/hJet0Pt_HltMB",hs1D["hJet0Pt_HltMB"]);
  inFile->GetObject("trackAna/hJet0Pt_HltJet6U",hs1D["hJet0Pt_HltJet6U"]);
  inFile->GetObject("trackAna/hJet0Pt_HltJet15U",hs1D["hJet0Pt_HltJet15U"]);
  inFile->GetObject("trackAna/hJet0Pt_HltJet30U",hs1D["hJet0Pt_HltJet30U"]);
  inFile->GetObject("trackAna/hJet0Pt_HltJet50U",hs1D["hJet0Pt_HltJet50U"]);

  Int_t ngroups = 8; // 1-->0.5GeV per bin
  map<TString, TH1F*>::iterator iter;
  for (iter=hs1D.begin(); iter != hs1D.end(); ++iter) {
    // check
    cout << iter->first << " " << iter->second->GetName() << endl;
    // rebin
    iter->second->Rebin(ngroups);
  }

  TCanvas * cJetPt = new TCanvas("cJetPt","cJetPt",500,500);
  CPlot cpJetPt("JetPt","Jet Pt","p_{T}^{corr jet} [GeV/c]","# evt");
  cpJetPt.SetLogy(1);
  cpJetPt.SetXRange(0,200);
  cpJetPt.AddHist1D(hs1D["hJet0Pt"],"All Jets","",kViolet+2);
  cpJetPt.AddHist1D(hs1D["hJet0Pt_HltMB"],"HLT_MinBiasBSC","",kBlue);
  cpJetPt.AddHist1D(hs1D["hJet0Pt_HltJet6U"],"HLT_L1Jet6U","",kAzure+6);
  cpJetPt.AddHist1D(hs1D["hJet0Pt_HltJet15U"],"HLT_Jet15U","",kGreen-3);
  cpJetPt.AddHist1D(hs1D["hJet0Pt_HltJet30U"],"HLT_Jet30U","",kOrange-5);
  cpJetPt.AddHist1D(hs1D["hJet0Pt_HltJet50U"],"HLT_Jet50U","",kOrange-3);
  cpJetPt.SetLegendHeader("Calojets |#eta|<3");
  cpJetPt.Draw(cJetPt,true,"gif");

  // === HLT Eff Ana ===
  map<TString, TGraphAsymmErrors* > gAEs;
  gAEs["gHltEff_HltMB"] = new TGraphAsymmErrors();
  gAEs["gHltEff_HltJet6U"] = new TGraphAsymmErrors();
  gAEs["gHltEff_HltJet15U"] = new TGraphAsymmErrors();
  gAEs["gHltEff_HltJet30U"] = new TGraphAsymmErrors();
  gAEs["gHltEff_HltJet50U"] = new TGraphAsymmErrors();
  // Calc Hlt Eff
  gAEs["gHltEff_HltMB"]->BayesDivide(hs1D["hJet0Pt_HltMB"],hs1D["hJet0Pt"]);
  gAEs["gHltEff_HltJet6U"]->BayesDivide(hs1D["hJet0Pt_HltJet6U"],hs1D["hJet0Pt"]);
  gAEs["gHltEff_HltJet15U"]->BayesDivide(hs1D["hJet0Pt_HltJet15U"],hs1D["hJet0Pt_HltJet6U"]);
  gAEs["gHltEff_HltJet30U"]->BayesDivide(hs1D["hJet0Pt_HltJet30U"],hs1D["hJet0Pt_HltJet15U"]);
  gAEs["gHltEff_HltJet50U"]->BayesDivide(hs1D["hJet0Pt_HltJet50U"],hs1D["hJet0Pt_HltJet30U"]);
  map<TString, TGraphAsymmErrors*>::iterator ig;
  for (ig=gAEs.begin(); ig != gAEs.end(); ++ig) {
    // check
    cout << ig->first << " " << ig->second->GetName() << endl;
    clearXErrorBar(ig->second);
  }

  TCanvas * cHltEff = new TCanvas("cHltEff","cHltEff",500,500);
  CPlot cpHltEff("HltEff","Hlt Eff","p_{T}^{corr jet} [GeV/c]","#epsilon_{HLT}");
  cpHltEff.SetXRange(0,200);
  cpHltEff.SetYRange(0,1.1);
  cpHltEff.AddGraph(gAEs["gHltEff_HltMB"],"HLT_MinBiasBSC","pz",kBlue);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet6U"],"HLT_L1Jet6U","pz",kAzure+6);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet15U"],"HLT_Jet15U","pz",kGreen-3);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet30U"],"HLT_Jet30U","pz",kOrange-5);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet50U"],"HLT_Jet50U","pz",kOrange-3);
  cpHltEff.SetLegendHeader("Calojets |#eta|<3");
  cpHltEff.SetLegend(0.58,0.38,0.91,0.67);
  cpHltEff.Draw(cHltEff,true,"gif");
}
