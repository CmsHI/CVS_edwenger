//------------------------------------------------------------------- 
// June 03 2010, Andre S. Yoon 
// - header file for invar_yield_ana_v9.C
// - efficiency correction is done for tracking
// - efficiency correction is done for event selection 
// - return various histograms/tgraph in struct
//--------------------------------------------------------------------


using namespace std;
#include <utility>

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


double GetZeroBinFraction(TH1F* hist){

   double total = hist->Integral();
   double non_zerobin = hist->Integral(2,hist->GetNbinsX());
   double fraction = (total-non_zerobin)/total;
   //double fraction = (total-non_zerobin)/total;                                                                                                                         
   return fraction;
}

double GetOTTBinFraction(TH1F* hist){

   double total = hist->Integral(2,hist->GetNbinsX());//except zero bin                                                                                                   
   double non_zerobin = hist->Integral(5,hist->GetNbinsX());
   double fraction = (total-non_zerobin)/total;
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
         cout<<"[getEffHists] Your numerator for Eff "<<name<<endl;
         TH3F *hpneff3d = (TH3F*) efileDIR->Get(name);
      }
      if(strcmp((key->GetName()),(histD))==0){
         cout<<"[getEffHists] Your denominator for Eff "<<name<<endl; 
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
         if (bin < 2)          bin += 1;
         else if(bin < 8)      bin += 2;
         else if (bin < 12)    bin += 4;
         else if (bin < 30)    bin += 6;
         else if (bin < 40)    bin += 8;
         else if (bin < 70)    bin += 10;
         else if (bin < 100)   bin += 15;
         else if (bin < 200)   bin += 20;
         else                  bin += 30;

	 // DATA
	 /*
	 if (bin < 2)          bin += 1;
	 else if(bin < 8)      bin += 2;
	 else if (bin < 12)    bin += 4;
	 else if (bin < 30)    bin += 6;
	 else if (bin < 40)    bin += 8;
	 else if (bin < 70)    bin += 25;
	 else if (bin < 100)   bin += 40;
	 else if (bin < 200)   bin += 55;
	 else                  bin += 100;
	 */
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

      // In order to scale the content properly (due to rebinning)                                                                                                       \
                                                                                                                                                                          
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
