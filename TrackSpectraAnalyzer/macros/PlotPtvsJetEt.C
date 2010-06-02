//-------------------------------------------------
// June 02 2010, Andre S. Yoon
// plot track pT and leading jet Et cooreation  
// MC vs DATA
//-------------------------------------------------

#include "commonStyle.h"

void PlotPtvsJetEt(){


   gStyle->SetPadRightMargin(0.13); // to show rainbow scale


   //------------ Canvas && dummy hist 
   TCanvas *cMC = new TCanvas("cMC","cMC",490,550);
   TCanvas *cDATA = new TCanvas("cDATA","cDATA",490,550);

   Char_t xTitle[100],yTitle[100];
   sprintf(yTitle,"Track p_{T} [GeV/c]");
   sprintf(xTitle,"Corrected leading Jet E_{T} [GeV]");

   TH1D *dum_MC = GetDummyHist(0,300,0,100,xTitle,yTitle);
   TH1D *dum_DATA = GetDummyHist(0,300,0,100,xTitle,yTitle);

   TLatex * tex;
   tex = new TLatex(0.64,0.88,"CMS Preliminary");
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->SetNDC();


   //------------- MC
   TFile *fMC = new TFile("./../root_files/TrkHistMC_QCD_10M.root");
   TH3F *hTrkPtEtaJetEt_MC = (TH3F*) fMC->Get("trackAna_mc/threeDHist/hTrkPtEtaJetEt");

   cMC->cd();
   cMC->SetLogz();
   dum_MC->Draw();
   TH2D *hPtEt_MC = (TH2D*) hTrkPtEtaJetEt_MC->Project3D("yze");//->Draw("COLZ");
   hPtEt_MC->Draw("COLZsame");

   char label_MC[100];
   sprintf(label_MC,"PYTHIA 7 TeV");
   TLegend *leg_MC = (TLegend*) GetDummyLegend(label_MC,hPtEt_MC);
   leg_MC->Draw();

   tex->Draw();

   //-------------- DATA       
   TFile *fDATA = new TFile("./../root_files/MB-C10-Merged.root");
   TH3F *hTrkPtEtaJetEt_DATA = (TH3F*) fDATA->Get("trackAna/threeDHist/hTrkPtEtaJetEt");
   
   cDATA->cd();
   cDATA->SetLogz();
   dum_DATA->Draw();
   TH2D *hPtEt_DATA = (TH2D*) hTrkPtEtaJetEt_DATA->Project3D("yze");
   hPtEt_DATA->Draw("COLZsame");

   char label_DATA[100];
   sprintf(label_DATA,"DATA 7 TeV");
   TLegend *leg_DATA = (TLegend*) GetDummyLegend(label_DATA,hPtEt_DATA);
   leg_DATA->Draw();

   tex->Draw();


   //--------------- Save figures
   char filenameMC[100];
   sprintf(filenameMC,"pt_jet_correlation_MC");
   printCanvases(cMC,filenameMC,0);
   
   char filenameDATA[100];
   sprintf(filenameDATA,"pt_jet_correlation_DATA");
   printCanvases(cDATA,filenameDATA,0);



}


TH1D* GetDummyHist(float xmin, float xmax, float ymin, float ymax, char *xttl, char *yttl) {

   TH1D *dum;
   dum = new TH1D("dum","",100,xmin,xmax);

   dum->SetMinimum(ymin);
   dum->SetMaximum(ymax);
   dum->SetStats(0);

   dum->GetYaxis()->SetTitle(yttl);
   dum->GetYaxis()->CenterTitle();
   dum->GetXaxis()->SetTitle(xttl);
   dum->GetXaxis()->CenterTitle();

   return dum;

}

TLegend* GetDummyLegend(char *label, TH1* hist){

   TLegend *leg1 = new TLegend(0.18,0.74,0.55,0.93);
   leg1->SetMargin(0.3);
   leg1->SetBorderSize(0);
   leg1->SetFillColor(kWhite);
   leg1->SetFillStyle(0);
   leg1->SetTextSize(0.038);
   leg1->SetMargin(0.3);
   leg1->AddEntry(hist,label,"");

   return leg1;
}
