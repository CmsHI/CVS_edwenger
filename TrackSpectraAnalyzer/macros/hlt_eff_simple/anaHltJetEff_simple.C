#include <TROOT.h>                   // access to gROOT, entry point to ROOT system
#include <TStyle.h>                  // class to handle plot styles in ROOT
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TChain.h"
#include "TGraphAsymmErrors.h"

#include "edwenger/TrackSpectraAnalyzer/macros/hlt_eff_simple/CPlot.h"           // helper class for plots
#include "edwenger/TrackSpectraAnalyzer/macros/hlt_eff_simple/tgraphTools.C"

#include <map>
#include <cassert>
using namespace std;

void anaHltJetEff_simple(const char * inFileName = "../process_aod/outputs/trkhists_try9_100k.root",
    TString outdir="plots/fromHist",
    Bool_t useHist = true,
    TString histDir = "trackAna/",
    Int_t reBinFactor = 8)
{
  CPlot::sOutDir = outdir;
  Float_t histJetEtMax = 300;
  Int_t numPtBins=75;
  TH1::SetDefaultSumw2();

  map<TString, TH1F* > hs1D;
  // === Checks ===
  // Plot Jet Pt distributions from various triggers
  if (useHist) {
    TFile * inFile = new TFile(inFileName);
    TH1F *hJet0Pt,*hJet0Pt_HltMB,*hJet0Pt_HltJet6U,*hJet0Pt_HltJet15U,*hJet0Pt_HltJet30U,*hJet0Pt_HltJet50U;
    hs1D["hJet0Pt"] = hJet0Pt;
    hs1D["hJet0Pt_HltJet6U"] = hJet0Pt_HltJet6U;
    hs1D["hJet0Pt_HltJet15U"] = hJet0Pt_HltJet15U;
    hs1D["hJet0Pt_HltJet30U"] = hJet0Pt_HltJet30U;
    hs1D["hJet0Pt_HltJet50U"] = hJet0Pt_HltJet50U;
    inFile->GetObject(histDir+"hJet0Pt",hs1D["hJet0Pt"]);
    inFile->GetObject(histDir+"hJet0Pt_HltJet6U",hs1D["hJet0Pt_HltJet6U"]);
    inFile->GetObject(histDir+"hJet0Pt_HltJet15U",hs1D["hJet0Pt_HltJet15U"]);
    inFile->GetObject(histDir+"hJet0Pt_HltJet30U",hs1D["hJet0Pt_HltJet30U"]);
    inFile->GetObject(histDir+"hJet0Pt_HltJet50U",hs1D["hJet0Pt_HltJet50U"]);

    Int_t ngroups = reBinFactor; // 1-->0.5GeV per bin
    map<TString, TH1F*>::iterator iter;
    for (iter=hs1D.begin(); iter != hs1D.end(); ++iter) {
      // check
      assert(iter->second);
      cout << iter->first << " " << iter->second->GetName() << endl;
      // rebin
      iter->second->Rebin(ngroups);
    }
  } else {
    TChain * nt_jet = new TChain("trackAna/nt_jet","ntuple: jets");
    nt_jet->Add(inFileName);
    //nt_jet->Print();
    hs1D["hJet0Pt"] = new TH1F("hJet0Pt","jet p_{T}; p_{T}^{corr jet} [GeV/c]", numPtBins, 0.0, histJetEtMax);
    hs1D["hJet0Pt_HltJet6U"] = new TH1F("hJet0Pt_HltJet6U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", numPtBins, 0.0, histJetEtMax);
    hs1D["hJet0Pt_HltJet15U"] = new TH1F("hJet0Pt_HltJet15U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", numPtBins, 0.0, histJetEtMax);
    hs1D["hJet0Pt_HltJet30U"] = new TH1F("hJet0Pt_HltJet30U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", numPtBins, 0.0, histJetEtMax);
    hs1D["hJet0Pt_HltJet50U"] = new TH1F("hJet0Pt_HltJet50U","jet p_{T}; p_{T}^{corr jet} [GeV/c]", numPtBins, 0.0, histJetEtMax);
    cout << "Analysis on " << nt_jet->GetEntries() << " events" << endl;
    /*
    nt_jet->Draw("jet>>hJet0Pt","abs(jeta)<2.5","goff");
    nt_jet->Draw("jet>>hJet0Pt_HltJet6U","abs(jeta)<2.5 && jet6 && jet>6","goff");
    nt_jet->Draw("jet>>hJet0Pt_HltJet15U","abs(jeta)<2.5 && jet15 && jet>15","goff");
    nt_jet->Draw("jet>>hJet0Pt_HltJet30U","abs(jeta)<2.5 && jet30 && jet>30","goff");
    nt_jet->Draw("jet>>hJet0Pt_HltJet50U","abs(jeta)<2.5 && jet50 && jet>50","goff");
    */
    nt_jet->Draw("jet>>hJet0Pt","","goff");
    nt_jet->Draw("jet>>hJet0Pt_HltJet6U","jet6 && jet>6","goff");
    nt_jet->Draw("jet>>hJet0Pt_HltJet15U","jet15 && jet>15","goff");
    nt_jet->Draw("jet>>hJet0Pt_HltJet30U","jet30 && jet>30","goff");
    nt_jet->Draw("jet>>hJet0Pt_HltJet50U","jet50 && jet>50","goff");
  }


  // === Begin Ana ===
  TCanvas * cJetPt = new TCanvas("cJetPt","cJetPt",500,500);
  CPlot cpJetPt("JetPt","Jet Pt","p_{T}^{corr jet} [GeV/c]","# evt");
  cpJetPt.SetLogy(1);
  cpJetPt.SetXRange(0,histJetEtMax);
  cpJetPt.AddHist1D(hs1D["hJet0Pt"],"MB_HF","",kViolet+2);
  cpJetPt.AddHist1D(hs1D["hJet0Pt_HltJet6U"],"HLT_L1Jet6U","",kAzure+6);
  cpJetPt.AddHist1D(hs1D["hJet0Pt_HltJet15U"],"HLT_Jet15U","",kGreen-3);
  cpJetPt.AddHist1D(hs1D["hJet0Pt_HltJet30U"],"HLT_Jet30U","",kOrange-5);
  cpJetPt.AddHist1D(hs1D["hJet0Pt_HltJet50U"],"HLT_Jet50U","",kRed-2);
  //cpJetPt.SetLegendHeader("Ak5 Calojets |#eta|<2.5");
  cpJetPt.SetLegendHeader("Ak5 Calojets");
  cpJetPt.Draw(cJetPt,true,"gif");

  // === HLT Eff Ana ===
  map<TString, TGraphAsymmErrors* > gAEs;
  gAEs["gHltEff_HltJet6U"] = new TGraphAsymmErrors();
  gAEs["gHltEff_HltJet15U"] = new TGraphAsymmErrors();
  gAEs["gHltEff_HltJet30U"] = new TGraphAsymmErrors();
  gAEs["gHltEff_HltJet50U"] = new TGraphAsymmErrors();
  // Calc Hlt Eff
  gAEs["gHltEff_HltJet6U"]->BayesDivide(hs1D["hJet0Pt_HltJet6U"],hs1D["hJet0Pt"]);
  gAEs["gHltEff_HltJet15U"]->BayesDivide(hs1D["hJet0Pt_HltJet15U"],hs1D["hJet0Pt_HltJet6U"]);
  gAEs["gHltEff_HltJet30U"]->BayesDivide(hs1D["hJet0Pt_HltJet30U"],hs1D["hJet0Pt_HltJet15U"]);
  gAEs["gHltEff_HltJet50U"]->BayesDivide(hs1D["hJet0Pt_HltJet50U"],hs1D["hJet0Pt_HltJet30U"]);
  map<TString, TGraphAsymmErrors*>::iterator ig;
  for (ig=gAEs.begin(); ig != gAEs.end(); ++ig) {
    // check
    assert(ig->second);
    ig->second->SetName(ig->first);
    cout << ig->first << " " << ig->second->GetName() << endl;
    clearXErrorBar(ig->second);
  }

  TCanvas * cHltEff = new TCanvas("cHltEff","cHltEff",500,500);
  CPlot cpHltEff("HltEff","Hlt Eff","p_{T}^{corr jet} [GeV/c]","#epsilon_{HLT}");
  cpHltEff.SetXRange(0,histJetEtMax);
  cpHltEff.SetYRange(0,1.1);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet6U"],"HLT_L1Jet6U","pz",kAzure+6);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet15U"],"HLT_Jet15U","pz",kGreen-3);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet30U"],"HLT_Jet30U","pz",kOrange-5);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet50U"],"HLT_Jet50U","pz",kRed-2);
  cpHltEff.SetLegendHeader("Ak5 Calojets");
  cpHltEff.SetLegend(0.58,0.21,0.91,0.50);
  cpHltEff.Draw(cHltEff,true,"gif");

  // All done, save hists
  TFile * outf = new TFile(Form("%s/anahlt.root",outdir.Data()),"RECREATE");
  for (map<TString, TH1F*>::iterator 
      iter=hs1D.begin(); iter != hs1D.end(); ++iter) {
    iter->second->Write();
  }
  for (map<TString, TGraphAsymmErrors*>::iterator
      ig=gAEs.begin(); ig != gAEs.end(); ++ig) {
    ig->second->Write();
  }
  outf->Close();
}
