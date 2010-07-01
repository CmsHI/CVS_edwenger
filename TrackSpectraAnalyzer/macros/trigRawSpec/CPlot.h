//==================================================================================================
// CPlot class:
// ------------
//   Class to manage a plot. Member methods are provided to facilitate properties of the plot
//   (e.g. styles, colors, combining histograms or graphs, etc.)
//
// USAGE: Need to be compiled on ROOT startup. Add the following line to rootlogon.C,
//
//        gROOT->Macro("CPlot.cc+");
//
//       (Modify path argument if rootlogon.C and CPlot.cc are in different directories)
//==================================================================================================

#ifndef CPLOT_HH
#define CPLOT_HH
#include <iostream>
#include <cassert>

#include <TROOT.h>
#include <TSystem.h>
#include <TPad.h>
#include <TFile.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TProfile.h>
#include <TLine.h>
#include <THStack.h>

//#include "RooGlobalFunc.h"
//#include "RooPlot.h"

//using namespace RooFit;

class CPlotItem {
public:
  CPlotItem():hist1D(0),hist2D(0),graph(0),prof(0),drawopt(""){}
  ~CPlotItem(){}

  TH1D* hist1D;
  TH2F* hist2D;
  TGraph* graph;
  TProfile* prof;
  TString drawopt;
};

class CPlot {
public:
  CPlot();
  CPlot(TString name, TString title, TString xtitle, TString ytitle);
//  CPlot(TString name, RooPlot* frame, TString title, TString xtitle, TString ytitle);
  ~CPlot(){}
  
  static TString sOutDir;  // output directory
  
//  // Clear the plot by resetting all object properties
//  void Clear(){}
  
  // Draw the plot to a given canvas
  void Draw(TPad *c, bool doSave=false, TString format="gif");
  
  // Adding a 1D histogram to the plot
  void AddHist1D(TH1D *h, TString drawopt="", int color=kBlack, int mksty=20, float mksz=-1, int linesty=1, int fillsty=0);    
  void AddHist1D(TH1D *h, TString label, TString drawopt, int color=kBlack, int mksty=20, float mksz=-1, int linesty=1, int fillsty=0);
  void AddHist1D(TFile *f, TString histName, TString drawopt="", int color=kBlack, int mksty=20, float mksz=-1, int linesty=1, int fillsty=0);
  void AddHist1D(TFile *f, TString histName, TString label, TString drawopt, int color=kBlack, int mksty=20, float mksz=-1, int linesty=1, int fillsty=0);
  
  // Adding a 1D histogram to a histogram stack
  void AddToStack(TH1D *h, int color);
  void AddToStack(TH1D *h, TString label, int color);
  void AddToStack(TFile *f, TString histName, int color);
  void AddToStack(TFile *f, TString histName, TString label, int color);
  
  // Adding a 2D histogram to the plot
  void AddHist2D(TH2F *h, TString drawopt="", int fillcolor=kWhite, int linecolor=kBlack);    
  void AddHist2D(TFile *f, TString histName, TString drawopt="", int fillcolor=kWhite, int linecolor=kBlack);

  // Adding a graph (with error bars) to the plot
  void AddGraph(TGraph *gr, TString drawopt, int color=kBlack, int marksty=kFullDotLarge, float mksz=-1, int linesty=1);    
  void AddGraph(TGraph *gr, TString label, TString drawopt, int color=kBlack, int marksty=kFullDotLarge, float mksz=-1, int linesty=1);
  void AddGraph(TFile *f, TString grName, TString drawopt, int color=kBlack, int marksty=kFullDotLarge, float mksz=-1, int linesty=1);
  void AddGraph(TFile *f, TString grName, TString label, TString drawopt, int color=kBlack, int marksty=kFullDotLarge, float mksz=-1, int linesty=1);

  // Adding a profile histogram to the plot
  void AddProfile(TProfile *gr, TString drawopt, int color=kBlack, int marksty=kFullDotLarge, float mksz=-1, int linesty=1);    
  void AddProfile(TProfile *gr, TString label, TString drawopt, int color=kBlack, int marksty=kFullDotLarge, float mksz=-1, int linesty=1);
  void AddProfile(TFile *f, TString prName, TString drawopt, int color=kBlack, int marksty=kFullDotLarge, float mksz=-1, int linesty=1);
  void AddProfile(TFile *f, TString prName, TString label, TString drawopt, int color=kBlack, int marksty=kFullDotLarge, float mksz=-1, int linesty=1);
  
  // Adding a text box to the plot
  void AddTextBox(TString text, double x1, double y1, double x2, double y2,
                  int bordersize=1, int textcolor=kBlack, int fillcolor=kWhite);
  void AddTextBox(double x1, double y1, double x2, double y2, 
                  int bordersize, int textcolor, int fillcolor, int nlines, ...);		  
  
  // Add a line between two points (x1,y1) and (x2,y2)
  void AddLine(double x1, double y1, double x2, double y2, int color=kBlack, int style=1);
  void AddLine(double x1, double y1, double x2, double y2, int color, int style, TString label);
  
  // Add a 1D function
  void AddFcn(TF1* fcn, int color=kBlack, int linesty=1);
  void AddFcn(TF1* fcn, TString label, int color=kBlack, int linesty=1); 
    
  // Set legend position
  void SetLegend(double x1, double y1, double x2, double y2) {
    assert(fLeg);
    fLeg->SetX1(x1); fLeg->SetY1(y1); fLeg->SetX2(x2); fLeg->SetY2(y2);
  }
  // Translate legend box
  void TransLegend(double dx, double dy) {
    assert(fLeg);
    fLeg->SetX1(fLeg->GetX1()+dx); fLeg->SetY1(fLeg->GetY1()+dy); 
    fLeg->SetX2(fLeg->GetX2()+dx); fLeg->SetY2(fLeg->GetY2()+dy);
  }  
  // Legend Header
  void SetLegendHeader(const char * header) {
    assert(fLeg);
    fLeg->SetHeader(header);
  }
  // Legend Style
  void SetLegendStyle(double tsize, double tfont=0) { fLegendLabelSize=tsize; fLegendLabelFont=tfont; }
  
  // Set stats box position
  void SetStats(double x, double y) { fStatsX = x; fStatsY = y; }
  
  // Translate stats box
  void TransStats(double dx, double dy) { fStatsX += dx; fStatsY += dy; }  
    
  //
  // Set general properties of the plot
  //
  void SetName(TString str)                { fName = str; }                // plot name (for output)
  void SetTitle(TString str)               { fTitle = str; }               // plot title
  void SetXTitle(TString str)              { fXTitle = str; }              // x-axis title
  void SetYTitle(TString str)              { fYTitle = str; }              // y-axis title
  void SetXRange(double xmin, double xmax) { fXmin = xmin; fXmax = xmax; } // x-axis range
  void SetYRange(double ymin, double ymax) { fYmin = ymin; fYmax = ymax; } // y-axis range
  void SetAxisLabeling(double labsize, double labfont, double titsize, double titfont, double xoffset, double yoffset) // axis labeling
  { fAxisLabelSize=labsize; fAxisLabelFont=labfont; fAxisTitleSize=titsize; fAxisTitleFont=titfont; fXAxisTitleOffset=xoffset; fYAxisTitleOffset=yoffset; }
  void SetLogx(int value=1)                { fLogx = value; }              // toggle logscale x-axis
  void SetLogy(int value=1)                { fLogy = value; }              // toggle logscale y-axis
  void SetGridx(bool value=1)              { fGridx = value; }             // toggle grid lines from x-axis ticks
  void SetGridy(bool value=1)              { fGridy = value; }             // toggle grid lines from y-axis ticks
  void Rebin(int ngroup)                   { fRebin = ngroup; }            // 1D histogram re-bin
  void ShowStats(int show=111)             { fShowStats = show; }          // display statistics
  void ShowLegend(int show=1)		   { fShowLegend = show; }          // display legend

  //
  // Accessors
  // 
  TLegend* GetLegend() { return fLeg; }
  
protected:
  std::vector<CPlotItem> fItems;        // list of items to be plotted
  std::vector<TPaveText*> fTextBoxes;   // list of text boxes
  std::vector<TLine*> fLines;           // list of lines
  std::vector<TF1*> fFcns;              // list of 1D functions
  THStack *fStack;                 // histogram stack
  TString fName;                   // plot name
  TString fTitle;                  // plot title
  TString fXTitle;                 // x-axis title
  TString fYTitle;                 // y-axis title
  double fXmin, fXmax;             // x-axis range
  double fYmin, fYmax;             // y-axis range
  double fAxisLabelFont, fAxisLabelSize, fAxisTitleFont, fAxisTitleSize, fXAxisTitleOffset, fYAxisTitleOffset; // axis labelling
  int fLogx, fLogy;                // logscale axes
  int fGridx, fGridy;              // grid lines
  int fRebin;                      // grouping for histogram re-bin
  TLegend *fLeg;                   // legend object
  int fShowStats;                  // whether to display statistics
  int fShowLegend;                  // whether to display legend
  double fLegendLabelFont, fLegendLabelSize;
  double fStatsX, fStatsY;         // x,y coordinates of top left corner of stats box
  
//  RooPlot *fRooPlot;
  
  static int sCount;               // number of CPlot instances
  static int sPlotStyle;
};

int CPlot::sCount = 0;
int CPlot::sPlotStyle = 0;
TString CPlot::sOutDir = ".";

#endif
