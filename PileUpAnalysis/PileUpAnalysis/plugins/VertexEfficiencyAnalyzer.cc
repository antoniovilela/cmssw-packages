#ifndef VertexEfficiencyAnalyzer_h
#define VertexEfficiencyAnalyzer_h
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TH1F.h"

class InputTag;
//class TrackAssociatorBase;
class VertexAssociatorBase;

class VertexEfficiencyAnalyzer  : public edm::EDAnalyzer {
 public:

  explicit VertexEfficiencyAnalyzer(const edm::ParameterSet& conf);

  virtual ~VertexEfficiencyAnalyzer(){}

  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  virtual void beginJob(const edm::EventSetup & setup);
  virtual void endJob();
 private:
  edm::InputTag tracksTag_;
  edm::InputTag verticesTag_;
  edm::InputTag trackAssociatorTag_;

  float nMatchedVertices_,nMatchedVerticesBx0Signal_,nMatchedVerticesBx0PileUp_;
  //TrackAssociatorBase* associatorByHits_;
  VertexAssociatorBase* associatorByTracks_;

  // Histograms
  TH1F* hRecoEfficiencyvsQualityCut_;
  TH1F* hRecoEfficiencyvsQualityCutBx0Signal_;
  TH1F* hRecoEfficiencyvsQualityCutBx0PileUp_;
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

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Provenance/interface/EventID.h"

#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/RecoCandidate/interface/TrackAssociation.h"

#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h"
#include "SimTracker/VertexAssociation/interface/VertexAssociatorBase.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/Records/interface/VertexAssociatorRecord.h"

#include <FWCore/ServiceRegistry/interface/Service.h>
#include <PhysicsTools/UtilAlgos/interface/TFileService.h>

#include <vector>
#include <map>

typedef edm::RefVector< std::vector<TrackingParticle> > TrackingParticleContainer;
typedef std::vector<TrackingParticle>                   TrackingParticleCollection;

typedef TrackingParticleRefVector::iterator               tp_iterator;
typedef TrackingParticle::g4t_iterator                   g4t_iterator;
typedef TrackingParticle::genp_iterator                 genp_iterator;
typedef TrackingVertex::genv_iterator                   genv_iterator;
typedef TrackingVertex::g4v_iterator                     g4v_iterator;

VertexEfficiencyAnalyzer::VertexEfficiencyAnalyzer(const edm::ParameterSet& conf):
  tracksTag_(conf.getParameter<edm::InputTag>("TracksTag")),
  verticesTag_(conf.getParameter<edm::InputTag>("VerticesTag")),
  trackAssociatorTag_(conf.getParameter<edm::InputTag>("TrackAssociatorTag")),
  nMatchedVertices_(0),nMatchedVerticesBx0Signal_(0),nMatchedVerticesBx0PileUp_(0){}

void VertexEfficiencyAnalyzer::beginJob(const edm::EventSetup& setup) {

  edm::ESHandle<VertexAssociatorBase> theTracksAssociator;
  setup.get<VertexAssociatorRecord>().get("VertexAssociatorByTracks",theTracksAssociator);
  associatorByTracks_ = (VertexAssociatorBase*)theTracksAssociator.product();

  // Book histograms
  edm::Service<TFileService> fs;
  hRecoEfficiencyvsQualityCut_ = fs->make<TH1F>("RecoEfficiencyvsQualityCut","RecoEfficiencyvsQualityCut",200,0.,1.);
  hRecoEfficiencyvsQualityCutBx0Signal_ = fs->make<TH1F>("RecoEfficiencyvsQualityCutBx0Signal","RecoEfficiencyvsQualityCutBx0Signal",200,0.,1.);
  hRecoEfficiencyvsQualityCutBx0PileUp_ = fs->make<TH1F>("RecoEfficiencyvsQualityCutBx0PileUp","RecoEfficiencyvsQualityCutBx0PileUp",200,0.,1.);
}

void VertexEfficiencyAnalyzer::endJob(){
  if(nMatchedVertices_) hRecoEfficiencyvsQualityCut_->Scale(1./nMatchedVertices_);
  if(nMatchedVerticesBx0Signal_) hRecoEfficiencyvsQualityCutBx0Signal_->Scale(1./nMatchedVerticesBx0Signal_);
  if(nMatchedVerticesBx0PileUp_) hRecoEfficiencyvsQualityCutBx0PileUp_->Scale(1./nMatchedVerticesBx0PileUp_);
}

void VertexEfficiencyAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& c){

  edm::Handle<TrackingParticleCollection> mergedPH;
  edm::Handle<TrackingVertexCollection>   mergedVH;

  event.getByLabel("mergedtruth","MergedTrackTruth",    mergedPH);
  event.getByLabel("mergedtruth","MergedTrackTruth",    mergedVH);

  // Get reconstructed tracks
  edm::Handle<edm::View<reco::Track> > trackCollectionH;
  event.getByLabel(tracksTag_,trackCollectionH);
  const edm::View<reco::Track> trkColl = *(trackCollectionH.product());
  /*edm::Handle<reco::TrackCollection> trackCollectionH;
  event.getByLabel(tracksTag_,trackCollectionH);
  const reco::TrackCollection& trkColl = *(trackCollectionH.product());*/

  // Get reconstructed vertices
  /*edm::Handle<edm::View<reco::Vertex> > vertexCollectionH;
  event.getByLabel(verticesTag_,vertexCollectionH);
  const edm::View<reco::Vertex> vtxColl = *(vertexCollectionH.product());*/
  edm::Handle<reco::VertexCollection> vertexCollectionH;
  event.getByLabel(verticesTag_,vertexCollectionH);
  const reco::VertexCollection& vtxColl = *(vertexCollectionH.product());

  // Access primary vertex
  const reco::Vertex& primaryVertex = vtxColl.front();
  bool goodPrimaryVertex = ((primaryVertex.isValid())&&(!primaryVertex.isFake()));

  // Find number of good vertices; match one closest to muon vertex 
  int nGoodVertices = 0;
  //for(edm::View<reco::Vertex>::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
  for(reco::VertexCollection::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
    if(!vtx->isValid()) continue; // skip non-valid vertices
    if(vtx->isFake()) continue; // skip vertex from beam spot
    ++nGoodVertices;
  }

  edm::LogVerbatim("Analysis") << " Number of reconstructed primary vertices in event: " << nGoodVertices;

  // TrackAssociator/VertexAssociator info 
  // SimToReco
  edm::Handle<reco::SimToRecoCollection> simToRecoCollectionH;
  event.getByLabel(trackAssociatorTag_,simToRecoCollectionH);
  
  edm::LogVerbatim("Analysis") << ">>> Accessing track association (Sim->Reco)";
  reco::SimToRecoCollection simToReco = *simToRecoCollectionH.product();

  edm::LogVerbatim("Analysis") << " Number of associated tracks: " << simToReco.size();

  edm::LogVerbatim("Analysis") << ">>> Accessing vertex association (Sim->Reco)";
  //reco::VertexSimToRecoCollection vtxSimToReco = associatorByTracks_->associateSimToReco(vertexCollectionH,mergedVH,event,simToReco);
  reco::VertexSimToRecoCollection vtxSimToReco = associatorByTracks_->associateSimToRecoBestTrackMatch(vertexCollectionH,mergedVH,event,simToReco);

  edm::LogVerbatim("Analysis") << " Number of associated vertices: " << vtxSimToReco.size();

  for(reco::VertexSimToRecoCollection::const_iterator itSimtoReco = vtxSimToReco.begin(); itSimtoReco != vtxSimToReco.end(); ++itSimtoReco){
    TrackingVertexRef simVertex = itSimtoReco->key;
    std::vector<std::pair<reco::VertexRef, double> > recoVertices = itSimtoReco->val;

    edm::LogVerbatim("Analysis") << "Sim Vertex at position: ("  << simVertex->position().x() << ","
                                 << simVertex->position().y() << "," << simVertex->position().z() << ")"
                                 <<  " matched to " << recoVertices.size() << " Reconstructed Vertices";

    // Check if sim vertex from signal or pile-up
    const EncodedEventId& evtId = simVertex->eventId();
    if(evtId.bunchCrossing() == 0){ // check if from signal bunch crossing
        if(evtId.event() == 0){ // comes from signal
             edm::LogVerbatim("Analysis") << "\t\tComes from signal";
        } else { // comes from pile-up
             edm::LogVerbatim("Analysis") << "\t\tComes from pile-up";
        }
    } else { // comes from other bunch crossing 
        edm::LogVerbatim("Analysis") << "\t\tComes from out-of-time pile-up";
    }

    for(std::vector<std::pair<reco::VertexRef, double> >::const_iterator vtxMatch = recoVertices.begin(); vtxMatch != recoVertices.end(); ++vtxMatch){
        reco::VertexRef recoVtx = vtxMatch->first;
        double assocQuality = vtxMatch->second;

        if(!recoVtx->isValid()) continue; // skip non-valid vertices
        if(recoVtx->isFake()) continue; // skip vertex from beam spot
 
        edm::LogVerbatim("Analysis") << "\t\tReco Vertex at position: ("
                                          << recoVtx->position().x() << "," << recoVtx->position().y() << ","
                                          << recoVtx->position().z() << ")" << " Quality: " << assocQuality;
    }

    // Get best match
    if(recoVertices.size() != 0){
        reco::VertexRef recoVtxBestMatch = recoVertices.begin()->first;
        if(recoVtxBestMatch->isValid()&&(!recoVtxBestMatch->isFake())){
             ++nMatchedVertices_; 
             if(evtId.bunchCrossing() == 0){
                  if(evtId.event() == 0) ++nMatchedVerticesBx0Signal_;
                  else ++nMatchedVerticesBx0PileUp_;
             }
             double qualityBestMatch = recoVertices.begin()->second;
             for(int ibin = 1; ibin <= hRecoEfficiencyvsQualityCut_->GetNbinsX(); ++ibin){
                  // Get bin lower edge
                  double xlow = hRecoEfficiencyvsQualityCut_->GetXaxis()->GetBinLowEdge(ibin);
                  if(qualityBestMatch >= xlow){
                       hRecoEfficiencyvsQualityCut_->Fill(xlow);
        	       if(evtId.bunchCrossing() == 0){
                          if(evtId.event() == 0) hRecoEfficiencyvsQualityCutBx0Signal_->Fill(xlow);
                          else hRecoEfficiencyvsQualityCutBx0PileUp_->Fill(xlow);
                       }
                  }
             }
        }
    }
  }

}

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(VertexEfficiencyAnalyzer);
