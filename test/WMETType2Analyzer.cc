/** \class WMETAnalyzer
 *
 *  \author J. Alcaraz
 */

////////// Header section /////////////////////////////////////////////
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/RandGauss.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TProfile.h"

class RandomEngine;
class FML3PtSmearer;

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
  edm::InputTag theGeneratorLabel;
  edm::InputTag theMuonCollectionLabel;
  edm::InputTag theGenMETCollectionLabel;
  edm::InputTag theCaloMETCollectionLabel;

  std::string theInputRootFileName;
  std::string theOutputRootFileName;

  bool useMuonCorrMET;

  // The input/output Root file
  TFile* theInputFile;
  TFile* theOutputFile;

  //Nu cut
  bool applyNuCut_;
  double nuPtCut_;
  double nuEtaCut_;

  //Nu smearing
  bool applyNuPtSmearing_;

  //Sample CaloMET phi
  bool sampleCaloMETPhi_;

  //Sample from histos or gaussian
  bool sampleFromHistos_;

  // Histograms
  TH1F* hMTInput;
  TH1F* hMETInput;
  TH1F* hPtNuInput;
  TH1F* hEtaNuInput;
  TH2F* hPtNuvsEtaNuInput;
  TH2F* hMTvsPtNuInput;
  TH2F* hMETvsPtNuInput;
  TH3F* hMTvsPtNuvsEtaNuInput;
  TH3F* hMETvsPtNuvsEtaNuInput;

  TH1F* hCaloMETInput;
  TH1F* hCaloMETPhiInput;
  TH1F* hVBPtInput;
  TH1F* hVBEtaInput;
  TH1F* hVBPhiInput;
  TH2F* hCaloMETvsVBPtInput;
  TH2F* hCaloMETPhivsVBPhiInput;
  TH2F* hMTvsVBPtInput;
  TH2F* hMETvsVBPtInput;
  	
  TH1F* hMT;
  //TH1F* hMTSmeared;
  TH1F* hMET;
  TH1F* hPtNu;
  //TH1F* hPtNuSmeared;
  TH1F* hEtaNu;	
  TH2F* hPtNuvsEtaNu;
  TH2F* hMTvsPtNu;
  TH2F* hMETvsPtNu;
  TH3F* hMTvsPtNuvsEtaNu;
  TH3F* hMETvsPtNuvsEtaNu;

  TH1F* hGenMET;
  TH1F* hGenMT;
  //TH2F* hGenMTvsPtNu;
  //TH2F* hGenMETvsPtNu;
  //TH3F* hGenMTvsPtNuvsEtaNu;
  //TH3F* hGenMETvsPtNuvsEtaNu;

  TH1F* hCaloMET;
  TH1F* hCaloMETPhi;
  TH1F* hVBPt;
  TH1F* hVBEta;
  TH1F* hVBPhi;
  TH2F* hCaloMETvsVBPt;
  TH2F* hCaloMETPhivsVBPhi;
  TH2F* hMTvsVBPt;
  TH2F* hMETvsVBPt;

  //TH1F* hVBPtRew;

  TH1F* hMTfromZmumu;
  TH1F* hMETfromZmumu;
  TH2F* hMTfromZmumuvsVBPt;
  TH2F* hMETfromZmumuvsVBPt;

  TH1F* hSampledCaloMET;
  TH1F* hSampledCaloMETPhi;

  TH2F* hCaloMEToverVBPtvsCaloMETInput;
  TH2F* hCaloMEToverVBPtvsCaloMET;
  TH1F* hMETCorrScale;
  TH2F* hCaloMETCorroverVBPtvsCaloMETCorr;

  //Random number engine/generator
  CLHEP::HepRandomEngine* fRandomEngine;
  CLHEP::RandGauss*       fRandomGenerator;  

  RandomEngine* FSRandom;
  //FM Pt Smearer
  FML3PtSmearer * myPtSmearer;
};

////////// Source code ////////////////////////////////////////////////
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

#include "FastSimulation/Utilities/interface/RandomEngine.h"
#include "FastSimulation/ParamL3MuonProducer/interface/FML3PtSmearer.h"

using namespace std;
using namespace edm;
using namespace reco;

/// Constructor
WMETType2Analyzer::WMETType2Analyzer(const ParameterSet& pset)
{
  theGeneratorLabel = pset.getUntrackedParameter<InputTag>("GeneratorCollectionLabel");
  theMuonCollectionLabel = pset.getUntrackedParameter<InputTag>("MuonCollectionLabel");
  theMuonCollectionLabel = edm::InputTag(theMuonCollectionLabel.label(),"WMuons");
  theGenMETCollectionLabel = pset.getUntrackedParameter<InputTag>("GenMETCollectionLabel");
  theCaloMETCollectionLabel = pset.getUntrackedParameter<InputTag>("CaloMETCollectionLabel");

  useMuonCorrMET = pset.getUntrackedParameter<bool>("UseMuonCorrectedMET",false);

  theInputRootFileName = pset.getUntrackedParameter<string>("InputRootFileName");
  theOutputRootFileName = pset.getUntrackedParameter<string>("OutputRootFileName");

  applyNuCut_ = pset.getUntrackedParameter<bool>("ApplyNuCut",false);
  nuPtCut_ = pset.getUntrackedParameter<double>("NuPtCut",0.0);
  nuEtaCut_ = pset.getUntrackedParameter<double>("NuEtaCut",5.0);

  applyNuPtSmearing_ = pset.getUntrackedParameter<bool>("ApplyNuPtSmearing",false);

  sampleCaloMETPhi_ = pset.getUntrackedParameter<bool>("SampleCaloMETPhi",false);

  sampleFromHistos_ = pset.getUntrackedParameter<bool>("SampleFromHistos",true);
}

/// Destructor
WMETType2Analyzer::~WMETType2Analyzer(){
}

void WMETType2Analyzer::beginJob(const EventSetup& eventSetup){
  // Create the root file
  theInputFile = new TFile(theInputRootFileName.c_str(), "READONLY");
  theInputFile->cd();

  hMTInput = (TH1F*)theInputFile->Get("hMT");
  hMETInput = (TH1F*)theInputFile->Get("hMET"); 
  hPtNuInput = (TH1F*)theInputFile->Get("hPtNu"); 
  hEtaNuInput = (TH1F*)theInputFile->Get("hEtaNu");
  hPtNuvsEtaNuInput = (TH2F*)theInputFile->Get("hPtNuvsEtaNu");
  hMTvsPtNuInput = (TH2F*)theInputFile->Get("hMTvsPtNu");
  hMETvsPtNuInput = (TH2F*)theInputFile->Get("hMETvsPtNu"); 
  hMTvsPtNuvsEtaNuInput = (TH3F*)theInputFile->Get("hMTvsPtNuvsEtaNu");
  hMETvsPtNuvsEtaNuInput = (TH3F*)theInputFile->Get("hMETvsPtNuvsEtaNu");

  hCaloMETInput = (TH1F*)theInputFile->Get("hCaloMET");
  hCaloMETPhiInput = (TH1F*)theInputFile->Get("hCaloMETPhi");
  hVBPtInput = (TH1F*)theInputFile->Get("hVBPt");
  hVBEtaInput = (TH1F*)theInputFile->Get("hVBEta");
  hVBPhiInput = (TH1F*)theInputFile->Get("hVBPhi");
  hCaloMETvsVBPtInput = (TH2F*)theInputFile->Get("hCaloMETvsVBPt");
  hCaloMETPhivsVBPhiInput = (TH2F*)theInputFile->Get("hCaloMETPhivsVBPhi");
  hMTvsVBPtInput = (TH2F*)theInputFile->Get("hMTvsVBPt");
  hMETvsVBPtInput = (TH2F*)theInputFile->Get("hMETvsVBPt");

  hCaloMEToverVBPtvsCaloMETInput = (TH2F*)theInputFile->Get("hCaloMEToverVBPtvsCaloMET");

  hMTInput->SetName("hMTInput");
  hMETInput->SetName("hMETInput");
  hPtNuInput->SetName("hPtNuInput");
  hEtaNuInput->SetName("hEtaNuInput");
  hPtNuvsEtaNuInput->SetName("hPtNuvsEtaNuInput");
  hMTvsPtNuInput->SetName("hMTvsPtNuInput");
  hMETvsPtNuInput->SetName("hMETvsPtNuInput");
  hMTvsPtNuvsEtaNuInput->SetName("hMTvsPtNuvsEtaNuInput");
  hMETvsPtNuvsEtaNuInput->SetName("hMETvsPtNuvsEtaNuInput");

  hCaloMETInput->SetName("hCaloMETInput");
  hCaloMETPhiInput->SetName("hCaloMETPhiInput");
  hVBPtInput->SetName("hVBPtInput");
  hVBEtaInput->SetName("hVBEtaInput");
  hVBPhiInput->SetName("hVBPhiInput");
  hCaloMETvsVBPtInput->SetName("hCaloMETvsVBPtInput");
  hCaloMETPhivsVBPhiInput->SetName("hCaloMETPhivsVBPhiInput");
  hMTvsVBPtInput->SetName("hMTvsVBPtInput");
  hMETvsVBPtInput->SetName("hMETvsVBPtInput");

  hCaloMEToverVBPtvsCaloMETInput->SetName("hCaloMEToverVBPtvsCaloMETInput");

  hMT = (TH1F*)hMTInput->Clone("hMT");
  hMET = (TH1F*)hMETInput->Clone("hMET"); 
  hPtNu = (TH1F*)hPtNuInput->Clone("hPtNu"); 
  hEtaNu = (TH1F*)hEtaNuInput->Clone("hEtaNu");	
  //hPtNuSmeared = (TH1F*)hPtNuInput->Clone("hPtNuSmeared");
  hPtNuvsEtaNu = (TH2F*)hPtNuvsEtaNuInput->Clone("hPtNuvsEtaNu");
  hMTvsPtNu = (TH2F*)hMTvsPtNuInput->Clone("hMTvsPtNu");
  hMETvsPtNu = (TH2F*)hMETvsPtNuInput->Clone("hMETvsPtNu"); 
  hMTvsPtNuvsEtaNu = (TH3F*)hMTvsPtNuvsEtaNuInput->Clone("hMTvsPtNuvsEtaNu");
  hMETvsPtNuvsEtaNu = (TH3F*)hMETvsPtNuvsEtaNuInput->Clone("hMETvsPtNuvsEtaNu");

  hGenMET = (TH1F*)hMETInput->Clone("hGenMET");
  hGenMT = (TH1F*)hMTInput->Clone("hGenMT");
  //hMTSmeared = (TH1F*)hMTInput->Clone("hMTSmeared");
  hCaloMET = (TH1F*)hCaloMETInput->Clone("hCaloMET");
  hCaloMETPhi = (TH1F*)hCaloMETPhiInput->Clone("hCaloMETPhi");
  hVBPt = (TH1F*)hVBPtInput->Clone("hVBPt");
  hVBEta = (TH1F*)hVBEtaInput->Clone("hVBEta");
  hVBPhi = (TH1F*)hVBPhiInput->Clone("hVBPhi");
  hCaloMETvsVBPt = (TH2F*)hCaloMETvsVBPtInput->Clone("hCaloMETvsVBPt");
  hCaloMETPhivsVBPhi = (TH2F*)hCaloMETPhivsVBPhiInput->Clone("hCaloMETPhivsVBPhi");
  hMTvsVBPt = (TH2F*)hMTvsVBPtInput->Clone("hMTvsVBPt");
  hMETvsVBPt = (TH2F*)hMETvsVBPtInput->Clone("hMETvsVBPt");

  //hVBPtRew = (TH1F*)hVBPtInput->Clone("hVBPtRew");

  hMTfromZmumu = (TH1F*)hMTInput->Clone("hMTfromZmumu");
  hMETfromZmumu = (TH1F*)hMETInput->Clone("hMETfromZmumu");
  hMTfromZmumuvsVBPt = (TH2F*)hMTvsVBPtInput->Clone("hMTfromZmumuvsVBPt");
  hMETfromZmumuvsVBPt = (TH2F*)hMETvsVBPtInput->Clone("hMETfromZmumuvsVBPt");

  hSampledCaloMET = (TH1F*)hCaloMETInput->Clone("hSampledCaloMET");
  hSampledCaloMETPhi = (TH1F*)hCaloMETPhiInput->Clone("hSampledCaloMETPhi");

  hMETCorrScale = (TH1F*)hMETInput->Clone("hMETCorrScale");

  hCaloMEToverVBPtvsCaloMET = (TH2F*)hCaloMEToverVBPtvsCaloMETInput->Clone("hCaloMEToverVBPtvsCaloMET");
  hCaloMETCorroverVBPtvsCaloMETCorr = (TH2F*)hCaloMEToverVBPtvsCaloMETInput->Clone("hCaloMETCorroverVBPtvsCaloMETCorr");

  hMT->Reset();
  hMET->Reset();
  hPtNu->Reset();
  hEtaNu->Reset();
  //hPtNuSmeared->Reset();
  hPtNuvsEtaNu->Reset();
  hMTvsPtNu->Reset();
  hMETvsPtNu->Reset();
  hMTvsPtNuvsEtaNu->Reset();
  hMETvsPtNuvsEtaNu->Reset();

  hGenMET->Reset();
  hGenMT->Reset();
  //hMTSmeared->Reset();
  hCaloMET->Reset();
  hCaloMETPhi->Reset();
  hVBPt->Reset();
  hVBEta->Reset();
  hVBPhi->Reset();
  hCaloMETvsVBPt->Reset();
  hCaloMETPhivsVBPhi->Reset();
  hMTvsVBPt->Reset();
  hMETvsVBPt->Reset();

  //hVBPtRew->Reset();

  hMTfromZmumu->Reset();
  hMETfromZmumu->Reset();
  hMTfromZmumuvsVBPt->Reset();
  hMETfromZmumuvsVBPt->Reset();

  hSampledCaloMET->Reset();
  hSampledCaloMETPhi->Reset();

  hMETCorrScale->Reset();

  hCaloMEToverVBPtvsCaloMET->Reset();
  hCaloMETCorroverVBPtvsCaloMETCorr->Reset();

  theOutputFile = new TFile(theOutputRootFileName.c_str(), "RECREATE");
  theOutputFile->cd();

  //Initialize random generator
  /*Service<RandomNumberGenerator> rng;
  long seed = (long)(rng->mySeed());
  LogTrace("") << ">>>> Seed= " << seed;
  fRandomEngine = new CLHEP::HepJamesRandom(seed);
  fRandomGenerator = new CLHEP::RandGauss(fRandomEngine);
  LogTrace("") << ">>>> Random Generator Initialized";*/
  
  // Initialize the random number generator service
  edm::Service<edm::RandomNumberGenerator> rng;
  if ( ! rng.isAvailable() ) {
    throw cms::Exception("Configuration") <<
      "No RandomGeneratorService!!! \n"
      "You must add the service in the configuration file\n"
      "or remove the module that requires it.";
  }

  FSRandom = new RandomEngine(&(*rng));
  myPtSmearer = new FML3PtSmearer(FSRandom);
}

void WMETType2Analyzer::endJob(){
  theOutputFile->cd();
  LogInfo("WMETType2Analyzer") << " Number of events used= " << hMT->GetEntries();

  // Write the histos to file
  /*hMTInput->Write();
  hMETInput->Write();
  hPtNuInput->Write();
  hMTvsPtNuInput->Write();
  hMETvsPtNuInput->Write();*/
 
  hMT->Write();
  hMET->Write();
  hPtNu->Write();
  hEtaNu->Write();
  //hPtNuSmeared->Write();
  hPtNuvsEtaNu->Write();
  hMTvsPtNu->Write();
  hMETvsPtNu->Write();
  hMTvsPtNuvsEtaNu->Write();
  hMETvsPtNuvsEtaNu->Write();

  /*hCaloMETInput->Write();
  hVBPtInput->Write(); 
  hCaloMETvsVBPtInput->Write();
  hCaloMETPhivsVBPhiInput->Write();
  hMTvsVBPtInput->Write();
  hMETvsVBPtInput->Write();*/

  hGenMET->Write();
  hGenMT->Write();
  //hMTSmeared->Write();
  hCaloMET->Write();
  hCaloMETPhi->Write();
  hVBPt->Write();
  hVBEta->Write();
  hVBPhi->Write();
  hCaloMETvsVBPt->Write();
  hCaloMETPhivsVBPhi->Write();
  hMTvsVBPt->Write();
  hMETvsVBPt->Write();

  hMTfromZmumu->Write();
  hMETfromZmumu->Write();
  hMTfromZmumuvsVBPt->Write();
  hMETfromZmumuvsVBPt->Write();

  hSampledCaloMET->Write();
  hSampledCaloMETPhi->Write();

  hMETCorrScale->Write();

  hCaloMEToverVBPtvsCaloMET->Write();
  hCaloMETCorroverVBPtvsCaloMETCorr->Write();

  theOutputFile->Close();
}

void WMETType2Analyzer::analyze(const Event & ev, const EventSetup&){
  
  theOutputFile->cd();

  double met_px = 0.;
  double met_py = 0.;

  /*Handle<TrackCollection> muonCollection;
  ev.getByLabel(theMuonCollectionLabel, muonCollection);
  int nmuons = muonCollection->size();
  if (nmuons!=1) return;*/

  Handle<CandidateCollection> muonCollection;
  ev.getByLabel(theMuonCollectionLabel, muonCollection);
  assert(muonCollection->size() > 0);

  /*TrackRef mu(muonCollection,0);
  if (mu->pt()<25.) return;
  if (fabs(mu->eta())>2.0) return;
  met_px -= mu->px();
  met_py -= mu->py();*/

  const Candidate* mu = &(*muonCollection)[0];
  if(!useMuonCorrMET){
	met_px -= mu->px();
	met_py -= mu->py();
  }

  //=================================================================  
  // PT of neutrino
  //double ptnu = -1.;
  //double etanu = 0.;
  //double pxnu,pynu;
  const Candidate* genW = 0;
  const Candidate* genMuon = 0;
  const Candidate* genNeutrino = 0;	
  //Candidate::LorentzVector myMup4;
  //Candidate::LorentzVector myNup4;
  Handle<CandidateCollection> genParticles;
  ev.getByLabel(theGeneratorLabel, genParticles);
  int pid_ids[2] = {13,14};
  for(int j = 0; j < 2; j++){	
    	for( unsigned int i = 0; i < genParticles->size(); ++ i ) {
      		const Candidate & genpart = (*genParticles)[i];
      		//LogTrace("") << ">>>>>>> pid,status,px,py,px,e= "  << genpart.pdgId() << " , " << genpart.status() << " , " << genpart.px() << " , " << genpart.py() << " , " << genpart.pz() << " , " << genpart.energy();	
      		int id1 = genpart.pdgId();

      		if (applyNuCut_&&(genpart.pt()<nuPtCut_)) continue;
      		if (applyNuCut_&&(fabs(genpart.eta())>nuEtaCut_)) continue;	
		if (abs(id1)!=pid_ids[j]) continue;
		const Candidate* mother = genpart.mother();
                int idmother = mother->pdgId();
                if (fabs(idmother)!=24 && idmother!=24) continue; 

      		genW = mother;
		if(pid_ids[j] == 13) genMuon = &(*genParticles)[i];
		else if(pid_ids[j] == 14) genNeutrino = &(*genParticles)[i];

      		break;
    	}
  }
  if (genW == 0) {LogTrace("") << ">>>>>>> No generated W found...skipping event";return;}		
  if (genNeutrino == 0) {LogTrace("") << ">>>>>>> No generated neutrino found...skipping event";return;}
  if (genMuon == 0) {LogTrace("") << ">>>>>>> No generated muon found...skipping event";return;}
  //const Candidate & genmuon = (*genParticles)[myMuindex];
  //const Candidate & genneutrino = (*genParticles)[myNuindex];
  Candidate::LorentzVector genMu_p4 = genMuon->p4();
  Candidate::LorentzVector genNu_p4 = genNeutrino->p4();
  /*ptnu = genneutrino.pt();
  etanu = genneutrino.eta();
  pxnu = genneutrino.px();
  pynu = genneutrino.py();*/	 
  //assert(ptnu >= 0.);
  LogTrace("") << ">>>> Gen neutrino px,py,pz,e= " << genNu_p4.px() << " , " << genNu_p4.py() << " , " << genNu_p4.pz() << " , " << genNu_p4.energy();
  LogTrace("") << ">>>> Gen muon px,py,pz,e= " << genMu_p4.px() << " , " << genMu_p4.py() << " , " << genMu_p4.pz() << " , " << genMu_p4.energy();

  //=================================================================
  //Apply smearing
  math::XYZTLorentzVector MuonFromNu;	
  if(applyNuPtSmearing_){	
  	math::XYZVector recP3(genNu_p4.px(),genNu_p4.py(),genNu_p4.pz());
  	MuonFromNu = myPtSmearer->smear(genNu_p4,recP3);
  	LogTrace("") << ">>> Smeared neutrino px,py,pz,e= " << MuonFromNu.px() << " , " << MuonFromNu.py() << " , " << MuonFromNu.pz() << " , " << MuonFromNu.energy();
  } else MuonFromNu = genNu_p4;

  //hPtNuSmeared->Fill(MuonFromNu.pt());

  //=================================================================
  Handle<GenMETCollection> genMETCollection;
  ev.getByLabel(theGenMETCollectionLabel, genMETCollection);
  GenMETCollection::const_iterator genMET = genMETCollection->begin();
  LogTrace("") << ">>> GenMET_et, GenMET_px, GenMET_py= " << genMET->pt() << ", " << genMET->px() << ", " << genMET->py();

  Handle<CaloMETCollection> caloMETCollection;
  ev.getByLabel(theCaloMETCollectionLabel, caloMETCollection);
  CaloMETCollection::const_iterator caloMET = caloMETCollection->begin();
  LogTrace("") << ">>> CaloMET_et, CaloMET_px, CaloMET_py= " << caloMET->pt() << ", " << caloMET->px() << ", " << caloMET->py();

  met_px += caloMET->px();
  met_py += caloMET->py();
  double met_et = sqrt(met_px*met_px+met_py*met_py);
  LogTrace("") << ">>> MET_et, MET_py, MET_py= " << met_et << ", " << met_px << ", " << met_py;

  double myCaloMEx = caloMET->px();
  double myCaloMEy = caloMET->py();
  if(useMuonCorrMET){
        myCaloMEx += mu->px();
        myCaloMEy += mu->py();
  }
  double myCaloMET = sqrt(myCaloMEx*myCaloMEx + myCaloMEy*myCaloMEy);
  math::XYZTLorentzVector recCaloMET(myCaloMEx,myCaloMEy,0.,myCaloMET);

  double w_et = mu->pt() + met_et;
  double w_px = mu->px() + met_px;
  double w_py = mu->py() + met_py;
  double massT = w_et*w_et - w_px*w_px - w_py*w_py;
  massT = (massT>0) ? sqrt(massT) : 0;
  LogTrace("") << "\t... W_et, W_px, W_py= " << w_et << ", " << w_px << ", " << w_py << " GeV";
  LogTrace("") << "\t... Invariant transverse mass= " << massT << " GeV";

  //hCaloMET->Fill(caloMET->pt());
  hCaloMET->Fill(recCaloMET.pt());
  hCaloMETPhi->Fill(recCaloMET.phi());

  hMT->Fill(massT);
  hMET->Fill(met_et);
  /*hPtNu->Fill(ptnu);
  hEtaNu->Fill(fabs(etanu));
  hPtNuvsEtaNu->Fill(fabs(etanu),ptnu);
  hMTvsPtNu->Fill(ptnu, massT);
  hMETvsPtNu->Fill(ptnu, met_et);
  hMTvsPtNuvsEtaNu->Fill(ptnu, fabs(etanu), massT);
  hMETvsPtNuvsEtaNu->Fill(ptnu, fabs(etanu), met_et);*/
  hPtNu->Fill(MuonFromNu.pt());
  hEtaNu->Fill(fabs(MuonFromNu.eta()));
  hPtNuvsEtaNu->Fill(fabs(MuonFromNu.eta()),MuonFromNu.pt());
  hMTvsPtNu->Fill(MuonFromNu.pt(), massT);
  hMETvsPtNu->Fill(MuonFromNu.pt(), met_et);
  hMTvsPtNuvsEtaNu->Fill(MuonFromNu.pt(), fabs(MuonFromNu.eta()), massT);
  hMETvsPtNuvsEtaNu->Fill(MuonFromNu.pt(), fabs(MuonFromNu.eta()), met_et);

  //=================================================================
  //Gen MT
  double w_et_gen = genMu_p4.pt() + genNu_p4.pt();
  double w_px_gen = genMu_p4.px() + genNu_p4.px();
  double w_py_gen = genMu_p4.py() + genNu_p4.py();
  double massT_gen = w_et_gen*w_et_gen - w_px_gen*w_px_gen - w_py_gen*w_py_gen;
  massT_gen = (massT_gen>0) ? sqrt(massT_gen) : 0;
  hGenMT->Fill(massT_gen);
  hGenMET->Fill(genMET->pt());

  //=================================================================
  //Scale correction
  //int binCaloMET = (hCaloMEToverVBPtvsCaloMETInput->GetXaxis())->FindBin(caloMET->pt());
  int binCaloMET = (hCaloMEToverVBPtvsCaloMETInput->GetXaxis())->FindBin(recCaloMET.pt());
  double Zscale = (hCaloMEToverVBPtvsCaloMETInput->ProfileX())->GetBinContent(binCaloMET);
  //double corrCaloMEx = caloMET->px()/Zscale;
  //double corrCaloMEy = caloMET->py()/Zscale;
  double corrCaloMEx = recCaloMET.px()/Zscale;
  double corrCaloMEy = recCaloMET.py()/Zscale;
  double corrCaloMET = sqrt(corrCaloMEx*corrCaloMEx + corrCaloMEy*corrCaloMEy);
  double corrMEx = corrCaloMEx - mu->px();
  double corrMEy = corrCaloMEy - mu->py();
  double corrMET = sqrt(corrMEx*corrMEx + corrMEy*corrMEy);
  LogTrace("") << ">>> Z Scale= " << Zscale;
  LogTrace("") << ">>> Corr MET_et, Corr MET_px, Corr MET_py= " << corrMET << ", " << corrMEx << ", " << corrMEy;

  w_et = mu->pt() + corrMET;
  w_px = mu->px() + corrMEx;
  w_py = mu->py() + corrMEy;
  massT = w_et*w_et - w_px*w_px - w_py*w_py;
  massT = (massT>0) ? sqrt(massT) : 0;
  LogTrace("") << "\t... W_et, W_px, W_py= " << w_et << ", " << w_px << ", " << w_py << " GeV";
  LogTrace("") << "\t... Invariant transverse mass= " << massT << " GeV";

  hMETCorrScale->Fill(corrMET);

  /*
  //=================================================================
  //Apply smearing
  math::XYZVector recP3(myNup4.px(),myNup4.py(),myNup4.pz());
  math::XYZTLorentzVector MuonFromNu = myPtSmearer->smear(myNup4,recP3);
  LogTrace("") << ">>> Smeared neutrino px,py,pz,e= " << MuonFromNu.px() << " , " << MuonFromNu.py() << " , " << MuonFromNu.pz() << " , " << MuonFromNu.energy();

  //hPtNuSmeared->Fill(MuonFromNu.pt());
  */

  //=================================================================
  /*double pxW = mu->px() + pxnu;
  double pyW = mu->py() + pynu;
  double ptW = sqrt(pxW*pxW + pyW*pyW);*/

  double pxW = mu->px() + MuonFromNu.px();
  double pyW = mu->py() + MuonFromNu.py();
  double pzW = mu->pz() + MuonFromNu.pz();
  double ptW = sqrt(pxW*pxW + pyW*pyW);
  math::XYZVector p3W((mu->px() + MuonFromNu.px()),(mu->py() + MuonFromNu.py()),(mu->pz() + MuonFromNu.pz()));

  /*double pxW = mu->px() + myNup4.px();
  double pyW = mu->py() + myNup4.py();
  double pzW = mu->pz() + myNup4.pz();
  double ptW = sqrt(pxW*pxW + pyW*pyW);
  math::XYZVector p3W((mu->px() + myNup4.px()),(mu->py() + myNup4.py()),(mu->pz() + myNup4.pz()));*/
  double etaW = p3W.eta();
  double phiW = p3W.phi();
	
  hVBPt->Fill(ptW);
  hVBEta->Fill(etaW);
  hVBPhi->Fill(phiW);
  //hCaloMETvsVBPt->Fill(ptW, caloMET->pt());
  //hCaloMETPhivsVBPhi->Fill(phiW, caloMET->phi());
  hCaloMETvsVBPt->Fill(ptW, recCaloMET.pt());
  hCaloMETPhivsVBPhi->Fill(phiW, recCaloMET.phi());
  hMTvsVBPt->Fill(ptW, massT);
  hMETvsVBPt->Fill(ptW, met_et);

  //hCaloMEToverVBPtvsCaloMET->Fill(caloMET->pt(),caloMET->pt()/ptW);
  hCaloMEToverVBPtvsCaloMET->Fill(recCaloMET.pt(),recCaloMET.pt()/ptW);
  hCaloMETCorroverVBPtvsCaloMETCorr->Fill(corrCaloMET,corrCaloMET/ptW);

  w_et = mu->pt() + MuonFromNu.pt();
  w_px = mu->px() + MuonFromNu.px();
  w_py = mu->py() + MuonFromNu.py();
  massT = w_et*w_et - w_px*w_px - w_py*w_py;
  massT = (massT>0) ? sqrt(massT) : 0;
  LogTrace("") << "\t... W_et, W_px, W_py= " << w_et << ", " << w_px << ", " << w_py << " GeV";
  LogTrace("") << "\t... Invariant transverse mass= " << massT << " GeV";

  //hMTSmeared->Fill(massT);

  //================================================================= 
  //Get MET from Z(data) sample
  /*int binWpt = (hMETvsVBPtInput->GetXaxis())->FindBin(ptW);
  double dataMET = (hMETvsVBPtInput->ProfileX())->GetBinContent(binWpt);

  double scale = dataMET/met_et;
  double w_et = mu->pt() + scale*met_et;
  double w_px = mu->px() + scale*met_px;
  double w_py = mu->py() + scale*met_py;*/

  /*int binWpt = (hCaloMETvsVBPtInput->GetXaxis())->FindBin(ptW);
  double dataMET = (hCaloMETvsVBPtInput->ProfileX())->GetBinContent(binWpt);*/

  int binWpt = (hCaloMETvsVBPtInput->GetXaxis())->FindBin(ptW);
  TH1D* histCaloMET_binWpt = hCaloMETvsVBPtInput->ProjectionY("_pbinWpt",binWpt,binWpt);
  double pt_mean = histCaloMET_binWpt->GetMean();
  double pt_sigma = histCaloMET_binWpt->GetRMS();
  //double dataMET = fRandomGenerator->fire(pt_mean,pt_sigma);
  //double dataMET = FSRandom->gaussShoot(pt_mean,pt_sigma);
  //double dataCaloMET = histCaloMET_binWpt->GetRandom();	
  double dataCaloMET = (sampleFromHistos_)?(histCaloMET_binWpt->GetRandom()):(FSRandom->gaussShoot(pt_mean,pt_sigma));
  LogTrace("") << "\t... W pt, bin in CaloMET dist.= " << ptW << " , " << binWpt;
  LogTrace("") << "\t... CaloMET from Z events= " << dataCaloMET;

  int binWphi = (hCaloMETPhivsVBPhiInput->GetXaxis())->FindBin(phiW);
  TH1D* histCaloMETPhi_binWphi = hCaloMETPhivsVBPhiInput->ProjectionY("_pbinWphi",binWphi,binWphi);
  double phi_mean = histCaloMETPhi_binWphi->GetMean();
  double phi_sigma = histCaloMETPhi_binWphi->GetRMS();
  //double dataPhi = fRandomGenerator->fire(phi_mean,phi_sigma);
  //double dataPhi = FSRandom->gaussShoot(phi_mean,phi_sigma);
  //double dataPhi = histCaloMETPhi_binWphi->GetRandom();	
  double dataPhi = (sampleFromHistos_)?(histCaloMETPhi_binWphi->GetRandom()):(FSRandom->gaussShoot(phi_mean,phi_sigma));
  LogTrace("") << "\t... W phi, bin in CaloMET Phi dist.= " << phiW << " , " << binWphi;
  LogTrace("") << "\t... CaloMET Phi from Z events= " << dataPhi;

  /*double scale = dataMET/caloMET->pt();
  double dataMEx = scale*caloMET->px();
  dataMEx -= mu->px();
  double dataMEy = scale*caloMET->py();
  dataMEy -= mu->py();
  dataMET = sqrt(dataMEx*dataMEx + dataMEy*dataMEy);*/

  /*double scale = dataCaloMET/ptW;
  double dataMEx = scale*pxW;
  dataMEx -= mu->px();
  double dataMEy = scale*pyW;
  dataMEy -= mu->py();
  double dataMET = sqrt(dataMEx*dataMEx + dataMEy*dataMEy);

  double dataMEx = dataCaloMET*cos(dataPhi);
  dataMEx -= mu->px();
  double dataMEy = dataCaloMET*sin(dataPhi);
  dataMEy -= mu->py();
  double dataMET = sqrt(dataMEx*dataMEx + dataMEy*dataMEy);*/

  double scale = dataCaloMET/ptW;
  double dataMEx = (sampleCaloMETPhi_)?(dataCaloMET*cos(dataPhi)):(scale*pxW);
  dataMEx -= mu->px();
  double dataMEy = (sampleCaloMETPhi_)?(dataCaloMET*sin(dataPhi)):(scale*pyW);
  dataMEy -= mu->py();
  double dataMET = sqrt(dataMEx*dataMEx + dataMEy*dataMEy); 
 	
  w_et = mu->pt() + dataMET;
  w_px = mu->px() + dataMEx;
  w_py = mu->py() + dataMEy;

  massT = w_et*w_et - w_px*w_px - w_py*w_py;
  massT = (massT>0) ? sqrt(massT) : 0;
  LogTrace("") << "\t... W_et, W_px, W_py= " << w_et << ", " << w_px << ", " << w_py << " GeV";
  LogTrace("") << "\t... Invariant transverse mass= " << massT << " GeV";

  hMTfromZmumu->Fill(massT);
  hMETfromZmumu->Fill(dataMET);
  hMTfromZmumuvsVBPt->Fill(ptW, massT);
  hMETfromZmumuvsVBPt->Fill(ptW, dataMET);

  hSampledCaloMET->Fill(dataCaloMET);
  hSampledCaloMETPhi->Fill(dataPhi);

  /*
  //=================================================================
  //Gen MT
  double w_et_gen = myMup4.pt() + myNup4.pt();
  double w_px_gen = myMup4.px() + myNup4.px();
  double w_py_gen = myMup4.py() + myNup4.py();
  double massT_gen = w_et_gen*w_et_gen - w_px_gen*w_px_gen - w_py_gen*w_py_gen;
  massT_gen = (massT_gen>0) ? sqrt(massT_gen) : 0;
  hGenMT->Fill(massT_gen);
  */
}

DEFINE_FWK_MODULE(WMETType2Analyzer);
