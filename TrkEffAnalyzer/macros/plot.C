void plot() {

  set_plot_style();

  TFile *f = new TFile("../trees.root");
  TTree *tsim = (TTree*) f->Get("simTrackTree");
  TTree *trec = (TTree*) f->Get("recTrackTree");

  tsim->Print();

  TH2F *hSim = new TH2F("hSim","Primary Charged Sim Tracks; #eta;p_{T} [GeV/c]",60,-3.,3.,80,0.,10.);
  tsim->Draw("pts:etas>>hSim","","goff");
  TH2F *hEff = new TH2F("hEff","Tracking Efficiency; #eta;p_{T} [GeV/c]",60,-3.,3.,80,0.,10.);
  tsim->Draw("pts:etas>>hEff","nrec>0","goff");

  hEff->Divide(hEff,hSim,1,1,"B");
  hEff->SetStats(0);
  hEff->SetMaximum(1.0); hEff->SetMinimum(0.0);

  TCanvas *c1 = new TCanvas("c1","c1",600,500);
  gPad->SetRightMargin(0.15);
  hEff->Draw("colz");

}


void set_plot_style() {

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

}
