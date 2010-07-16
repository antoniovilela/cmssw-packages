////////// Header section /////////////////////////////////////////////
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/RandGauss.h"

#include "TFile.h"
#include "TTree.h"


class WMuNuAnalyzer: public edm::EDAnalyzer {
public:
  /// Constructor
  WMuNuAnalyzer(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~WMuNuAnalyzer();

  // Operations

  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  virtual void beginJob(const edm::EventSetup& eventSetup) ;
  virtual void endJob() ;

private:
  // Input from cfg file
  edm::InputTag genParticleTag_;
  edm::InputTag muonTag_;
  edm::InputTag isoTag_;
  edm::InputTag genMETTag_;
  edm::InputTag metTag_;

  std::string CSA07WeightLabel;

  double isoCone_;

  //Use weighted events (for Alpgen)
  bool runOnSoup_;
  
  // TTree 
  TTree* data;
  double tree_evtWeight;
  double tree_muonIso;
  double tree_muonNormIso;
  double tree_muonPt;
  double tree_muonEta;
  double tree_muonPhi;
  double tree_MT;
  double tree_MET;
  double tree_MEx;
  double tree_MEy;
  double tree_GenMET;
  double tree_GenMEx;
  double tree_GenMEy;
  double tree_GenMT;
  double tree_CaloMET;
  double tree_CaloMEx;
  double tree_CaloMEy;
  double tree_CaloMETPhi;
  double tree_genWPt;
  double tree_genWEta;
  double tree_genWPhi;
};

////////// Source code ////////////////////////////////////////////////
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuIsoDeposit.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/GenMET.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

using namespace std;
using namespace edm;
using namespace reco;

/// Constructor
WMuNuAnalyzer::WMuNuAnalyzer(const ParameterSet& pset)
{
  genParticleTag_ = pset.getParameter<InputTag>("GenParticleTag");
  muonTag_ = pset.getParameter<InputTag>("MuonTag");
  isoTag_ = pset.getParameter<InputTag>("IsolationTag");
  genMETTag_ = pset.getParameter<InputTag>("GenMETTag");
  metTag_ = pset.getParameter<InputTag>("METTag");

  isoCone_ = pset.getParameter<double>("IsoCone");

  runOnSoup_ = pset.getUntrackedParameter<bool>("RunOnSoup",false);
  if(runOnSoup_) CSA07WeightLabel=pset.getParameter<std::string>("CSA07WeightLabel");
}

/// Destructor
WMuNuAnalyzer::~WMuNuAnalyzer(){
}

void WMuNuAnalyzer::beginJob(const EventSetup& eventSetup){
  edm::Service<TFileService> fs;

  
  // Set output TTree
  data = fs->make<TTree>("data","data");
  data->Branch("evtWeight",&tree_evtWeight,"evtWeight/D");
  data->Branch("muonIso",&tree_muonIso,"muonIso/D");
  data->Branch("muonNormIso",&tree_muonNormIso,"muonNormIso/D");
  data->Branch("muonPt",&tree_muonPt,"muonPt/D");
  data->Branch("muonEta",&tree_muonEta,"muonEta/D");
  data->Branch("muonPhi",&tree_muonPhi,"muonPhi/D");
  data->Branch("MT",&tree_MT,"MT/D");
  data->Branch("MET",&tree_MET,"MET/D");
  data->Branch("MEx",&tree_MEx,"MEx/D");
  data->Branch("MEy",&tree_MEy,"MEy/D");
  data->Branch("GenMET",&tree_GenMET,"GenMET/D");
  data->Branch("GenMEx",&tree_GenMEx,"GenMEx/D");
  data->Branch("GenMEy",&tree_GenMEy,"GenMEy/D");
  data->Branch("GenMT",&tree_GenMT,"GenMT/D");
  data->Branch("CaloMET",&tree_CaloMET,"CaloMET/D");
  data->Branch("CaloMEx",&tree_CaloMEx,"CaloMEx/D");
  data->Branch("CaloMEy",&tree_CaloMEy,"CaloMEy/D");
  data->Branch("CaloMETPhi",&tree_CaloMETPhi,"CaloMETPhi/D");
  data->Branch("genWPt",&tree_genWPt,"genWPt/D");
  data->Branch("genWEta",&tree_genWEta,"genWEta/D");
  data->Branch("genWPhi",&tree_genWPhi,"genWPhi/D");
}

void WMuNuAnalyzer::endJob(){}

void WMuNuAnalyzer::analyze(const Event & ev, const EventSetup&){
  //CSA07 event weight
  edm::Handle<double> CSA07Weight;
  if(runOnSoup_){
	ev.getByLabel(CSA07WeightLabel,"weight",CSA07Weight);
	tree_evtWeight = *CSA07Weight.product();
  } else tree_evtWeight = 1.0;

  // Get muon collection
  edm::Handle<reco::MuonCollection> muonCollection;
  ev.getByLabel(muonTag_, muonCollection);
  assert(muonCollection->size() > 0);

  // Get isolation collection
  edm::Handle<reco::MuIsoDepositAssociationMap> isodepMap;
  ev.getByLabel(isoTag_, isodepMap);

  // Find highest pt muon
  const reco::Muon* muon = 0;
  double aux_muonpt = -1.;	 
  for(reco::MuonCollection::const_iterator mu = muonCollection->begin(); mu != muonCollection->end(); ++mu) {
	// Make sure it passes the selection cuts
	if(mu->pt() < 25.0) continue;
	if(fabs(mu->eta()) > 2.0) continue;

	if(mu->pt() > aux_muonpt){
		aux_muonpt = mu->pt();
		muon = &(*mu);
	}
  }
  if (muon == 0) {LogTrace("") << ">>>>>>> No muon found...skipping event";return;}
  LogTrace("") << ">>>>>>> Selected muon px,py,pz,e= " << muon->px() << " , " << muon->py() << " , " << muon->pz() << " , " << muon->energy();

  reco::TrackRef muTrack = muon->combinedMuon();
  const reco::MuIsoDeposit dep = (*isodepMap)[muTrack];
  float ptsum = dep.depositWithin(isoCone_);
  float ptsumOverpt = ptsum/muon->pt();
  LogTrace("") << "\t... Isol, Track pt, = " << muon->pt() << " GeV, " << " ptsum = " << ptsum << " ptsum/pt = " << ptsumOverpt;

  tree_muonIso = ptsum;
  tree_muonNormIso = ptsumOverpt;
  tree_muonPt = muon->pt();
  tree_muonEta = muon->eta();
  tree_muonPhi = muon->phi();

  //=================================================================  
  // Generator Information
  const GenParticle* genW = 0;
  const GenParticle* genMuon = 0;
  const GenParticle* genNeutrino = 0;	
  edm::Handle<reco::GenParticleCollection> genParticles;
  ev.getByLabel(genParticleTag_, genParticles);
  for( size_t i = 0; i < genParticles->size(); ++ i ) {
      		const reco::GenParticle& genpart = (*genParticles)[i];
      		//LogTrace("") << ">>>>>>> pid,status,px,py,px,e= "  << genpart.pdgId() << " , " << genpart.status() << " , " << genpart.px() << " , " << genpart.py() << " , " << genpart.pz() << " , " << genpart.energy();	
		
		if(abs(genpart.pdgId()) != 24) continue;
		if(genpart.status() != 3) continue;
		genW = &(*genParticles)[i];
		for(size_t j = 0; j < genpart.numberOfDaughters(); ++j) {
                       	const Candidate* daughter = genpart.daughter(j);
                       	if(abs(daughter->pdgId()) == 13) genMuon = dynamic_cast<const reco::GenParticle*>(daughter);
                       	else if((abs(daughter->pdgId()) == 14)) genNeutrino = dynamic_cast<const reco::GenParticle*>(daughter);
		}
      		break;
  }
  if (genW == 0) {LogTrace("") << ">>>>>>> No generated W found...skipping event";return;}		
  if (genNeutrino == 0) {LogTrace("") << ">>>>>>> No generated neutrino found...skipping event";return;}
  if (genMuon == 0) {LogTrace("") << ">>>>>>> No generated muon found...skipping event";return;}
  reco::Candidate::LorentzVector genW_p4 = genW->p4();
  reco::Candidate::LorentzVector genMu_p4 = genMuon->p4();
  reco::Candidate::LorentzVector genNu_p4 = genNeutrino->p4();
  LogTrace("") << ">>>> Gen W px,py,pz,e= " << genW_p4.px() << " , " << genW_p4.py() << " , " << genW_p4.pz() << " , " << genW_p4.energy();
  LogTrace("") << ">>>> Gen neutrino px,py,pz,e= " << genNu_p4.px() << " , " << genNu_p4.py() << " , " << genNu_p4.pz() << " , " << genNu_p4.energy();
  LogTrace("") << ">>>> Gen muon px,py,pz,e= " << genMu_p4.px() << " , " << genMu_p4.py() << " , " << genMu_p4.pz() << " , " << genMu_p4.energy();

  tree_genWPt = genW_p4.pt();
  tree_genWEta = genW_p4.eta();
  tree_genWPhi = genW_p4.phi();

  //=================================================================
  // Get MET collections (assumed corrected)
  edm::Handle<reco::GenMETCollection> genMETCollection;
  ev.getByLabel(genMETTag_, genMETCollection);
  reco::GenMETCollection::const_iterator genMET = genMETCollection->begin();
  LogTrace("") << ">>> GenMET_et, GenMET_px, GenMET_py= " << genMET->pt() << ", " << genMET->px() << ", " << genMET->py();

  edm::Handle<reco::CaloMETCollection> METCollection;
  ev.getByLabel(metTag_, METCollection);
  reco::CaloMETCollection::const_iterator met = METCollection->begin();
  LogTrace("") << ">>> MET_et, MET_px, MET_py= " << met->pt() << ", " << met->px() << ", " << met->py();

  double CaloMET_Ex = met->px();
  double CaloMET_Ey = met->py();

  CaloMET_Ex += muon->px();
  CaloMET_Ey += muon->py();
  
  double CaloMET_Et = sqrt(CaloMET_Ex*CaloMET_Ex + CaloMET_Ey*CaloMET_Ey);
  math::XYZTLorentzVector myCaloMET(CaloMET_Ex,CaloMET_Ey,0.,CaloMET_Et);
  LogTrace("") << ">>> CaloMET_et, MET_px, MET_py= " << myCaloMET.pt() << ", " << myCaloMET.px() << ", " << myCaloMET.py();
  
  double w_et = muon->pt() + met->pt();
  double w_px = muon->px() + met->px();
  double w_py = muon->py() + met->py();
  double massT = w_et*w_et - w_px*w_px - w_py*w_py;
  massT = (massT>0) ? sqrt(massT) : 0;
  LogTrace("") << "\t... W_pt, W_px, W_py= " << sqrt(w_px*w_px + w_py*w_py) << ", " << w_px << ", " << w_py << " GeV";
  LogTrace("") << "\t... Invariant transverse mass= " << massT << " GeV";

  tree_CaloMET = CaloMET_Et;
  tree_CaloMEx = CaloMET_Ex;
  tree_CaloMEy = CaloMET_Ey;
  tree_CaloMETPhi =  myCaloMET.phi();
  tree_MT = massT;
  tree_MET = met->pt();
  tree_MEx = met->px();
  tree_MEy = met->py();

  //=================================================================
  //Gen MT
  /*double w_et_gen = genMu_p4.pt() + genNu_p4.pt();
  double w_px_gen = genMu_p4.px() + genNu_p4.px();
  double w_py_gen = genMu_p4.py() + genNu_p4.py();*/
  double w_et_gen = genMu_p4.pt() + genMET->pt();
  double w_px_gen = genMu_p4.px() + genMET->px();
  double w_py_gen = genMu_p4.py() + genMET->py();
  double massT_gen = w_et_gen*w_et_gen - w_px_gen*w_px_gen - w_py_gen*w_py_gen;
  massT_gen = (massT_gen>0) ? sqrt(massT_gen) : 0;

  tree_GenMT = massT_gen;
  tree_GenMET = genMET->pt();
  tree_GenMEx = genMET->px();
  tree_GenMEy = genMET->py();

  data->Fill();
}

DEFINE_FWK_MODULE(WMuNuAnalyzer);
