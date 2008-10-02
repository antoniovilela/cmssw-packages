#ifndef PileUpAnalysis_h
#define PileUpAnalysis_h
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class InputTag;
class TrackAssociatorBase;

class PileUpAnalysis  : public edm::EDAnalyzer {
 public:

  explicit PileUpAnalysis(const edm::ParameterSet& conf);

  virtual ~PileUpAnalysis(){}

  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  virtual void beginJob(const edm::EventSetup & setup);
 private:
  edm::InputTag tracksTag_;
  edm::InputTag trackAssociatorTag_;
  TrackAssociatorBase* associatorByHits_;
  
};

#endif

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Provenance/interface/EventID.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"

//#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
//#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
//#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFramePlaybackInfo.h"

#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/RecoCandidate/interface/TrackAssociation.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"

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

PileUpAnalysis::PileUpAnalysis(const edm::ParameterSet& conf){
  tracksTag_ = conf.getParameter<edm::InputTag>("TracksTag");
  trackAssociatorTag_ = conf.getParameter<edm::InputTag>("TrackAssociatorTag");
}

void PileUpAnalysis::beginJob(const edm::EventSetup& setup) {
  edm::ESHandle<MagneticField> theMF;
  setup.get<IdealMagneticFieldRecord>().get(theMF);
  edm::ESHandle<TrackAssociatorBase> theHitsAssociator;
  setup.get<TrackAssociatorRecord>().get("TrackAssociatorByHits",theHitsAssociator);
  associatorByHits_ = (TrackAssociatorBase *) theHitsAssociator.product();
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

  for(unsigned int ibunch = 0; ibunch < nrEvents.size(); ++ibunch){
    edm::LogVerbatim("Analysis") << " Number of added pile-up's for bunch index " << ibunch << ": " << nrEvents[ibunch]; 
  }

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

  // Build map giving nr. of tracks vs (bx,evt)
  edm::LogVerbatim("Analysis") << " Tracking particles summary:";
  TrackMultMap trackMultMap;
  for (TrackingParticleCollection::const_iterator iTrack = mergedPH->begin(); iTrack != mergedPH->end(); ++iTrack) {
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
     edm::LogVerbatim("Analysis") << " Number of tracks in (bx,evt) (" << it->first.first << "," << it->first.second << "): " << it->second; 
  }

  // Add TrackAssociator info 

  // RecoToSim
  edm::Handle<edm::View<reco::Track> > trackCollectionH;
  event.getByLabel(tracksTag_,trackCollectionH);
  const edm::View<reco::Track> tC = *(trackCollectionH.product());

  edm::Handle<reco::RecoToSimCollection> recoToSimCollectionH;
  event.getByLabel(trackAssociatorTag_,recoToSimCollectionH);

  const reco::RecoToSimCollection& recoToSim = *recoToSimCollectionH.product();

  //reco::RecoToSimCollection recoToSim = associatorByHits_->associateRecoToSim(trackCollectionH,mergedPH,&event);

  for(edm::View<reco::Track>::size_type i=0; i < tC.size(); ++i) {
    edm::RefToBase<reco::Track> track(trackCollectionH, i);
    if(recoToSim.find(track) != recoToSim.end()){
        std::vector<std::pair<TrackingParticleRef, double> > tp = recoToSim[track];
	edm::LogVerbatim("Analysis") << "Reco Track pT: "  << track->pt() 
	                             <<  " matched to " << tp.size() << " MC Tracks";
	for (std::vector<std::pair<TrackingParticleRef, double> >::const_iterator it = tp.begin(); 
	     it != tp.end(); ++it) {
	     TrackingParticleRef tpr = it->first;
	     double assocChi2 = it->second;
	     edm::LogVerbatim("Analysis") << "\t\tMCTrack " << tpr.index() << " pT: " << tpr->pt() << " NShared: " << assocChi2;
	}
     } else edm::LogWarning("Analysis") << "->   Track pT: " << track->pt() <<  " matched to 0  MC Tracks";
  }
  
}

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(PileUpAnalysis);
