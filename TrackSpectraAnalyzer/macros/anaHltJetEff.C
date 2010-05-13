#include "commonStyle.h"

void anaHltJetEff(){

   //gROOT->Reset();
   //gROOT->ProcessLine(".x rootlogon_division.C");

   bool mc = true;
   bool spectra = true;

   Int_t color[12]={kViolet+2,kBlue,kAzure+6,kGreen-3,kOrange-5,kOrange-3,kOrange+4,kRed-3,kRed,1,1,1};
   //TFile *f1 = new TFile("../root_files/trkAna_test.root");
   //TFile *f1 = new TFile("../root_files/ANA_v4_10M_2nd.root");
   //TFile *f1 = new TFile("../root_files/ANA_v4_10M_1st_DATA.root"); 
    TFile *f1 = TFile::Open("dcache:/net/hisrv0001/home/edwenger/t2bat/MinimumBias/MB-C10-PR9-TRKANASKIM-v2/38a0275e6a2a62980cf9b1868edbd493/ROOTuple_HighPurity_49_1.root");

   //data
   //TFile *f1 = new TFile("../root_files/output_trkAnaSkimAOD_HighStat_etaMax2.5_D0999.0_DZ999.0_pTerr0.1_nHits8_nVtxTrk3.0_generalTracks.root");
   //TFile *f1 = new TFile("../root_files/output_trkAnaSkimAOD_HighStat_DATA_etaMax2.5_D0999.0_DZ999.0_pTerr0.1_nHits8_nVtxTrk3.0_generalTracks.root");

   //TNtuple *ntjet = (TNtuple*) f1->Get("ntjet");
   //TDirectoryFile *dSpec = f1->GetDirectory("trackAna");
   //dSpec->cd();
   TChain * nt_jet = new TChain("trackAna/nt_jet","nt_jet");
   nt_jet->Add("dcache:///net/hisrv0001/home/edwenger/t2bat/MinimumBias/MB-C10-PR9-TRKANASKIM-v2/38a0275e6a2a62980cf9b1868edbd493/ROOTuple_HighPurity_*.root");
   cout << "# of events: " << nt_jet->GetEntries() << endl;

   TH1D *hJetMB = new TH1D("hJetMB","",400,0,200);
   TH1D *hJet6U = new TH1D("hJet6U","",400,0,200);
   TH1D *hJet15U = new TH1D("hJet15U","",400,0,200);
   TH1D *hJet30U = new TH1D("hJet30U","",400,0,200);
   TH1D *hJet50U = new TH1D("hJet50U","",400,0,200);


   hJetMB->Sumw2();
   hJet6U->Sumw2();
   hJet15U->Sumw2();
   hJet30U->Sumw2();
   hJet50U->Sumw2();

   // leading jet spectra
   //ntjet_pre1->Draw("jet>>hJetMB","abs(jeta)<2.5 && mb==1","goff");

   nt_jet->Draw("jet>>hJetMB","abs(jeta)<2.5 && mb==1","goff"); 
   nt_jet->Draw("jet>>hJet6U","abs(jeta)<2.5 && jet6==1","goff");
   nt_jet->Draw("jet>>hJet15U","abs(jeta)<2.5 && jet15==1","goff");
   nt_jet->Draw("jet>>hJet30U","abs(jeta)<2.5 && jet30==1","goff");
   nt_jet->Draw("jet>>hJet50U","abs(jeta)<2.5 && jet50==1","goff");


   cout<<"number of bin in MB "<<hJetMB->GetNbinsX()<<endl;
   cout<<"number of bin in 6U "<<hJet6U->GetNbinsX()<<endl;

   //---------

   TCanvas *call = new TCanvas("call","call",510,640);
   call->cd();
   //TPad *call_pd = (TPad*)gPad;
   //call_pd->cd();
   //call_pd->SetLogy();

   // ------------------------------- Pad settings 

   pp1 = new TPad("p1","p1",0,0.34,1,1,0,0,0);
   pp1->SetBottomMargin(0.0);
   pp1->SetTopMargin(0.05*(1/0.72));

   pp1->Draw();
   pp1->cd();
   pp1->SetNumber(1);

   call->cd();

   pp1_1 = new TPad("p1_1","p1_1",0,0.0,1,0.34,0,0,0);
   pp1_1->SetTopMargin(0);
   pp1_1->SetBottomMargin(0.14*(1/0.34));
   pp1_1->Draw();
   pp1_1->cd();
   pp1_1->SetNumber(2);


   // ----- pad 1
   pp1->cd();
   pp1->SetLogy();

   int upper = 35;
   //int upper = 150; 
   double prescale = 1.15;

   Char_t xTitle[100],yTitle[100];

   sprintf(yTitle,"dN/dE_{T}^{Jet}");
   sprintf(xTitle,"Leading jet E_{T} (corrected)");

   //TH1D *dum = GetDummyHist(4e-3,3e5,xTitle,yTitle);
   TH1D *dum = GetDummyHist(4e-2,3e5,xTitle,yTitle);    
   dum->GetXaxis()->SetNdivisions(908);
   dum->GetYaxis()->SetNdivisions(305);
   dum->GetXaxis()->SetRange(1,upper);
   dum->Draw();
   
   
   hJetMB->Scale(prescale);
   th1Style1(hJetMB,15,20,1,15,2.8,1,1);
   th1Style1(hJet6U,16,20,1,16,2.8,1,1);
   th1Style1(hJet15U,17,20,1,17,2.8,1,1);
   th1Style1(hJet30U,18,20,1,18,2.8,1,1);
   th1Style1(hJet50U,19,20,1,19,2.8,1,1);

   // -------- pad 2
   pp1_1->cd();
   //pp1_1->SetLogy();
   sprintf(yTitle,"Ratio");

   //TH1D *dum2 = GetDummyHist(1E-10,500,xTitle,yTitle);
   TH1D *dum2 = GetDummyHist(-0.1,1.4,xTitle,yTitle);  
   //TH1D *dum2 = GetDummyHist(0.01,1.4,xTitle,yTitle);  

   /*
   dum2->GetYaxis()->SetLabelSize(0.1);
   dum2->GetYaxis()->SetTitleSize(0.12);
   dum2->GetYaxis()->SetTitleOffset(0.67);

   dum2->GetXaxis()->SetLabelSize(0.109);
   dum2->GetXaxis()->SetLabelOffset(0.05);
   dum2->GetXaxis()->SetTitleSize(0.125);
   dum2->GetXaxis()->SetTitleOffset(1.45);
   */

   dum2->GetYaxis()->SetLabelSize(0.07);
   dum2->GetYaxis()->SetTitleSize(0.10);
   dum2->GetYaxis()->SetTitleOffset(0.69);

   dum2->GetXaxis()->SetLabelSize(0.09);
   dum2->GetXaxis()->SetLabelOffset(0.05);
   dum2->GetXaxis()->SetTitleSize(0.11);
   dum2->GetXaxis()->SetTitleOffset(1.45);



   dum2->GetYaxis()->SetNdivisions(302);
   dum2->GetXaxis()->SetRange(1,upper);

   dum2->Draw();

   TH1D* hJetMB_dum = hJetMB->Clone("hJetMB_dum");
   TH1D* hJet6U_dum = hJet6U->Clone("hJet6U_dum");
   TH1D* hJet15U_dum = hJet15U->Clone("hJet15U_dum");
   TH1D* hJet30U_dum = hJet30U->Clone("hJet30U_dum");
   TH1D* hJet50U_dum = hJet50U->Clone("hJet50U_dum");

   hJetMB_dum->Sumw2();
   hJet6U_dum->Sumw2();
   hJet15U_dum->Sumw2();
   hJet30U_dum->Sumw2();
   hJet50U_dum->Sumw2();

   cout<<"number of bin in MB_dum "<<hJetMB_dum->GetNbinsX()<<endl;
   cout<<"number of bin in 6U_dum "<<hJet6U_dum->GetNbinsX()<<endl;


   hJet6U_dum->Divide(hJetMB);
   hJet15U_dum->Divide(hJet6U);
   hJet30U_dum->Divide(hJet15U);
   hJet50U_dum->Divide(hJet30U);

   th1Style1(hJet6U_dum,16,20,1,16,2.8,1,1);
   th1Style1(hJet15U_dum,17,20,1,17,2.8,1,1);
   th1Style1(hJet30U_dum,18,20,1,18,2.8,1,1);
   th1Style1(hJet50U_dum,19,20,1,19,2.8,1,1);
   
   double cut = 0.99;
   //double cut = 0.6;  
   double pt_mb = 0;
   double pt_6u = 0;
   double pt_15u= 0;
   double pt_30u= 0;
   double pt_50u= 0;

   pt_6u = GetJetPtMin(hJet6U_dum,cut);
   pt_15u = GetJetPtMin(hJet15U_dum,cut);
   pt_30u = GetJetPtMin(hJet30U_dum,cut);
   pt_50u = GetJetPtMin(hJet50U_dum,cut);


   pp1->cd();

   TLegend *leg1 = new TLegend(0.48,0.45,0.87,0.91);
   leg1->SetMargin(0.3);
   leg1->SetBorderSize(0);
   leg1->SetFillColor(kWhite);
   leg1->SetFillStyle(0);
   leg1->SetTextSize(0.039);
   leg1->SetMargin(0.3);
   leg1->SetHeader("pp #rightarrow Jet+X, 7 TeV, | #eta|<2.5");
   //leg1->AddEntry(hJetMB,"MB BSC (prescale - 10)","pl");
   leg1->AddEntry(hJetMB,"MB BSC","pl");       
   leg1->AddEntry(hJet6U,"JET6U","pl");
   leg1->AddEntry(hJet15U,"JET15U","pl");
   leg1->AddEntry(hJet30U,"JET30U","pl");
   leg1->AddEntry(hJet50U,"JET50U","pl");
   leg1->Draw();


   if(mc){
      printCanvases(call, "jet_turn_on",0);
      //printCanvases(call2, "jet_turn_on_spectra",0);
   }else{
      printCanvases(call, "jet_turn_on_DATA",0);
      //printCanvases(call2, "jet_turn_on_spectra_DATA",0);
   }

}

TH1D* GetDummyHist(Float_t min, Float_t max,Char_t *xttl,Char_t *yttl) {

   TH1D *dum;
   dum = new TH1D("dum","",100,0.0,200);
   //dum = new TH1D("dum","",100,0.0,16);                                                                                          
   dum->SetMinimum(min);
   dum->SetMaximum(max);
   dum->SetStats(0);

   dum->GetYaxis()->SetTitle(yttl);
   dum->GetYaxis()->CenterTitle();
   dum->GetXaxis()->SetTitle(xttl);
   dum->GetXaxis()->CenterTitle();

   return dum;
}

TH1D* GetDummyHist2(Float_t min, Float_t max,Char_t *xttl,Char_t *yttl) {

   TH1D *dum2;
   dum2 = new TH1D("dum2","",100,0.0,40);
   //dum = new TH1D("dum","",100,0.0,16);                                                                                                                       
   dum2->SetMinimum(min);
   dum2->SetMaximum(max);
   dum2->SetStats(0);

   dum2->GetYaxis()->SetTitle(yttl);
   dum2->GetYaxis()->CenterTitle();
   dum2->GetXaxis()->SetTitle(xttl);
   dum2->GetXaxis()->CenterTitle();

   return dum2;
}

double GetJetPtMin(TH1D *hist, double cut1){
   
   int bins = hist->GetNbinsX();
   double minpt = 0;

   for(int it = 1; it< bins+1; it++){
      double ratio = hist->GetBinContent(it);
      //cout<<"ratio = "<<ratio<<endl;
      if(ratio>cut1){
         cout<<"jet pt with ratio > "<<cut1<<" = "<<hist->GetBinCenter(it)<<endl;
	 minpt = hist->GetBinCenter(it);
	 return  minpt;
      }
   }
}


