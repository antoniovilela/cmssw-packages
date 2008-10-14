#ifndef PileUpAnalysis_h
#define PileUpAnalysis_h
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"

#include <vector>
 
class InputTag;
class TrackAssociatorBase;
class VertexAssociatorBase;

class PileUpAnalysis  : public edm::EDAnalyzer {
 public:

  explicit PileUpAnalysis(const edm::ParameterSet& conf);

  virtual ~PileUpAnalysis(){}

  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  virtual void beginJob(const edm::EventSetup & setup);
 private:
  edm::InputTag tracksTag_;
  edm::InputTag verticesTag_;
  edm::InputTag trackAssociatorTag_;

  std::vector<int> bunchRange_;

  //TrackAssociatorBase* associatorByHits_;
  VertexAssociatorBase* associatorByTracks_;

  // Histograms
  TH1F* hTrackVzSignal_;
  TH1F* hTrackDzSignal_;
  TH1F* hTrackSigzSignal_;
  TH1F* hTrackDxySignal_;
  TH1F* hTrackSigxySignal_;
  TH1F* hTrackVzPileUp_;
  TH1F* hTrackDzPileUp_;
  TH1F* hTrackSigzPileUp_;
  TH1F* hTrackDxyPileUp_;
  TH1F* hTrackSigxyPileUp_; 

  TH1F* hTrackDistPVBx0Signal_;
  TH1F* hTrackDistPVBx0PileUp_;
  TH1F* hTrackDistPVOtherPileUp_;

  TH1F* hTrackDistMuonBx0Signal_;
  TH1F* hTrackDistMuonBx0PileUp_;
  TH1F* hTrackDistMuonOtherPileUp_;

  TH1F* hPrimVtxPosXBx0Signal_;
  TH1F* hPrimVtxPosXBx0PileUp_;
  TH1F* hPrimVtxPosXOtherPileUp_;

  TH1F* hPrimVtxPosYBx0Signal_;
  TH1F* hPrimVtxPosYBx0PileUp_;
  TH1F* hPrimVtxPosYOtherPileUp_;

  TH1F* hPrimVtxPosZBx0Signal_;
  TH1F* hPrimVtxPosZBx0PileUp_;
  TH1F* hPrimVtxPosZOtherPileUp_;

  TH1F* hMuonDistPV_;

  std::vector<TH1F*> hVecNrPileUp_;

  TH1F* hTrkMultBx0Signal_;
  TH1F* hTrkMultBx0PileUp_;
  TH1F* hTrkMultOtherPileUp_;

  TH1F* hNTracksAwayPV_;

  TH1F* hNPrimVertices_;
  TH1F* hNPrimVerticesSingleInt_;
  TH1F* hNPrimVerticesPileUp_;

  TProfile* profNPVvsNPUBx0_;
  TProfile* profNPUBx0vsNPV_;
  TH2F* hNPUBx0vsNPV_;
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

//#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
//#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
//#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFramePlaybackInfo.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/RecoCandidate/interface/TrackAssociation.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h"
#include "SimTracker/VertexAssociation/interface/VertexAssociatorBase.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/Records/interface/VertexAssociatorRecord.h"

//#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"

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

typedef std::map<std::pair<int,int>,unsigned int> TrackMultMap;

PileUpAnalysis::PileUpAnalysis(const edm::ParameterSet& conf):
  tracksTag_(conf.getParameter<edm::InputTag>("TracksTag")),
  verticesTag_(conf.getParameter<edm::InputTag>("VerticesTag")),
  trackAssociatorTag_(conf.getParameter<edm::InputTag>("TrackAssociatorTag")),
  bunchRange_(conf.getParameter<std::vector<int> >("BunchCrossings")){}

void PileUpAnalysis::beginJob(const edm::EventSetup& setup) {
  /*edm::ESHandle<MagneticField> theMF;
  setup.get<IdealMagneticFieldRecord>().get(theMF);
  edm::ESHandle<TrackAssociatorBase> theHitsAssociator;
  setup.get<TrackAssociatorRecord>().get("TrackAssociatorByHits",theHitsAssociator);
  associatorByHits_ = (TrackAssociatorBase *) theHitsAssociator.product();*/

  edm::ESHandle<VertexAssociatorBase> theTracksAssociator;
  setup.get<VertexAssociatorRecord>().get("VertexAssociatorByTracks",theTracksAssociator);
  associatorByTracks_ = (VertexAssociatorBase*)theTracksAssociator.product();

  // Book histograms
  edm::Service<TFileService> fs;
  hTrackVzSignal_ = fs->make<TH1F>("TrackVzSignal","TrackVzSignal",100,-10.,10.);
  hTrackDzSignal_ = fs->make<TH1F>("TrackDzSignal","TrackDzSignal",100,-10.,10.);
  hTrackSigzSignal_ = fs->make<TH1F>("TrackSigzSignal","TrackSigzSignal",100,-4.,4.);
  hTrackDxySignal_ = fs->make<TH1F>("TrackDxySignal","TrackDxySignal",100,-10.,10.);
  hTrackSigxySignal_ = fs->make<TH1F>("TrackSigxySignal","TrackSigxySignal",100,-4.,4.);
  hTrackVzPileUp_ = fs->make<TH1F>("TrackVzPileUp","TrackVzPileUp",100,-10.,10.);
  hTrackDzPileUp_ = fs->make<TH1F>("TrackDzPileUp","TrackDzPileUp",100,-10.,10.);
  hTrackSigzPileUp_ = fs->make<TH1F>("TrackSigzPileUp","TrackSigzPileUp",100,-4.,4.);
  hTrackDxyPileUp_ = fs->make<TH1F>("TrackDxyPileUp","TrackDxyPileUp",100,-10.,10.);
  hTrackSigxyPileUp_ = fs->make<TH1F>("TrackSigxyPileUp","TrackSigxyPileUp",100,-4.,4.);

  hTrackDistPVBx0Signal_ = fs->make<TH1F>("TrackDistPVBx0Signal","TrackDistPVBx0Signal",100,0.,10.);
  hTrackDistPVBx0PileUp_ = fs->make<TH1F>("TrackDistPVBx0PileUp","TrackDistPVBx0PileUp",100,0.,10.);
  hTrackDistPVOtherPileUp_ = fs->make<TH1F>("TrackDistPVOtherPileUp","TrackDistPVOtherPileUp",100,0.,10.);

  hTrackDistMuonBx0Signal_ = fs->make<TH1F>("TrackDistMuonBx0Signal","TrackDistMuonBx0Signal",100,-10.,10.);
  hTrackDistMuonBx0PileUp_ = fs->make<TH1F>("TrackDistMuonBx0PileUp","TrackDistMuonBx0PileUp",100,-10.,10.);
  hTrackDistMuonOtherPileUp_ = fs->make<TH1F>("TrackDistMuonOtherPileUp","TrackDistMuonOtherPileUp",100,-10.,10.);

  hPrimVtxPosXBx0Signal_ = fs->make<TH1F>("PrimVtxPosXBx0Signal","PrimVtxPosXBx0Signal",100,-10.,10.);
  hPrimVtxPosXBx0PileUp_ = fs->make<TH1F>("PrimVtxPosXBx0PileUp","PrimVtxPosXBx0PileUp",100,-10.,10.);
  hPrimVtxPosXOtherPileUp_ = fs->make<TH1F>("PrimVtxPosXOtherPileUp","PrimVtxPosXOtherPileUp",100,-10.,10.);

  hPrimVtxPosYBx0Signal_ = fs->make<TH1F>("PrimVtxPosYBx0Signal","PrimVtxPosYBx0Signal",100,-10.,10.);
  hPrimVtxPosYBx0PileUp_ = fs->make<TH1F>("PrimVtxPosYBx0PileUp","PrimVtxPosYBx0PileUp",100,-10.,10.);
  hPrimVtxPosYOtherPileUp_ = fs->make<TH1F>("PrimVtxPosYOtherPileUp","PrimVtxPosYOtherPileUp",100,-10.,10.);

  hPrimVtxPosZBx0Signal_ = fs->make<TH1F>("PrimVtxPosZBx0Signal","PrimVtxPosZBx0Signal",100,-10.,10.);
  hPrimVtxPosZBx0PileUp_ = fs->make<TH1F>("PrimVtxPosZBx0PileUp","PrimVtxPosZBx0PileUp",100,-10.,10.);
  hPrimVtxPosZOtherPileUp_ = fs->make<TH1F>("PrimVtxPosZOtherPileUp","PrimVtxPosZOtherPileUp",100,-10.,10.);

  hMuonDistPV_ = fs->make<TH1F>("MuonDistPV","MuonDistPV",100,-10.,10.);

  char hname[20];
  for(std::vector<int>::const_iterator bunch = bunchRange_.begin(); bunch != bunchRange_.end(); ++bunch){
    sprintf(hname,"NrPileUpBx%d",*bunch);
    hVecNrPileUp_.push_back(fs->make<TH1F>(hname,hname,10,0,10));
  }

  hTrkMultBx0Signal_ = fs->make<TH1F>("TrkMultBx0Signal","TrkMultBx0Signal",200,0,1000);
  hTrkMultBx0PileUp_ = fs->make<TH1F>("TrkMultBx0PileUp","TrkMultBx0PileUp",200,0,1000);
  hTrkMultOtherPileUp_ = fs->make<TH1F>("TrkMultOtherPileUp","TrkMultOtherPileUp",200,0,1000);

  hNTracksAwayPV_ = fs->make<TH1F>("NTracksAwayPV","NTracksAwayPV",100,0,100);
 
  hNPrimVertices_ = fs->make<TH1F>("NPrimVertices","NPrimVertices",10,0,10);
  hNPrimVerticesSingleInt_ = fs->make<TH1F>("NPrimVerticesSingleInt","NPrimVerticesSingleInt",10,0,10);
  hNPrimVerticesPileUp_ = fs->make<TH1F>("NPrimVerticesPileUp","NPrimVerticesPileUp",10,0,10);

  profNPVvsNPUBx0_ = fs->make<TProfile>("profNPVvsNPUBx0","profNPVvsNPUBx0",10,0,10);
  profNPUBx0vsNPV_ = fs->make<TProfile>("profNPUBx0vsNPV","profNPUBx0vsNPV",10,0,10);
  hNPUBx0vsNPV_ = fs->make<TH2F>("NPUBx0vsNPV","NPUBx0vsNPV",10,0,10,10,0,10);
}

void PileUpAnalysis::analyze(const edm::Event& event, const edm::EventSetup& c){

  /*edm::Handle<TrackingParticleCollection> elecPH;
  edm::Handle<TrackingParticleCollection> rawPH;
  edm::Handle<TrackingVertexCollection>   rawVH;*/
  edm::Handle<TrackingParticleCollection> mergedPH;
  edm::Handle<TrackingVertexCollection>   mergedVH;

  /*event.getByLabel("trackingtruthprod",                 rawPH);
  event.getByLabel("trackingtruthprod",                 rawVH);
  event.getByLabel("electrontruth","ElectronTrackTruth",elecPH);*/
  event.getByLabel("mergedtruth","MergedTrackTruth",    mergedPH);
  event.getByLabel("mergedtruth","MergedTrackTruth",    mergedVH);

  // Access CrossingFramePalybackInfo
  
  edm::Handle<CrossingFramePlaybackInfo>  playbackInfo;
  event.getByLabel("mix", playbackInfo);

  std::vector<edm::EventID> ids;
  std::vector<int> fileNrs;
  std::vector<unsigned int> nrEvents;
  const unsigned int s = 0;
  playbackInfo->getEventStartInfo(ids,fileNrs,nrEvents,s);

  if(bunchRange_.size() != nrEvents.size()){
    throw edm::Exception(edm::errors::Configuration,"PileUpAnalysisError") << " Expecting different bunch ranges\n"; 
  }

  bool singleInteraction = true;
  for(unsigned int ibunch = 0; ibunch < nrEvents.size(); ++ibunch){
    edm::LogVerbatim("Analysis") << " Number of added pile-up's for bunch index " << ibunch << ": " << nrEvents[ibunch]; 
    hVecNrPileUp_[ibunch]->Fill(nrEvents[ibunch]);
    if(nrEvents[ibunch] != 0) singleInteraction = false;
  }

  if(singleInteraction) edm::LogVerbatim("Analysis") << ">>>> Single Interaction Event";

  /*cout << endl << "Dump of initial vertices: " << endl;
  for (TrackingVertexCollection::const_iterator iVertex = rawVH->begin(); iVertex != rawVH->end(); ++iVertex) {
    cout << *iVertex;
  }

  cout << endl << "Dump of initital tracks: " << endl;
  for (TrackingParticleCollection::const_iterator iTrack = rawPH->begin(); iTrack != rawPH->end(); ++iTrack) {
    cout << *iTrack;
  }

  cout << endl << "Dump of electron tracks: " << endl;
  for (TrackingParticleCollection::const_iterator iTrack = elecPH->begin(); iTrack != elecPH->end(); ++iTrack) {
    cout << *iTrack;
  }*/

  /*cout << endl << "Dump of merged vertices: " << endl;
  for (TrackingVertexCollection::const_iterator iVertex = mergedVH->begin(); iVertex != mergedVH->end(); ++iVertex) {
    cout << endl << *iVertex;
    cout << "Daughters of this vertex:" << endl;
    for (tp_iterator iTrack = iVertex->daughterTracks_begin(); iTrack != iVertex->daughterTracks_end(); ++iTrack) {
      cout << **iTrack;
    }
  }*/

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

  // Get muon
  edm::Handle<edm::View<reco::Muon> > muonCollectionH;
  event.getByLabel("muons", muonCollectionH);
  edm::View<reco::Muon> muonColl = *(muonCollectionH.product());
  double ptcut = 10.;
  double maxpupt = 0.;
  const reco::Muon* muonPt10 = 0;
  for(edm::View<reco::Muon>::const_iterator mu = muonColl.begin(); mu != muonColl.end(); ++mu){
    if(!mu->isGlobalMuon()) continue;
    if(mu->pt() < ptcut) continue;
    if(mu->pt() > maxpupt){
        muonPt10 = &(*mu);
        maxpupt = muonPt10->pt();
    }
  }

  // Skip event if no muon found
  if(muonPt10 == 0){
    edm::LogError("Analysis") << ">>>>>  No muon passing cuts found in event..skipping";
    return;
  } else {
    edm::LogVerbatim("Analysis") << " Muon pt,eta,phi = " << muonPt10->pt() << ", " << muonPt10->eta() << ", " << muonPt10->phi();
  }

  // Fill difference of muon vertex to PV
  if(goodPrimaryVertex) hMuonDistPV_->Fill(muonPt10->vz() - primaryVertex.position().z());

  // TrackAssociator/VertexAssociator info 
  // RecoToSim
  edm::Handle<reco::RecoToSimCollection> recoToSimCollectionH;
  event.getByLabel(trackAssociatorTag_,recoToSimCollectionH);
  
  edm::LogVerbatim("Analysis") << ">>> Accessing track association (Reco->Sim)";
  //const reco::RecoToSimCollection& recoToSim = *recoToSimCollectionH.product();
  reco::RecoToSimCollection recoToSim = *recoToSimCollectionH.product();
  //reco::RecoToSimCollection recoToSim = associatorByHits_->associateRecoToSim(trackCollectionH,mergedPH,&event);

  edm::LogVerbatim("Analysis") << ">>> Accessing vertex association (Reco->Sim)";
  reco::VertexRecoToSimCollection vtxRecoToSim = associatorByTracks_->associateRecoToSim(vertexCollectionH,mergedVH,event,recoToSim);

  for(size_t ivtx = 0; ivtx < vtxColl.size(); ++ivtx) {
    //edm::RefToBase<reco::Vertex> vertex(vertexCollectionH, ivtx);
    reco::VertexRef vertex(vertexCollectionH,ivtx);

    if(!vertex->isValid()) continue; // skip non-valid vertices
    if(vertex->isFake()) continue; // skip vertex from beam spot

    // Get TrackingVertex corresponding to reconstructed vertex
    if(vtxRecoToSim.find(vertex) != vtxRecoToSim.end()){
        std::vector<std::pair<TrackingVertexRef, double> > tv = vtxRecoToSim[vertex];
        edm::LogVerbatim("Analysis") << "Reco Vertex position: ("  << vertex->position().x() << ","
                                     << vertex->position().y() << "," << vertex->position().z() << ")"
                                     <<  " matched to " << tv.size() << " MC Vertices";
        for (std::vector<std::pair<TrackingVertexRef, double> >::const_iterator it = tv.begin(); it != tv.end(); ++it) {
             TrackingVertexRef tvtx = it->first;
             double assocQuality = it->second;
             edm::LogVerbatim("Analysis") << "\t\tMCVertex " << tvtx.index() << " position: ("
                                          << tvtx->position().x() << "," << tvtx->position().y() << ","
                                          << tvtx->position().z() << ")" << " Quality: " << assocQuality;
        }

        // Check if best TV match comes from pile-up
        if(tv.size() != 0){
             TrackingVertexRef tvtx = tv.begin()->first;
             const EncodedEventId& evtId = tvtx->eventId();
             if(evtId.bunchCrossing() == 0){ // check if from signal bunch crossing
                 if(evtId.event() == 0){ // check if it comes from signal or pile-up
                     edm::LogVerbatim("Analysis") << "\t\tReco Vertex associated to MC Vertex " << tvtx.index()
                                                  << " position: (" << tvtx->position().x() << ","
                                                  << tvtx->position().y() << "," << tvtx->position().z() << ")"
                                                  << " comes from signal";
                     hPrimVtxPosXBx0Signal_->Fill(tvtx->position().x());
                     hPrimVtxPosYBx0Signal_->Fill(tvtx->position().y());
                     hPrimVtxPosZBx0Signal_->Fill(tvtx->position().z());
                 } else { // comes from pile-up
                     edm::LogVerbatim("Analysis") << "\t\tReco Vertex associated to MC Vertex " << tvtx.index()
                                                  << " position: (" << tvtx->position().x() << ","
                                                  << tvtx->position().y() << "," << tvtx->position().z() << ")"
                                                  << " comes from pile-up";
                     hPrimVtxPosXBx0PileUp_->Fill(tvtx->position().x());
                     hPrimVtxPosYBx0PileUp_->Fill(tvtx->position().y());
                     hPrimVtxPosZBx0PileUp_->Fill(tvtx->position().z());
                 }
             } else {// other bunch crossings
                     edm::LogVerbatim("Analysis") << "\t\tReco Vertex associated to MC Vertex " << tvtx.index()
                                                  << " position: (" << tvtx->position().x() << ","
                                                  << tvtx->position().y() << "," << tvtx->position().z() << ")"
                                                  << " comes from out-of-time bunch crossing";
                     hPrimVtxPosXOtherPileUp_->Fill(tvtx->position().x());
                     hPrimVtxPosYOtherPileUp_->Fill(tvtx->position().y());
                     hPrimVtxPosZOtherPileUp_->Fill(tvtx->position().z());
             }
        }
    }
  }

  // Find number of good vertices; match one closest to muon vertex 
  int nGoodVertices = 0;
  double mindist = 0.3;
  //const reco::Vertex* vertexAssocToMuon = 0;
  //edm::View<reco::Vertex>::const_iterator vertexAssocToMuon = vtxColl.end();
  reco::VertexCollection::const_iterator vertexAssocToMuon = vtxColl.end();
  //for(edm::View<reco::Vertex>::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
  for(reco::VertexCollection::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
    if(!vtx->isValid()) continue; // skip non-valid vertices
    if(vtx->isFake()) continue; // skip vertex from beam spot
    ++nGoodVertices;

    double dz = muonPt10->vz() - vtx->position().z();
    if(fabs(dz) < mindist){
        //vertexAssocToMuon = &(*vtx);
        vertexAssocToMuon = vtx;
        mindist = fabs(dz);
    }
  }
 
  edm::LogVerbatim("Analysis") << " Number of reconstructed primary vertices in event: " << nGoodVertices;  
  hNPrimVertices_->Fill(nGoodVertices);
  if(singleInteraction) hNPrimVerticesSingleInt_->Fill(nGoodVertices);
  else hNPrimVerticesPileUp_->Fill(nGoodVertices);

  // Number of events on bx 0
  int nPUBx0 = playbackInfo->getNrEvents(0,0);
  profNPVvsNPUBx0_->Fill(nPUBx0,nGoodVertices);
  profNPUBx0vsNPV_->Fill(nGoodVertices,nPUBx0);
  hNPUBx0vsNPV_->Fill(nGoodVertices,nPUBx0);

  // Check matched vertex to muon
  if(vertexAssocToMuon == vtxColl.end()){
    edm::LogError("Analysis") << ">>>>>  Could not associate any vertex to muon..skipping";
    return;
  } else {
    bool primVtxisMuon = (vertexAssocToMuon == vtxColl.begin()); // Hardest vertex is the one corresponding to the muon
    if(!primVtxisMuon){
        edm::LogError("Analysis") << ">>>>>  Hardest vertex is not associated to the muon..skipping";
        return;
    }
  }

  // Access TrackingVertex collection
  edm::LogVerbatim("Analysis") << " Tracking vertices summary:";
  for(TrackingVertexCollection::const_iterator iVertex = mergedVH->begin(); iVertex != mergedVH->end(); ++iVertex) {
    LogDebug("Analysis") << *iVertex; 
  }
 
  // Build map giving nr. of tracks vs (bx,evt)
  edm::LogVerbatim("Analysis") << " Tracking particles summary:";
  TrackMultMap trackMultMap;
  for(TrackingParticleCollection::const_iterator iTrack = mergedPH->begin(); iTrack != mergedPH->end(); ++iTrack) {
    //cout << *iTrack;
    const EncodedEventId& evtId = iTrack->eventId();
    /*std::cout << " Bunch crossing id: " << evtId.bunchCrossing() << std::endl;
    std::cout << " Event id: " << evtId.event() << std::endl;*/
    std::pair<int,int> key(evtId.bunchCrossing(),evtId.event()); 
    TrackMultMap::iterator itr = trackMultMap.find(key);
    if(itr == trackMultMap.end()) trackMultMap.insert(TrackMultMap::value_type(key,0));
    else ++(itr->second); 
  }

  // Print out track mult.
  for(TrackMultMap::const_iterator it = trackMultMap.begin(); it != trackMultMap.end(); ++it){
    int bunchCrossing = it->first.first;
    int event = it->first.second;
    unsigned int ntracks = it->second;
    edm::LogVerbatim("Analysis") << " Number of tracks in (bx,evt) (" << bunchCrossing << "," << event << "): " << ntracks; 

    if(bunchCrossing == 0){ // track multiplicity for bunchCrossing 0
        if(event == 0){ // signal
             hTrkMultBx0Signal_->Fill(ntracks);
        } else { // pile-up
             hTrkMultBx0PileUp_->Fill(ntracks);
        }
    } else hTrkMultOtherPileUp_->Fill(ntracks); // pile-up from other bunch crossing's   
  }

  /*// Get reconstructed tracks
  edm::Handle<edm::View<reco::Track> > trackCollectionH;
  event.getByLabel(tracksTag_,trackCollectionH);
  const edm::View<reco::Track> trkColl = *(trackCollectionH.product());*/

  /*// TrackAssociator info 

  // RecoToSim
  edm::Handle<reco::RecoToSimCollection> recoToSimCollectionH;
  event.getByLabel(trackAssociatorTag_,recoToSimCollectionH);

  const reco::RecoToSimCollection& recoToSim = *recoToSimCollectionH.product();

  //reco::RecoToSimCollection recoToSim = associatorByHits_->associateRecoToSim(trackCollectionH,mergedPH,&event);*/


  // Find simulated track corresponding to reconstructed one
  
  //const reco::Vertex& primaryVertex = vtxColl[0];
  //bool goodPrimaryVertex = ((primaryVertex.isValid())&&(!primaryVertex.isFake()));
  double distminpv = 0.4;
  int ntracks_away = 0;
  for(edm::View<reco::Track>::size_type i=0; i < trkColl.size(); ++i) {
  //for(size_te itrk = 0; itrk < trkColl.size(); ++itrk) {
    edm::RefToBase<reco::Track> track(trackCollectionH, i);
    //reco::TrackRef track(trackCollectionH, itrk);

    math::XYZPoint trackDistPV;
    trackDistPV.SetX(track->vx() - primaryVertex.position().x());
    trackDistPV.SetY(track->vy() - primaryVertex.position().y());
    trackDistPV.SetZ(track->vz() - primaryVertex.position().z());

    if(goodPrimaryVertex&&(sqrt(trackDistPV.mag2()) >= distminpv)) ++ntracks_away;

    if(recoToSim.find(track) != recoToSim.end()){
        std::vector<std::pair<TrackingParticleRef, double> > tp = recoToSim[track];
	edm::LogVerbatim("Analysis") << "Reco Track pT: "  << track->pt() 
	                             <<  " matched to " << tp.size() << " MC Tracks";
	for (std::vector<std::pair<TrackingParticleRef, double> >::const_iterator it = tp.begin(); 
	     it != tp.end(); ++it) {
	     TrackingParticleRef tpr = it->first;
	     double assocQuality = it->second;
	     edm::LogVerbatim("Analysis") << "\t\tMCTrack " << tpr.index() << " pT: " << tpr->pt() << " NShared: " << assocQuality;
	}
         
        // Check if best TP match comes from pile-up
        if(tp.size() != 0){
             TrackingParticleRef tpr = tp.begin()->first;
             //double associationQuality = tp.begin()->second;	
             const EncodedEventId& evtId = tpr->eventId();
	     if(evtId.bunchCrossing() == 0){ // check if from signal bunch crossing
                 if(evtId.event() == 0){ // check if it comes from signal or pile-up
	             edm::LogVerbatim("Analysis") << "\t\tReco Track associated to MCTrack " << tpr.index() << " pT: " << tpr->pt() << " comes from signal";		
                     //hTrackVzSignal_->Fill(fabs(track->vz()));
                     //hTrackDzSignal_->Fill(fabs(track->dz()));
                     hTrackVzSignal_->Fill(track->vz());
                     hTrackDzSignal_->Fill(track->dz());
                     hTrackSigzSignal_->Fill(track->dz()/track->dzError());
                     hTrackDxySignal_->Fill(track->dxy());
                     hTrackSigxySignal_->Fill(track->dxy()/track->dxyError());

                     //if(goodPrimaryVertex) hTrackDistPVBx0Signal_->Fill(track->vz() - primaryVertex.position().z());
                     if(goodPrimaryVertex) hTrackDistPVBx0Signal_->Fill(sqrt(trackDistPV.mag2()));
                     hTrackDistMuonBx0Signal_->Fill(track->vz() - muonPt10->vz()); 
                 } else {
                     edm::LogVerbatim("Analysis") << "\t\tReco Track associated to MCTrack " << tpr.index() << " pT: " << tpr->pt() << " comes from pile-up";
                     //hTrackVzPileUp_->Fill(fabs(track->vz()));
                     //hTrackDzPileUp_->Fill(fabs(track->dz()));
                     hTrackVzPileUp_->Fill(track->vz());
                     hTrackDzPileUp_->Fill(track->dz());
                     hTrackSigzPileUp_->Fill(track->dz()/track->dzError());
                     hTrackDxyPileUp_->Fill(track->dxy());
                     hTrackSigxyPileUp_->Fill(track->dxy()/track->dxyError());

                     //if(goodPrimaryVertex) hTrackDistPVBx0PileUp_->Fill(track->vz() - primaryVertex.position().z());
                     if(goodPrimaryVertex) hTrackDistPVBx0PileUp_->Fill(sqrt(trackDistPV.mag2()));
                     hTrackDistMuonBx0PileUp_->Fill(track->vz() - muonPt10->vz());
                }
	     } else { // other bunch crossings
                     edm::LogVerbatim("Analysis") << "\t\tReco Track associated to MCTrack " << tpr.index() << " pT: " << tpr->pt() << " comes from out-of-time bunch crossing";
                     //if(goodPrimaryVertex) hTrackDistPVOtherPileUp_->Fill(track->vz() - primaryVertex.position().z());
                     if(goodPrimaryVertex) hTrackDistPVOtherPileUp_->Fill(sqrt(trackDistPV.mag2()));
                     hTrackDistMuonOtherPileUp_->Fill(track->vz() - muonPt10->vz());
             }
        }
    } else edm::LogVerbatim("Analysis") << "->   Track pT: " << track->pt() <<  " matched to 0  MC Tracks";
  }
  hNTracksAwayPV_->Fill(ntracks_away);
}

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(PileUpAnalysis);
