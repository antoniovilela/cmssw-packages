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
  edm::InputTag vertexTag_;
  edm::InputTag trackAssociatorTag_;

  float nMatchedVertexes_,nMatchedVertexesBx0Signal_,nMatchedVertexesBx0PileUp_;
  float nVertexes_,nVertexesBx0Signal_,nVertexesBx0PileUp_;
 
  //TrackAssociatorBase* associatorByHits_;
  VertexAssociatorBase* associatorByTracks_;

  // Histograms
  TH1F* hRecoEfficiencyvsQualityCut_;
  TH1F* hRecoEfficiencyvsQualityCutBx0Signal_;
  TH1F* hRecoEfficiencyvsQualityCutBx0PileUp_;

  TH1F* hRecoEfficiencyvsSumPt2_;
  TH1F* hRecoEfficiencyvsSumPt2Bx0Signal_;
  TH1F* hRecoEfficiencyvsSumPt2Bx0PileUp_;

  TH1F* hSimVertexSumPt2_;
  TH1F* hSimVertexSumPt2Bx0Signal_;
  TH1F* hSimVertexSumPt2Bx0PileUp_;

  TH1F* hRecoEfficiencyvsSumPt_;
  TH1F* hRecoEfficiencyvsSumPtBx0Signal_;
  TH1F* hRecoEfficiencyvsSumPtBx0PileUp_;

  TH1F* hSimVertexSumPt_;
  TH1F* hSimVertexSumPtBx0Signal_;
  TH1F* hSimVertexSumPtBx0PileUp_;
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
  vertexTag_(conf.getParameter<edm::InputTag>("VertexTag")),
  trackAssociatorTag_(conf.getParameter<edm::InputTag>("TrackAssociatorTag")),
  nMatchedVertexes_(0),nMatchedVertexesBx0Signal_(0),nMatchedVertexesBx0PileUp_(0),
  nVertexes_(0),nVertexesBx0Signal_(0),nVertexesBx0PileUp_(0){}

void VertexEfficiencyAnalyzer::beginJob(const edm::EventSetup& setup) {

  edm::ESHandle<VertexAssociatorBase> theTracksAssociator;
  setup.get<VertexAssociatorRecord>().get("VertexAssociatorByTracks",theTracksAssociator);
  associatorByTracks_ = (VertexAssociatorBase*)theTracksAssociator.product();

  // Book histograms
  edm::Service<TFileService> fs;
  hRecoEfficiencyvsQualityCut_ = fs->make<TH1F>("RecoEfficiencyvsQualityCut","RecoEfficiencyvsQualityCut",200,0.,1.);
  hRecoEfficiencyvsQualityCutBx0Signal_ = fs->make<TH1F>("RecoEfficiencyvsQualityCutBx0Signal","RecoEfficiencyvsQualityCutBx0Signal",200,0.,1.);
  hRecoEfficiencyvsQualityCutBx0PileUp_ = fs->make<TH1F>("RecoEfficiencyvsQualityCutBx0PileUp","RecoEfficiencyvsQualityCutBx0PileUp",200,0.,1.);

  hRecoEfficiencyvsSumPt2_ = fs->make<TH1F>("RecoEfficiencyvsSumPt2","RecoEfficiencyvsSumPt2",200,0.,100.);
  hRecoEfficiencyvsSumPt2Bx0Signal_ = fs->make<TH1F>("RecoEfficiencyvsSumPt2Bx0Signal","RecoEfficiencyvsSumPt2Bx0Signal",200,0.,100.);
  hRecoEfficiencyvsSumPt2Bx0PileUp_ = fs->make<TH1F>("RecoEfficiencyvsSumPt2Bx0PileUp","RecoEfficiencyvsSumPt2Bx0PileUp",200,0.,100.);

  hSimVertexSumPt2_ = fs->make<TH1F>("SimVertexSumPt2","SimVertexSumPt2",200,0.,100.);  
  hSimVertexSumPt2Bx0Signal_ = fs->make<TH1F>("SimVertexSumPt2Bx0Signal","SimVertexSumPt2Bx0Signal",200,0.,100.);
  hSimVertexSumPt2Bx0PileUp_ = fs->make<TH1F>("SimVertexSumPt2Bx0PileUp","SimVertexSumPt2Bx0PileUp",200,0.,100.);

  hRecoEfficiencyvsSumPt_ = fs->make<TH1F>("RecoEfficiencyvsSumPt","RecoEfficiencyvsSumPt",200,0.,100.);
  hRecoEfficiencyvsSumPtBx0Signal_ = fs->make<TH1F>("RecoEfficiencyvsSumPtBx0Signal","RecoEfficiencyvsSumPtBx0Signal",200,0.,100.);
  hRecoEfficiencyvsSumPtBx0PileUp_ = fs->make<TH1F>("RecoEfficiencyvsSumPtBx0PileUp","RecoEfficiencyvsSumPtBx0PileUp",200,0.,100.);

  hSimVertexSumPt_ = fs->make<TH1F>("SimVertexSumPt","SimVertexSumPt",200,0.,100.);
  hSimVertexSumPtBx0Signal_ = fs->make<TH1F>("SimVertexSumPtBx0Signal","SimVertexSumPtBx0Signal",200,0.,100.);
  hSimVertexSumPtBx0PileUp_ = fs->make<TH1F>("SimVertexSumPtBx0PileUp","SimVertexSumPtBx0PileUp",200,0.,100.);
}

void VertexEfficiencyAnalyzer::endJob(){
  /*if(nMatchedVertexes_) hRecoEfficiencyvsQualityCut_->Scale(1./nMatchedVertexes_);
  if(nMatchedVertexesBx0Signal_) hRecoEfficiencyvsQualityCutBx0Signal_->Scale(1./nMatchedVertexesBx0Signal_);
  if(nMatchedVertexesBx0PileUp_) hRecoEfficiencyvsQualityCutBx0PileUp_->Scale(1./nMatchedVertexesBx0PileUp_);*/
  if(nVertexes_) hRecoEfficiencyvsQualityCut_->Scale(1./nVertexes_);
  if(nVertexesBx0Signal_) hRecoEfficiencyvsQualityCutBx0Signal_->Scale(1./nVertexesBx0Signal_);
  if(nVertexesBx0PileUp_) hRecoEfficiencyvsQualityCutBx0PileUp_->Scale(1./nVertexesBx0PileUp_);
  
  hRecoEfficiencyvsSumPt_->Divide(hSimVertexSumPt_);
  hRecoEfficiencyvsSumPtBx0Signal_->Divide(hSimVertexSumPtBx0Signal_);
  hRecoEfficiencyvsSumPtBx0PileUp_->Divide(hSimVertexSumPtBx0PileUp_);

  hRecoEfficiencyvsSumPt_->Divide(hSimVertexSumPt_);
  hRecoEfficiencyvsSumPtBx0Signal_->Divide(hSimVertexSumPtBx0Signal_);
  hRecoEfficiencyvsSumPtBx0PileUp_->Divide(hSimVertexSumPtBx0PileUp_);
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

  // Get reconstructed vertexes
  /*edm::Handle<edm::View<reco::Vertex> > vertexCollectionH;
  event.getByLabel(vertexTag_,vertexCollectionH);
  const edm::View<reco::Vertex> vtxColl = *(vertexCollectionH.product());*/
  edm::Handle<reco::VertexCollection> vertexCollectionH;
  event.getByLabel(vertexTag_,vertexCollectionH);
  const reco::VertexCollection& vtxColl = *(vertexCollectionH.product());

  // Access primary vertex
  const reco::Vertex& primaryVertex = vtxColl.front();
  bool goodPrimaryVertex = ((primaryVertex.isValid())&&(!primaryVertex.isFake()));

  // Find number of good vertexes; match one closest to muon vertex 
  int nGoodVertexes = 0;
  //for(edm::View<reco::Vertex>::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
  for(reco::VertexCollection::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
    if(!vtx->isValid()) continue; // skip non-valid vertexes
    if(vtx->isFake()) continue; // skip vertex from beam spot
    ++nGoodVertexes;
  }

  edm::LogVerbatim("Analysis") << " Number of reconstructed primary vertexes in event: " << nGoodVertexes;

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

  edm::LogVerbatim("Analysis") << " Number of associated vertexes: " << vtxSimToReco.size();

  //for(reco::VertexSimToRecoCollection::const_iterator itSimtoReco = vtxSimToReco.begin(); itSimtoReco != vtxSimToReco.end(); ++itSimtoReco){
  size_t iVertex = 0;
  for(TrackingVertexCollection::const_iterator it = mergedVH->begin(); it != mergedVH->end(); ++it,++iVertex){
    //TrackingVertexRef simVertex = itSimtoReco->key;
    TrackingVertexRef simVertex(mergedVH,iVertex);

    double sumPtTracks_sim = 0.;
    double sumPt2Tracks_sim = 0.;
    for(TrackingVertex::tp_iterator trackingPart = simVertex->daughterTracks_begin();
                                    trackingPart != simVertex->daughterTracks_end(); ++trackingPart){
        const TrackingVertex::tp_iterator::value_type& tpRef = *trackingPart;
         
        //sumPt2Tracks_sim += trackingPart->pt()*trackingPart->pt();
        sumPtTracks_sim += tpRef->pt();
        sumPt2Tracks_sim += tpRef->pt()*tpRef->pt(); 
    }

    LogTrace("Analysis") << "Sim Vertex at position: ("  << simVertex->position().x() << ","
                                 << simVertex->position().y() << "," << simVertex->position().z() << ")"
                                 << "\n              Sum(pt) = " << sumPtTracks_sim
                                 << "\n             Sum(pt2) = " << sumPt2Tracks_sim
                                 << "\n      N source tracks = " << simVertex->nSourceTracks()
                                 << "\n    N daughter tracks = " << simVertex->nDaughterTracks();

    // Check source tracks
    bool hasMotherVertex = false;
    for(TrackingVertex::tp_iterator trackingPart = simVertex->sourceTracks_begin();
                                    trackingPart != simVertex->sourceTracks_end(); ++trackingPart){
         const TrackingVertex::tp_iterator::value_type& tpRef = *trackingPart;
         // Check if tracking particle has a parent vertex
         const TrackingVertexRef& parentVtx = tpRef->parentVertex();
         if(parentVtx == simVertex){
            LogTrace("Analysis") << "\tSame vertex as original..skipping vertex";
            continue;
         } 
         LogTrace("Analysis") << "\tParent Vertex at position: ("  << parentVtx->position().x() << ","
                              << parentVtx->position().y() << "," << parentVtx->position().z() << ")";
         hasMotherVertex = true;
    }

    /*// Check gen vertexes
    for(TrackingVertex::genv_iterator genVertex = simVertex->genVertices_begin();
                                      genVertex != simVertex->genVertices_end(); ++genVertex){
        const TrackingVertex::genv_iterator::value_type& genVtxRef = *genVertex;
        LogTrace("Analysis") << "\tGen Vertex at position: ("  << genVtxRef->position().x() << ","
                             << genVtxRef->position().y() << "," << genVtxRef->position().z() << ")";
    }*/

    // Consider only primary sim vertexes
    if(hasMotherVertex) continue;
    ++nVertexes_;
    hSimVertexSumPt2_->Fill(sumPt2Tracks_sim);
    hSimVertexSumPt_->Fill(sumPtTracks_sim);
    
    /*std::vector<std::pair<reco::VertexRef, double> > recoVertexes = itSimtoReco->val;

    edm::LogVerbatim("Analysis") << "Sim Vertex at position: ("  << simVertex->position().x() << ","
                                 << simVertex->position().y() << "," << simVertex->position().z() << ")"
                                 << " Sum(pt2) = " << sumPt2Tracks_sim 
                                 <<  " matched to " << recoVertexes.size() << " Reconstructed Vertexes";*/

    // Check if sim vertex from signal or pile-up
    const EncodedEventId& evtId = simVertex->eventId();
    if(evtId.bunchCrossing() == 0){ // check if from signal bunch crossing
        if(evtId.event() == 0){ // comes from signal
             LogTrace("Analysis") << "\t\tComes from signal";
             ++nVertexesBx0Signal_;
             hSimVertexSumPt2Bx0Signal_->Fill(sumPt2Tracks_sim);
             hSimVertexSumPtBx0Signal_->Fill(sumPtTracks_sim);
        } else { // comes from pile-up
             LogTrace("Analysis") << "\t\tComes from pile-up";
             ++nVertexesBx0PileUp_;
             hSimVertexSumPt2Bx0PileUp_->Fill(sumPt2Tracks_sim);
             hSimVertexSumPtBx0PileUp_->Fill(sumPtTracks_sim);
        }
    } else { // comes from other bunch crossing 
        LogTrace("Analysis") << "\t\tComes from out-of-time pile-up";
    }

    /*for(std::vector<std::pair<reco::VertexRef, double> >::const_iterator vtxMatch = recoVertexes.begin(); vtxMatch != recoVertexes.end(); ++vtxMatch){
        reco::VertexRef recoVtx = vtxMatch->first;
        double assocQuality = vtxMatch->second;

        if(!recoVtx->isValid()) continue; // skip non-valid vertex
        if(recoVtx->isFake()) continue; // skip vertex from beam spot
 
        edm::LogVerbatim("Analysis") << "\t\tReco Vertex at position: ("
                                          << recoVtx->position().x() << "," << recoVtx->position().y() << ","
                                          << recoVtx->position().z() << ")" << " Quality: " << assocQuality;
    }*/

    if(vtxSimToReco.find(simVertex) != vtxSimToReco.end()){
        std::vector<std::pair<reco::VertexRef, double> > recoVertexes = vtxSimToReco[simVertex];
        LogTrace("Analysis") <<  " \t\t  Matched to " << recoVertexes.size() << " Reconstructed vertexes";

        for(std::vector<std::pair<reco::VertexRef, double> >::const_iterator vtxMatch = recoVertexes.begin(); vtxMatch != recoVertexes.end(); ++vtxMatch){
             reco::VertexRef recoVtx = vtxMatch->first;
             double assocQuality = vtxMatch->second;

             if(!recoVtx->isValid()) continue; // skip non-valid vertex
             if(recoVtx->isFake()) continue; // skip vertex from beam spot
 
             LogTrace("Analysis") << "\t\tReco Vertex at position: ("
                                          << recoVtx->position().x() << "," << recoVtx->position().y() << ","
                                          << recoVtx->position().z() << ")" << " Quality: " << assocQuality;
        }

        // Get best match
        if(recoVertexes.size() != 0){
            reco::VertexRef recoVtxBestMatch = recoVertexes.begin()->first;
            if(recoVtxBestMatch->isValid()&&(!recoVtxBestMatch->isFake())){
                ++nMatchedVertexes_; 
                hRecoEfficiencyvsSumPt2_->Fill(sumPt2Tracks_sim);
                hRecoEfficiencyvsSumPt_->Fill(sumPtTracks_sim);
                if(evtId.bunchCrossing() == 0){
                    if(evtId.event() == 0){
                        ++nMatchedVertexesBx0Signal_;
                        hRecoEfficiencyvsSumPt2Bx0Signal_->Fill(sumPt2Tracks_sim);
                        hRecoEfficiencyvsSumPtBx0Signal_->Fill(sumPtTracks_sim);
                    } else{
                        ++nMatchedVertexesBx0PileUp_;
                        hRecoEfficiencyvsSumPt2Bx0PileUp_->Fill(sumPt2Tracks_sim);
                        hRecoEfficiencyvsSumPtBx0PileUp_->Fill(sumPtTracks_sim);
                    }
                }
                double qualityBestMatch = recoVertexes.begin()->second;
                for(int ibin = 1; ibin <= hRecoEfficiencyvsQualityCut_->GetNbinsX(); ++ibin){
                    // Get bin center
                    double xlow = hRecoEfficiencyvsQualityCut_->GetXaxis()->GetBinLowEdge(ibin);
                    double xbin = hRecoEfficiencyvsQualityCut_->GetXaxis()->GetBinCenter(ibin);
                    if(qualityBestMatch >= xlow){
                        hRecoEfficiencyvsQualityCut_->Fill(xbin);
        	        if(evtId.bunchCrossing() == 0){
                            if(evtId.event() == 0) hRecoEfficiencyvsQualityCutBx0Signal_->Fill(xbin);
                            else hRecoEfficiencyvsQualityCutBx0PileUp_->Fill(xbin);
                        }
                    }
                }
                // Efficiency versus sum(pt2)
                /*double sumPt2Tracks = 0.;
                for(reco::Vertex::trackRef_iterator vtxTrack = recoVtxBestMatch->tracks_begin();
                                                 vtxTrack != recoVtxBestMatch->tracks_end(); ++vtxTrack){
                    edm::RefToBase<reco::Track> track = *vtxTrack;
                    sumPt2Tracks += track->pt()*track->pt();
                }*/
            }  
        }
    }
  }

}

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(VertexEfficiencyAnalyzer);
