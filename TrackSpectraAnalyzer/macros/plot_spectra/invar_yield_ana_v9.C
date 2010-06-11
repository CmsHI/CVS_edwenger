//-------------------------------------------------------------------
// June 03 2010, Andre S. Yoon
// - efficiency correction is done for tracking
// - efficiency correction is done for event selection
// - return various histograms/tgraph in struct
//--------------------------------------------------------------------


using namespace std;
#include <utility>
#include "invar_yield_ana_v9.h"
#include "/home/sungho/plots/common/utilities.h"

struct invar_yield_ana_v9_data {
   TH1D *hInvX;
   TH1D *hRInvX; // rebinned   
   TGraphErrors *InvX;
   TGraphErrors *RInvX; // rebinned   
   double integratedLum;
};

const float pi = 3.14159265358979323846;
typedef pair<TH3D*, TH3D*> 3DHistPair;

const int NHIST = 5;

//-------------------------------------------------------------------------------------------------------------------------------
// 1. files needed -  1. source, 2. evt correction 3. trk correction
// 2. parameters needed - eta min, eta max
// 3. flags needed - gen, mc, trk correction, jet Et based or track pT based classification, evt correction (zero bin, 123 bins)
//                   spectra as a cross section or number of particles, 
//------------------------------------------------------------------------------------------------------------------------------- 

invar_yield_ana_v9_data  invar_yield_ana_v9_graph(char *cFile, char *cFileEVT, char *cFileTRK1, char *cFileTRK2, char *cFileTRK3, char *cFileTRK4, char *cFileTRK5,
						  char *dir, char *dir_corr, bool gen, bool trkeffcrct, bool multcrct, bool seccrct, bool mc, bool jetcrct, 
						  bool evteffcorr, bool zerobin, bool onetwothreebin, bool cross, bool oneoverpt, bool rescrct,
						  double ieta, double feta)
   
{
   cout<<"\n"<<endl;
   cout<<"[Loading]==========================================================="<<endl;
   cout<<"[ana] File to be processed for dndpt "<<cFile<<endl;
   cout<<"[ana] File to be processed for evt eff correction "<<cFileEVT<<endl;
   cout<<"[ana] File to be processed for eff/fake"<<endl;
   cout<<cFileTRK1<<endl;
   cout<<cFileTRK2<<endl; 
   cout<<cFileTRK3<<endl;
   cout<<cFileTRK4<<endl;
   cout<<cFileTRK5<<endl;
   cout<<"[Loading]==========================================================="<<endl;
   cout<<"\n"<<endl;


   cout<<"[Preparing TRK eff correction]======================================="<<endl;
   float pt_1st, pt_2nd, pt_3rd, pt_4th;
   pt_1st = 9, pt_2nd = 12, pt_3rd = 40, pt_4th = 100;
   
   float jet_1st, jet_2nd, jet_3rd, jet_4th;
   jet_1st = 30, jet_2nd = 50, jet_3rd = 90, jet_4th = 180;
   //jet_1st = 25, jet_2nd = 50, jet_3rd = 90, jet_4th = 180;
   //jet_1st = 30, jet_2nd = 50, jet_3rd = 90, jet_4th = 180;
   //jet_1st = 25, jet_2nd = 45, jet_3rd = 90, jet_4th = 180;


   char* cFileTRKArray[NHIST];

   char hist1[100], hist2[100], hist3[100], hist4[100];
   sprintf(hist1,"heff3D"), sprintf(hist2,"hsim3D"), sprintf(hist3,"hfak3D"), sprintf(hist4,"hrec3D");
   
   TH3F *hPNEff3D[NHIST], *hPDEff3D[NHIST], *hPNFake3D[NHIST], *hPDFake3D[NHIST];
   std::pair<TH3F*,TH3F*> h3DEff[NHIST], h3DFake[NHIST];

   // multiply reconstructed!    
   char histMult1[100], histMult2[100];
   sprintf(histMult1,"hmul3D"), sprintf(histMult2,"hsim3D");
   
   TH3F *hPNMult3D[NHIST], *hPDMult3D[NHIST];
   std::pair<TH3F*,TH3F*> h3DMult[NHIST];
   
   // secondary !    
   char histSec1[100], histSec2[100];
   sprintf(histSec1,"hsec3D"), sprintf(histSec2,"hrec3D");

   TH3F *hPNSec3D[NHIST], *hPDSec3D[NHIST];
   std::pair<TH3F*,TH3F*> h3DSec[NHIST];

   

   if(trkeffcrct && (!gen)) {
      for(int i=0;i<NHIST;i++){
	 
	 if(i==0) cFileTRKArray[i] = cFileTRK1;
	 else if(i==1) cFileTRKArray[i] = cFileTRK2;
	 else if(i==2) cFileTRKArray[i] = cFileTRK3;
	 else if(i==3) cFileTRKArray[i] = cFileTRK4;
	 else if(i==4) cFileTRKArray[i] = cFileTRK5;
	 cout<<"TRK correction file to be processed : "<<cFileTRKArray[i]<<endl;
	 
	 // trk eff/fr correction
	 h3DEff[i] = getEffHists(cFileTRKArray[i],dir_corr,hist1,hist2);
	 h3DFake[i] = getEffHists(cFileTRKArray[i],dir_corr,hist3,hist4);
	 
	 hPNEff3D[i] = h3DEff[i].first, hPDEff3D[i] = h3DEff[i].second;
	 hPNFake3D[i] = h3DFake[i].first, hPDFake3D[i] = h3DFake[i].second;

	 // multiply reconstructed!
	 h3DMult[i] = getEffHists(cFileTRKArray[i],dir_corr,histMult1,histMult2);
	 hPNMult3D[i] = h3DMult[i].first, hPDMult3D[i] = h3DMult[i].second;

	 // secondary !    
	 h3DSec[i] = getEffHists(cFileTRKArray[i],dir_corr,histSec1,histSec2);
         hPNSec3D[i] = h3DSec[i].first, hPDSec3D[i] = h3DSec[i].second;
	 
      }
   }
   cout<<"[Preparing TRK eff correction]======================================="<<endl;
   cout<<"\n"<<endl;

   


   cout<<"[Get number of evetns ]========================================"<<endl;
   //-------- Get Number for evt sel correction
   TFile *fEVT = new TFile(cFileEVT);

   if(zerobin){
      TH1F *hNSD_GEN = (TH1F*) fEVT->Get("preTrgAna/hGenRecMultNSD_STD");    
      double zerobinFraction = GetZeroBinFraction(hNSD_GEN);
      cout<<"ZERO bin fractiosn (from ??) : "<<zerobinFraction<<endl;
   }

   if(onetwothreebin){
      TH1F *hNSDvtx_CORR_123 = (TH1F*) fEVT->Get("preTrgAna/hGenRecMultNSD_STD");
      double zottbinFraction = GetZOTTBinFraction(hNSDvtx_CORR_123);
      cout<<"0, 1,2,3 bin fraction (from ??) : "<<zottbinFraction<<endl;

      TH3F *hSpectra3D_mult1 = (TH3F*) fEVT->Get("looseTrackAna_STD/threeDHist/hTrkPtEtaJetEt_mult1");
      TH3F *hSpectra3D_mult2 = (TH3F*) fEVT->Get("looseTrackAna_STD/threeDHist/hTrkPtEtaJetEt_mult2");
      TH3F *hSpectra3D_mult3 = (TH3F*) fEVT->Get("looseTrackAna_STD/threeDHist/hTrkPtEtaJetEt_mult3");
   }
   //cout<<"[Get numbers for evt sel correction]========================================"<<endl;

   //------------------------- Ana source
   TFile *f1 = new TFile(cFile);

   double NumEvt = 0;
   double NumEvtCorrected = 0;
   double NumEvtCorrectedMult = 0;

   char dirMult[100];
   if(strcmp(dir,"looseTrackAna_STD")==0) sprintf(dirMult,"looseTrackAna_STD/hRecMult_STD_corr");
   else sprintf(dirMult,"trackAna_STD/hRecMult_STD_corr");
   
   TH1F *hNSDvtx_CORR = (TH1F*) f1->Get(dirMult);

   TDirectoryFile *dSpec = f1->GetDirectory(dir);
   cout<<"Directory to be looked at "<<dir<<endl;

   dSpec->cd();

   if(gen && mc){
      NumEvt = hGenNevt->GetEntries();
      NumEvtCorrected = CorrectNevt(hGenNevt);
      NumEvtCorrectedMult =  NumEvtCorrected;
   }else {
      NumEvt = hNevt->GetEntries();
      if(evteffcorr) NumEvtCorrected = CorrectNevt(hNevt);
      if(evteffcorr) NumEvtCorrectedMult = CorrectNevtWithMult(hNSDvtx_CORR);
   }

   if(evteffcorr && zerobin && !onetwothreebin){
      NumEvtCorrectedMult = NumEvtCorrectedMult/(1-zerobinFraction);
      cout<<"Corrected for ZERO bin only !"<<endl;
   }else if(evteffcorr && zerobin && onetwothreebin){
      NumEvtCorrectedMult = NumEvtCorrectedMult/(1-zottbinFraction);
      cout<<"temporary correction"<<endl;
      NumEvtCorrectedMult = NumEvtCorrectedMult*(6.98903/6.97963);
      cout<<"Corrected for ZERO bin,1,2,3 bin!"<<endl;
   }


   cout<<"Number of event is "<<NumEvt<<endl;
   cout<<"Number of correted event is "<<NumEvtCorrected<<endl;
   cout<<"Number of correted event (based on MULT dist) is "<<NumEvtCorrectedMult<<endl;

   Double_t totMB_xsec = 71.26; // mb
   Double_t intLum = NumEvt/totMB_xsec;
   if(!cross && evteffcorr) intLum = NumEvtCorrectedMult;      
   if(!cross && !evteffcorr) intLum =  NumEvt;
   
   cout<<"Int. LUM is "<<intLum<<endl;
   cout<<"[Get number of evetns ]========================================"<<endl;
   cout<<"\n"<<endl;


   cout<<"[Correcting and analyzing ]========================================"<<endl;
   threeDHist->cd();    

   TIter next(gDirectory->GetListOfKeys());
   TKey *key;

   char nameHist[200];
   if(oneoverpt){
      if(gen) sprintf(nameHist,"hGenTrkPtEtaJetEtW");
      else sprintf(nameHist,"hTrkPtEtaJetEtW");
   }else{
      if(gen) sprintf(nameHist,"hGenTrkPtEtaJetEt");
      else sprintf(nameHist,"hTrkPtEtaJetEt");
   }
   

   while ((key=(TKey*)next())) {
      if( strcmp((key->GetName()),(nameHist))==0) {
	 TH3F *hSpectra3D = (TH3F*) gDirectory->Get(nameHist);
	 cout<<"Name of Histogram matched is "<<nameHist<<endl;
	 break;
      }
   }

   if(onetwothreebin){
      //hSpectra3D->Add(hSpectra3D_mult1);
      //hSpectra3D->Add(hSpectra3D_mult2);
      //hSpectra3D->Add(hSpectra3D_mult3);
   }


   PrintDetailsOfHist(hSpectra3D);
   hSpectra3D->Sumw2();

   int  nbinX = hSpectra3D->GetNbinsX();
   int  nbinY = hSpectra3D->GetNbinsY();
   int  nbinZ = hSpectra3D->GetNbinsZ();

   int binMaxEta = hSpectra3D->GetXaxis()->FindBin(feta);
   int binMinEta = hSpectra3D->GetXaxis()->FindBin(-1.0*feta);

   cout<<"[DEBUG]--------------------"<<endl;
   cout<<"for input eta "<<feta<<" found max eta (bin center): "<<hSpectra3D->GetXaxis()->GetBinCenter(binMaxEta)<<endl;
   cout<<"for input eta "<<-1.0*feta<<" found min eta (bin center): "<<hSpectra3D->GetXaxis()->GetBinCenter(binMinEta)<<endl;
   cout<<"for input eta "<<feta<<" found max eta (bin up edge): "<<hSpectra3D->GetXaxis()->GetBinUpEdge(binMaxEta)<<endl;
   cout<<"for input eta "<<-1.0*feta<<" found min eta (bin low edge): "<<hSpectra3D->GetXaxis()->GetBinLowEdge(binMinEta)<<endl;
   cout<<"[DEBUG]--------------------"<<endl;

   float deta = feta + feta; // symmetric case       

   bool invariant = true;
   int entries1K = 0;
   int entries10K = 0;

   float var_1st, var_2nd, var_3rd, var_4th;

   for(int j=0;j<(nbinX*nbinY*nbinZ);j++){

      int xbin = (j % nbinX);
      int ybin = ((j-xbin)/nbinX) % nbinY;
      int zbin = (j-(xbin+((ybin)*nbinX)))/(nbinX*nbinY);
      
      float  eta = hSpectra3D->GetXaxis()->GetBinCenter(xbin+1);
      float  pt = hSpectra3D->GetYaxis()->GetBinCenter(ybin+1);
      float  jet = hSpectra3D->GetZaxis()->GetBinCenter(zbin+1);
      float  dbin = hSpectra3D->GetYaxis()->GetBinWidth(ybin+1);

      float  dn = hSpectra3D->GetBinContent(xbin+1,ybin+1,zbin+1);
      float  edn = hSpectra3D->GetBinError(xbin+1,ybin+1,zbin+1);

      //int entries = hSpectra3D->GetEntries();
      int entries = (int) dn; // true for dn/dpt 

      if(entries<1000) entries1K++;
      if(entries<10000) entries10K++;

      float var_for_corr = 0;

      if(jetcrct) {
	 var_for_corr = jet;
	 var_1st = jet_1st, var_2nd = jet_2nd, var_3rd = jet_3rd, var_4th = jet_4th;
      }else{
	 var_for_corr = pt;
         var_1st = pt_1st, var_2nd = pt_2nd, var_3rd = pt_3rd, var_4th = pt_4th;
      }

      if(trkeffcrct && (!gen)) {
	
	 if(var_for_corr<var_1st){
	    double eff = GetEffFactor(hPNEff3D[0],hPDEff3D[0],pt,eta,jet);
	    double fak = GetFakFactor(hPNFake3D[0],hPDFake3D[0],pt,eta,jet);
	    if(multcrct) double mlt = GetFakFactor(hPNMult3D[0],hPDMult3D[0],pt,eta,jet);
	    if(seccrct)  double sec = GetFakFactor(hPNSec3D[0],hPDSec3D[0],pt,eta,jet);
	 }else if(var_for_corr>=var_1st && var_for_corr<var_2nd){
	    double eff = GetEffFactor(hPNEff3D[1],hPDEff3D[1],pt,eta,jet);
            double fak = GetFakFactor(hPNFake3D[1],hPDFake3D[1],pt,eta,jet);
	    if(multcrct) double mlt = GetFakFactor(hPNMult3D[1],hPDMult3D[1],pt,eta,jet);
	    if(seccrct)  double sec = GetFakFactor(hPNSec3D[1],hPDSec3D[1],pt,eta,jet);
	 }else if(var_for_corr>=var_2nd && var_for_corr<var_3rd){
	    double eff = GetEffFactor(hPNEff3D[2],hPDEff3D[2],pt,eta,jet);
            double fak = GetFakFactor(hPNFake3D[2],hPDFake3D[2],pt,eta,jet);
	    if(multcrct) double mlt = GetFakFactor(hPNMult3D[2],hPDMult3D[2],pt,eta,jet);
	    if(seccrct)  double sec = GetFakFactor(hPNSec3D[2],hPDSec3D[2],pt,eta,jet);
	 }else if(var_for_corr>=var_3rd && var_for_corr<var_4th){
	    double eff = GetEffFactor(hPNEff3D[3],hPDEff3D[3],pt,eta,jet);
            double fak = GetFakFactor(hPNFake3D[3],hPDFake3D[3],pt,eta,jet);
	    if(multcrct) double mlt = GetFakFactor(hPNMult3D[3],hPDMult3D[3],pt,eta,jet);
	    if(seccrct)  double sec = GetFakFactor(hPNSec3D[3],hPDSec3D[3],pt,eta,jet);
	 }else{
	    double eff = GetEffFactor(hPNEff3D[4],hPDEff3D[4],pt,eta,jet);
            double fak = GetFakFactor(hPNFake3D[4],hPDFake3D[4],pt,eta,jet);
	    if(multcrct) double mlt = GetFakFactor(hPNMult3D[4],hPDMult3D[4],pt,eta,jet);
	    if(seccrct)  double sec = GetFakFactor(hPNSec3D[4],hPDSec3D[4],pt,eta,jet);
	 }

	 // efficiency and fake rate !
	 if(eff==0 || fak ==1){
	    dn = dn;
	    edn = edn;
	 }else{
	    dn = dn*((1-fak)/eff);
	    edn = edn*((1-fak)/eff);
	 }

	 // multiply reconstructed !
	 if(multcrct){
	    if(mlt==1 || mlt==0){ 
	       dn = dn, edn = edn; // no correction!
	    }else{
	       dn = dn*(1./(1+mlt)), edn = edn*(1./(1+mlt));
	    }
	 }

	 // secondary !
	 if(seccrct){
	    if(sec==1 || sec==0){
	       dn = dn, edn = edn; // no correction! 
	    }else{
	       dn = dn*(1-sec), edn = edn*(1-sec);
	    }
	 }
      }

      if(invariant) {
	 if(oneoverpt) {
	    dn = dn/(2*pi*deta*dbin);
	    edn=  edn/(2*pi*deta*dbin);
	 }else{
	    dn = dn/(2*pi*pt*deta*dbin);
            edn=  edn/(2*pi*pt*deta*dbin);
	 }
      }
      
      hSpectra3D->SetBinContent(xbin+1,ybin+1,zbin+1,dn);
      hSpectra3D->SetBinError(xbin+1,ybin+1,zbin+1,edn);
   }

   if(trkeffcrct && (!gen)) cout<<"correction applied !!!!!"<<endl;
   cout<<"number of entry with entries < 1000 : "<<entries1K<<endl;
   cout<<"number of entry with entries < 10000 : "<<entries10K<<endl;
   cout<<"[Correcting and analyzing ]========================================"<<endl;
   cout<<"\n"<<endl;

   invar_yield_ana_v9_data data;

   // make sure i don't include(exclude) eta region i want!
   // bin edges have to be matched to max eta and min eta!
   //if(minpt) hSpectra3D->GetYaxis()->SetRange(2,200);
   //data.hInvX = (TH1D*) hSpectra3D->ProjectionY("hInvX",binMinEta,binMaxEta,
   TH1D* hInvX_pre = (TH1D*) hSpectra3D->ProjectionY("hInvX",binMinEta,binMaxEta,
						     hSpectra3D->GetZaxis()->GetFirst(),
						     hSpectra3D->GetZaxis()->GetLast(),
						     "e");    

   
   if(rescrct) {
      TH1D* hRInvX_pre = (TH1D*) hInvX_pre->Clone("hRInvX_pre");
      std::pair<TH1D*,TH1D*> correctedSPEC = CorrectForMomRes(cFileTRKArray[0],hInvX_pre,ieta,feta,0.5,15);
      data.hInvX = (TH1D*) correctedSPEC.first;
      data.hRInvX = (TH1D*) correctedSPEC.second;
   }else{
      data.hInvX = (TH1D*) hInvX_pre->Clone("hInvX");
      TH1D *hInvX_dum = (TH1D*) data.hInvX->Clone("hInvX_dum");
      data.hRInvX = (TH1D*) RebinIt(hInvX_dum,true);
   }


   data.integratedLum = intLum;
   return (data);

}


