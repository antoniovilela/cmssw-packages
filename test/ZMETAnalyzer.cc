/** \class ZMETAnalyzer
 *
 *  \author J. Alcaraz
 */

////////// Header section /////////////////////////////////////////////
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "TFile.h"
#include "TTree.h" 
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"

class ZMETAnalyzer: public edm::EDAnalyzer {
public:
  /// Constructor
  ZMETAnalyzer(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~ZMETAnalyzer();

  // Operations

  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  virtual void beginJob(const edm::EventSetup& eventSetup) ;
  virtual void endJob() ;

private:
  // Input from cfg file
  edm::InputTag theZCollectionLabel;
  edm::InputTag theMETCollectionLabel;

  bool useMuonCorrMET;

  unsigned int theNbinsMT;
  double theMTMin;
  double theMTMax;
  unsigned int theNbinsMET;
  double theMETMin;
  double theMETMax;
  unsigned int theNbinsPtNu;
  double thePtNuMin;
  double thePtNuMax;
  unsigned int theNbinsEtaNu;
  double theEtaNuMin;
  double theEtaNuMax;

  unsigned int theNbinsCaloMET;
  double theCaloMETMin;
  double theCaloMETMax;
  unsigned int theNbinsVBPt;
  double theVBPtMin;
  double theVBPtMax;
  unsigned int theNbinsVBEta;
  double theVBEtaMin;
  double theVBEtaMax;

  unsigned int theNbinsCaloMETPhi;
  double theCaloMETPhiMin;
  double theCaloMETPhiMax; 
  unsigned int theNbinsVBPhi;
  double theVBPhiMin;
  double theVBPhiMax;
  std::string theRootFileName;

  // The output Root file
  TFile* theFile;

  // Output TTree
  TTree* mytree;
  double tree_VBPt;
  double tree_VBEta;
  double tree_VBPhi;
  double tree_NuPt;
  double tree_NuEta;
  double tree_NuPhi;
  double tree_CaloMET;
  double tree_CaloMET_Phi;
  double tree_CaloMEx;
  double tree_CaloMEy;
  double tree_MET;
  double tree_MEx;
  double tree_MEy;
  double tree_MT;
	
  // Histograms
  TH1F* hMT;
  TH1F* hMET;
  TH1F* hPtNu;
  TH1F* hEtaNu;
  TH2F* hPtNuvsEtaNu;
  TH2F* hMTvsPtNu;
  TH2F* hMETvsPtNu; 
  TH2F* hMTvsEtaNu;
  TH2F* hMETvsEtaNu;
  TH3F* hMTvsPtNuvsEtaNu;
  TH3F* hMETvsPtNuvsEtaNu;

  TH1F* hCaloMET;
  TH1F* hCaloMETPhi;
  TH1F* hVBPt;
  TH1F* hVBEta;
  TH1F* hVBPhi;
  TH1F* hresVBPt; 	
  TH2F* hMETvsVBPt;
  TH2F* hMETvsVBEta;
  TH3F* hMETvsVBPtvsVBEta;
  TH2F* hMTvsVBPt;
  TH2F* hMTvsVBEta;
  TH3F* hMTvsVBPtvsVBEta;
  TH2F* hCaloMETvsVBPt;
  TH2F* hCaloMETvsVBEta;
  TH2F* hCaloMETvsVBPhi;
  TH3F* hCaloMETvsVBPtvsVBEta;
  TH2F* hCaloMEToverVBPtvsVBPt;
  TH2F* hCaloMEToverVBPtvsCaloMET;
  TH2F* hCaloMETPhivsVBPhi;	
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

#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;
using namespace edm;
using namespace reco;

/// Constructor
ZMETAnalyzer::ZMETAnalyzer(const ParameterSet& pset)
{
  //theMuonCollectionLabel = pset.getUntrackedParameter<InputTag>("MuonCollectionLabel");
  theZCollectionLabel = pset.getUntrackedParameter<InputTag>("ZCollectionLabel");
  theZCollectionLabel = edm::InputTag(theZCollectionLabel.label(),"ZGMGM");
  theMETCollectionLabel = pset.getUntrackedParameter<InputTag>("METCollectionLabel");

  useMuonCorrMET = pset.getUntrackedParameter<bool>("UseMuonCorrectedMET",false);

  theNbinsMT = pset.getUntrackedParameter<unsigned int>("NbinsMT");
  theMTMin = pset.getUntrackedParameter<double>("MTMin");
  theMTMax = pset.getUntrackedParameter<double>("MTMax");
  theNbinsMET = pset.getUntrackedParameter<unsigned int>("NbinsMET");
  theMETMin = pset.getUntrackedParameter<double>("METMin");
  theMETMax = pset.getUntrackedParameter<double>("METMax");
  theNbinsPtNu = pset.getUntrackedParameter<unsigned int>("NbinsPtNu");
  thePtNuMin = pset.getUntrackedParameter<double>("PtNuMin");
  thePtNuMax = pset.getUntrackedParameter<double>("PtNuMax");
  theNbinsEtaNu = pset.getUntrackedParameter<unsigned int>("NbinsEtaNu");
  theEtaNuMin = pset.getUntrackedParameter<double>("EtaNuMin");
  theEtaNuMax = pset.getUntrackedParameter<double>("EtaNuMax");
  
  theNbinsCaloMET = pset.getUntrackedParameter<unsigned int>("NbinsCaloMET");
  theCaloMETMin = pset.getUntrackedParameter<double>("CaloMETMin");
  theCaloMETMax = pset.getUntrackedParameter<double>("CaloMETMax");
  theNbinsVBPt = pset.getUntrackedParameter<unsigned int>("NbinsVBPt");
  theVBPtMin = pset.getUntrackedParameter<double>("VBPtMin");
  theVBPtMax = pset.getUntrackedParameter<double>("VBPtMax");
  theNbinsVBEta = pset.getUntrackedParameter<unsigned int>("NbinsVBEta");
  theVBEtaMin = pset.getUntrackedParameter<double>("VBEtaMin");
  theVBEtaMax = pset.getUntrackedParameter<double>("VBEtaMax");

  theNbinsCaloMETPhi = pset.getUntrackedParameter<unsigned int>("NbinsCaloMETPhi");
  theCaloMETPhiMin = pset.getUntrackedParameter<double>("CaloMETPhiMin");
  theCaloMETPhiMax = pset.getUntrackedParameter<double>("CaloMETPhiMax");
  theNbinsVBPhi = pset.getUntrackedParameter<unsigned int>("NbinsVBPhi");
  theVBPhiMin = pset.getUntrackedParameter<double>("VBPhiMin");
  theVBPhiMax = pset.getUntrackedParameter<double>("VBPhiMax");

  theRootFileName = pset.getUntrackedParameter<string>("RootFileName");
}

/// Destructor
ZMETAnalyzer::~ZMETAnalyzer(){
}

void ZMETAnalyzer::beginJob(const EventSetup& eventSetup){
  // Create the root file
  theFile = new TFile(theRootFileName.c_str(), "RECREATE");
  theFile->cd();

  mytree = new TTree("ZTTree","ZTTree");
  mytree->Branch("VBPt",&tree_VBPt,"VBPt/D");
  mytree->Branch("VBEta",&tree_VBEta,"VBEta/D");
  mytree->Branch("VBPhi",&tree_VBPhi,"VBPhi/D");
  mytree->Branch("NuPt",&tree_NuPt,"NuPt/D");
  mytree->Branch("NuEta",&tree_NuEta,"NuEta/D");
  mytree->Branch("NuPhi",&tree_NuPhi,"NuPhi/D");
  mytree->Branch("CaloMET",&tree_CaloMET,"CaloMET/D");
  mytree->Branch("CaloMET_Phi",&tree_CaloMET_Phi,"CaloMET_Phi/D");
  mytree->Branch("CaloMEx",&tree_CaloMEx,"CaloMEx/D");	
  mytree->Branch("CaloMEy",&tree_CaloMEy,"CaloMEy/D");
  mytree->Branch("MET",&tree_MET,"MET/D");
  mytree->Branch("MEx",&tree_MEx,"MEx/D");
  mytree->Branch("MEy",&tree_MEy,"MEy/D");
  mytree->Branch("MT",&tree_MT,"MT/D");

  char chname[256];
  char chtitle[256];

  snprintf(chname, 255, "hMT");
  snprintf(chtitle, 255, "MT (GeV) in Z events");
  hMT = new TH1F(chname, chtitle, theNbinsMT, theMTMin, theMTMax);

  snprintf(chname, 255, "hMET");
  snprintf(chtitle, 255, "MET (GeV) in Z events");
  hMET = new TH1F(chname, chtitle, theNbinsMET, theMETMin, theMETMax);

  snprintf(chname, 255, "hPtNu");
  snprintf(chtitle, 255, "PT #nu (GeV) in Z events");
  hPtNu = new TH1F(chname, chtitle, theNbinsPtNu, thePtNuMin, thePtNuMax);

  snprintf(chname, 255, "hEtaNu");
  snprintf(chtitle, 255, "#eta #nu in Z events");
  hEtaNu = new TH1F(chname, chtitle, theNbinsEtaNu, theEtaNuMin, theEtaNuMax);

  snprintf(chname, 255, "hPtNuvsEtaNu");
  snprintf(chtitle, 255, "PT #nu (GeV) vs #eta #nu in Z events");
  hPtNuvsEtaNu = new TH2F(chname, chtitle, theNbinsEtaNu, theEtaNuMin, theEtaNuMax
                                      , theNbinsPtNu, thePtNuMin, thePtNuMax);

  snprintf(chname, 255, "hMTvsPtNu");
  snprintf(chtitle, 255, "MT (GeV) vs PT #nu (GeV) in Z events");
  hMTvsPtNu = new TH2F(chname, chtitle, theNbinsPtNu, thePtNuMin, thePtNuMax
                                      , theNbinsMT, theMTMin, theMTMax);

  snprintf(chname, 255, "hMETvsPtNu");
  snprintf(chtitle, 255, "MET (GeV) vs PT #nu (GeV) in Z events");
  hMETvsPtNu = new TH2F(chname, chtitle, theNbinsPtNu, thePtNuMin, thePtNuMax
                                      , theNbinsMET, theMETMin, theMETMax);

  snprintf(chname, 255, "hMTvsEtaNu");
  snprintf(chtitle, 255, "MT (GeV) vs #eta #nu in Z events");
  hMTvsEtaNu = new TH2F(chname, chtitle, theNbinsEtaNu, theEtaNuMin, theEtaNuMax
                                      , theNbinsMT, theMTMin, theMTMax);
 
  snprintf(chname, 255, "hMETvsEtaNu");
  snprintf(chtitle, 255, "MET (GeV) vs #eta #nu in Z events");
  hMETvsEtaNu = new TH2F(chname, chtitle, theNbinsEtaNu, theEtaNuMin, theEtaNuMax
                                      , theNbinsMET, theMETMin, theMETMax);

  snprintf(chname, 255, "hMTvsPtNuvsEtaNu");
  snprintf(chtitle, 255, "MT (GeV) vs PT #nu (GeV) vs #eta #nu in Z events");
  hMTvsPtNuvsEtaNu = new TH3F(chname, chtitle, theNbinsPtNu, thePtNuMin, thePtNuMax
				      , theNbinsEtaNu, theEtaNuMin, theEtaNuMax
                                      , theNbinsMT, theMTMin, theMTMax);

  snprintf(chname, 255, "hMETvsPtNuvsEtaNu");
  snprintf(chtitle, 255, "MET (GeV) vs PT #nu (GeV) vs #eta #nu in Z events");
  hMETvsPtNuvsEtaNu = new TH3F(chname, chtitle, theNbinsPtNu, thePtNuMin, thePtNuMax
                                      , theNbinsEtaNu, theEtaNuMin, theEtaNuMax
                                      , theNbinsMET, theMETMin, theMETMax);

  snprintf(chname, 255, "hCaloMET");
  snprintf(chtitle, 255, "Calo MET (GeV) in Z events");
  hCaloMET = new TH1F(chname, chtitle, theNbinsCaloMET, theCaloMETMin, theCaloMETMax);

  snprintf(chname, 255, "hCaloMETPhi");
  snprintf(chtitle, 255, "Calo MET Phi in Z events");
  hCaloMETPhi = new TH1F(chname, chtitle, theNbinsCaloMETPhi, theCaloMETPhiMin, theCaloMETPhiMax);

  snprintf(chname, 255, "hVBPt");
  snprintf(chtitle, 255, "VB PT (GeV) in Z events");
  hVBPt = new TH1F(chname, chtitle, theNbinsVBPt, theVBPtMin, theVBPtMax);

  snprintf(chname, 255, "hVBEta");
  snprintf(chtitle, 255, "VB Eta in Z events");
  hVBEta = new TH1F(chname, chtitle, theNbinsVBEta, theVBEtaMin, theVBEtaMax);

  snprintf(chname, 255, "hVBPhi");
  snprintf(chtitle, 255, "VB Phi in Z events");
  hVBPhi = new TH1F(chname, chtitle, theNbinsVBPhi, theVBPhiMin, theVBPhiMax);

  snprintf(chname, 255, "hresVBPt");
  snprintf(chtitle, 255, "(VB Pt - CaloMET)/VBPt in Z events");	
  hresVBPt = new TH1F(chname, chtitle, theNbinsVBPt, -1., 1.);
	
  snprintf(chname, 255, "hMETvsVBPt");
  snprintf(chtitle, 255, "MET (GeV) vs VB PT (GeV) in Z events");
  hMETvsVBPt = new TH2F(chname, chtitle, theNbinsVBPt, theVBPtMin, theVBPtMax
                                      , theNbinsMET, theMETMin, theMETMax);

  snprintf(chname, 255, "hMETvsVBEta");
  snprintf(chtitle, 255, "MET (GeV) vs VB Eta in Z events");
  hMETvsVBEta = new TH2F(chname, chtitle, theNbinsVBEta, theVBEtaMin, theVBEtaMax
                                      , theNbinsMET, theMETMin, theMETMax);

  snprintf(chname, 255, "hMETvsVBPtvsVBEta");
  snprintf(chtitle, 255, "MET (GeV) vs VB PT (GeV) vs VB Eta in Z events");
  hMETvsVBPtvsVBEta = new TH3F(chname, chtitle, theNbinsVBPt, theVBPtMin, theVBPtMax
                                      , theNbinsVBEta, theVBEtaMin, theVBEtaMax
                                      , theNbinsMET, theMETMin, theMETMax);

  snprintf(chname, 255, "hMTvsVBPt");
  snprintf(chtitle, 255, "MT (GeV) vs VB PT (GeV) in Z events");
  hMTvsVBPt = new TH2F(chname, chtitle, theNbinsVBPt, theVBPtMin, theVBPtMax
                                      , theNbinsMT, theMTMin, theMTMax);

  snprintf(chname, 255, "hMTvsVBEta");
  snprintf(chtitle, 255, "MT (GeV) vs VB Eta in Z events");
  hMTvsVBEta = new TH2F(chname, chtitle, theNbinsVBEta, theVBEtaMin, theVBEtaMax
                                      , theNbinsMT, theMTMin, theMTMax);

  snprintf(chname, 255, "hMTvsVBPtvsVBEta");
  snprintf(chtitle, 255, "MT (GeV) vs VB PT (GeV) vs VB Eta in Z events");
  hMTvsVBPtvsVBEta = new TH3F(chname, chtitle, theNbinsVBPt, theVBPtMin, theVBPtMax
                                      , theNbinsVBEta, theVBEtaMin, theVBEtaMax
                                      , theNbinsMT, theMTMin, theMTMax);

  snprintf(chname, 255, "hCaloMETvsVBPt");
  snprintf(chtitle, 255, "Calo MET (GeV) vs VB PT (GeV) in Z events");
  hCaloMETvsVBPt = new TH2F(chname, chtitle, theNbinsVBPt, theVBPtMin, theVBPtMax
                                      , theNbinsCaloMET, theCaloMETMin, theCaloMETMax);

  snprintf(chname, 255, "hCaloMETvsVBEta");
  snprintf(chtitle, 255, "Calo MET (GeV) vs VB Eta in Z events");
  hCaloMETvsVBEta = new TH2F(chname, chtitle, theNbinsVBEta, theVBEtaMin, theVBEtaMax
                                      , theNbinsCaloMET, theCaloMETMin, theCaloMETMax);

  snprintf(chname, 255, "hCaloMETvsVBPhi");
  snprintf(chtitle, 255, "Calo MET (GeV) vs VB Phi in Z events");
  hCaloMETvsVBPhi = new TH2F(chname, chtitle, theNbinsVBPhi, theVBPhiMin, theVBPhiMax
                                      , theNbinsCaloMET, theCaloMETMin, theCaloMETMax);
  
  snprintf(chname, 255, "hCaloMETvsVBPtvsVBEta");
  snprintf(chtitle, 255, "Calo MET (GeV) vs VB PT (GeV) vs VB Eta in Z events");
  hCaloMETvsVBPtvsVBEta = new TH3F(chname, chtitle, theNbinsVBPt, theVBPtMin, theVBPtMax
                                      , theNbinsVBEta, theVBEtaMin, theVBEtaMax
                                      , theNbinsCaloMET, theCaloMETMin, theCaloMETMax);

  snprintf(chname, 255, "hCaloMEToverVBPtvsVBPt");
  snprintf(chtitle, 255, "Calo MET/VB PT vs VB PT (GeV) in Z events");
  hCaloMEToverVBPtvsVBPt = new TH2F(chname, chtitle, theNbinsVBPt, theVBPtMin, theVBPtMax
                                      , theNbinsVBPt, 0., 2.);

  snprintf(chname, 255, "hCaloMEToverVBPtvsCaloMET");
  snprintf(chtitle, 255, "Calo MET/VB PT vs CaloMET (GeV) in Z events");
  hCaloMEToverVBPtvsCaloMET = new TH2F(chname, chtitle, theNbinsCaloMET, theCaloMETMin, theCaloMETMax
                                      , theNbinsVBPt, 0., 2.);

  snprintf(chname, 255, "hCaloMETPhivsVBPhi");
  snprintf(chtitle, 255, "Calo MET Phi vs VB Phi in Z events");
  hCaloMETPhivsVBPhi = new TH2F(chname, chtitle, theNbinsVBPhi, theVBPhiMin, theVBPhiMax
                                      , theNbinsCaloMETPhi, theCaloMETPhiMin, theCaloMETPhiMax);
}

void ZMETAnalyzer::endJob(){
  theFile->cd();
  LogInfo("ZMETAnalyzer") << " Number of events used= " << hMT->GetEntries();

  // Write TTree
  mytree->Write();

  // Write the histos to file
  hMT->Write();
  hMET->Write();
  hPtNu->Write();
  hEtaNu->Write();
  hPtNuvsEtaNu->Write();
  hMTvsPtNu->Write();
  hMETvsPtNu->Write();
  hMTvsEtaNu->Write();
  hMETvsEtaNu->Write();
  hMTvsPtNuvsEtaNu->Write();
  hMETvsPtNuvsEtaNu->Write();

  hCaloMET->Write();
  hCaloMETPhi->Write();
  hVBPt->Write();
  hVBEta->Write();
  hVBPhi->Write();
  hresVBPt->Write();
  hMETvsVBPt->Write();
  hMETvsVBEta->Write();
  hMETvsVBPtvsVBEta->Write();
  hMTvsVBPt->Write();
  hMTvsVBEta->Write();
  hMTvsVBPtvsVBEta->Write();
  hCaloMETvsVBPt->Write();
  hCaloMETvsVBEta->Write();
  hCaloMETvsVBPhi->Write();
  hCaloMETvsVBPtvsVBEta->Write();
  hCaloMEToverVBPtvsVBPt->Write();
  hCaloMEToverVBPtvsCaloMET->Write();
  hCaloMETPhivsVBPhi->Write();

  theFile->Close();
}

void ZMETAnalyzer::analyze(const Event & ev, const EventSetup&){
  
  theFile->cd();

  double met_px = 0.;
  double met_py = 0.;

  /*Handle<TrackCollection> muonCollection;
  ev.getByLabel(theMuonCollectionLabel, muonCollection);
  int nmuons = muonCollection->size();
  if (nmuons!=2) return;*/
  Handle<CandidateCollection> ZCollection;
  ev.getByLabel(theZCollectionLabel,ZCollection);
  assert(ZCollection->size() > 0);

  const Candidate& myZ = (*ZCollection)[0];
  assert(myZ.numberOfDaughters() == 2);

  /*TrackRef mu(muonCollection,0);
  if (mu->pt()<25.) return;
  if (fabs(mu->eta())>2.0) return;
  met_px -= mu->px();
  met_py -= mu->py();

  TrackRef nu(muonCollection,1);
  double ptnu = nu->pt();
  double etanu = nu->eta();*/

  const Candidate* mu = myZ.daughter(0);
  if(!useMuonCorrMET){
	met_px -= mu->px();
  	met_py -= mu->py();	
  }

  const Candidate* nu = myZ.daughter(1);
  double ptnu = nu->pt();
  double etanu = nu->eta();
  double phinu = nu->phi();

  Handle<CaloMETCollection> metCollection;
  ev.getByLabel(theMETCollectionLabel, metCollection);
  CaloMETCollection::const_iterator caloMET = metCollection->begin();
  LogTrace("") << ">>> Input CaloMET_et, CaloMET_py, CaloMET_py= " << caloMET->et() << ", " << caloMET->px() << ", " << caloMET->py();
  met_px += caloMET->px();
  met_py += caloMET->py();
  if(useMuonCorrMET){
	met_px += nu->px();
        met_py += nu->py();
  } 	
  double met_et = sqrt(met_px*met_px+met_py*met_py);
  LogTrace("") << ">>> MET_et, MET_py, MET_py= " << met_et << ", " << met_px << ", " << met_py;

  double myCaloMEx = caloMET->px();
  double myCaloMEy = caloMET->py();
  if(useMuonCorrMET){
	myCaloMEx += mu->px();
	myCaloMEx += nu->px();
	myCaloMEy += mu->py();
        myCaloMEy += nu->py();
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

  double resVBPt = (myZ.pt() - caloMET->pt())/myZ.pt();
 	
  double fromZtoW = 80.4/91.2;
  // Fill TTree
  tree_VBPt = fromZtoW*myZ.pt();
  tree_VBEta = myZ.eta();
  tree_VBPhi = myZ.phi();  
  tree_NuPt = fromZtoW*ptnu;
  tree_NuEta = etanu;
  tree_NuPhi = phinu;
  /*tree_CaloMET = fromZtoW*caloMET->pt();
  tree_CaloMET_Phi = caloMET->phi();
  tree_CaloMEx = fromZtoW*caloMET->px();
  tree_CaloMEy = fromZtoW*caloMET->py();*/
  tree_CaloMET = fromZtoW*recCaloMET.pt();
  tree_CaloMET_Phi = recCaloMET.phi();
  tree_CaloMEx = fromZtoW*recCaloMET.px();
  tree_CaloMEy = fromZtoW*recCaloMET.py();
  tree_MET = fromZtoW*met_et;
  tree_MEx = fromZtoW*met_px;
  tree_MEy = fromZtoW*met_py;
  tree_MT = fromZtoW*massT;
  mytree->Fill();

  // Fill histos
  hMT->Fill(fromZtoW*massT);
  hMET->Fill(fromZtoW*met_et);
  hPtNu->Fill(fromZtoW*ptnu);
  hEtaNu->Fill(fabs(etanu));
  hPtNuvsEtaNu->Fill(fabs(etanu),fromZtoW*ptnu);
  hMTvsPtNu->Fill(fromZtoW*ptnu, fromZtoW*massT);
  hMETvsPtNu->Fill(fromZtoW*ptnu, fromZtoW*met_et);
  hMTvsEtaNu->Fill(fabs(etanu), fromZtoW*massT);
  hMETvsEtaNu->Fill(fabs(etanu), fromZtoW*met_et);
  hMTvsPtNuvsEtaNu->Fill(fromZtoW*ptnu, fabs(etanu), fromZtoW*massT);
  hMETvsPtNuvsEtaNu->Fill(fromZtoW*ptnu, fabs(etanu), fromZtoW*met_et);

  hCaloMET->Fill(fromZtoW*caloMET->pt());
  hCaloMETPhi->Fill(recCaloMET.phi());
  hVBPt->Fill(fromZtoW*myZ.pt());
  hVBEta->Fill(fabs(myZ.eta()));
  hVBPhi->Fill(myZ.phi());
  hresVBPt->Fill(resVBPt);	
  hMETvsVBPt->Fill(fromZtoW*myZ.pt(), fromZtoW*met_et);
  hMETvsVBEta->Fill(fabs(myZ.eta()), fromZtoW*met_et);
  hMETvsVBPtvsVBEta->Fill(fromZtoW*myZ.pt(), fabs(myZ.eta()), fromZtoW*met_et);
  hMTvsVBPt->Fill(fromZtoW*myZ.pt(), fromZtoW*massT);
  hMTvsVBEta->Fill(fabs(myZ.eta()), fromZtoW*massT);
  hMTvsVBPtvsVBEta->Fill(fromZtoW*myZ.pt(), fabs(myZ.eta()), fromZtoW*massT);	
  /*hCaloMETvsVBPt->Fill(fromZtoW*myZ.pt(), fromZtoW*caloMET->pt());
  hCaloMETvsVBEta->Fill(fabs(myZ.eta()), fromZtoW*caloMET->pt());
  hCaloMETvsVBPhi->Fill(myZ.phi(), fromZtoW*caloMET->pt());
  hCaloMETvsVBPtvsVBEta->Fill(fromZtoW*myZ.pt(), fabs(myZ.eta()), fromZtoW*caloMET->pt());
  hCaloMEToverVBPtvsVBPt->Fill(fromZtoW*myZ.pt(),caloMET->pt()/myZ.pt());
  hCaloMEToverVBPtvsCaloMET->Fill(fromZtoW*caloMET->pt(),caloMET->pt()/myZ.pt());
  hCaloMETPhivsVBPhi->Fill(myZ.phi(), caloMET->phi());*/
  hCaloMETvsVBPt->Fill(fromZtoW*myZ.pt(), fromZtoW*recCaloMET.pt());
  hCaloMETvsVBEta->Fill(fabs(myZ.eta()), fromZtoW*recCaloMET.pt());
  hCaloMETvsVBPhi->Fill(myZ.phi(), fromZtoW*recCaloMET.pt());
  hCaloMETvsVBPtvsVBEta->Fill(fromZtoW*myZ.pt(), fabs(myZ.eta()), fromZtoW*recCaloMET.pt());
  hCaloMEToverVBPtvsVBPt->Fill(fromZtoW*myZ.pt(),recCaloMET.pt()/myZ.pt());
  hCaloMEToverVBPtvsCaloMET->Fill(fromZtoW*recCaloMET.pt(),recCaloMET.pt()/myZ.pt());
  hCaloMETPhivsVBPhi->Fill(myZ.phi(), recCaloMET.phi());
}

DEFINE_FWK_MODULE(ZMETAnalyzer);
