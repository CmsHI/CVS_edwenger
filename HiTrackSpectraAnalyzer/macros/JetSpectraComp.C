



void JetSpectraComp(){


  TFile *f1 = new TFile("../root_files/data/trkhistDATA_dec072011_sub1_hptskpr_spectra_jet80_hiopt_abcdef.root");
  TFile *f2 = new TFile("../root_files/data/trkhistDATA_nov042011_jet50_yj_sub2.root");

  // variables
  bool save = true;

  double ptmin = 100;
  double ptmax = 350;

  double ymin = 1E-5;
  double ymax = 9;

  double ymax_r = 2.0;

  double rebinf = 5;
  
  // this is for rebinning
  TH1F *hJetPt_dum = (TH1F*) f1->Get("hitrackAna_jetMode1/hJet0Pt_cbin0to1");
  hJetPt_dum->SetName("hJetPt_dum");

  // rebinning
  int  bin = 1;
  double bins[80];
  int nbins = 0;

  TAxis *xaxis = hJetPt_dum->GetXaxis();
  cout<<"Number of bins = "<<xaxis->GetLast()<<endl;
  int nlast = xaxis->GetLast();
  while (bin <nlast) {
    bins[nbins] = xaxis->GetBinLowEdge(bin);
    nbins++;
    if (bin < 20)      bin += 2;      // 2 GeV * 20 = 40 GeV
    else if (bin < 80) bin += 6;      // 2 GeV * 80 = 160 GeV 
    else if (bin < 120) bin += 12;    // 2 GeV * 120 = 240 GeV 
    else               bin += 25;
  }             
  bins[nbins] = xaxis->GetBinUpEdge(nlast);

  // Line at 1
  TLine *line = new TLine(ptmin,1,1.02*ptmax,1);
  line->SetLineStyle(2), line->SetLineWidth(2);


  // New 
  TH1F *hJetPt_c1_f1_pre = (TH1F*) f1->Get("hitrackAna_jetMode1/hJet0Pt_cbin0to1");
  TH1F *hJetPt_c2_f1_pre = (TH1F*) f1->Get("hitrackAna_jetMode1/hJet0Pt_cbin2to3");
  TH1F *hJetPt_c3_f1_pre = (TH1F*) f1->Get("hitrackAna_jetMode1/hJet0Pt_cbin4to11");
  TH1F *hJetPt_c4_f1_pre = (TH1F*) f1->Get("hitrackAna_jetMode1/hJet0Pt_cbin12to19");
  TH1F *hJetPt_c5_f1_pre = (TH1F*) f1->Get("hitrackAna_jetMode1/hJet0Pt_cbin20to27");
  TH1F *hJetPt_c6_f1_pre = (TH1F*) f1->Get("hitrackAna_jetMode1/hJet0Pt_cbin28to35");

  hJetPt_c1_f1_pre->Sumw2(), hJetPt_c2_f1_pre->Sumw2(), hJetPt_c3_f1_pre->Sumw2();
  hJetPt_c4_f1_pre->Sumw2(), hJetPt_c5_f1_pre->Sumw2(), hJetPt_c6_f1_pre->Sumw2();

  //hJetPt_c1_f1->Rebin(rebinf), hJetPt_c2_f1->Rebin(rebinf), hJetPt_c3_f1->Rebin(rebinf);
  //hJetPt_c4_f1->Rebin(rebinf), hJetPt_c5_f1->Rebin(rebinf), hJetPt_c6_f1->Rebin(rebinf);

  TH1F *hJetPt_c1_f1 = new TH1F("hJetPt_c1_f1","rebinned histo",nbins,bins);
  TH1F *hJetPt_c2_f1 = new TH1F("hJetPt_c2_f1","rebinned histo",nbins,bins);
  TH1F *hJetPt_c3_f1 = new TH1F("hJetPt_c3_f1","rebinned histo",nbins,bins);
  TH1F *hJetPt_c4_f1 = new TH1F("hJetPt_c4_f1","rebinned histo",nbins,bins);
  TH1F *hJetPt_c5_f1 = new TH1F("hJetPt_c5_f1","rebinned histo",nbins,bins);
  TH1F *hJetPt_c6_f1 = new TH1F("hJetPt_c6_f1","rebinned histo",nbins,bins);

  for (bin=1;bin<nlast;bin++) {
    hJetPt_c1_f1->Fill(xaxis->GetBinCenter(bin),hJetPt_c1_f1_pre->GetBinContent(bin));
    hJetPt_c2_f1->Fill(xaxis->GetBinCenter(bin),hJetPt_c2_f1_pre->GetBinContent(bin));
    hJetPt_c3_f1->Fill(xaxis->GetBinCenter(bin),hJetPt_c3_f1_pre->GetBinContent(bin));
    hJetPt_c4_f1->Fill(xaxis->GetBinCenter(bin),hJetPt_c4_f1_pre->GetBinContent(bin));
    hJetPt_c5_f1->Fill(xaxis->GetBinCenter(bin),hJetPt_c5_f1_pre->GetBinContent(bin));
    hJetPt_c6_f1->Fill(xaxis->GetBinCenter(bin),hJetPt_c6_f1_pre->GetBinContent(bin));
  }

  for (int i =0; i<hJetPt_c1_f1->GetNbinsX();i++){
    double bw = hJetPt_c1_f1->GetXaxis()->GetBinWidth(i+1);
    hJetPt_c1_f1->SetBinContent(i+1,hJetPt_c1_f1->GetBinContent(i+1)/bw);
    hJetPt_c1_f1->SetBinError(i+1,hJetPt_c1_f1->GetBinError(i+1)/bw);
    
    hJetPt_c2_f1->SetBinContent(i+1,hJetPt_c2_f1->GetBinContent(i+1)/bw);
    hJetPt_c2_f1->SetBinError(i+1,hJetPt_c2_f1->GetBinError(i+1)/bw);

    hJetPt_c3_f1->SetBinContent(i+1,hJetPt_c3_f1->GetBinContent(i+1)/bw);
    hJetPt_c3_f1->SetBinError(i+1,hJetPt_c3_f1->GetBinError(i+1)/bw);

    hJetPt_c4_f1->SetBinContent(i+1,hJetPt_c4_f1->GetBinContent(i+1)/bw);
    hJetPt_c4_f1->SetBinError(i+1,hJetPt_c4_f1->GetBinError(i+1)/bw);

    hJetPt_c5_f1->SetBinContent(i+1,hJetPt_c5_f1->GetBinContent(i+1)/bw);
    hJetPt_c5_f1->SetBinError(i+1,hJetPt_c5_f1->GetBinError(i+1)/bw);

    hJetPt_c6_f1->SetBinContent(i+1,hJetPt_c6_f1->GetBinContent(i+1)/bw);
    hJetPt_c6_f1->SetBinError(i+1,hJetPt_c6_f1->GetBinError(i+1)/bw);
  }

  // normalization
  double nevt_c1_f1 = hJetPt_c1_f1->Integral(hJetPt_c1_f1->GetXaxis()->FindBin(ptmin),hJetPt_c1_f1->GetXaxis()->GetLast());
  double nevt_c2_f1 = hJetPt_c2_f1->Integral(hJetPt_c2_f1->GetXaxis()->FindBin(ptmin),hJetPt_c2_f1->GetXaxis()->GetLast());
  double nevt_c3_f1 = hJetPt_c3_f1->Integral(hJetPt_c3_f1->GetXaxis()->FindBin(ptmin),hJetPt_c3_f1->GetXaxis()->GetLast());
  double nevt_c4_f1 = hJetPt_c4_f1->Integral(hJetPt_c4_f1->GetXaxis()->FindBin(ptmin),hJetPt_c4_f1->GetXaxis()->GetLast());
  double nevt_c5_f1 = hJetPt_c5_f1->Integral(hJetPt_c5_f1->GetXaxis()->FindBin(ptmin),hJetPt_c5_f1->GetXaxis()->GetLast());
  double nevt_c6_f1 = hJetPt_c6_f1->Integral(hJetPt_c6_f1->GetXaxis()->FindBin(ptmin),hJetPt_c6_f1->GetXaxis()->GetLast());
  
  cout<<"Sample 1: Number of jets (ET > "<<ptmin<<") cbin 1 = "<<nevt_c1_f1<<" cbin 2 = "<<nevt_c2_f1<<" cbin 3 = "<<nevt_c3_f1
      <<" cbin 4 = "<<nevt_c4_f1<<" cbin 5 = "<<nevt_c5_f1<<" cbin 6 = "<<nevt_c6_f1<<endl;

  hJetPt_c1_f1->Scale(1./nevt_c1_f1), hJetPt_c2_f1->Scale(1./nevt_c2_f1), hJetPt_c3_f1->Scale(1./nevt_c3_f1);
  hJetPt_c4_f1->Scale(1./nevt_c4_f1), hJetPt_c5_f1->Scale(1./nevt_c5_f1), hJetPt_c6_f1->Scale(1./nevt_c6_f1);

  // Old
  TH1F *hJetPt_c1_f2_pre = (TH1F*) f2->Get("hitrackAna_jetMode1/hJet0Pt_cbin0to1");
  TH1F *hJetPt_c2_f2_pre = (TH1F*) f2->Get("hitrackAna_jetMode1/hJet0Pt_cbin2to3");
  TH1F *hJetPt_c3_f2_pre = (TH1F*) f2->Get("hitrackAna_jetMode1/hJet0Pt_cbin4to11");
  TH1F *hJetPt_c4_f2_pre = (TH1F*) f2->Get("hitrackAna_jetMode1/hJet0Pt_cbin12to19");
  TH1F *hJetPt_c5_f2_pre = (TH1F*) f2->Get("hitrackAna_jetMode1/hJet0Pt_cbin20to27");
  TH1F *hJetPt_c6_f2_pre = (TH1F*) f2->Get("hitrackAna_jetMode1/hJet0Pt_cbin28to35");

  hJetPt_c1_f2_pre->Sumw2(), hJetPt_c2_f2_pre->Sumw2(), hJetPt_c3_f2_pre->Sumw2();
  hJetPt_c4_f2_pre->Sumw2(), hJetPt_c5_f2_pre->Sumw2(), hJetPt_c6_f2_pre->Sumw2();
  
  //hJetPt_c1_f2->Rebin(rebinf), hJetPt_c2_f2->Rebin(rebinf), hJetPt_c3_f2->Rebin(rebinf);
  //hJetPt_c4_f2->Rebin(rebinf), hJetPt_c5_f2->Rebin(rebinf), hJetPt_c6_f2->Rebin(rebinf);

  TH1F *hJetPt_c1_f2 = new TH1F("hJetPt_c1_f2","rebinned histo",nbins,bins);
  TH1F *hJetPt_c2_f2 = new TH1F("hJetPt_c2_f2","rebinned histo",nbins,bins);
  TH1F *hJetPt_c3_f2 = new TH1F("hJetPt_c3_f2","rebinned histo",nbins,bins);
  TH1F *hJetPt_c4_f2 = new TH1F("hJetPt_c4_f2","rebinned histo",nbins,bins);
  TH1F *hJetPt_c5_f2 = new TH1F("hJetPt_c5_f2","rebinned histo",nbins,bins);
  TH1F *hJetPt_c6_f2 = new TH1F("hJetPt_c6_f2","rebinned histo",nbins,bins);

  for (bin=1;bin<nlast;bin++) {
    hJetPt_c1_f2->Fill(xaxis->GetBinCenter(bin),hJetPt_c1_f2_pre->GetBinContent(bin));
    hJetPt_c2_f2->Fill(xaxis->GetBinCenter(bin),hJetPt_c2_f2_pre->GetBinContent(bin));
    hJetPt_c3_f2->Fill(xaxis->GetBinCenter(bin),hJetPt_c3_f2_pre->GetBinContent(bin));
    hJetPt_c4_f2->Fill(xaxis->GetBinCenter(bin),hJetPt_c4_f2_pre->GetBinContent(bin));
    hJetPt_c5_f2->Fill(xaxis->GetBinCenter(bin),hJetPt_c5_f2_pre->GetBinContent(bin));
    hJetPt_c6_f2->Fill(xaxis->GetBinCenter(bin),hJetPt_c6_f2_pre->GetBinContent(bin));
  }


  for (int i =0; i<hJetPt_c1_f2->GetNbinsX();i++){
    double bw = hJetPt_c1_f2->GetXaxis()->GetBinWidth(i+1);
    hJetPt_c1_f2->SetBinContent(i+1,hJetPt_c1_f2->GetBinContent(i+1)/bw);
    hJetPt_c1_f2->SetBinError(i+1,hJetPt_c1_f2->GetBinError(i+1)/bw);

    hJetPt_c2_f2->SetBinContent(i+1,hJetPt_c2_f2->GetBinContent(i+1)/bw);
    hJetPt_c2_f2->SetBinError(i+1,hJetPt_c2_f2->GetBinError(i+1)/bw);

    hJetPt_c3_f2->SetBinContent(i+1,hJetPt_c3_f2->GetBinContent(i+1)/bw);
    hJetPt_c3_f2->SetBinError(i+1,hJetPt_c3_f2->GetBinError(i+1)/bw);

    hJetPt_c4_f2->SetBinContent(i+1,hJetPt_c4_f2->GetBinContent(i+1)/bw);
    hJetPt_c4_f2->SetBinError(i+1,hJetPt_c4_f2->GetBinError(i+1)/bw);

    hJetPt_c5_f2->SetBinContent(i+1,hJetPt_c5_f2->GetBinContent(i+1)/bw);
    hJetPt_c5_f2->SetBinError(i+1,hJetPt_c5_f2->GetBinError(i+1)/bw);

    hJetPt_c6_f2->SetBinContent(i+1,hJetPt_c6_f2->GetBinContent(i+1)/bw);
    hJetPt_c6_f2->SetBinError(i+1,hJetPt_c6_f2->GetBinError(i+1)/bw);
  }


  double nevt_c1_f2 = hJetPt_c1_f2->Integral(hJetPt_c1_f2->GetXaxis()->FindBin(ptmin),hJetPt_c1_f2->GetXaxis()->GetLast());
  double nevt_c2_f2 = hJetPt_c2_f2->Integral(hJetPt_c2_f2->GetXaxis()->FindBin(ptmin),hJetPt_c2_f2->GetXaxis()->GetLast());
  double nevt_c3_f2 = hJetPt_c3_f2->Integral(hJetPt_c3_f2->GetXaxis()->FindBin(ptmin),hJetPt_c3_f2->GetXaxis()->GetLast());
  double nevt_c4_f2 = hJetPt_c4_f2->Integral(hJetPt_c4_f2->GetXaxis()->FindBin(ptmin),hJetPt_c4_f2->GetXaxis()->GetLast());
  double nevt_c5_f2 = hJetPt_c5_f2->Integral(hJetPt_c5_f2->GetXaxis()->FindBin(ptmin),hJetPt_c5_f2->GetXaxis()->GetLast());
  double nevt_c6_f2 = hJetPt_c6_f2->Integral(hJetPt_c6_f2->GetXaxis()->FindBin(ptmin),hJetPt_c6_f2->GetXaxis()->GetLast());

  cout<<"Sample 2: Number of jets (ET > "<<ptmin<<") cbin 1 = "<<nevt_c1_f2<<" cbin 2 = "<<nevt_c2_f2<<" cbin 3 = "<<nevt_c3_f2
      <<" cbin 4 = "<<nevt_c4_f2<<" cbin 5 = "<<nevt_c5_f2<<" cbin 6 = "<<nevt_c6_f2<<endl;

  hJetPt_c1_f2->Scale(1./nevt_c1_f2), hJetPt_c2_f2->Scale(1./nevt_c2_f2), hJetPt_c3_f2->Scale(1./nevt_c3_f2);
  hJetPt_c4_f2->Scale(1./nevt_c4_f2), hJetPt_c5_f2->Scale(1./nevt_c5_f2), hJetPt_c6_f2->Scale(1./nevt_c6_f2);

  // Normalization
  TLegend *legd = new TLegend(0.35,0.73,0.72,0.90);
  legd->SetMargin(0.2);
  legd->SetBorderSize(0);
  legd->SetFillColor(kWhite);
  legd->SetFillStyle(0);
  legd->SetTextSize(0.035);
  legd->SetMargin(0.4);
  legd->SetHeader("Leading jet distribution (|#eta|<2.0)");
  legd->AddEntry(hJetPt_c1_f1,"2011","pl");
  legd->AddEntry(hJetPt_c1_f2,"2010","pl");


  // 0-5% ===========================================
  TCanvas *c0 = new TCanvas("c0","Jet Et distribution",500,500);
  c0->SetLogy();

  hJetPt_c1_f2->SetStats(0);
  hJetPt_c1_f2->GetYaxis()->SetTitle("dN/dE_{T}");
  hJetPt_c1_f2->SetMarkerSize(1.2);
  hJetPt_c1_f2->SetAxisRange(ptmin,ptmax,"X");
  hJetPt_c1_f2->SetAxisRange(ymin,ymax,"Y");
  hJetPt_c1_f2->Draw("pz");
  
  hJetPt_c1_f1->SetMarkerSize(1.2);
  hJetPt_c1_f1->SetMarkerColor(kRed);
  hJetPt_c1_f1->Draw("pzsame");

  legd->Draw();
  drawText("0-5 %",0.25,0.20);

  TCanvas *c0_1 = new TCanvas("c0_1","Jet Et distribution ratio",500,500);
  TH1F* hJetPt_c1_f1_cln = (TH1F*) hJetPt_c1_f1->Clone("hJetPt_c1_f1");
  TH1F* hJetPt_c1_f2_cln = (TH1F*) hJetPt_c1_f2->Clone("hJetPt_c1_f2");
  hJetPt_c1_f1_cln->Divide(hJetPt_c1_f2_cln);

  hJetPt_c1_f1_cln->SetStats(0);
  hJetPt_c1_f1_cln->GetYaxis()->SetTitle("Ratio (2011/2010)");
  hJetPt_c1_f1_cln->SetMarkerSize(1.2);
  hJetPt_c1_f1_cln->SetAxisRange(ptmin,ptmax,"X");
  hJetPt_c1_f1_cln->SetAxisRange(0,ymax_r,"Y");
  hJetPt_c1_f1_cln->Draw("pz");
  drawText("0-5 %",0.25,0.20);
  line->Draw();



  // 5-10% ===========================================  
  TCanvas *c1 = new TCanvas("c1","Jet Et distribution",500,500);
  c1->SetLogy();

  hJetPt_c2_f2->SetStats(0);
  hJetPt_c2_f2->GetYaxis()->SetTitle("dN/dE_{T}");
  hJetPt_c2_f2->SetMarkerSize(1.2);
  hJetPt_c2_f2->SetAxisRange(ptmin,ptmax,"X");
  hJetPt_c2_f2->SetAxisRange(ymin,ymax,"Y");
  hJetPt_c2_f2->Draw("pz");

  hJetPt_c2_f1->SetMarkerSize(1.2);
  hJetPt_c2_f1->SetMarkerColor(kRed);
  hJetPt_c2_f1->Draw("pzsame");

  legd->Draw();
  drawText("5-10 %",0.25,0.20);

  TCanvas *c1_1 = new TCanvas("c1_1","Jet Et distribution ratio",500,500);
  TH1F* hJetPt_c2_f1_cln = (TH1F*) hJetPt_c2_f1->Clone("hJetPt_c2_f1");
  TH1F* hJetPt_c2_f2_cln = (TH1F*) hJetPt_c2_f2->Clone("hJetPt_c2_f2");
  hJetPt_c2_f1_cln->Divide(hJetPt_c2_f2_cln);

  hJetPt_c2_f1_cln->SetStats(0);
  hJetPt_c2_f1_cln->GetYaxis()->SetTitle("Ratio (2011/2010)");
  hJetPt_c2_f1_cln->SetMarkerSize(1.2);
  hJetPt_c2_f1_cln->SetAxisRange(ptmin,ptmax,"X");
  hJetPt_c2_f1_cln->SetAxisRange(0,ymax_r,"Y");
  hJetPt_c2_f1_cln->Draw("pz");
  drawText("5-10 %",0.25,0.20);
  line->Draw();


  // 10-30% =====================================
  TCanvas *c2 = new TCanvas("c2","Jet Et distribution",500,500);
  c2->SetLogy();

  hJetPt_c3_f2->SetStats(0);
  hJetPt_c3_f2->GetYaxis()->SetTitle("dN/dE_{T}");
  hJetPt_c3_f2->SetMarkerSize(1.2);
  hJetPt_c3_f2->SetAxisRange(ptmin,ptmax,"X");
  hJetPt_c3_f2->SetAxisRange(ymin,ymax,"Y");
  hJetPt_c3_f2->Draw("pz");

  hJetPt_c3_f1->SetMarkerSize(1.2);
  hJetPt_c3_f1->SetMarkerColor(kRed);
  hJetPt_c3_f1->Draw("pzsame");

  legd->Draw();
  drawText("10-30 %",0.25,0.20);


  TCanvas *c2_1 = new TCanvas("c2_1","Jet Et distribution ratio",500,500);
  TH1F* hJetPt_c3_f1_cln = (TH1F*) hJetPt_c3_f1->Clone("hJetPt_c3_f1");
  TH1F* hJetPt_c3_f2_cln = (TH1F*) hJetPt_c3_f2->Clone("hJetPt_c3_f2");
  hJetPt_c3_f1_cln->Divide(hJetPt_c3_f2_cln);

  hJetPt_c3_f1_cln->SetStats(0);
  hJetPt_c3_f1_cln->GetYaxis()->SetTitle("Ratio (2011/2010)");
  hJetPt_c3_f1_cln->SetMarkerSize(1.2);
  hJetPt_c3_f1_cln->SetAxisRange(ptmin,ptmax,"X");
  hJetPt_c3_f1_cln->SetAxisRange(0,ymax_r,"Y");
  hJetPt_c3_f1_cln->Draw("pz");
  drawText("10-30 %",0.25,0.20);
  line->Draw();

  // 30-50 % ===============================
  TCanvas *c3 = new TCanvas("c3","Jet Et distribution",500,500);
  c3->SetLogy();

  hJetPt_c4_f2->SetStats(0);
  hJetPt_c4_f2->GetYaxis()->SetTitle("dN/dE_{T}");
  hJetPt_c4_f2->SetMarkerSize(1.2);
  hJetPt_c4_f2->SetAxisRange(ptmin,ptmax,"X");
  hJetPt_c4_f2->SetAxisRange(ymin,ymax,"Y");
  hJetPt_c4_f2->Draw("pz");

  hJetPt_c4_f1->SetMarkerSize(1.2);
  hJetPt_c4_f1->SetMarkerColor(kRed);
  hJetPt_c4_f1->Draw("pzsame");

  legd->Draw();
  drawText("30-50 %",0.25,0.20);

  TCanvas *c3_1 = new TCanvas("c3_1","Jet Et distribution ratio",500,500);
  TH1F* hJetPt_c4_f1_cln = (TH1F*) hJetPt_c4_f1->Clone("hJetPt_c4_f1");
  TH1F* hJetPt_c4_f2_cln = (TH1F*) hJetPt_c4_f2->Clone("hJetPt_c4_f2");
  hJetPt_c4_f1_cln->Divide(hJetPt_c4_f2_cln);

  hJetPt_c4_f1_cln->SetStats(0);
  hJetPt_c4_f1_cln->GetYaxis()->SetTitle("Ratio (2011/2010)");
  hJetPt_c4_f1_cln->SetMarkerSize(1.2);
  hJetPt_c4_f1_cln->SetAxisRange(ptmin,ptmax,"X");
  hJetPt_c4_f1_cln->SetAxisRange(0,ymax_r,"Y");
  hJetPt_c4_f1_cln->Draw("pz");
  drawText("30-50 %",0.25,0.20);
  line->Draw();


  // 50-70 % ==================================
  TCanvas *c4 = new TCanvas("c4","Jet Et distribution",500,500);
  c4->SetLogy();

  hJetPt_c5_f2->SetStats(0);
  hJetPt_c5_f2->GetYaxis()->SetTitle("dN/dE_{T}");
  hJetPt_c5_f2->SetMarkerSize(1.2);
  hJetPt_c5_f2->SetAxisRange(ptmin,ptmax,"X");
  hJetPt_c5_f2->SetAxisRange(ymin,ymax,"Y");
  hJetPt_c5_f2->Draw("pz");

  hJetPt_c5_f1->SetMarkerSize(1.2);
  hJetPt_c5_f1->SetMarkerColor(kRed);
  hJetPt_c5_f1->Draw("pzsame");

  legd->Draw();
  drawText("50-70 %",0.25,0.20);

  TCanvas *c4_1 = new TCanvas("c4_1","Jet Et distribution ratio",500,500);
  TH1F* hJetPt_c5_f1_cln = (TH1F*) hJetPt_c5_f1->Clone("hJetPt_c5_f1");
  TH1F* hJetPt_c5_f2_cln = (TH1F*) hJetPt_c5_f2->Clone("hJetPt_c5_f2");
  hJetPt_c5_f1_cln->Divide(hJetPt_c5_f2_cln);

  hJetPt_c5_f1_cln->SetStats(0);
  hJetPt_c5_f1_cln->GetYaxis()->SetTitle("Ratio (2011/2010)");
  hJetPt_c5_f1_cln->SetMarkerSize(1.2);
  hJetPt_c5_f1_cln->SetAxisRange(ptmin,ptmax,"X");
  hJetPt_c5_f1_cln->SetAxisRange(0,ymax_r,"Y");
  hJetPt_c5_f1_cln->Draw("pz");
  drawText("50-70 %",0.25,0.20);
  line->Draw();


  // 70-90% =================================
  TCanvas *c5 = new TCanvas("c5","Jet Et distribution",500,500);
  c5->SetLogy();

  hJetPt_c6_f2->SetStats(0);
  hJetPt_c6_f2->GetYaxis()->SetTitle("dN/dE_{T}");
  hJetPt_c6_f2->SetMarkerSize(1.2);
  hJetPt_c6_f2->SetAxisRange(ptmin,ptmax,"X");
  hJetPt_c6_f2->SetAxisRange(ymin,ymax,"Y");
  hJetPt_c6_f2->Draw("pz");

  hJetPt_c6_f1->SetMarkerSize(1.2);
  hJetPt_c6_f1->SetMarkerColor(kRed);
  hJetPt_c6_f1->Draw("pzsame");

  legd->Draw();
  drawText("70-90 %",0.25,0.20);

  TCanvas *c5_1 = new TCanvas("c5_1","Jet Et distribution ratio",500,500);
  TH1F* hJetPt_c6_f1_cln = (TH1F*) hJetPt_c6_f1->Clone("hJetPt_c6_f1");
  TH1F* hJetPt_c6_f2_cln = (TH1F*) hJetPt_c6_f2->Clone("hJetPt_c6_f2");
  hJetPt_c6_f1_cln->Divide(hJetPt_c6_f2_cln);

  hJetPt_c6_f1_cln->SetStats(0);
  hJetPt_c6_f1_cln->GetYaxis()->SetTitle("Ratio (2011/2010)");
  hJetPt_c6_f1_cln->SetMarkerSize(1.2);
  hJetPt_c6_f1_cln->SetAxisRange(ptmin,ptmax,"X");
  hJetPt_c6_f1_cln->SetAxisRange(0,ymax_r,"Y");
  hJetPt_c6_f1_cln->Draw("pz");
  drawText("70-90 %",0.25,0.20);
  line->Draw();


  // save output files
  if(save){
    c0->cd(), c0->Print("./figs/jet_et_dist_0to5.gif");
    c0_1->cd(), c0_1->Print("./figs/jet_et_dist_0to5_ratio.gif");

    c1->cd(), c1->Print("./figs/jet_et_dist_5to10.gif");
    c1_1->cd(), c1_1->Print("./figs/jet_et_dist_5to10_ratio.gif");

    c2->cd(), c2->Print("./figs/jet_et_dist_10to30.gif");
    c2_1->cd(), c2_1->Print("./figs/jet_et_dist_10to30_ratio.gif");

    c3->cd(), c3->Print("./figs/jet_et_dist_30to50.gif");
    c3_1->cd(), c3_1->Print("./figs/jet_et_dist_30to50_ratio.gif");

    c4->cd(), c4->Print("./figs/jet_et_dist_50to70.gif");
    c4_1->cd(), c4_1->Print("./figs/jet_et_dist_50to70_ratio.gif");

    c5->cd(), c5->Print("./figs/jet_et_dist_70to90.gif");
    c5_1->cd(), c5_1->Print("./figs/jet_et_dist_70to90_ratio.gif");


  }

}


void drawText(const char *text, float xp, float yp){
  TLatex *tex = new TLatex(xp,yp,text);
  tex->SetTextFont(63);
  tex->SetTextSize(20);
  tex->SetTextColor(kBlack);
  tex->SetLineWidth(1);
  tex->SetNDC();
  tex->Draw();
}
