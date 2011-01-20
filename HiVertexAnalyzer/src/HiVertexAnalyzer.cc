// Original Author:  Edward Allen Wenger,32 4-A06,+41227676980,
//         Created:  Fri May  7 13:11:39 CEST 2010
// $Id: HiVertexAnalyzer.cc,v 1.1 2010/11/05 20:14:48 sungho Exp $
//

#include "edwenger/HiVertexAnalyzer/interface/HiVertexAnalyzer.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "edwenger/HiVertexAnalyzer/interface/HiVertexComparator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

HiVertexAnalyzer::HiVertexAnalyzer(const edm::ParameterSet& iConfig)
:
  vtxlabel_(iConfig.getUntrackedParameter<edm::InputTag>("vtxlabel")),
  nTracksBins_(iConfig.getUntrackedParameter<int>("nTracksBins"))
{

}


// ------------ method called to for each event  ------------
void
HiVertexAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
    edm::LogVerbatim("HiVertexAnalyzer") << "vtx #" << i << " has " 
		     << vtxs[i].tracksSize() << " tracks";
  
}


// ------------ method called once each job just before starting event loop  ------------
void 
HiVertexAnalyzer::beginJob()
{

  hVtxSize = f->make<TH1D>("hVtxSize","number of reconstructed vertices",10,-0.5,9.5);
  hVtxTracks = f->make<TH1D>("hVtxTracks","number of tracks fitted to vertex",nTracksBins_,-0.5,(float)nTracksBins_-0.5);
  hVtxZ = f->make<TH1D>("hVtxZ","z position of best reconstructed vertex",120,-30.0,30.0);
  hMultVtxTracks = f->make<TH2D>("hMultVtxTracks","number of tracks fitted to vertex; most populated; 2nd most populated",nTracksBins_,-0.5,(float)nTracksBins_-0.5,nTracksBins_,-0.5,(float)nTracksBins_-0.5);
  hMultVtxZ = f->make<TH2D>("hMultVtxZ","z position of reconstructed vertex; most populated; 2nd most populated",120,-30.0,30.0,120,-30.0,30.0);
  hAllVtxZ = f->make<TH1D>("hAllVtxZ","z position of all reconstructed vertices",120,-30.0,30.0);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HiVertexAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HiVertexAnalyzer);
