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
  edm::InputTag vertexTag_;
  edm::InputTag trackAssociatorTag_;

  float nMatchedTracks_,nMatchedTracksBx0Signal_,nMatchedTracksBx0PileUp_;
  float nTracks_,nTracksBx0Signal_,nTracksBx0PileUp_;
  //TrackAssociatorBase* associatorByHits_;

  // Histograms
  TH1F* hRecoEfficiencyvsQualityCut_;
  TH1F* hRecoEfficiencyvsQualityCutBx0Signal_;
  TH1F* hRecoEfficiencyvsQualityCutBx0PileUp_;

  TH1F* hRecoEfficiencyvsPt_;
  TH1F* hRecoEfficiencyvsPtBx0Signal_;
  TH1F* hRecoEfficiencyvsPtBx0PileUp_;

  TH1F* hSimTracksPt_;
  TH1F* hSimTracksPtBx0Signal_;
  TH1F* hSimTracksPtBx0PileUp_;

  TH1F* hRecoEfficiencyvsEta_;
  TH1F* hRecoEfficiencyvsEtaBx0Signal_;
  TH1F* hRecoEfficiencyvsEtaBx0PileUp_;

  TH1F* hSimTracksEta_;
  TH1F* hSimTracksEtaBx0Signal_;
  TH1F* hSimTracksEtaBx0PileUp_;
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
  vertexTag_(conf.getParameter<edm::InputTag>("VertexTag")),
  trackAssociatorTag_(conf.getParameter<edm::InputTag>("TrackAssociatorTag")),
  nMatchedTracks_(0),nMatchedTracksBx0Signal_(0),nMatchedTracksBx0PileUp_(0),
  nTracks_(0),nTracksBx0Signal_(0),nTracksBx0PileUp_(0){}

void TrackEfficiencyAnalyzer::beginJob(const edm::EventSetup& setup) {
  // Book histograms
  edm::Service<TFileService> fs;
  hRecoEfficiencyvsQualityCut_ = fs->make<TH1F>("RecoEfficiencyvsQualityCut","RecoEfficiencyvsQualityCut",200,0.,1.);
  hRecoEfficiencyvsQualityCutBx0Signal_ = fs->make<TH1F>("RecoEfficiencyvsQualityCutBx0Signal","RecoEfficiencyvsQualityCutBx0Signal",200,0.,1.);
  hRecoEfficiencyvsQualityCutBx0PileUp_ = fs->make<TH1F>("RecoEfficiencyvsQualityCutBx0PileUp","RecoEfficiencyvsQualityCutBx0PileUp",200,0.,1.);
  hRecoEfficiencyvsPt_ = fs->make<TH1F>("RecoEfficiencyvsPt","RecoEfficiencyvsPt",100,0.,20.);
  hRecoEfficiencyvsPtBx0Signal_ = fs->make<TH1F>("RecoEfficiencyvsPtBx0Signal","RecoEfficiencyvsPtBx0Signal",100,0.,20.);
  hRecoEfficiencyvsPtBx0PileUp_ = fs->make<TH1F>("RecoEfficiencyvsPtBx0PileUp","RecoEfficiencyvsPtBx0PileUp",100,0.,20.);
  hSimTracksPt_ = fs->make<TH1F>("SimTracksPt","SimTracksPt",100,0.,20.);
  hSimTracksPtBx0Signal_ = fs->make<TH1F>("SimTracksPtBx0Signal","SimTracksPtBx0Signal",100,0.,20.);
  hSimTracksPtBx0PileUp_ = fs->make<TH1F>("SimTracksPtBx0PileUp","SimTracksPtBx0PileUp",100,0.,20.);
  hRecoEfficiencyvsEta_ = fs->make<TH1F>("RecoEfficiencyvsEta","RecoEfficiencyvsEta",100,-3.0,3.0);
  hRecoEfficiencyvsEtaBx0Signal_ = fs->make<TH1F>("RecoEfficiencyvsEtaBx0Signal","RecoEfficiencyvsEtaBx0Signal",100,-3.0,3.0);
  hRecoEfficiencyvsEtaBx0PileUp_ = fs->make<TH1F>("RecoEfficiencyvsEtaBx0PileUp","RecoEfficiencyvsEtaBx0PileUp",100,-3.0,3.0);
  hSimTracksEta_ = fs->make<TH1F>("SimTracksEta","SimTracksEta",100,-3.0,3.0);
  hSimTracksEtaBx0Signal_ = fs->make<TH1F>("SimTracksEtaBx0Signal","SimTracksEtaBx0Signal",100,-3.0,3.0);
  hSimTracksEtaBx0PileUp_ = fs->make<TH1F>("SimTracksEtaBx0PileUp","SimTracksEtaBx0PileUp",100,-3.0,3.0);
}

void TrackEfficiencyAnalyzer::endJob(){
  /*if(nMatchedTracks_) hRecoEfficiencyvsQualityCut_->Scale(1./nMatchedTracks_);
  if(nMatchedTracksBx0Signal_) hRecoEfficiencyvsQualityCutBx0Signal_->Scale(1./nMatchedTracksBx0Signal_);
  if(nMatchedTracksBx0PileUp_) hRecoEfficiencyvsQualityCutBx0PileUp_->Scale(1./nMatchedTracksBx0PileUp_);*/
  if(nTracks_) hRecoEfficiencyvsQualityCut_->Scale(1./nTracks_);
  if(nTracksBx0Signal_) hRecoEfficiencyvsQualityCutBx0Signal_->Scale(1./nTracksBx0Signal_);
  if(nTracksBx0PileUp_) hRecoEfficiencyvsQualityCutBx0PileUp_->Scale(1./nTracksBx0PileUp_);

  hRecoEfficiencyvsPt_->Divide(hSimTracksPt_);
  hRecoEfficiencyvsPtBx0Signal_->Divide(hSimTracksPtBx0Signal_);
  hRecoEfficiencyvsPtBx0PileUp_->Divide(hSimTracksPtBx0PileUp_);

  hRecoEfficiencyvsEta_->Divide(hSimTracksEta_);
  hRecoEfficiencyvsEtaBx0Signal_->Divide(hSimTracksEtaBx0Signal_);
  hRecoEfficiencyvsEtaBx0PileUp_->Divide(hSimTracksEtaBx0PileUp_);
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

  /*// Get reconstructed vertexes
  edm::Handle<edm::View<reco::Vertex> > vertexCollectionH;
  event.getByLabel(vertexTag_,vertexCollectionH);
  const edm::View<reco::Vertex> vtxColl = *(vertexCollectionH.product());

  // Access primary vertex
  const reco::Vertex& primaryVertex = vtxColl.front();
  bool goodPrimaryVertex = ((primaryVertex.isValid())&&(!primaryVertex.isFake()));*/

  // TrackAssociator info 
  // SimToReco
  edm::Handle<reco::SimToRecoCollection> simToRecoCollectionH;
  event.getByLabel(trackAssociatorTag_,simToRecoCollectionH);
  
  edm::LogVerbatim("Analysis") << ">>> Accessing track association (Sim->Reco)";
  reco::SimToRecoCollection simToReco = *simToRecoCollectionH.product();

  edm::LogVerbatim("Analysis") << " Number of associated tracks: " << simToReco.size();

  //for(reco::SimToRecoCollection::const_iterator itSimtoReco = simToReco.begin(); itSimtoReco != simToReco.end(); ++itSimtoReco){
  size_t iTrack = 0;
  double rmax = 20.;
  double zmax = 60.;
  double ptmin = 0.9;
  for(TrackingParticleCollection::const_iterator it = mergedPH->begin(); it != mergedPH->end(); ++it,++iTrack){
    //TrackingParticleRef simTrack = itSimtoReco->key;
    TrackingParticleRef simTrack(mergedPH,iTrack);
    // Consider only tracks passing cuts
    math::XYZPoint simTrackVtx = simTrack->vertex();
    if(simTrackVtx.r() > rmax) continue;
    if(fabs(simTrackVtx.z()) > zmax) continue;
    if(simTrack->charge() == 0) continue;

    ++nTracks_;
    hSimTracksPt_->Fill(simTrack->pt());
    hSimTracksEta_->Fill(simTrack->eta());
    LogTrace("Analysis") << "Sim Track pt,charge,vx,vy,vz: "  << simTrack->pt() << ", " << simTrack->charge() << ", " << simTrack->vx() << ", " << simTrack->vy() << ", "  << simTrack->vz();
    // Check if sim track from signal or pile-up
    const EncodedEventId& evtId = simTrack->eventId();
    if(evtId.bunchCrossing() == 0){ // check if from signal bunch crossing
        if(evtId.event() == 0){ // comes from signal
             LogTrace("Analysis") << "\t\tComes from signal";
             ++nTracksBx0Signal_;
             hSimTracksPtBx0Signal_->Fill(simTrack->pt());
             hSimTracksEtaBx0Signal_->Fill(simTrack->eta());
        } else { // comes from pile-up
             LogTrace("Analysis") << "\t\tComes from pile-up";
             ++nTracksBx0PileUp_;
             hSimTracksPtBx0PileUp_->Fill(simTrack->pt());
             hSimTracksEtaBx0PileUp_->Fill(simTrack->eta());
        }
    } else { // comes from other bunch crossing 
        LogTrace("Analysis") << "\t\tComes from out-of-time pile-up";
    }

    // Find tracking particle in sim to reco association
    //std::vector<std::pair<edm::RefToBase<reco::Track>, double> > recoTracks = itSimtoReco->val;
    if(simToReco.find(simTrack) != simToReco.end()){
        std::vector<std::pair<edm::RefToBase<reco::Track>, double> > recoTracks = simToReco[simTrack];
        LogTrace("Analysis") <<  " \t\t  Matched to " << recoTracks.size() << " Reconstructed Tracks";

        for(std::vector<std::pair<edm::RefToBase<reco::Track>, double> >::const_iterator trkMatch = recoTracks.begin(); trkMatch != recoTracks.end(); ++trkMatch){
             edm::RefToBase<reco::Track> recoTrack = trkMatch->first;
             double assocQuality = trkMatch->second;

             LogTrace("Analysis") << "\t\t    Reco Track pt: " << recoTrack->pt() << " Quality: " << assocQuality;
        }

        // Get best match
        if(recoTracks.size() != 0){
             edm::RefToBase<reco::Track> recoTrackBestMatch = recoTracks.begin()->first;
             ++nMatchedTracks_;
             hRecoEfficiencyvsPt_->Fill(simTrack->pt());
             hRecoEfficiencyvsEta_->Fill(simTrack->eta()); 
             if(evtId.bunchCrossing() == 0){
                  if(evtId.event() == 0) {
                       ++nMatchedTracksBx0Signal_;
                       hRecoEfficiencyvsPtBx0Signal_->Fill(simTrack->pt());
                       hRecoEfficiencyvsEtaBx0Signal_->Fill(simTrack->eta());
                  }
                  else {
                       ++nMatchedTracksBx0PileUp_;
                       hRecoEfficiencyvsPtBx0PileUp_->Fill(simTrack->pt());
                       hRecoEfficiencyvsEtaBx0PileUp_->Fill(simTrack->eta());
                  }
             }
             double qualityBestMatch = recoTracks.begin()->second;
             if(simTrack->pt() < ptmin) continue;
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
        }
    }
  }

}

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(TrackEfficiencyAnalyzer);
