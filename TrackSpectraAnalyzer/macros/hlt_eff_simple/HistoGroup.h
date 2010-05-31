#ifndef HISTOGROUP_H
#define HISTOGROUP_H

#include <map>
#include <cassert>
#include "TString.h"
#include "TH1F.h"

class HistoGroup
{
  public:
    // methods
    HistoGroup(TString name, Int_t n=0, Float_t xmin=0, Float_t xmax=0);
    void Add(TH1F* h1,Float_t sc=1);
    void Add(TString hname,Float_t sc=1);
    void Save();

    // data
    TString name_;
    std::map<TString,TH1F*> hm_;
    std::map<TString,TH1F*> hscm_;
    std::map<TString,Float_t> scales_;
    // histo properties
    Int_t nbins_;
    Float_t xmin_;
    Float_t xmax_;
};

HistoGroup::HistoGroup(TString name, Int_t n, Float_t xmin, Float_t xmax) :
  name_(name),
  nbins_(n),
  xmin_(xmin),
  xmax_(xmax)
{
}

void HistoGroup::Add(TH1F* h1,Float_t sc) {
  assert(h1);
  hm_[h1->GetName()] = h1;
  scales_[h1->GetName()] = sc;

  // now maked scaled histograms
  TString nameSc(Form("%s_scaled",h1->GetName()));
  TH1F * h2 = (TH1F*)h1->Clone(nameSc);
  h2->Scale(sc);
  hscm_[nameSc] = h2;
}

void HistoGroup::Add(TString hname,Float_t sc) {
  TH1F * h1 = new TH1F(hname,"",nbins_,xmin_,xmax_);
  Add(h1,sc);
}

void HistoGroup::Save()
{
  for (std::map<TString, TH1F*>::iterator 
      iter=hm_.begin(); iter != hm_.end(); ++iter) {
    iter->second->Write();
  }
  for (std::map<TString, TH1F*>::iterator 
      iter=hscm_.begin(); iter != hscm_.end(); ++iter) {
    iter->second->Write();
  }
}
#endif
