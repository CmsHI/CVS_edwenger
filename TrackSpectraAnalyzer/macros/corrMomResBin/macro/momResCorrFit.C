void momResCorrFit()
{
  float ptMin=0,ptMax=170;
  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  TF1 * fResCorr = new TF1("fResCorr","[0]+[1]*x+[2]*x^2+[3]*x^3+[4]*x^4",ptMin,ptMax);
  fResCorr->SetParameters(1.00418,-6.5107e-05,4.36783e-06,-4.29539e-08,1.50819e-10);
  fResCorr->SetMinimum(0.9);
  fResCorr->SetMaximum(1.1);
  fResCorr->Draw();

  TF1 * fResCorr2 = new TF1("fResCorr2","[0]+[1]*x+[2]*x^2+[3]*x^3+[4]*x^4",ptMin,ptMax);
  fResCorr2->SetParameters(1.00418,-6.53075e-05,4.3842e-06,-4.30483e-08,1.50985e-10);
  fResCorr2->SetLineColor(2);
  fResCorr2->Draw("same");

  TCanvas * c3 = new TCanvas("c3","c3",500,500);
  TF1 *fRat = new TF1("fRat","fResCorr2/fResCorr",ptMin,ptMax);
  fRat->Draw();
}
