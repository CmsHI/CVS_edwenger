// Original Author:  Edward Allen Wenger,32 4-A06,+41227676980,
//         Created:  Fri May  7 13:11:39 CEST 2010
// $Id: VertexAnalyzer.cc,v 1.3 2010/05/13 09:26:48 edwenger Exp $
//

#include "edwenger/VertexAnalyzer/interface/VertexAnalyzer.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "edwenger/VertexAnalyzer/interface/VertexComparator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

VertexAnalyzer::VertexAnalyzer(const edm::ParameterSet& iConfig)
:
  vtxlabel_(iConfig.getUntrackedParameter<edm::InputTag>("vtxlabel")),
  nTracksBins_(iConfig.getUntrackedParameter<int>("nTracksBins"))
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

  for(unsigned i=0; i<vtxs.size(); i++)
    hAllVtxZ->Fill(vtxs[i].z());

  if(vtxs.size()<2) return;

  hMultVtxTracks->Fill(vtxs[0].tracksSize(),vtxs[1].tracksSize());
  hMultVtxZ->Fill(vtxs[0].z(),vtxs[1].z());

  for(unsigned i=0; i<vtxs.size(); i++)
    edm::LogVerbatim("VertexAnalyzer") << "vtx #" << i << " has " 
		     << vtxs[i].tracksSize() << " tracks";
  
}


// ------------ method called once each job just before starting event loop  ------------
void 
VertexAnalyzer::beginJob()
{

  hVtxSize = f->make<TH1D>("hVtxSize","number of reconstructed vertices",10,-0.5,9.5);
  hVtxTracks = f->make<TH1D>("hVtxTracks","number of tracks fitted to vertex",nTracksBins_,-0.5,(float)nTracksBins_-0.5);
  hVtxZ = f->make<TH1D>("hVtxZ","z position of best reconstructed vertex",80,-20.0,20.0);
  hMultVtxTracks = f->make<TH2D>("hMultVtxTracks","number of tracks fitted to vertex; most populated; 2nd most populated",nTracksBins_,-0.5,(float)nTracksBins_-0.5,nTracksBins_,-0.5,(float)nTracksBins_-0.5);
  hMultVtxZ = f->make<TH2D>("hMultVtxZ","z position of reconstructed vertex; most populated; 2nd most populated",80,-20.0,20.0,80,-20.0,20.0);
  hAllVtxZ = f->make<TH1D>("hAllVtxZ","z position of all reconstructed vertices",80,-20.0,20.0);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
VertexAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(VertexAnalyzer);
