/* \class WToMuNuSelector
 *
 * \author Juan Alcaraz, CIEMAT
 *
 */
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/MuonReco/interface/Muon.h"

class WToMuNuSelector : public edm::EDFilter {
public:
  WToMuNuSelector (const edm::ParameterSet &);
  virtual bool filter(edm::Event&, const edm::EventSetup&);
private:
  edm::InputTag muonTag_;
  edm::InputTag metTag_;
  edm::InputTag isoTag_;
  edm::InputTag jetTag_;
  double ptThrForZCount_;
  double ptCut_;
  double etaCut_;
  double massTMin_;
  double massTMax_;
  double eJetMin_;
  int nJetMax_;
  double acopCut_;
};

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/MuonReco/interface/MuIsoDeposit.h"
#include "DataFormats/GeometryVector/interface/Phi.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"

using namespace edm;
using namespace std;
using namespace reco;

WToMuNuSelector::WToMuNuSelector( const ParameterSet & cfg ) :
      muonTag_(cfg.getParameter<edm::InputTag> ("MuonTag")),
      metTag_(cfg.getParameter<edm::InputTag> ("METTag")),
      isoTag_(cfg.getParameter<edm::InputTag> ("IsolationTag")),
      jetTag_(cfg.getParameter<edm::InputTag> ("JetTag")),
      ptThrForZCount_(cfg.getParameter<double>("PtThrForZCount")),
      ptCut_(cfg.getParameter<double>("PtCut")),
      etaCut_(cfg.getParameter<double>("EtaCut")),
      massTMin_(cfg.getParameter<double>("MassTMin")),
      massTMax_(cfg.getParameter<double>("MassTMax")),
      eJetMin_(cfg.getParameter<double>("EJetMin")),
      nJetMax_(cfg.getParameter<int>("NJetMax")),
      acopCut_(cfg.getParameter<double>("AcopCut"))
{
	std::string alias;
        produces<CandidateCollection>( alias = "WMuons" ).setBranchAlias( alias );
}

bool WToMuNuSelector::filter (Event & ev, const EventSetup &) {
      double met_px = 0.;
      double met_py = 0.;

      // Note:    try/catch sequences should disappear in the future
      //          GetByLabel will return a false bool value 
      //          if the collection is not present

      Handle<TrackCollection> muonCollection;
      try {
            ev.getByLabel(muonTag_, muonCollection);
      } catch (...) {
            LogTrace("") << ">>> Muon collection does not exist !!!";
            return false;
      }
      for (unsigned int i=0; i<muonCollection->size(); i++) {
            TrackRef mu(muonCollection,i);
            met_px -= mu->px();
            met_py -= mu->py();
      }

      Handle<CaloMETCollection> metCollection;
      try {
            ev.getByLabel(metTag_, metCollection);
      } catch (...) {
            LogTrace("") << ">>> MET collection does not exist !!!";
            return false;
      }
      CaloMETCollection::const_iterator caloMET = metCollection->begin();
      LogTrace("") << ">>> CaloMET_et, CaloMET_py, CaloMET_py= " << caloMET->et() << ", " << caloMET->px() << ", " << caloMET->py();;
      met_px += caloMET->px();
      met_py += caloMET->py();
      double met_et = sqrt(met_px*met_px+met_py*met_py);

      edm::Handle<reco::MuIsoAssociationMap> isoMap_;
      try {
            ev.getByLabel(isoTag_, isoMap_);
      } catch (...) {
            LogTrace("") << ">>> ISO collection does not exist !!!";
            return false;
      }

      Handle<CaloJetCollection> jetCollection;
      try {
            ev.getByLabel(jetTag_, jetCollection);
      } catch (...) {
            LogTrace("") << ">>> CALOJET collection does not exist !!!";
            return false;
      }

      CaloJetCollection::const_iterator jet = jetCollection->begin();
      int njets = 0;
      for (jet=jetCollection->begin(); jet!=jetCollection->end(); jet++) {
            if (jet->et()>eJetMin_) njets++;
      }
      LogTrace("") << ">>> Number of jets " << jetCollection->size() << "; above " << eJetMin_ << " GeV: " << njets;
      LogTrace("") << ">>> Number of jets above " << eJetMin_ << " GeV: " << njets;
      if (njets>nJetMax_) return false;

      std::vector<TrackRef> mymuons;
      mymuons.clear();		
      unsigned int nmuons = 0;
      unsigned int nmuonsForZ = 0;
      for (unsigned int i=0; i<muonCollection->size(); i++) {
            TrackRef mu(muonCollection,i);
            LogTrace("") << "> Processing muon number " << i << "...";
            double pt = mu->pt();
            if (pt>ptThrForZCount_) nmuonsForZ++;
            LogTrace("") << "\t... pt= " << pt << " GeV";
            if (pt<ptCut_) continue;
            double eta = mu->eta();
            LogTrace("") << "\t... eta= " << eta;
            if (fabs(eta)>etaCut_) continue;
            bool iso = (*isoMap_)[mu];
            LogTrace("") << "\t... isolated? " << iso;
            if (!iso) continue;
            double w_et = mu->pt() + met_et;
            double w_px = mu->px() + met_px;
            double w_py = mu->py() + met_py;
            double massT = w_et*w_et - w_px*w_px - w_py*w_py;
            massT = (massT>0) ? sqrt(massT) : 0;
            LogTrace("") << "\t... W_et, W_px, W_py= " << w_et << ", " << w_px << ", " << w_py << " GeV";
            LogTrace("") << "\t... Invariant transverse mass= " << massT << " GeV";
            if (massT<massTMin_) continue;
            if (massT>massTMax_) continue;
            Geom::Phi<double> deltaphi(mu->phi()-atan2(met_py,met_px));
            double acop = deltaphi.value();
            if (acop<0) acop = - acop;
            acop = M_PI - acop;
            LogTrace("") << "\t... acop= " << acop;
            if (acop>acopCut_) continue;
            nmuons++;
	    mymuons.push_back(mu);	
      }
      LogTrace("") << "> Muon counts to reject Z= " << nmuonsForZ;
      if (nmuonsForZ>=2) {
            LogTrace("") << ">>>> Event REJECTED";
            return false;
      }
      LogTrace("") << "> Number of muons for W= " << nmuons;
      if (nmuons<1) {
            LogTrace("") << ">>>> Event REJECTED";
            return false;
      }
      LogTrace("") << ">>>> Event SELECTED!!!";

      std::auto_ptr<CandidateCollection> Wmuoncoll( new CandidateCollection );	
      const double mumass = 0.1057;//from PDG	
      for(std::vector<TrackRef>::const_iterator itr = mymuons.begin(); itr != mymuons.end(); itr++){
	LogTrace("") << "\t...";
        RecoChargedCandidate* candmu = new RecoChargedCandidate;
        candmu->setCharge( (*itr)->charge() );
        candmu->setVertex( (*itr)->vertex() );
        Track::Vector p = (*itr)->momentum();
        LogTrace("") << "\t... pt= " << (*itr)->pt() << " GeV";
        LogTrace("") << "\t... eta= " << (*itr)->eta();         
        double t = sqrt( mumass*mumass + p.mag2() );
        candmu->setP4( Candidate::LorentzVector( p.x(), p.y(), p.z(), t ) );
        candmu->setTrack( *itr );
        candmu->setPdgId( 13 );
        Wmuoncoll->push_back(candmu);
      }
      ev.put(Wmuoncoll,"WMuons");			
      return true;

}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( WToMuNuSelector );
