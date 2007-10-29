/* \class ZToMuMuSelector
 *
 * \author Juan Alcaraz, CIEMAT
 *
 */
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/MuonReco/interface/Muon.h"

class ZToMuMuSelector : public edm::EDFilter {
public:
  ZToMuMuSelector (const edm::ParameterSet &);
  virtual bool filter(edm::Event&, const edm::EventSetup&);
private:
  edm::InputTag muonTag_;
  edm::InputTag isoTag_;
  double ptCut_;
  double etaCut_;
  double massZMin_;
  double massZMax_;

  bool onlyGlobalMuons_;
  edm::InputTag trackerTag_;
  edm::InputTag isoTrackerTag_;
  int minTrackerHits_;
};

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/MuonReco/interface/MuIsoDeposit.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"

#include "PhysicsTools/CandUtils/interface/AddFourMomenta.h"

using namespace edm;
using namespace std;
using namespace reco;

ZToMuMuSelector::ZToMuMuSelector( const ParameterSet & cfg ) :
      muonTag_(cfg.getParameter<edm::InputTag> ("MuonTag")),
      isoTag_(cfg.getParameter<edm::InputTag> ("IsolationTag")),
      ptCut_(cfg.getParameter<double>("PtCut")),
      etaCut_(cfg.getParameter<double>("EtaCut")),
      massZMin_(cfg.getParameter<double>("MassZMin")),
      massZMax_(cfg.getParameter<double>("MassZMax")),

      onlyGlobalMuons_(cfg.getParameter<bool>("OnlyGlobalMuons")),
      trackerTag_(cfg.getUntrackedParameter<edm::InputTag> ("TrackerTag",edm::InputTag("ctfWithMaterialTracks"))),
      isoTrackerTag_(cfg.getUntrackedParameter<edm::InputTag> ("TrackerIsolationTag",edm::InputTag("zMuMuTrackerIsolations"))),
      minTrackerHits_(cfg.getUntrackedParameter<int>("MinTrackerHits",7))
{
	std::string alias;
	//produces<CandidateCollection>( alias = "muonsForZ" ).setBranchAlias( alias );
	produces<CandidateCollection>( alias = "ZGMGM" ).setBranchAlias( alias );
	//produces<CandidateCollection>( alias = "ZGMTK" ).setBranchAlias( alias );
}

bool ZToMuMuSelector::filter (Event & ev, const EventSetup &) {

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
      /*Handle<CandidateCollection> muonCollection;
      try {
            ev.getByLabel(muonTag_, muonCollection);
      } catch (...) {
            LogTrace("") << ">>> Muon collection does not exist !!!";
            return false;
      }*/

      Handle<reco::MuIsoAssociationMap> isoMap;
      try {
            ev.getByLabel(isoTag_, isoMap);
      } catch (...) {
            LogTrace("") << ">>> ISO Muon collection does not exist !!!";
            return false;
      }

      Handle<TrackCollection> trackerCollection;
      Handle<reco::MuIsoAssociationMap> isoTrackerMap;
      if (!onlyGlobalMuons_) {
            try {
                  ev.getByLabel(trackerTag_, trackerCollection);
            } catch (...) {
                  LogTrace("") << ">>> Tracker collection does not exist !!!";
                  return false;
            }

            try {
                  ev.getByLabel(isoTrackerTag_, isoTrackerMap);
            } catch (...) {
                  LogTrace("") << ">>> ISO Tracker collection does not exist !!!";
                  return false;
            }
      }

      //std::auto_ptr<CandidateCollection> Zcoll( new CandidateCollection );

      //TrackRef mymu1;
      //TrackRef mymu2;
      std::vector<pair<unsigned int,unsigned int> > GMGMpairs;
      GMGMpairs.clear();
      std::vector<pair<unsigned int,unsigned int> > GMTKpairs;
      GMTKpairs.clear(); 	
      unsigned int npairs = 0;
      bool globalCombinationFound = false;
      for (unsigned int i=0; i<muonCollection->size(); i++) {
            TrackRef mu(muonCollection,i);
            LogTrace("") << "> Processing muon number " << i << "...";
            double pt = mu->pt();
            LogTrace("") << "\t... pt= " << pt << " GeV";
            if (pt<ptCut_) continue;
            double eta = mu->eta();
            LogTrace("") << "\t... eta= " << eta;
            if (fabs(eta)>etaCut_) continue;
            bool iso = (*isoMap)[mu];
            LogTrace("") << "\t... isolated? " << iso;
            if (!iso) continue;
	    //CandidateRef muRef(muonCollection,i);	
	    //mymu1 = mu; 	
            for (unsigned int j=i+1; j<muonCollection->size(); j++) {
                  TrackRef mu2(muonCollection,j);
                  LogTrace("") << "> Processing second muon number " << j << "...";
                  double pt2 = mu2->pt();
                  LogTrace("") << "\t... pt2= " << pt2 << " GeV";
                  if (pt2<ptCut_) continue;
                  double eta2 = mu2->eta();
                  LogTrace("") << "\t... eta2= " << eta2;
                  if (fabs(eta2)>etaCut_) continue;
                  bool iso2 = (*isoMap)[mu2];
                  LogTrace("") << "\t... isolated2? " << iso2;
                  if (!iso2) continue;

                  double z_en = mu->p() + mu2->p();
                  double z_px = mu->px() + mu2->px();
                  double z_py = mu->py() + mu2->py();
                  double z_pz = mu->pz() + mu2->pz();
                  double massZ = z_en*z_en - z_px*z_px - z_py*z_py - z_pz*z_pz;
                  massZ = (massZ>0) ? sqrt(massZ) : 0;
                  LogTrace("") << "\t... Z_en, Z_px, Z_py, Z_pz= " << z_en << ", " << z_px << ", " << z_py << ", " << z_pz << " GeV";
                  LogTrace("") << "\t... (GM-GM) Invariant reconstructed mass= " << massZ << " GeV";
                  if (massZ<massZMin_) continue;
                  if (massZ>massZMax_) continue;
                  globalCombinationFound = true;
                  npairs++;
		  //CandidateRef muRef(muonCollection,i);
		  //mymu1 = mu;
            	  //mymu2 = mu2;
		  GMGMpairs.push_back(std::pair<unsigned int,unsigned int>(i,j));	
            }

            if (onlyGlobalMuons_ || globalCombinationFound) continue;

            for (unsigned int j=0; j<trackerCollection->size(); j++) {
                  TrackRef mu2(trackerCollection,j);
                  LogTrace("") << "> Processing track number " << j << "...";
                  double pt2 = mu2->pt();
                  LogTrace("") << "\t... pt3= " << pt2 << " GeV";
                  if (pt2<ptCut_) continue;
                  double eta2 = mu2->eta();
                  LogTrace("") << "\t... eta3= " << eta2;
                  if (fabs(eta2)>etaCut_) continue;
                  int nhits2 = mu2->numberOfValidHits();
                  LogTrace("") << "\t... nhits3= " << nhits2;
                  if (nhits2<minTrackerHits_) continue;
                  bool iso2 = (*isoTrackerMap)[mu2];
                  LogTrace("") << "\t... isolated3? " << iso2;
                  if (!iso2) continue;

                  double z_en = mu->p() + mu2->p();
                  double z_px = mu->px() + mu2->px();
                  double z_py = mu->py() + mu2->py();
                  double z_pz = mu->pz() + mu2->pz();
                  double massZ = z_en*z_en - z_px*z_px - z_py*z_py - z_pz*z_pz;
                  massZ = (massZ>0) ? sqrt(massZ) : 0;
                  LogTrace("") << "\t... Z_en, Z_px, Z_py, Z_pz= " << z_en << ", " << z_px << ", " << z_py << ", " << z_pz << " GeV";
                  LogTrace("") << "\t... (GM-TK) Invariant reconstructed mass= " << massZ << " GeV";
                  if (massZ<massZMin_) continue;
                  if (massZ>massZMax_) continue;
                  npairs++;
		  GMTKpairs.push_back(std::pair<unsigned int,unsigned int>(i,j));
            }
      }

      LogTrace("") << "> Number of Z pairs found= " << npairs;
      if (npairs<1) {
            LogTrace("") << ">>>> Event REJECTED";
            return false;
      }
      LogTrace("") << ">>>> Event SELECTED!!!";

      //Create candidates from track refs
      //std::auto_ptr<CandidateCollection> Zmuonscoll( new CandidateCollection );
      std::auto_ptr<CandidateCollection> ZGMGMcoll( new CandidateCollection );	
      const double mumass = 0.1057;//from PDG
      for(std::vector<pair<unsigned int,unsigned int> >::const_iterator itr = GMGMpairs.begin(); itr != GMGMpairs.end(); itr++){
	TrackRef mymu1(muonCollection,(*itr).first);
	TrackRef mymu2(muonCollection,(*itr).second);
	LogTrace("") << "\t...";
	RecoChargedCandidate* candmu1 = new RecoChargedCandidate;
      	candmu1->setCharge( mymu1->charge() );
      	candmu1->setVertex( mymu1->vertex() );
      	Track::Vector p1 = mymu1->momentum();
      	LogTrace("") << "\t... pt1= " << mymu1->pt() << " GeV";
      	LogTrace("") << "\t... eta1= " << mymu1->eta();		
      	double t1 = sqrt( mumass*mumass + p1.mag2() );
      	candmu1->setP4( Candidate::LorentzVector( p1.x(), p1.y(), p1.z(), t1 ) );
      	candmu1->setTrack( mymu1 );
      	candmu1->setPdgId( 13 );
      	//Zmuonscoll->push_back(candmu1);
      	RecoChargedCandidate* candmu2 = new RecoChargedCandidate;
      	candmu2->setCharge( mymu2->charge() );
      	candmu2->setVertex( mymu2->vertex() );
      	Track::Vector p2 = mymu2->momentum();
      	LogTrace("") << "\t... pt2= " << mymu2->pt() << " GeV";
      	LogTrace("") << "\t... eta2= " << mymu2->eta();
      	double t2 = sqrt( mumass*mumass + p2.mag2() );
      	candmu2->setP4( Candidate::LorentzVector( p2.x(), p2.y(), p2.z(), t2 ) );
      	candmu2->setTrack( mymu2 );
      	candmu2->setPdgId( 13 );
      	//Zmuonscoll->push_back(candmu2);
      	//ev.put(Zmuonscoll,"muonsForZ");
			
      	//Create composite candidate for Z
      	//std::auto_ptr<CandidateCollection> Zcoll( new CandidateCollection );	
      	CompositeCandidate* Zcand(new CompositeCandidate);
      	/*Candidate::LorentzVector mu1_p4(mymu1->px(),mymu1->py(),mymu1->pz(),mymu1->p());	
      	Candidate mucand1(mymu1->charge(),mu1_p4);
      	//Zcand->addDaughter(reco::ShallowCloneCandidate( reco::CandidateBaseRef( mymu1 ) ) );*/
      	//const Candidate* mu1clone = candmu1->clone();
      	Zcand->addDaughter(*candmu1);
      	/*Candidate::LorentzVector mu2_p4(mymu2->px(),mymu2->py(),mymu2->pz(),mymu2->p());
      	Candidate mucand2(mymu2->charge(),mu2_p4);
      	//Zcand->addDaughter(reco::ShallowCloneCandidate( reco::CandidateBaseRef( mymu2 ) ) );*/
      	Zcand->addDaughter(*candmu2);
      	AddFourMomenta addP4;
      	addP4.set( *Zcand );
      	//Zcand->setCharge(0);
      	Zcand->setPdgId(23);
      	LogTrace("") << "\t... Z_en, Z_px, Z_py, Z_pz= " << Zcand->energy() << ", " << Zcand->px() << ", " << Zcand->py() << ", " << Zcand->pz() << " GeV";	
      	LogTrace("") << "\t... Saved Z Invariant reconstructed mass= " << Zcand->p4().M() << " GeV";		

      	ZGMGMcoll->push_back(Zcand);
      }	
      ev.put(ZGMGMcoll,"ZGMGM");
	   
      return true;

}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( ZToMuMuSelector );
