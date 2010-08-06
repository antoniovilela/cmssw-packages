
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

class TFileService;
class TH1F;
//class TH2F;

class SimpleTrackAnalyzer: public edm::EDAnalyzer
{
  public:
    typedef std::map<std::string,TH1F*> HistoMapTH1F;
 
    explicit SimpleTrackAnalyzer(edm::ParameterSet const&);
    ~SimpleTrackAnalyzer();

    void beginJob();
    void analyze(edm::Event const&, edm::EventSetup const&);
  private:
    void bookHistos(HistoMapTH1F&, edm::Service<TFileService> const&);    

    edm::InputTag trackTag_;

    double minPt_;
    double maxPt_;
    unsigned int nBinsPt_;
    double minEta_;
    double maxEta_;
    unsigned int nBinsEta_;
    double minPtSum_;
    double maxPtSum_;
    unsigned int nBinsPtSum_;
    unsigned int minNTracks_;
    unsigned int maxNTracks_;
    unsigned int nBinsNTracks_;

    HistoMapTH1F histosTH1F_;
    //HistoMapTH2F histosTH2F_;
};

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

SimpleTrackAnalyzer::SimpleTrackAnalyzer(edm::ParameterSet const& pset):
  trackTag_(pset.getParameter<edm::InputTag>("TrackTag")),
  minPt_(pset.getParameter<double>("MinPt")),
  maxPt_(pset.getParameter<double>("MaxPt")),  
  nBinsPt_(pset.getParameter<unsigned int>("NBinsPt")),
  minEta_(pset.getParameter<double>("MinEta")),          
  maxEta_(pset.getParameter<double>("MaxEta")),
  nBinsEta_(pset.getParameter<unsigned int>("NBinsEta")),
  minPtSum_(pset.getParameter<double>("MinPtSum")),
  maxPtSum_(pset.getParameter<double>("MaxPtSum")),
  nBinsPtSum_(pset.getParameter<unsigned int>("NBinsPtSum")),
  minNTracks_(pset.getParameter<unsigned int>("MinNTracks")),
  maxNTracks_(pset.getParameter<unsigned int>("MaxNTracks")),
  nBinsNTracks_(pset.getParameter<unsigned int>("NBinsNTracks")) {}

SimpleTrackAnalyzer::~SimpleTrackAnalyzer(){}

void SimpleTrackAnalyzer::beginJob(){
  edm::Service<TFileService> fs;

  TH1::SetDefaultSumw2(true);

  bookHistos(histosTH1F_,fs);
  //bookHistos(histosTH2F_,fs);
}

void SimpleTrackAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<reco::Track> > trackCollectionH;
  event.getByLabel(trackTag_,trackCollectionH);
  const edm::View<reco::Track>& trackColl = *(trackCollectionH.product());
     
  int nTracks = 0;
  double ptSum = 0.;
  edm::View<reco::Track>::const_iterator track = trackColl.begin();
  edm::View<reco::Track>::const_iterator tracks_end = trackColl.end();
  for(; track != tracks_end; ++track){
     // Any selection here

     ++nTracks;  
     ptSum += track->pt();
     histosTH1F_["TrackPt"]->Fill( track->pt() );
     histosTH1F_["TrackEta"]->Fill( track->eta() );
  }
  histosTH1F_["PtSum"]->Fill( ptSum );
  histosTH1F_["NTracks"]->Fill( nTracks ); 
}

void SimpleTrackAnalyzer::bookHistos(HistoMapTH1F& histos, edm::Service<TFileService> const& fs){
  histos["TrackPt"] = fs->make<TH1F>("TrackPt","TrackPt",nBinsPt_,minPt_,maxPt_);
  histos["TrackEta"] = fs->make<TH1F>("TrackEta","TrackEta",nBinsEta_,minEta_,maxEta_);
  histos["PtSum"] = fs->make<TH1F>("TrackPtSum","TrackPtSum",nBinsPtSum_,minPtSum_,maxPtSum_);
  histos["NTracks"] = fs->make<TH1F>("NTracks","NTracks",nBinsNTracks_,minNTracks_,maxNTracks_);
}

DEFINE_FWK_MODULE(SimpleTrackAnalyzer);
