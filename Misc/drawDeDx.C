void drawDeDx() {

  TFile *f = new TFile("dEdx_HI_output.root");
  TTree *t = (TTree*) f->Get("anapxl/track_tree");
  
  // dE/dx vs. momentum Plot

  TCanvas *cDeDx = new TCanvas("cDeDx","dE/dx distribution",550,500);

  TH2D *hdedx = new TH2D("hdedx",";p (GeV/c); dE/dx (MeV/cm)",450,0,1.5,400,0,20);
  hdedx->SetMinimum(0.99);

  t->Draw("dedx:p>>hdedx","","colz");
  gPad->SetLogz();

  TF1 *fP = new TF1("fP","[1]*([0]*[0]/x/x)+[2]",0.2,2); // [0]=mass, [1]=K, [2]=C
  fP->SetParameters(0.9383,2.468,2.679);
  fP->SetLineWidth(2);

  TF1 *fK = new TF1("fK","[1]*([0]*[0]/x/x)+[2]",0.2,2); 
  fK->SetParameters(0.4937,fP->GetParameter(1),fP->GetParameter(2));
  fK->SetLineWidth(2);

  TF1 *fPi = new TF1("fPi","[1]*([0]*[0]/x/x)+[2]",0.2,2); 
  fPi->SetParameters(0.1396,fP->GetParameter(1),fP->GetParameter(2));
  fPi->SetLineWidth(2);

  fP->Draw("same");
  fK->Draw("same");
  fPi->Draw("same");

  /// Mass Plot

  TString formula = Form("p*sqrt((dedx-%f)/%f)",fP->GetParameter(2),fP->GetParameter(1));
  t->SetAlias("mass",formula.Data());
  
  TH1D *hmass = new TH1D("hmass","particles with p=0.2-0.5 GeV/c;mass (GeV/c^{2})",150,0.0,1.5);
  TCanvas *cMass = new TCanvas("cMass","mass distribution",550,500);
  t->Draw("mass>>hmass","p>0.2 && p<0.5 && dedx<20");
  gPad->SetLogy();


}
