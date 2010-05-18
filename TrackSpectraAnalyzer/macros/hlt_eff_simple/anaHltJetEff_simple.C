#include <TROOT.h>                   // access to gROOT, entry point to ROOT system
#include <TStyle.h>                  // class to handle plot styles in ROOT
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TGraphAsymmErrors.h"

#include "CPlot.h"           // helper class for plots

void anaHltJetEff_simple(const char * inFileName = "../process_aod/outputs/trkhists_try7_50k.root",
    TString outdir="plots")
{
  CPlot::sOutDir = outdir;
  TFile * inFile = new TFile(inFileName);

  // Plot Jet Pt distributions from various triggers
  TH1F *hJet0Pt,*hJet0Pt_HltMB,*hJet0Pt_HltJet6U,*hJet0Pt_HltJet15U,*hJet0Pt_HltJet30U,*hJet0Pt_HltJet50U;
  inFile->GetObject("trackAna/hJet0Pt",hJet0Pt);
  inFile->GetObject("trackAna/hJet0Pt_HltMB",hJet0Pt_HltMB);
  inFile->GetObject("trackAna/hJet0Pt_HltJet6U",hJet0Pt_HltJet6U);
  inFile->GetObject("trackAna/hJet0Pt_HltJet15U",hJet0Pt_HltJet15U);
  inFile->GetObject("trackAna/hJet0Pt_HltJet30U",hJet0Pt_HltJet30U);
  inFile->GetObject("trackAna/hJet0Pt_HltJet50U",hJet0Pt_HltJet50U);
  Int_t ngroups = 8; // 1-->0.5GeV per bin
  hJet0Pt->Rebin(ngroups);
  hJet0Pt_HltMB->Rebin(ngroups);
  hJet0Pt_HltJet6U->Rebin(ngroups);
  hJet0Pt_HltJet15U->Rebin(ngroups);
  hJet0Pt_HltJet30U->Rebin(ngroups);
  hJet0Pt_HltJet50U->Rebin(ngroups);

  TCanvas * cJetPt = new TCanvas("cJetPt","cJetPt",500,500);
  CPlot cpJetPt("JetPt","Jet Pt","p_{T}^{corr jet} [GeV/c]","# evt");
  cpJetPt.SetLogy(1);
  cpJetPt.SetXRange(0,200);
  cpJetPt.AddHist1D(hJet0Pt,"All Jets |#eta|<3","",kViolet+2);
  cpJetPt.AddHist1D(hJet0Pt_HltMB,"HLT_MinBiasBSC","",kBlue);
  cpJetPt.AddHist1D(hJet0Pt_HltJet6U,"HLT_L1Jet6U","",kAzure+6);
  cpJetPt.AddHist1D(hJet0Pt_HltJet15U,"HLT_Jet15U","",kGreen-3);
  cpJetPt.AddHist1D(hJet0Pt_HltJet30U,"HLT_Jet30U","",kOrange-5);
  cpJetPt.AddHist1D(hJet0Pt_HltJet50U,"HLT_Jet50U","",kOrange-3);
  cpJetPt.Draw(cJetPt,true,"gif");

  // Calc Hlt Eff
  TGraphAsymmErrors *gHltEff_HltMB = new TGraphAsymmErrors();
  TGraphAsymmErrors *gHltEff_HltJet6U = new TGraphAsymmErrors();
  TGraphAsymmErrors *gHltEff_HltJet15U = new TGraphAsymmErrors();
  TGraphAsymmErrors *gHltEff_HltJet30U = new TGraphAsymmErrors();
  TGraphAsymmErrors *gHltEff_HltJet50U = new TGraphAsymmErrors();
  gHltEff_HltMB->BayesDivide(hJet0Pt_HltMB,hJet0Pt);
  gHltEff_HltJet6U->BayesDivide(hJet0Pt_HltJet6U,hJet0Pt);
  gHltEff_HltJet15U->BayesDivide(hJet0Pt_HltJet15U,hJet0Pt_HltJet6U);
  gHltEff_HltJet30U->BayesDivide(hJet0Pt_HltJet30U,hJet0Pt_HltJet15U);
  gHltEff_HltJet50U->BayesDivide(hJet0Pt_HltJet50U,hJet0Pt_HltJet30U);

  TCanvas * cHltEff = new TCanvas("cHltEff","cHltEff",500,500);
  CPlot cpHltEff("HltEff","Hlt Eff","p_{T}^{corr jet} [GeV/c]","#epsilon_{HLT}");
  cpHltEff.SetXRange(0,200);
  cpHltEff.SetYRange(0,1.2);
  cpHltEff.AddGraph(gHltEff_HltMB,"HLT_MinBiasBSC","pz",kBlue);
  cpHltEff.AddGraph(gHltEff_HltJet6U,"HLT_L1Jet6U","pz",kAzure+6);
  cpHltEff.AddGraph(gHltEff_HltJet15U,"HLT_Jet15U","pz",kGreen-3);
  cpHltEff.AddGraph(gHltEff_HltJet30U,"HLT_Jet30U","pz",kOrange-5);
  cpHltEff.AddGraph(gHltEff_HltJet50U,"HLT_Jet50U","pz",kOrange-3);
  cpHltEff.Draw(cHltEff,false);
}
