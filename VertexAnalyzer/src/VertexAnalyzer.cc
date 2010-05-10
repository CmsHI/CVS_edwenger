// Original Author:  Edward Allen Wenger,32 4-A06,+41227676980,
//         Created:  Fri May  7 13:11:39 CEST 2010
// $Id: VertexAnalyzer.cc,v 1.1 2010/05/07 15:05:14 edwenger Exp $
//

#include "edwenger/VertexAnalyzer/interface/VertexAnalyzer.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "edwenger/VertexAnalyzer/interface/VertexComparator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

VertexAnalyzer::VertexAnalyzer(const edm::ParameterSet& iConfig)
:
  vtxlabel_(iConfig.getUntrackedParameter<edm::InputTag>("vtxlabel"))
{

}


// ------------ method called to for each event  ------------
void
VertexAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  //------- Vertices ---------------------
  edm::Handle<reco::VertexCollection> vtxsH;
  iEvent.getByLabel(vtxlabel_,vtxsH);

  reco::VertexCollection vtxs = *vtxsH;
  std::sort(vtxs.begin(),vtxs.end(),MoreTracksThenLowerChi2<reco::Vertex>());

  hVtxSize->Fill(vtxs.size());
  
  if(!vtxs.size()) return;

  hVtxTracks->Fill(vtxs[0].tracksSize());
  hVtxZ->Fill(vtxs[0].z());

  if(vtxs.size()<2) return;

  for(unsigned i=0; i<vtxs.size(); i++)
    edm::LogVerbatim("VertexAnalyzer") << "vtx #" << i << " has " 
		     << vtxs[i].tracksSize() << " tracks";
  
}


// ------------ method called once each job just before starting event loop  ------------
void 
VertexAnalyzer::beginJob()
{

  hVtxSize = f->make<TH1D>("hVtxSize","number of reconstructed vertices",10,-0.5,9.5);
  hVtxTracks = f->make<TH1D>("hVtxTracks","number of tracks fitted to vertex",50,-0.5,49.5);
  hVtxZ = f->make<TH1D>("hVtxZ","z position of best reconstructed vertex",80,-20.0,20.0);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
VertexAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(VertexAnalyzer);
