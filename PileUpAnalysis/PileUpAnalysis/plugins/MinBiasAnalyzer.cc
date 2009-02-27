#ifndef MinBiasAnalyzer_h
#define MinBiasAnalyzer_h
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TH1F.h"

#include <vector>
#include <map>

class InputTag;

class MinBiasAnalyzer  : public edm::EDAnalyzer {
 public:

  explicit MinBiasAnalyzer(const edm::ParameterSet& conf);

  virtual ~MinBiasAnalyzer(){}

  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  virtual void beginJob(const edm::EventSetup & setup);
  virtual void endJob();
 private:
  void bookHistos(int);

  edm::InputTag tracksTag_;
  edm::InputTag vertexTag_;

  int evtCount;
  // Histograms
  /*TH1F* hRecoEfficiencyvsSumPt2_;
  TH1F* hRecoEfficiencyvsSumPt_;
  TH1F* hRecoEfficiencyvsPosZ_;*/

  std::map<int,std::vector<TH1F*> > histoMap_;
};

#endif

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Provenance/interface/EventID.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include <FWCore/ServiceRegistry/interface/Service.h>
#include <PhysicsTools/UtilAlgos/interface/TFileService.h>

#include "HepMC/GenEvent.h"

#include <sstream>

MinBiasAnalyzer::MinBiasAnalyzer(const edm::ParameterSet& conf):
  tracksTag_(conf.getParameter<edm::InputTag>("TracksTag")),
  vertexTag_(conf.getParameter<edm::InputTag>("VertexTag")),evtCount(0) {}

void MinBiasAnalyzer::beginJob(const edm::EventSetup& setup) {

  // Book histograms
  /*edm::Service<TFileService> fs;
  hRecoEfficiencyvsSumPt2_ = fs->make<TH1F>("RecoEfficiencyvsSumPt2","RecoEfficiencyvsSumPt2",200,0.,200.);
  hRecoEfficiencyvsSumPt_ = fs->make<TH1F>("RecoEfficiencyvsSumPt","RecoEfficiencyvsSumPt",200,0.,50.);
  hRecoEfficiencyvsPosZ_ = fs->make<TH1F>("RecoEfficiencyvsPosZ","RecoEfficiencyvsPosZ",200,-30.,30.);*/
}

void MinBiasAnalyzer::endJob() {
  /*hRecoEfficiencyvsSumPt2_->Scale(1/(double)evtCount);
  hRecoEfficiencyvsSumPt_->Scale(1/(double)evtCount);
  hRecoEfficiencyvsPosZ_->Scale(1/(double)evtCount);*/
  for(std::map<int,std::vector<TH1F*> >::iterator itMap = histoMap_.begin(); itMap != histoMap_.end(); ++itMap){
    std::vector<TH1F*>& vecHistos = itMap->second;
    for(std::vector<TH1F*>::iterator histo = vecHistos.begin(); histo != vecHistos.end(); ++histo) (*histo)->Scale(1/(double)evtCount);
  }
}

void MinBiasAnalyzer::bookHistos(int processId){
  if(histoMap_.find(processId) == histoMap_.end()){
    edm::Service<TFileService> fs;

    std::stringstream dirStream;
    dirStream << "ProcessId" << processId;
  
    TFileDirectory subDir = fs->mkdir(dirStream.str().c_str());
    TH1F* h_RecoEfficiencyvsSumPt2 = subDir.make<TH1F>("RecoEfficiencyvsSumPt2","RecoEfficiencyvsSumPt2",200,0.,200.);
    TH1F* h_RecoEfficiencyvsSumPt = subDir.make<TH1F>("RecoEfficiencyvsSumPt","RecoEfficiencyvsSumPt",200,0.,50.);
    TH1F* h_RecoEfficiencyvsPosZ = subDir.make<TH1F>("RecoEfficiencyvsPosZ","RecoEfficiencyvsPosZ",200,-30.,30.);
    std::vector<TH1F*> vecHistos;
    vecHistos.push_back(h_RecoEfficiencyvsSumPt2);
    vecHistos.push_back(h_RecoEfficiencyvsSumPt);
    vecHistos.push_back(h_RecoEfficiencyvsPosZ);

    histoMap_.insert(std::make_pair(processId,vecHistos));
  }
}

void MinBiasAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& c){

  // Get generated HepMC events
  edm::Handle<edm::HepMCProduct> evtHepMCH;
  event.getByLabel("source",evtHepMCH);
  const HepMC::GenEvent* hepMCEvt = evtHepMCH->GetEvent();
  int processId = hepMCEvt->signal_process_id();
  if(histoMap_.find(processId) == histoMap_.end()) bookHistos(processId);

  // Get reconstructed tracks
  edm::Handle<edm::View<reco::Track> > trackCollectionH;
  event.getByLabel(tracksTag_,trackCollectionH);
  const edm::View<reco::Track> trkColl = *(trackCollectionH.product());

  // Get reconstructed vertexes
  edm::Handle<edm::View<reco::Vertex> > vertexCollectionH;
  event.getByLabel(vertexTag_,vertexCollectionH);
  const edm::View<reco::Vertex> vtxColl = *(vertexCollectionH.product());

  // Access primary vertex
  const reco::Vertex& primaryVertex = vtxColl.front();
  bool goodPrimaryVertex = ((primaryVertex.isValid())&&(!primaryVertex.isFake()));

  // Find number of good vertexes; match one closest to muon vertex 
  int nGoodVertexes = 0;
  for(edm::View<reco::Vertex>::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
    if(!vtx->isValid()) continue; // skip non-valid vertex
    if(vtx->isFake()) continue; // skip vertex from beam spot
    ++nGoodVertexes;
  }

  edm::LogVerbatim("Analysis") << " Number of reconstructed primary vertexes in event: " << nGoodVertexes;

  ++evtCount;
  if(!goodPrimaryVertex) return;

  double sumPtTracks = 0.;
  double sumPt2Tracks = 0.;
  for(reco::Vertex::trackRef_iterator vtxTrack = primaryVertex.tracks_begin();
                                      vtxTrack != primaryVertex.tracks_end(); ++vtxTrack){
    edm::RefToBase<reco::Track> track = *vtxTrack;
    sumPtTracks += track->pt();
    sumPt2Tracks += track->pt()*track->pt();
  }

  /*hRecoEfficiencyvsSumPt2_->Fill(sumPt2Tracks);
  hRecoEfficiencyvsSumPt_->Fill(sumPtTracks);
  hRecoEfficiencyvsPosZ_->Fill(primaryVertex.z());*/
  std::vector<TH1F*>& vecHistos = histoMap_[processId];
  vecHistos[0]->Fill(sumPt2Tracks);
  vecHistos[1]->Fill(sumPtTracks);
  vecHistos[2]->Fill(primaryVertex.z());
}

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(MinBiasAnalyzer);
