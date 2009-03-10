/** original class: WMETAnalyzer (author: J. Alcaraz)
 *
 *  modified for class: WMETType2Analyzer (Antonio.Vilela.Pereira@cern.ch) 
 * 
 */

////////// Header section /////////////////////////////////////////////
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/RandGauss.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

//class RandomEngine;
//class FML3PtSmearer;

class WMETType2Analyzer: public edm::EDAnalyzer {
public:
  /// Constructor
  WMETType2Analyzer(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~WMETType2Analyzer();

  // Operations

  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  virtual void beginJob(const edm::EventSetup& eventSetup) ;
  virtual void endJob() ;

private:
  // Input from cfg file
  edm::InputTag genParticleTag_;
  edm::InputTag muonTag_;
  //edm::InputTag isoTag_;
  edm::InputTag genMETTag_;
  edm::InputTag metTag_;

  std::string theInputRootFileName;
  std::string theOutputRootFileName;

  std::string CSA07WeightLabel;

  // The input/output Root file
  TFile* theInputFile;
  TFile* theOutputFile;

  //double isoCone_;

  //Nu smearing
  //bool applyNuPtSmearing_;

  //Sample CaloMET phi
  bool sampleCaloMETPhi_;
  bool use1DCaloMETPhisampling_;

  //Sample from histos or gaussian
  bool sampleFromHistos_;

  //Use weighted events (for Alpgen)
  bool runOnSoup_;
  
  // Histograms
  TH1F* hCaloMETPhiMinusVBPhiInput;
  TH2F* hCaloMETvsVBPtInput;
  TH2F* hCaloMETPhivsVBPhiInput;

  TH2F* hCaloMETParalvsVBPtInput;
  TH1F* hCaloMETPerpInput;
 
  // TTree 
  TTree* data;
  double tree_evtWeight;
  double tree_MT;
  double tree_MET;
  double tree_muonPt;
  double tree_muonEta;
  double tree_muonPhi;
  double tree_muonIso;
  double tree_muonNormIso;
  double tree_PtNu;
  double tree_EtaNu;
  double tree_PtMuFromNu;
  double tree_EtaMuFromNu;
  double tree_GenMET;
  double tree_GenMT;
  double tree_CaloMET;
  double tree_CaloMETPhi;
  double tree_VBPt;
  double tree_VBEta;
  double tree_VBPhi;
  double tree_genWPt;
  double tree_genWEta;
  double tree_genWPhi;
  double tree_MTfromMuNu;
  double tree_MTfromZmumu;
  double tree_METfromZmumu;
  double tree_SampledCaloMET;
  double tree_SampledCaloMETPhi;
  double tree_MTfromZmumu_v2;
  double tree_METfromZmumu_v2;
  double tree_SampledCaloMET_v2;
  double tree_SampledCaloMEx_v2;
  double tree_SampledCaloMEy_v2;
  double tree_SampledCaloMETParal;
  double tree_SampledCaloMETPerp; 
	
  //Random number engine/generator
  CLHEP::HepRandomEngine* fRandomEngine;
  CLHEP::RandGauss*       fRandomGenerator;  

  //RandomEngine* FSRandom;
  //FM Pt Smearer
  //FML3PtSmearer * myPtSmearer;
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
#include "DataFormats/MuonReco/interface/MuonFwd.h"
//#include "DataFormats/MuonReco/interface/MuIsoDeposit.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETFwd.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

//#include "FastSimulation/Utilities/interface/RandomEngine.h"
//#include "FastSimulation/ParamL3MuonProducer/interface/FML3PtSmearer.h"

using namespace std;
using namespace edm;
using namespace reco;

/// Constructor
WMETType2Analyzer::WMETType2Analyzer(const ParameterSet& pset)
{
  genParticleTag_ = pset.getParameter<InputTag>("GenParticleTag");
  muonTag_ = pset.getParameter<InputTag>("MuonTag");
  //isoTag_ = pset.getParameter<InputTag>("IsolationTag");
  genMETTag_ = pset.getParameter<InputTag>("GenMETTag");
  metTag_ = pset.getParameter<InputTag>("METTag");

  //isoCone_ = pset.getParameter<double>("IsoCone");

  theInputRootFileName = pset.getUntrackedParameter<string>("InputRootFileName");
  theOutputRootFileName = pset.getUntrackedParameter<string>("OutputRootFileName");

  runOnSoup_ = pset.getUntrackedParameter<bool>("RunOnSoup",false);
  if(runOnSoup_) CSA07WeightLabel=pset.getParameter<std::string>("CSA07WeightLabel");

  //applyNuPtSmearing_ = pset.getUntrackedParameter<bool>("ApplyNuPtSmearing",false);
  sampleCaloMETPhi_ = pset.getUntrackedParameter<bool>("SampleCaloMETPhi",true);
  use1DCaloMETPhisampling_ = pset.getUntrackedParameter<bool>("Use1DCaloMETPhi",true);
  sampleFromHistos_ = pset.getUntrackedParameter<bool>("SampleFromHistos",true);
}

/// Destructor
WMETType2Analyzer::~WMETType2Analyzer(){
}

void WMETType2Analyzer::beginJob(const EventSetup& eventSetup){
  //edm::Service<TFileService> fs;
  TH1::SetDefaultSumw2(true);

  // Get input root file
  theInputFile = new TFile(theInputRootFileName.c_str(), "READONLY");
  theInputFile->cd();

  hCaloMETPhiMinusVBPhiInput = (TH1F*)theInputFile->Get("hCaloMETPhiMinusVBPhi");
  hCaloMETvsVBPtInput = (TH2F*)theInputFile->Get("hCaloMETvsVBPt");
  hCaloMETPhivsVBPhiInput = (TH2F*)theInputFile->Get("hCaloMETPhivsVBPhi");
  hCaloMETParalvsVBPtInput = (TH2F*)theInputFile->Get("hCaloMETParalvsVBPt");
  hCaloMETPerpInput = (TH1F*)theInputFile->Get("hCaloMETPerp");

  hCaloMETPhiMinusVBPhiInput->SetName("hCaloMETPhiMinusVBPhiInput");
  hCaloMETvsVBPtInput->SetName("hCaloMETvsVBPtInput");
  hCaloMETPhivsVBPhiInput->SetName("hCaloMETPhivsVBPhiInput");
  hCaloMETParalvsVBPtInput->SetName("hCaloMETParalvsVBPtInput");
  hCaloMETPerpInput->SetName("hCaloMETPerpInput");

  // Set output TTree
  data = new TTree("data","data");
  data->Branch("evtWeight",&tree_evtWeight,"evtWeight/D");
  data->Branch("MT",&tree_MT,"MT/D");
  data->Branch("MET",&tree_MET,"MET/D");
  data->Branch("muonPt",&tree_muonPt,"muonPt/D");
  data->Branch("muonEta",&tree_muonEta,"muonEta/D");
  data->Branch("muonPhi",&tree_muonPhi,"muonPhi/D");
  data->Branch("muonIso",&tree_muonIso,"muonIso/D");
  data->Branch("muonNormIso",&tree_muonNormIso,"muonNormIso/D");
  data->Branch("PtNu",&tree_PtNu,"PtNu/D");
  data->Branch("EtaNu",&tree_EtaNu,"EtaNu/D");
  data->Branch("PtMuFromNu",&tree_PtMuFromNu,"PtMuFromNu/D");
  data->Branch("EtaMuFromNu",&tree_EtaMuFromNu,"EtaMuFromNu/D");
  data->Branch("GenMET",&tree_GenMET,"GenMET/D");
  data->Branch("GenMT",&tree_GenMT,"GenMT/D");
  data->Branch("CaloMET",&tree_CaloMET,"CaloMET/D");
  data->Branch("CaloMETPhi",&tree_CaloMETPhi,"CaloMETPhi/D");
  data->Branch("VBPt",&tree_VBPt,"VBPt/D"); 
  data->Branch("VBEta",&tree_VBEta,"VBEta/D");
  data->Branch("VBPhi",&tree_VBPhi,"VBPhi/D");
  data->Branch("genWPt",&tree_genWPt,"genWPt/D");
  data->Branch("genWEta",&tree_genWEta,"genWEta/D");
  data->Branch("genWPhi",&tree_genWPhi,"genWPhi/D");
  data->Branch("MTfromMuNu",&tree_MTfromMuNu,"MTfromMuNu/D");
  data->Branch("MTfromZmumu",&tree_MTfromZmumu,"MTfromZmumu/D");
  data->Branch("METfromZmumu",&tree_METfromZmumu,"METfromZmumu/D");
  data->Branch("SampledCaloMET",&tree_SampledCaloMET,"SampledCaloMET/D");
  data->Branch("SampledCaloMETPhi",&tree_SampledCaloMETPhi,"SampledCaloMETPhi/D");
  data->Branch("MTfromZmumuv2",&tree_MTfromZmumu_v2,"MTfromZmumuv2/D");
  data->Branch("METfromZmumuv2",&tree_METfromZmumu_v2,"METfromZmumuv2/D");
  data->Branch("SampledCaloMETv2",&tree_SampledCaloMET_v2,"SampledCaloMETv2/D");
  data->Branch("SampledCaloMExv2",&tree_SampledCaloMEx_v2,"SampledCaloMExv2/D");
  data->Branch("SampledCaloMEyv2",&tree_SampledCaloMEy_v2,"SampledCaloMEyv2/D");
  data->Branch("SampledCaloMETParal",&tree_SampledCaloMETParal,"SampledCaloMETParal/D");
  data->Branch("SampledCaloMETPerp",&tree_SampledCaloMETPerp,"SampledCaloMETPerp/D");

  theOutputFile = new TFile(theOutputRootFileName.c_str(), "RECREATE");
  theOutputFile->cd();

  // Initialize random generator
  edm::Service<edm::RandomNumberGenerator> rng;
  long seed = (long)(rng->mySeed());
  LogTrace("") << ">>>> Seed= " << seed;
  fRandomEngine = new CLHEP::HepJamesRandom(seed);
  fRandomGenerator = new CLHEP::RandGauss(fRandomEngine);
  LogTrace("") << ">>>> Random Generator Initialized";
  
  // Initialize the random number generator service
  /*edm::Service<edm::RandomNumberGenerator> rng;
  if ( ! rng.isAvailable() ) {
    throw cms::Exception("Configuration") <<
      "No RandomGeneratorService!!! \n"
      "You must add the service in the configuration file\n"
      "or remove the module that requires it.";
  }

  FSRandom = new RandomEngine(&(*rng));
  myPtSmearer = new FML3PtSmearer(FSRandom);*/
}

void WMETType2Analyzer::endJob(){
  theOutputFile->cd();
  //LogInfo("WMETType2Analyzer") << " Number of events used= " << hMT->GetEntries();

  /*hCaloMETvsVBPtInput->Write();
  hCaloMETPhivsVBPhiInput->Write();
  hCaloMETPhiMinusVBPhiInput->Write();*/

  data->Write();

  theOutputFile->Close();
}

void WMETType2Analyzer::analyze(const Event & ev, const EventSetup&){
  TH1::SetDefaultSumw2(true);
  theOutputFile->cd();

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

  /*// Get isolation collection
  edm::Handle<reco::MuIsoDepositAssociationMap> isodepMap;
  ev.getByLabel(isoTag_, isodepMap);*/

  // Find highest pt muon
  const reco::Muon* muon = 0;
  double aux_muonpt = -1.;	 
  for(reco::MuonCollection::const_iterator mu = muonCollection->begin(); mu != muonCollection->end(); ++mu) {
	// Make sure it passes the selection cuts
        if(!mu->isGlobalMuon()) continue;
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
  //const reco::MuIsoDeposit dep = (*isodepMap)[muTrack];
  //float ptsum = dep.depositWithin(isoCone_);
  float ptsum = muon->isolationR03().sumPt;
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

  //=================================================================
  //Apply smearing
  /*math::XYZTLorentzVector MuonFromNu;	
  if(applyNuPtSmearing_){	
  	math::XYZVector recP3(genNu_p4.px(),genNu_p4.py(),genNu_p4.pz());
  	MuonFromNu = myPtSmearer->smear(genNu_p4,recP3);
  	LogTrace("") << ">>> Smeared neutrino px,py,pz,e= " << MuonFromNu.px() << " , " << MuonFromNu.py() << " , " << MuonFromNu.pz() << " , " << MuonFromNu.energy();
  } else MuonFromNu = genNu_p4;

  //hPtNuSmeared->Fill(MuonFromNu.pt());*/

  // Leave like this for now	 
  math::XYZTLorentzVector MuonFromNu = genNu_p4;

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

  tree_CaloMET = myCaloMET.pt();
  tree_CaloMETPhi =  myCaloMET.phi();
  tree_MT = massT;
  tree_MET = met->pt();
  tree_PtNu = genNu_p4.pt();
  tree_EtaNu = genNu_p4.eta();
  tree_PtMuFromNu = MuonFromNu.pt();
  tree_EtaMuFromNu = MuonFromNu.eta(); 

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

  tree_genWPt = genW_p4.pt();
  tree_genWEta = genW_p4.eta();
  tree_genWPhi = genW_p4.phi();
  tree_GenMT = massT_gen;
  tree_GenMET = genMET->pt();

  //=================================================================
  // Calculate W using reconstructed Muon and generated Neutrino
  math::XYZVector myW_p3((muon->px() + MuonFromNu.px()),(muon->py() + MuonFromNu.py()),(muon->pz() + MuonFromNu.pz()));
  double myW_e = sqrt(80.43*80.43 + myW_p3.mag2()); 
  math::XYZTLorentzVector myW_p4((muon->px() + MuonFromNu.px()),(muon->py() + MuonFromNu.py()),(muon->pz() + MuonFromNu.pz()),myW_e);

  tree_VBPt = myW_p4.pt();
  tree_VBEta = myW_p4.eta();
  tree_VBPhi = myW_p4.phi();
	
  double w_et_MuNu = muon->pt() + MuonFromNu.pt();
  double w_px_MuNu = muon->px() + MuonFromNu.px();
  double w_py_MuNu = muon->py() + MuonFromNu.py();
  double massT_MuNu = w_et_MuNu*w_et_MuNu - w_px_MuNu*w_px_MuNu - w_py_MuNu*w_py_MuNu;
  massT_MuNu = (massT_MuNu>0) ? sqrt(massT_MuNu) : 0;
  LogTrace("") << "\t... W_pt, W_px, W_py= " << sqrt(w_px_MuNu*w_px_MuNu + w_py_MuNu*w_py_MuNu) << ", " << w_px_MuNu << ", " << w_py_MuNu << " GeV";
  LogTrace("") << "\t... Invariant transverse mass= " << massT_MuNu << " GeV";

  tree_MTfromMuNu = massT_MuNu;
	
  //================================================================= 
  // Get MET from Z(data) sample
  // Find bin corresponding to W pt
  int binWpt = (hCaloMETvsVBPtInput->GetXaxis())->FindBin(myW_p4.pt());
  // Get projection in the W pt bin
  TH1D* histCaloMET_binWpt = hCaloMETvsVBPtInput->ProjectionY("_pbinWpt",binWpt,binWpt);
  double pt_mean = histCaloMET_binWpt->GetMean();
  double pt_sigma = histCaloMET_binWpt->GetRMS();
  // Shoot random CaloMET from the Z histograms
  //double dataCaloMET = (sampleFromHistos_)?(histCaloMET_binWpt->GetRandom()):(FSRandom->gaussShoot(pt_mean,pt_sigma));
  double dataCaloMET = (sampleFromHistos_)?(histCaloMET_binWpt->GetRandom()):(fRandomGenerator->fire(pt_mean,pt_sigma));
  LogTrace("") << "\t... W pt, bin in CaloMET dist.= " << myW_p4.pt() << " , " << binWpt;
  LogTrace("") << "\t... CaloMET from Z events= " << dataCaloMET;

  // Using 2D CaloMETPhi vs Z phi 
  // Find bin corresponding to W phi
  int binWphi = (hCaloMETPhivsVBPhiInput->GetXaxis())->FindBin(myW_p4.phi());
  // Get projection in phi bin
  TH1D* histCaloMETPhi_binWphi = hCaloMETPhivsVBPhiInput->ProjectionY("_pbinWphi",binWphi,binWphi);
  double phi_mean2D = histCaloMETPhi_binWphi->GetMean();
  double phi_sigma2D = histCaloMETPhi_binWphi->GetRMS();
  // For 1D CaloMETPhi - Z phi
  double phi_mean1D = hCaloMETPhiMinusVBPhiInput->GetMean();
  double phi_sigma1D = hCaloMETPhiMinusVBPhiInput->GetRMS();
  // Shoot random CaloMET Phi from histos
  double phi_mean = (use1DCaloMETPhisampling_)?phi_mean1D:phi_mean2D;
  double phi_sigma = (use1DCaloMETPhisampling_)?phi_sigma1D:phi_sigma2D;
  double phiFromHistos = (use1DCaloMETPhisampling_)?(myW_p4.phi() + hCaloMETPhiMinusVBPhiInput->GetRandom()):(histCaloMETPhi_binWphi->GetRandom()); 
  double phiRandom = (use1DCaloMETPhisampling_)?(myW_p4.phi() + fRandomGenerator->fire(phi_mean,phi_sigma)):(fRandomGenerator->fire(phi_mean,phi_sigma));
  //double dataPhi = (sampleFromHistos_)?phiFromHistos:(FSRandom->gaussShoot(phi_mean,phi_sigma));
  double dataPhi = (sampleFromHistos_)?phiFromHistos:phiRandom;
  dataPhi = (dataPhi < -M_PI)?(dataPhi + 2*M_PI):(dataPhi);
  dataPhi = (dataPhi > M_PI)?(dataPhi - 2*M_PI):(dataPhi);
  LogTrace("") << "\t... W phi, bin in CaloMET Phi dist.= " << myW_p4.phi() << " , " << binWphi;
  LogTrace("") << "\t... CaloMET Phi from Z events= " << dataPhi;

  double scale = dataCaloMET/myW_p4.pt();
  double dataMEx = (sampleCaloMETPhi_)?(dataCaloMET*cos(dataPhi)):(scale*myW_p4.px());
  dataMEx -= muon->px();
  double dataMEy = (sampleCaloMETPhi_)?(dataCaloMET*sin(dataPhi)):(scale*myW_p4.py());
  dataMEy -= muon->py();
  double dataMET = sqrt(dataMEx*dataMEx + dataMEy*dataMEy); 
 	
  double w_et_sampled = muon->pt() + dataMET;
  double w_px_sampled = muon->px() + dataMEx;
  double w_py_sampled = muon->py() + dataMEy;
  double massT_sampled = w_et_sampled*w_et_sampled - w_px_sampled*w_px_sampled - w_py_sampled*w_py_sampled;
  massT_sampled = (massT_sampled>0) ? sqrt(massT_sampled) : 0;
  LogTrace("") << "\t... W_pt, W_px, W_py= " << sqrt(w_px_sampled*w_px_sampled + w_py_sampled*w_py_sampled) << ", " << w_px_sampled << ", " << w_py_sampled << " GeV";
  LogTrace("") << "\t... Invariant transverse mass= " << massT_sampled << " GeV";

  tree_MTfromZmumu = massT_sampled;
  tree_METfromZmumu = dataMET;
  tree_SampledCaloMET = dataCaloMET;
  tree_SampledCaloMETPhi = dataPhi;

  // Use parallel/perpendicular components. Parallel as a funciont of the boson pt and perpendicular not
  // Find bin corresponding to W pt
  int binWpt_paral = (hCaloMETParalvsVBPtInput->GetXaxis())->FindBin(myW_p4.pt());
  // Get projection in the W pt bin
  TH1D* histCaloMETParal_binWpt = hCaloMETParalvsVBPtInput->ProjectionY("_pbinWpt_paral",binWpt_paral,binWpt_paral);
  double pt_mean_paral = histCaloMETParal_binWpt->GetMean();
  double pt_sigma_paral = histCaloMETParal_binWpt->GetRMS();
  // Shoot random CaloMET (parallel) from the Z histograms
  double dataCaloMETParal = (sampleFromHistos_)?(histCaloMETParal_binWpt->GetRandom()):(fRandomGenerator->fire(pt_mean_paral,pt_sigma_paral));
  LogTrace("") << "\t... W pt, bin in CaloMET (parallel) dist.= " << myW_p4.pt() << " , " << binWpt_paral;
  LogTrace("") << "\t... CaloMET parallel from Z events= " << dataCaloMETParal;

  // Shoot perpendicular component
  double pt_mean_perp = hCaloMETPerpInput->GetMean();
  double pt_sigma_perp = hCaloMETPerpInput->GetRMS();
  double dataCaloMETPerp = (sampleFromHistos_)?(hCaloMETPerpInput->GetRandom()):(fRandomGenerator->fire(pt_mean_perp,pt_sigma_perp));
  LogTrace("") << "\t... CaloMET perpendicular from Z events= " << dataCaloMETPerp;

  // Rotate back from system in which the boson is in the x axis
  double cosW = myW_p4.px()/myW_p4.pt();
  double sinW = myW_p4.py()/myW_p4.pt();

  double dataCaloMEx_v2 = cosW*dataCaloMETParal - sinW*dataCaloMETPerp;
  double dataCaloMEy_v2 = sinW*dataCaloMETParal + cosW*dataCaloMETPerp;
  double dataCaloMET_v2 = sqrt(dataCaloMEx_v2*dataCaloMEx_v2 + dataCaloMEy_v2*dataCaloMEy_v2);
  LogTrace("") << "\t... CaloMEx, CaloMEy, CaloMET from Z events= " << dataCaloMEx_v2 << " , " << dataCaloMEy_v2 << " , " << dataCaloMET_v2;
  
  double dataMEx_v2 = dataCaloMEx_v2 - muon->px();
  double dataMEy_v2 = dataCaloMEy_v2 - muon->py();
  double dataMET_v2 = sqrt(dataMEx_v2*dataMEx_v2 + dataMEy_v2*dataMEy_v2);
 
  double w_et_v2 = muon->pt() + dataMET_v2;
  double w_px_v2 = muon->px() + dataMEx_v2;
  double w_py_v2 = muon->py() + dataMEy_v2;
  double massT_v2 = w_et_v2*w_et_v2 - w_px_v2*w_px_v2 - w_py_v2*w_py_v2;
  massT_v2 = (massT_v2>0) ? sqrt(massT_v2) : 0;
  LogTrace("") << "\t... W_pt, W_px, W_py= " << sqrt(w_px_v2*w_px_v2 + w_py_v2*w_py_v2) << ", " << w_px_v2 << ", " << w_py_v2 << " GeV";
  LogTrace("") << "\t... Invariant transverse mass= " << massT_v2 << " GeV";   

  tree_MTfromZmumu_v2 = massT_v2;
  tree_METfromZmumu_v2 = dataMET_v2;
  tree_SampledCaloMET_v2 = dataCaloMET_v2;
  tree_SampledCaloMEx_v2 = dataCaloMEx_v2;
  tree_SampledCaloMEy_v2 = dataCaloMEy_v2;
  tree_SampledCaloMETParal = dataCaloMETParal;
  tree_SampledCaloMETPerp = dataCaloMETPerp;

  data->Fill();
}

DEFINE_FWK_MODULE(WMETType2Analyzer);
