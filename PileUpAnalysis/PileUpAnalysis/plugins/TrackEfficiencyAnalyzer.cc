#ifndef TrackEfficiencyAnalyzer_h
#define TrackEfficiencyAnalyzer_h
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TH1F.h"

class InputTag;
//class TrackAssociatorBase;

class TrackEfficiencyAnalyzer  : public edm::EDAnalyzer {
 public:

  explicit TrackEfficiencyAnalyzer(const edm::ParameterSet& conf);

  virtual ~TrackEfficiencyAnalyzer(){}

  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  virtual void beginJob(const edm::EventSetup & setup);
  virtual void endJob();
 private:
  edm::InputTag tracksTag_;
  edm::InputTag verticesTag_;
  edm::InputTag trackAssociatorTag_;

  float nMatchedTracks_,nMatchedTracksBx0Signal_,nMatchedTracksBx0PileUp_;
  //TrackAssociatorBase* associatorByHits_;

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

TrackEfficiencyAnalyzer::TrackEfficiencyAnalyzer(const edm::ParameterSet& conf):
  tracksTag_(conf.getParameter<edm::InputTag>("TracksTag")),
  verticesTag_(conf.getParameter<edm::InputTag>("VerticesTag")),
  trackAssociatorTag_(conf.getParameter<edm::InputTag>("TrackAssociatorTag")),
  nMatchedTracks_(0),nMatchedTracksBx0Signal_(0),nMatchedTracksBx0PileUp_(0){}

void TrackEfficiencyAnalyzer::beginJob(const edm::EventSetup& setup) {
  // Book histograms
  edm::Service<TFileService> fs;
  hRecoEfficiencyvsQualityCut_ = fs->make<TH1F>("RecoEfficiencyvsQualityCut","RecoEfficiencyvsQualityCut",200,0.,1.);
  hRecoEfficiencyvsQualityCutBx0Signal_ = fs->make<TH1F>("RecoEfficiencyvsQualityCutBx0Signal","RecoEfficiencyvsQualityCutBx0Signal",200,0.,1.);
  hRecoEfficiencyvsQualityCutBx0PileUp_ = fs->make<TH1F>("RecoEfficiencyvsQualityCutBx0PileUp","RecoEfficiencyvsQualityCutBx0PileUp",200,0.,1.);
}

void TrackEfficiencyAnalyzer::endJob(){
  if(nMatchedTracks_) hRecoEfficiencyvsQualityCut_->Scale(1./nMatchedTracks_);
  if(nMatchedTracksBx0Signal_) hRecoEfficiencyvsQualityCutBx0Signal_->Scale(1./nMatchedTracksBx0Signal_);
  if(nMatchedTracksBx0PileUp_) hRecoEfficiencyvsQualityCutBx0PileUp_->Scale(1./nMatchedTracksBx0PileUp_);
}

void TrackEfficiencyAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& c){

  edm::Handle<TrackingParticleCollection> mergedPH;
  edm::Handle<TrackingVertexCollection>   mergedVH;

  event.getByLabel("mergedtruth","MergedTrackTruth",    mergedPH);
  event.getByLabel("mergedtruth","MergedTrackTruth",    mergedVH);

  // Get reconstructed tracks
  edm::Handle<edm::View<reco::Track> > trackCollectionH;
  event.getByLabel(tracksTag_,trackCollectionH);
  const edm::View<reco::Track> trkColl = *(trackCollectionH.product());

  // Get reconstructed vertices
  edm::Handle<edm::View<reco::Vertex> > vertexCollectionH;
  event.getByLabel(verticesTag_,vertexCollectionH);
  const edm::View<reco::Vertex> vtxColl = *(vertexCollectionH.product());

  // Access primary vertex
  const reco::Vertex& primaryVertex = vtxColl.front();
  bool goodPrimaryVertex = ((primaryVertex.isValid())&&(!primaryVertex.isFake()));

  // TrackAssociator info 
  // SimToReco
  edm::Handle<reco::SimToRecoCollection> simToRecoCollectionH;
  event.getByLabel(trackAssociatorTag_,simToRecoCollectionH);
  
  edm::LogVerbatim("Analysis") << ">>> Accessing track association (Sim->Reco)";
  reco::SimToRecoCollection simToReco = *simToRecoCollectionH.product();

  edm::LogVerbatim("Analysis") << " Number of associated tracks: " << simToReco.size();

  for(reco::SimToRecoCollection::const_iterator itSimtoReco = simToReco.begin(); itSimtoReco != simToReco.end(); ++itSimtoReco){
    TrackingParticleRef simTrack = itSimtoReco->key;
    std::vector<std::pair<edm::RefToBase<reco::Track>, double> > recoTracks = itSimtoReco->val;

    edm::LogVerbatim("Analysis") << "Sim Track pt: "  << simTrack->pt() 
                                 <<  " matched to " << recoTracks.size() << " Reconstructed Tracks";

    // Check if sim track from signal or pile-up
    const EncodedEventId& evtId = simTrack->eventId();
    if(evtId.bunchCrossing() == 0){ // check if from signal bunch crossing
        if(evtId.event() == 0){ // comes from signal
             edm::LogVerbatim("Analysis") << "\t\tComes from signal";
        } else { // comes from pile-up
             edm::LogVerbatim("Analysis") << "\t\tComes from pile-up";
        }
    } else { // comes from other bunch crossing 
        edm::LogVerbatim("Analysis") << "\t\tComes from out-of-time pile-up";
    }

    for(std::vector<std::pair<edm::RefToBase<reco::Track>, double> >::const_iterator trkMatch = recoTracks.begin(); trkMatch != recoTracks.end(); ++trkMatch){
        edm::RefToBase<reco::Track> recoTrack = trkMatch->first;
        double assocQuality = trkMatch->second;

        edm::LogVerbatim("Analysis") << "\t\tReco Track pt: " << recoTrack->pt() << " Quality: " << assocQuality;
    }

    // Get best match
    if(recoTracks.size() != 0){
        edm::RefToBase<reco::Track> recoTrackBestMatch = recoTracks.begin()->first;
        ++nMatchedTracks_; 
        if(evtId.bunchCrossing() == 0){
             if(evtId.event() == 0) ++nMatchedTracksBx0Signal_;
             else ++nMatchedTracksBx0PileUp_;
        }
        double qualityBestMatch = recoTracks.begin()->second;
             for(int ibin = 1; ibin <= hRecoEfficiencyvsQualityCut_->GetNbinsX(); ++ibin){
                  // Get bin center
                  double xlow = hRecoEfficiencyvsQualityCut_->GetXaxis()->GetBinCenter(ibin);
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

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(TrackEfficiencyAnalyzer);
