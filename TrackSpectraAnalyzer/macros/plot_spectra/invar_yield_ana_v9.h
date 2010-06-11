//------------------------------------------------------------------- 
// June 03 2010, Andre S. Yoon 
// - header file for invar_yield_ana_v9.C
// - efficiency correction is done for tracking
// - efficiency correction is done for event selection 
// - return various histograms/tgraph in struct
//--------------------------------------------------------------------


using namespace std;
#include <utility>
//#include "/home/sungho/plots/common/utilities.h"

std::pair<TH1D*,TH1D*> CorrectForMomRes(char* file, TH1D* hist, double ieta, double feta, double xmin, double xmax){ 


   cout<<"\n"<<endl;
   cout<<"[Mom resolution and binning correction ]========================================"<<endl;

   TFile *fEVT = new TFile(file);
   TH3F *hRes3D = (TH3F*) fEVT->Get("trkEffAnalyzer/hresStoR3D");

   int binmaxeta = hRes3D->GetXaxis()->FindBin(feta);
   int binmineta = hRes3D->GetXaxis()->FindBin(-1.0*feta);

   hRes3D->GetXaxis()->SetRange(binmineta,binmaxeta);
   TH2D *hRes2D = (TH2D*) hRes3D->Project3D("zy"); 

   TF1* fitF = new TF1("fitpythia","[0]*(1+(x/[1])+(pow(x,2)/[2])+(pow(x,3)/[3])+(pow(x,4)/[4])+(pow(x,5)/[5]))^[6]",xmin,xmax);
   fitF->SetParameters(2.96361e+08,2.66339e-01,1.34086e-01,2.96428e-01,1.74015e+00,5.16931e+00,-1.70643e+00);
   fitF->SetLineColor(2);
   fitF->SetLineWidth(2);
   hist->Fit(fitF,"NR");

   int nbins = hist->GetNbinsX();
   cout<<"nbins = "<<nbins<<endl;

   for(int i=0;i<nbins;i++){

      bool nullArea = false;

      TH1D *hRes1D_temp = (TH1D*) hRes2D->ProjectionY("",i+1,i+1,"e");

      // check if works..
      int firstB = GetFirstNonEmptyBin(hRes1D_temp);
      int lastB = GetLastNonEmptyBin(hRes1D_temp);

      double area = hRes1D_temp->Integral();

      if(area==0) nullArea =true;
      else hRes1D_temp->Scale(1./area);

      for(int j=firstB;j<lastB+1;j++){

	 if(!nullArea) double corr = hRes1D_temp->GetBinContent(j);
	 else double corr = 1.0; // no information on res-> no correction

	 double pt = hRes1D_temp->GetBinCenter(j);

	 int rpt_bin = hRes2D->GetYaxis()->FindBin(pt); // should be same as j!
	 int spt_bin = i+1;
	 double smeared = corr*(fitF->Eval(pt));

	 hRes2D->SetBinContent(spt_bin,rpt_bin,smeared);
	 hRes2D->SetBinError(spt_bin,rpt_bin,0);
      }
      delete hRes1D_temp;
   }
   

   TH1D *histCorr = (TH1D*) hRes2D->ProjectionY("",1,hRes2D->GetXaxis()->GetLast(),"e");

   TH1D* hSPEC = (TH1D*) hist->Clone("hSPEC");
   TH1D* hSPECR = (TH1D*) RebinIt(hist,true);

   TH1D* histC = (TH1D*) histCorr->Clone("histC");
   TH1D* histCR = (TH1D*) RebinIt(histCorr,true);

   TH1D* histC_ratio = (TH1D*) ratio_hist_to_func(histC,fitF);
   TH1D* histCR_ratio = (TH1D*) ratio_hist_to_func(histCR,fitF);


   for(int i=0;i<histC_ratio->GetNbinsX();i++){
      double yield    = hSPEC->GetBinContent(i+1);
      double yielderr = hSPEC->GetBinError(i+1);
      double corrR = histC_ratio->GetBinContent(i+1);
      if(corrR==0) corrR = 1; // corrR=0 when yield =0
      hSPEC->SetBinContent(i+1,yield/corrR);
      hSPEC->SetBinError(i+1,yielderr/corrR);
   }

   for(int i=0;i<histCR_ratio->GetNbinsX();i++){
      double yield    = hSPECR->GetBinContent(i+1);
      double yielderr = hSPECR->GetBinError(i+1);
      double corrR = histCR_ratio->GetBinContent(i+1);
      if(corrR==0) corrR = 1;
      hSPECR->SetBinContent(i+1,yield/corrR);
      hSPECR->SetBinError(i+1,yielderr/corrR);
   }

   
   //To see how well spectra is fit and the correction factor
   TCanvas *cIn = new TCanvas("cIn","cIn",500,450);
   cIn->cd();
   cIn->SetLogy();

   hist->SetMarkerSize(0.8);
   hist->Draw("pz");
   fitF->Draw("same");

   TCanvas *cIn2 = new TCanvas("cIn2","cIn2",500,450);
   cIn2->cd();
   histC_ratio->Draw("hist");
   histCR_ratio->SetLineColor(kRed);
   histCR_ratio->Draw("histsame");

   

   cout<<"[Mom resolution and binning correction ]========================================"<<endl;
   cout<<"\n"<<endl;

   return  std::pair<TH1D*,TH1D*>(hSPEC,hSPECR);

}


int GetFirstNonEmptyBin(TH1D* hist){
   int bin = 0;
   for(int i=0;i<hist->GetNbinsX();i++){
      bin = i+1;
      //cout<<hist->GetBinContent(bin)<<endl;                                                                                                                           
      if(hist->GetBinContent(bin)>0) break;
   }
   //cout<<"bin = "<<bin<<endl;                                                                                                                                         
   return bin;
}

int GetLastNonEmptyBin(TH1D* hist){
   int bin = 0;
   for(int i=0;i<hist->GetNbinsX();i++){
      bin = (hist->GetNbinsX())-i;
      if(hist->GetBinContent(bin)>0) break;
   }
   return bin;
}


double GetEffFactor(TH3F* num, TH3F* den, double pt, double eta, double jet){

   int num_bin = num->FindBin(eta,pt,jet);
   int den_bin = den->FindBin(eta,pt,jet);

   //double num_bin_width = num->GetYaxis()->GetBinWidth(num_bin);
   //double den_bin_width = den->GetYaxis()->GetBinWidth(den_bin);

   double n_num = num->GetBinContent(num_bin);
   double n_den = den->GetBinContent(den_bin);
   if(n_den == 0  || n_num == 0 ) return 1;
   else return n_num/n_den;
}


double GetFakFactor(TH3F* num, TH3F* den, double pt, double eta, double jet){

   int num_bin = num->FindBin(eta,pt,jet);
   int den_bin = den->FindBin(eta,pt,jet);

   //double num_bin_width = num->GetYaxis()->GetBinWidth(num_bin);
   //double den_bin_width = den->GetYaxis()->GetBinWidth(den_bin);

   double n_num = num->GetBinContent(num_bin);
   double n_den = den->GetBinContent(den_bin);
   if(n_den == 0) return 1; // meaing nothing is reconstructed.                                                                                                           
   if(n_num == 0) return 0;
   else return n_num/n_den; // be careful with def, with MTV, its 1-(n_num/n_den)                                                                                         
}


double CorrectNevt(TH1F* histEVT){
   double nevt = 0;
   for(int i=0;i<histEVT->GetNbinsX();i++){
      nevt = nevt + (histEVT->GetBinContent(i+1)/histEVT->GetBinCenter(i+1));
   }
   return nevt;
}

double CorrectNevtWithMult(TH1F* histEVT){
   return (double) histEVT->Integral();
}



// defined differently

double GetZeroBinFraction(TH1F* hist){

   double total = hist->Integral();
   //double non_zerobin = hist->Integral(2,hist->GetNbinsX());
   //double fraction = (total-non_zerobin)/total;
   double  zerobin = hist->Integral(1,1);
   double fraction = (zerobin)/total;
   return fraction;
}

double GetZOTTBinFraction(TH1F* hist){

   double total = hist->Integral();
   //double non_zerobin = hist->Integral(5,hist->GetNbinsX());// i - {0,1,2,3}
   //double fraction = (total-non_zerobin)/total;
   double zerobin = hist->Integral(1,4);
   double fraction = (zerobin)/total;

   return fraction;
}

double GetOTTBinFraction(TH1F* hist){

   double total = hist->Integral(2,hist->GetNbinsX());//except zero bin 
   //double non_zerobin = hist->Integral(5,hist->GetNbinsX());
   //double fraction = (total-non_zerobin)/total;

   double zerobin = hist->Integral(1,4);
   double fraction = (zerobin)/total;

   return fraction;
}

double CorrectNevtV2(TH1F* histDen, TH1F* histNum){

   double nevt = 0;
   TH1F* histNum_dum = (TH1F*) histNum->Clone("histNum_dum");
   TGraphAsymmErrors *gNSDvtx = new TGraphAsymmErrors();

   gNSDvtx->BayesDivide(histNum,histDen);

   int n_bins = gNSDvtx->GetN();
   const int NBINS = n_bins;
   double x[NBINS], y[NBINS];

   for(int i=0;i<n_bins;i++){
      gNSDvtx->GetPoint(i,x[i],y[i]);
   }

   for(int i=0;i<histNum->GetNbinsX();i++){
      histNum_dum->SetBinContent(i+1,(1/y[i])*histNum->GetBinContent(i+1));
   }

   return histNum_dum->Integral();
}


std::pair<TH3F*,TH3F*> getEffHists(char *file, char *dirC, char *histN, char *histD){

   TFile *efile = new TFile(file,"read");
   TDirectoryFile *efileDIR = efile->GetDirectory(dirC);
   //cout<<"[getEffHists] File to be processed: "<<file<<" with directory "<<dirC<<endl;                                                                                  

   TIter next(efileDIR->GetListOfKeys());
   TKey *key;

   Char_t name[100];

   while ((key=(TKey*)next())) {
      sprintf(name,"%s",key->GetName());
      if(strcmp((key->GetName()),(histN))==0){
         //cout<<"[getEffHists] Your numerator for Eff "<<name<<endl;
         TH3F *hpneff3d = (TH3F*) efileDIR->Get(name);
      }
      if(strcmp((key->GetName()),(histD))==0){
         //cout<<"[getEffHists] Your denominator for Eff "<<name<<endl; 
         TH3F *hpdeff3d = (TH3F*) efileDIR->Get(name);
      }
   }
   return std::pair<TH3F*,TH3F*>(hpneff3d,hpdeff3d);
}


void PrintDetailsOfHist(TH3F* hSpectra){

   int  nbinX = hSpectra->GetNbinsX();
   int  nbinY = hSpectra->GetNbinsY();
   int  nbinZ = hSpectra->GetNbinsZ();

   cout<<"Number of bins in X (eta) : "<<nbinX
       <<" in Y (pt) : "<<nbinY
       <<" in Z (Jet ET) : "<<nbinZ<<endl;

   int binXmin = hSpectra->GetXaxis()->GetBinLowEdge(1);
   int binXmax = hSpectra->GetXaxis()->GetBinUpEdge(nbinX);

   int binYmin = hSpectra->GetYaxis()->GetBinLowEdge(1);
   int binYmax = hSpectra->GetYaxis()->GetBinUpEdge(nbinY);

   int binZmin = hSpectra->GetZaxis()->GetBinLowEdge(1);
   int binZmax = hSpectra->GetZaxis()->GetBinUpEdge(nbinZ);

   cout<<" eta ragnes[bin] : "<<binXmin<<" to " <<binXmax<<endl;
   cout<<" pt ragnes[bin] : "<<binYmin<<" to " <<binYmax<<endl;
   cout<<" jet ragnes[bin] : "<<binZmin<<" to " <<binZmax<<endl;

   /*
   int binMaxEta = hSpectra->GetXaxis()->FindBin(feta);
   int binMinEta = hSpectra->GetXaxis()->FindBin(-1.0*feta);

   cout<<"[DEBUG]-----"<<endl;
   cout<<"for input eta "<<feta<<" found max eta (bin center): "<<hSpectra->GetBinCenter(binMaxEta)<<endl;
   cout<<"for input eta "<<-1.0*feta<<" found min eta (bin center): "<<hSpectra->GetBinCenter(binMinEta)<<endl;
   cout<<"[DEBUG]-----"<<endl;

   float maxEta = hSpectra->GetXaxis()->GetBinCenter(binMaxEta);
   float minEta =hSpectra->GetXaxis()->GetBinCenter(binMinEta);
   */
}



/*
TH1D* RebinIt(TH1D* hist, bool REBIN){

   // ########################################### prepare variable bin ######                                                                                             
   Double_t dBin = hist->GetBinWidth(1);//assume hist has constan bin width                                                                                               

   Int_t NumBins = hist->GetNbinsX();
   const Int_t Nlines = NumBins;

   Int_t binTemp = 0;
   Int_t nbins = 0;
   Double_t binsTemp[Nlines+1];
   Int_t totBins = Nlines;

   for(Int_t i = 0; i < totBins ; i++){
      binsTemp[i] = binTemp;
      binTemp++;
   }
   binsTemp[totBins] = totBins;

   Int_t bin = 0;
   Double_t bins[Nlines+1];
   Int_t nbins = 0;
   Double_t binWidth = hist->GetBinWidth(1);
   cout<<"bin width of original histogram "<<binWidth<<endl;

   if(REBIN){
      while (bin < totBins) {
         bins[nbins] = binWidth*binsTemp[bin];
         //cout<<"bins[nbins] = "<<bins[nbins]<<endl;                                                                                                                     
         nbins++;
	 // MC
         //if (bin < 2)          bin += 1;
         //else if(bin < 8)      bin += 2;
         //else if (bin < 12)    bin += 4;
         //else if (bin < 30)    bin += 6;
         //else if (bin < 40)    bin += 8;
         //else if (bin < 70)    bin += 10;
         //else if (bin < 100)   bin += 15;
         //else if (bin < 200)   bin += 20;
         //else                  bin += 30;

	 // DATA

	 //if (bin < 2)          bin += 1;
	 if (bin < 3)          bin += 1;      
	 else if(bin < 8)      bin += 2;
	 else if (bin < 12)    bin += 4;
	 else if (bin < 30)    bin += 6;
	 else if (bin < 40)    bin += 8;
	 else if (bin < 70)    bin += 25;
	 else if (bin < 100)   bin += 40;
	 else if (bin < 200)   bin += 55;
	 else                  bin += 100;

      }
      bins[nbins] = binWidth*binTemp[totBins];
      //cout<<"bins[nbins] = "<<bins[nbins]<<endl;                                                                                                                        
   }

   cout<<"number of bins after rebinned = "<<nbins<<endl;

   //################################################# Rebin ###################                                                                                          

   Char_t hname[500];
   sprintf(hname,"pre_hRInvX");
   hist->Rebin(nbins,hname,bins);

   pre_hRInvX->SetNameTitle(hname,hname);
   cout<<"[RebinIt][check] name of the rebinned histogram  "<<pre_hRInvX->GetName()<<endl;

   const Int_t RNlines = nbins;

   Float_t ptR[RNlines], xsecR[RNlines];
   Float_t err_ptR[RNlines], err_xsecR[RNlines];


   for(Int_t j = 0; j<nbins; j++ ){

      // In order to scale the content properly (due to rebinning)

      Float_t dbin = pre_hRInvX->GetBinWidth(j+1);
      Float_t ratio = dbin/dBin;
      Float_t content = pre_hRInvX->GetBinContent(j+1);
      Float_t err = pre_hRInvX->GetBinError(j+1);

      pre_hRInvX->SetBinContent(j+1,(content/ratio));
      pre_hRInvX->SetBinError(j+1,(err/ratio));

      ptR[j] = pre_hRInvX->GetBinCenter(j+1);
      xsecR[j] = (content/ratio);
      err_ptR[j] = 0;
      err_xsecR[j] = (err/ratio);

   }

   //data.hRInvX = (TH1D*) pre_hRInvX->Clone("hRInvX");                                                                                                                   
   return pre_hRInvX;

}

*/
