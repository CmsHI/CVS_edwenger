#include <TROOT.h>                   // access to gROOT, entry point to ROOT system
#include <TStyle.h>                  // class to handle plot styles in ROOT
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TChain.h"
#include "TGraphAsymmErrors.h"
#include "TCut.h"

#include "edwenger/TrackSpectraAnalyzer/macros/trigRawSpec/CPlot.h"           // helper class for plots
#include "edwenger/TrackSpectraAnalyzer/macros/trigRawSpec/HisTGroup.h"
#include "edwenger/TrackSpectraAnalyzer/macros/trigRawSpec/tgraphTools.C"

#include <map>
#include <cassert>
using namespace std;

void anaHltJetEff_simple(TString sampleName="Data",
    const char * inFileName = "/net/hibat0003/d00/scratch/frankma/data/MinimumBias/MB-C10-PR9-MBskim-v0_proc0628_trkAnaNoFilter/trkhists_trkAnaSkimAOD_*.root",
    TString outdir="plots/MB-C10-PR9-MBskim-v0_p0628_a2",
    TString histDir = "trackAna/",
    Bool_t useHist = false)
{
  CPlot::sOutDir = outdir;
  Float_t histJetEtMax = 300;
  Int_t numPtBins=75;
  TH1::SetDefaultSumw2();

  HisTGroup<TH1D> hgJet0Et("Jet0Et",numPtBins,0,histJetEtMax);
  // === Checks ===
  // Plot Jet Pt distributions from various triggers
  if (useHist) {
    TFile * inFile = new TFile(inFileName);
    return;
  } else {
    TChain * nt_jet = new TChain(histDir+"nt_jet","ntuple: jets");
    nt_jet->Add(inFileName);
    //nt_jet->Print();
    hgJet0Et.Add1D("MB");
    hgJet0Et.Add1D("HltJet15U");
    hgJet0Et.Add1D("HltJet30U");
    hgJet0Et.Add1D("HltJet50U");
    cout << "Analysis on " << nt_jet->GetEntries() << " events" << endl;

    TCut baseJetSel="mb";
    //TCut baseJetSel="jet15";
    nt_jet->Draw(Form("jet>>%s",hgJet0Et.GetH("MB")->GetName()),baseJetSel,"goff");
    nt_jet->Draw(Form("jet>>%s",hgJet0Et.GetH("HltJet15U")->GetName()),baseJetSel&&"jet15 && jet>10","goff");
    baseJetSel="jet15";
    nt_jet->Draw(Form("jet>>%s",hgJet0Et.GetH("HltJet30U")->GetName()),baseJetSel&&"jet30 && jet>20","goff");
    nt_jet->Draw(Form("jet>>%s",hgJet0Et.GetH("HltJet50U")->GetName()),baseJetSel&&"jet50 && jet>30","goff");
  }


  // ===Plotting Pad ===
  TCanvas * cJetTurnOn = new TCanvas("cJetTurnOn","cJetTurnOn",510,640);
  cJetTurnOn->cd();
  TPad * pUpper = new TPad("pUpper","pUpper",0,0.34,1,1,0,0,0);
  pUpper->SetBottomMargin(0);
  pUpper->Draw();
  pUpper->cd();
  pUpper->SetNumber(1);
  cJetTurnOn->cd();
  TPad * pLower = new TPad("pLower","pLower",0,0,1,0.34,0,0,0);
  pLower->SetTopMargin(0);
  pLower->SetBottomMargin(0.1*(1./0.34));
  pLower->Draw();
  pLower->cd();
  pLower->SetNumber(2);

  // === Begin Ana ===
  CPlot::sPlotStyle = 50;
  pUpper->cd();
  CPlot cpJetPt("JetPt","Jet Pt","E_{T}^{corr jet} [GeV/c]","# Events/4 GeV");
  cpJetPt.SetLogy(1);
  cpJetPt.SetXRange(0,histJetEtMax);
  cpJetPt.AddHist1D(hgJet0Et.H("MB"),"MinBias All","E",kViolet+2);
  cpJetPt.AddHist1D(hgJet0Et.H("HltJet15U"),"HLT: Jet15^{Raw}","E",kGreen-3);
  cpJetPt.AddHist1D(hgJet0Et.H("HltJet30U"),"HLT: Jet30^{Raw}","E",kOrange-5);
  cpJetPt.AddHist1D(hgJet0Et.H("HltJet50U"),"HLT: Jet50^{Raw}","E",kRed-2);
  cpJetPt.SetLegendHeader(sampleName);
  cpJetPt.SetLegend(0.58,0.54,0.98,0.82);
  cpJetPt.SetAxisLabeling(15,63,18,63,4,2);
  cpJetPt.Draw(pUpper,false);

  // === HLT Eff Ana ===
  map<TString, TGraphAsymmErrors* > gAEs;
  gAEs["gHltEff_HltJet15U"] = new TGraphAsymmErrors();
  gAEs["gHltEff_HltJet30U"] = new TGraphAsymmErrors();
  gAEs["gHltEff_HltJet50U"] = new TGraphAsymmErrors();
  // Calc Hlt Eff
  gAEs["gHltEff_HltJet15U"]->BayesDivide(hgJet0Et.H("HltJet15U"),hgJet0Et.H("MB"));
  gAEs["gHltEff_HltJet30U"]->BayesDivide(hgJet0Et.H("HltJet30U"),hgJet0Et.H("HltJet15U"));
  gAEs["gHltEff_HltJet50U"]->BayesDivide(hgJet0Et.H("HltJet50U"),hgJet0Et.H("HltJet30U"));
  map<TString, TGraphAsymmErrors*>::iterator ig;
  for (ig=gAEs.begin(); ig != gAEs.end(); ++ig) {
    // check
    assert(ig->second);
    ig->second->SetName(ig->first);
    cout << ig->first << " " << ig->second->GetName() << endl;
    clearXErrorBar(ig->second);
  }

  pLower->cd();
  CPlot::sPlotStyle = 0;
  CPlot cpHltEff("HltEff","Hlt Eff","E_{T}^{corr jet} [GeV/c]","ratio");
  cpHltEff.SetXRange(0,histJetEtMax);
  cpHltEff.SetYRange(0,1.1);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet15U"],"HLT: Jet15U","pz",kGreen-3);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet30U"],"HLT: Jet30U","pz",kOrange-5);
  cpHltEff.AddGraph(gAEs["gHltEff_HltJet50U"],"HLT: Jet50U","pz",kRed-2);
  cpHltEff.SetLegendHeader(sampleName);
  cpHltEff.ShowLegend(0);
  cpHltEff.SetAxisLabeling(15,63,18,63,4,2);
  cpHltEff.Draw(pLower,false);

  // All done, save hists
  gSystem->mkdir(outdir.Data(),true);
  cJetTurnOn->Print(Form("%s/cJetTurnOn.gif",outdir.Data()));
  cJetTurnOn->Print(Form("%s/cJetTurnOn.pdf",outdir.Data()));
  TFile * outf = new TFile(Form("%s/anahlt.root",outdir.Data()),"RECREATE");
  hgJet0Et.Save();
  for (map<TString, TGraphAsymmErrors*>::iterator
      ig=gAEs.begin(); ig != gAEs.end(); ++ig) {
    ig->second->Write();
  }
  outf->Close();
}
