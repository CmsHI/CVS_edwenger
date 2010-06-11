




TH1D* ratio_hist_to_func(TH1D* num, TF1* f3){

   cout<<"[Ratio to fit used]"<<endl;

   TH1D *hRatio = (TH1D*) num->Clone("hRatio");

   int nbin = num->GetNbinsX();

   for(int i=0;i<nbin;i++){

      double cms_value = (double) f3->Eval(hRatio->GetBinCenter(i+1));
      double ratio = hRatio->GetBinContent(i+1)/cms_value;                                                                                                   
      double ratio_err = hRatio->GetBinError(i+1)/cms_value;   

      hRatio->SetBinContent(i+1,ratio);
      hRatio->SetBinError(i+1,ratio_err);
   }

   return hRatio;
}


TH1D* ratio_hist_to_funcDiff(TH1D* num, TF1* f3){

   cout<<"[Ratio to fit used]"<<endl;

   TH1D *hRatio = (TH1D*) num->Clone("hRatio");

   int nbin = num->GetNbinsX();

   for(int i=0;i<nbin;i++){

      double cms_value = (double) f3->Eval(hRatio->GetBinCenter(i+1));
      double ratio = hRatio->GetBinContent(i+1)/cms_value;
      double ratio_err = hRatio->GetBinError(i+1)/cms_value;

      hRatio->SetBinContent(i+1,ratio);
      hRatio->SetBinError(i+1,ratio_err);
   }

   return hRatio;
}



TH1D* ratio_hist_to_func(TH1D* num, TF1* f3, double minx, double maxx){

   cout<<"[Ratio to fit with range used]"<<endl;

   TH1D *hRatio = (TH1D*) num->Clone("hRatio");

   int nbin = num->GetNbinsX();

   for(int i=0;i<nbin;i++){

      double cms_value = (double) f3->Eval(hRatio->GetBinCenter(i+1));

      if(hRatio->GetBinCenter(i+1)>minx && hRatio->GetBinCenter(i+1)<maxx){
         double ratio = hRatio->GetBinContent(i+1)/cms_value;
         double ratio_err = hRatio->GetBinError(i+1)/cms_value;
      }else{
         double ratio = -999;
         double ratio_err = 0.0;
      }

      hRatio->SetBinContent(i+1,ratio);
      hRatio->SetBinError(i+1,ratio_err);
   }

   return hRatio;

}


TGraphErrors* ratio_func_to_func(TGraphErrors* num, TF1* f3){

   cout<<"[Ratio to fit used]"<<endl;

   TGraphErrors *tg;

   int nbin = num->GetN();

   const int nlines = nbin;
   double pt[nlines], pterr[nlines];
   double xsec[nlines], xsecerr[nlines];
   double ratio[nlines], ratioerr[nlines];

   for(int i=0;i<nbin;i++){

      num->GetPoint(i,pt[i],xsec[i]);
      double cms_value = (double) f3->Eval(pt[i]);

      ratio[i] = xsec[i]/cms_value;
      ratioerr[i] = xsecerr[i]/cms_value;
      pterr[i] = 0;
   }

   tg = new TGraphErrors(nlines,pt,ratio,pterr,ratioerr);
   return tg;
}

TGraphErrors* ratio_func_to_func(TGraphErrors* num, TF1* f3, double minx, double maxx){

   cout<<"[Ratio to fit used]"<<endl;

   TGraphErrors *tg;

   int nbin = num->GetN();

   const int nlines = nbin;
   double pt[nlines], pterr[nlines];
   double xsec[nlines], xsecerr[nlines];
   double ratio[nlines], ratioerr[nlines];

   for(int i=0;i<nbin;i++){

      num->GetPoint(i,pt[i],xsec[i]);
      double cms_value = (double) f3->Eval(pt[i]);

      if(pt[i]>minx && pt[i]<maxx){
         ratio[i] = xsec[i]/cms_value;
         ratioerr[i] = xsecerr[i]/cms_value;
      }else{
         ratio[i] = -999;
         ratioerr[i] = 0.0;
      }

      pterr[i] = 0;
   }

   tg = new TGraphErrors(nlines,pt,ratio,pterr,ratioerr);
   return tg;
}

// convert tgraph  to hist
// assume predefined hist
TH1D* HistIt(TGraphErrors* tg, TH1D* histC){

   //TH1D* histC = (TH1D*) hist->Clone("histC");
   int nbins = tg->GetN();

   const int nlines = nbins;
   
   /*
   double *x[nlines], *y[nlines];
   
   x = (double*) tg->GetX();
   y = (double*) tg->GetY();
   
   double dbin = (x[1]-x[0]);
   double nbinx = nbins;
   
   hist = new TH1D("hist","",nbins,x[0]-(dbin/2),x[nbinx]+(dbin/2));
   hist->Sumw2();
   */

   double pt[nlines], xsec[nlines];
   double pterr[nlines], xsecerr[nlines];

   for(int i = 0; i<nbins; i++ ){
      tg->GetPoint(i,pt[i],xsec[i]);
      pterr[i] = tg->GetErrorX(i);
      xsecerr[i] = tg->GetErrorY(i);
      histC->SetBinContent(i+1,xsec[i]);
      histC->SetBinError(i+1,xsecerr[i]);
   }
   return histC;
}



// convert hist to tgraph
TGraphErrors* TgraphIt(TH1D* hist){

   TGraphErrors *tg;
   int nbins = hist->GetNbinsX();

   const int nlines = nbins;

   float pt[nlines], xsec[nlines];
   float pterr[nlines], xsecerr[nlines];

   for(int i = 0; i<nbins; i++ ){
      pt[i] = hist->GetBinCenter(i+1);
      xsec[i] = hist->GetBinContent(i+1);
      xsecerr[i] = hist->GetBinError(i+1);
      pterr[i] = 0;
   }

   tg = new TGraphErrors(nlines,pt,xsec,pterr,xsecerr);
   return tg;
}


TH1D* RebinIt(TH1D* hist_pre, bool REBIN){

   // ########################################### prepare variable bin ######

   TH1D* hist = (TH1D*) hist_pre->Clone("hist");
   //hist->Reset();

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
	 // DATA

	 if (bin < 3)          bin += 1;
	 else if(bin < 8)      bin += 2;
	 else if (bin < 12)    bin += 4;
	 else if (bin < 30)    bin += 6;
	 else if (bin < 40)    bin += 8;
	 else if (bin < 70)    bin += 25;
	 else if (bin < 100)   bin += 40;
	 else if (bin < 200)   bin += 55;
	 else                  bin += 100;

	 //bin += 2;
      }
      bins[nbins] = binWidth*binTemp[totBins];
      //cout<<"bins[nbins] = "<<bins[nbins]<<endl;	
   }
   
   cout<<"number of bins after rebinned = "<<nbins<<endl;
   
   //################################################# Rebin ###################            
   Char_t hname[500];
   sprintf(hname,"pre_hRInvX");
   TH1D* pre_hRInvX = (TH1D*) hist->Rebin(nbins,hname,bins);

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

   delete hist_pre;
   delete hist;

   return pre_hRInvX;
   
}
