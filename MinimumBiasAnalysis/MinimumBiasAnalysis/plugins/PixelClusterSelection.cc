#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <map>

class TH1F;
class TH2F;
class TrackerGeometry;

class PixelClusterSelection : public edm::EDFilter {
public:
  explicit PixelClusterSelection( const edm::ParameterSet & );
  ~PixelClusterSelection();
  
private:
  bool filter(edm::Event&, edm::EventSetup const&);
  void beginJob();
  bool beginRun(edm::Run&, edm::EventSetup const&);

  edm::InputTag pixelRecHitsTag_;
  edm::InputTag vertexTag_;
  bool applyfilter_;

  const TrackerGeometry* trackerGeom_;

  struct {
      TH1F* h_clusterSize_;
      TH2F* h_clusterSizeVsPosZ_;
      TH2F* h_clusterSizeVsPosZRange_;

      TH1F* h_nCompatibleClusters_;
      TH1F* h_nValidClusters_;
      TH2F* h_nCompatibleClustersVsnValidClusters_;

      TH1F* h_compatRatio_;
      TH2F* h_compatRatioVsnValidClusters_;
  } histos_;

};

#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"

#include "TH1F.h"
#include "TH2F.h"

//using namespace reco;

PixelClusterSelection::PixelClusterSelection(const edm::ParameterSet& pset):
  pixelRecHitsTag_(pset.getParameter<edm::InputTag>("pixelRecHitsTag")),
  vertexTag_(pset.getParameter<edm::InputTag>("vertexTag")),
  applyfilter_(pset.getUntrackedParameter<bool>("applyfilter",true)) {}

PixelClusterSelection::~PixelClusterSelection() {}

void PixelClusterSelection::beginJob(){
  edm::Service<TFileService> fs;

  histos_.h_clusterSize_ = fs->make<TH1F>("clusterSize","clusterSize",20,0,20);
  histos_.h_clusterSizeVsPosZ_ = fs->make<TH2F>("clusterSizeVsPosZ","clusterSizeVsPosZ",200,-30.,30.,20,0,20);
  histos_.h_clusterSizeVsPosZRange_ = fs->make<TH2F>("clusterSizeVsPosZRange","clusterSizeVsPosZRange",200,-30.,30.,20,0,20);

  histos_.h_nCompatibleClusters_ = fs->make<TH1F>("nCompatibleClusters","nCompatibleClusters",200,0,400);
  histos_.h_nValidClusters_ = fs->make<TH1F>("nValidClusters","nValidClusters",200,0,400);
  histos_.h_nCompatibleClustersVsnValidClusters_ = fs->make<TH2F>("nCompatibleClustersVsnValidClusters","nCompatibleClustersVsnValidClusters",200,0,400,200,0,400);
  
  histos_.h_compatRatio_ = fs->make<TH1F>("compatRatio","compatRatio",200,0.,1.);
  histos_.h_compatRatioVsnValidClusters_ = fs->make<TH2F>("compatRatioVsnValidClusters","compatRatioVsnValidClusters",200,0,400,200,0.,1.);
}

bool PixelClusterSelection::beginRun(edm::Run& run, edm::EventSetup const& setup){
  // Get tracker geometry
  edm::ESHandle<TrackerGeometry> trackerGeomH;
  setup.get<TrackerDigiGeometryRecord>().get(trackerGeomH);
  trackerGeom_ = trackerGeomH.product();

  return true;
}

bool PixelClusterSelection::filter(edm::Event& event, const edm::EventSetup& setup)
{
  bool accept = false;

  // Get pixel hit collections
  edm::Handle<SiPixelRecHitCollection> pixelCollH;
  event.getByLabel(pixelRecHitsTag_, pixelCollH);

  const SiPixelRecHitCollection& pixelHits = *pixelCollH;

  // Get offline vertex
  edm::Handle<edm::View<reco::Vertex> > vertexCollectionH;
  event.getByLabel(vertexTag_,vertexCollectionH);
  const edm::View<reco::Vertex>& vtxColl = *(vertexCollectionH.product());

  // Access primary vertex
  const reco::Vertex& primaryVertex = vtxColl.front();
  bool goodPrimaryVertex = ((primaryVertex.isValid())&&(!primaryVertex.isFake()));

  if(!goodPrimaryVertex) return false;

  float nCompatibleClusters = 0.;
  float nValidClusters = 0.; 
  if(pixelHits.size() > 0)
  {
     for(SiPixelRecHitCollection::DataContainer::const_iterator
           recHit = pixelHits.data().begin(),
           recHitEnd = pixelHits.data().end();
           recHit != recHitEnd; ++recHit){

        if(recHit->isValid()){
           DetId id = recHit->geographicalId();
           const PixelGeomDetUnit* pgdu = dynamic_cast<const PixelGeomDetUnit*>(trackerGeom_->idToDetUnit(id));
           const RectangularPixelTopology* theTopol = dynamic_cast<const RectangularPixelTopology*>( &(pgdu->specificTopology()) );
           std::vector<SiPixelCluster::Pixel> pixels = recHit->cluster()->pixels();

           bool pixelOnEdge = false;
           for(std::vector<SiPixelCluster::Pixel>::const_iterator pixel = pixels.begin();
                                                                  pixel != pixels.end(); ++pixel){
              int pos_x = (int)pixel->x;
              int pos_y = (int)pixel->y;

              if(theTopol->isItEdgePixelInX(pos_x) || theTopol->isItEdgePixelInY(pos_y)){
                 pixelOnEdge = true;
                 break;
              }
           }
    
           //if(!pixelOnEdge)
           if(id.subdetId() == int(PixelSubdetector::PixelBarrel)){
              PXBDetId pid(id);
 
              LocalPoint lpos = LocalPoint(recHit->localPosition().x(),
                                           recHit->localPosition().y(),
                                           recHit->localPosition().z());

              GlobalPoint gpos = trackerGeom_->idToDet(id)->toGlobal(lpos);

              float hitPosZ = gpos.z(); 
              float hitPosR = gpos.perp(); 
              int   hitClusterSizeY = recHit->cluster()->sizeY();

              float extrapolatedWidthY = 2*fabs(hitPosZ - primaryVertex.position().z())/hitPosR + 0.5;

              histos_.h_clusterSize_->Fill(hitClusterSizeY);
              histos_.h_clusterSizeVsPosZ_->Fill(hitPosZ,hitClusterSizeY);
              if(fabs(extrapolatedWidthY - hitClusterSizeY) <= 1.0){
                 ++nCompatibleClusters;
                 histos_.h_clusterSizeVsPosZRange_->Fill(hitPosZ,hitClusterSizeY);
              }
              ++nValidClusters;
           }
        }
     }
  }

  histos_.h_nCompatibleClusters_->Fill(nCompatibleClusters);
  histos_.h_nValidClusters_->Fill(nValidClusters);
  histos_.h_nCompatibleClustersVsnValidClusters_->Fill(nValidClusters,nCompatibleClusters);

  float compatRatio = nCompatibleClusters/nValidClusters;
  histos_.h_compatRatio_->Fill(compatRatio);
  histos_.h_compatRatioVsnValidClusters_->Fill(nValidClusters,compatRatio);

  // Selection criteria
  accept = true;

  if(applyfilter_) return accept;
  else return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(PixelClusterSelection);
