#ifndef HISTOGROUP_H
#define HISTOGROUP_H

#include <map>
#include <cassert>
#include "TString.h"
#include "TH1F.h"
#include "TFile.h"
#include <iostream>

class HisGroup
{
  public:
    // methods
    HisGroup(TString name, Int_t n=0, Float_t xmin=0, Float_t xmax=0);
    void Add(TH1F* h1,Float_t sc=1);
    void Add(TString hname,Float_t sc=1);
    void Add(TFile * inFile, TString hname,Float_t sc=1);
    void Save();
    void Scale();
    TH1F * Sum();

    // data
    TString name_;
    std::map<TString,TH1F*> hm_;
    std::map<TString,TH1F*> hscm_;
    std::map<TString,Float_t> scales_;
    // group properties
    Int_t nbins_;
    Float_t xmin_;
    Float_t xmax_;

    // group relations
    TH1F * hSum_;
};

HisGroup::HisGroup(TString name, Int_t n, Float_t xmin, Float_t xmax) :
  name_(name),
  nbins_(n),
  xmin_(xmin),
  xmax_(xmax)
{
  TH1::SetDefaultSumw2();
}

void HisGroup::Add(TH1F* h1,Float_t sc)
{
  assert(h1);
  hm_[h1->GetName()] = h1;
  scales_[h1->GetName()] = sc;
}

void HisGroup::Add(TString hname,Float_t sc)
{
  TH1F * h1 = new TH1F(hname,"",nbins_,xmin_,xmax_);
  Add(h1,sc);
}

void HisGroup::Add(TFile * inFile, TString hname,Float_t sc)
{
  assert(inFile);
  TH1F * h1;
  inFile->GetObject(hname.Data(),h1);
  Add(h1,sc);
}

void HisGroup::Save()
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

void HisGroup::Scale()
{
  for (std::map<TString, TH1F*>::iterator 
      iter=hm_.begin(); iter != hm_.end(); ++iter) {
    // now maked scaled histograms
    TString nameSc(Form("%s_scaled",iter->second->GetName()));
    TH1F * h2 = (TH1F*)iter->second->Clone(nameSc.Data());
    h2->Scale(scales_[iter->first]);
    hscm_[nameSc] = h2;
  }
}

TH1F * HisGroup::Sum()
{
  for (std::map<TString, TH1F*>::iterator 
      iter=hm_.begin(); iter != hm_.end(); ++iter) {
    if (iter==hm_.begin()) {
      hSum_ = (TH1F*)iter->second->Clone(name_+"_Sum");
      std::cout << "first to add: " << iter->first << endl;
    } else {
      std::cout << "add more: " << iter->first << endl;
      hSum_->Add(iter->second);
    }
  }
  return hSum_;
}
#endif
