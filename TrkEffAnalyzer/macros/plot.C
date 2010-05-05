void plot() {

  set_plot_style();

  TFile *f = new TFile("../trees.root");
  TTree *tsim = (TTree*) f->Get("simTrackTree");
  TTree *trec = (TTree*) f->Get("recTrackTree");

  // sim-to-reco hists
  TH2F *hSim = new TH2F("hSim","Primary Charged Sim Tracks; #eta;p_{T} [GeV/c]",60,-3.,3.,80,0.,10.);
  tsim->Draw("pts:etas>>hSim","","goff");
  TH2F *hAcc = new TH2F("hAcc","Tracking Acceptance; #eta;p_{T} [GeV/c]",60,-3.,3.,80,0.,10.);
  tsim->Draw("pts:etas>>hAcc","acc","goff");
  TH2F *hEff = new TH2F("hEff","Tracking Efficiency; #eta;p_{T} [GeV/c]",60,-3.,3.,80,0.,10.);
  tsim->Draw("pts:etas>>hEff","nrec>0","goff");
  TH2F *hMul = new TH2F("hMul","Multiple Reconstruction; #eta;p_{T} [GeV/c]",60,-3.,3.,80,0.,10.);
  tsim->Draw("pts:etas>>hMul","nrec>1","goff");

  // reco-to-sim hists
  TH2F *hRec = new TH2F("hRec","Reconstructed Tracks; #eta;p_{T} [GeV/c]",60,-3.,3.,80,0.,10.);
  trec->Draw("ptr:etar>>hRec","","goff");
  TH2F *hFak = new TH2F("hFak","Fake Reconstruction; #eta;p_{T} [GeV/c]",60,-3.,3.,80,0.,10.);
  trec->Draw("ptr:etar>>hFak","nsim==0","goff");

  // ratio histograms
  TH2F *rAcc = (TH2F*) hAcc->Clone("rAcc");
  TH2F *rEff = (TH2F*) hEff->Clone("rEff");
  TH2F *rMul = (TH2F*) hMul->Clone("rMul");
  TH2F *rFak = (TH2F*) hFak->Clone("rFak");

  //---------------------------------------------

  // acceptance fraction
  TCanvas *c1 = new TCanvas("c1","c1",600,500);
  gPad->SetRightMargin(0.15);
  rAcc->Divide(hAcc,hSim,1,1,"B");
  rAcc->SetStats(0);
  rAcc->SetMaximum(1.0); rAcc->SetMinimum(0.0);
  rAcc->Draw("colz");

  // reco efficiency fraction
  TCanvas *c2 = new TCanvas("c2","c2",600,500);
  gPad->SetRightMargin(0.15);
  rEff->Divide(hEff,hAcc,1,1,"B");
  rEff->SetStats(0);
  rEff->SetMaximum(1.0); rEff->SetMinimum(0.0);
  rEff->Draw("colz");

  // multiple reco fraction
  TCanvas *c3 = new TCanvas("c3","c3",600,500);
  gPad->SetRightMargin(0.15);
  rMul->Divide(hMul,hAcc,1,1,"B");
  rMul->SetStats(0);
  rMul->SetMaximum(1.0); rMul->SetMinimum(0.0);
  rMul->Draw("colz");

  // fake reco fraction
  TCanvas *c4 = new TCanvas("c4","c4",600,500);
  gPad->SetRightMargin(0.15);
  rFak->Divide(hFak,hRec,1,1,"B");
  rFak->SetStats(0);
  rFak->SetMaximum(1.0); rFak->SetMinimum(0.0);
  rFak->Draw("colz");

}


void set_plot_style() {

  // nicer colz plots

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

}
