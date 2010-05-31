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
    HistoGroup(TString name);
    void Add(TH1F* h1,Float_t sc=1);

    // data
    TString name_;
    std::map<TString,TH1F*> hm_;
    std::map<TString,Float_t> scales_;
};

HistoGroup::HistoGroup(TString name) :
  name_(name)
{
}

void HistoGroup::Add(TH1F* h1,Float_t sc) {
  assert(h1);
  hm_[h1->GetName()] = h1;
  scales_[h1->GetName()] = sc;
}
#endif
