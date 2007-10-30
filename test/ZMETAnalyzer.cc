/** \class ZMETAnalyzer
 *
 *  \author J. Alcaraz
 */

////////// Header section /////////////////////////////////////////////
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "TFile.h"
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
  unsigned int theNbinsMT;
  double theMTMin;
  double theMTMax;
  unsigned int theNbinsMET;
  double theMETMin;
  double theMETMax;
  unsigned int theNbinsPtNu;
  double thePtNuMin;
  double thePtNuMax;

  unsigned int theNbinsCaloMET;
  double theCaloMETMin;
  double theCaloMETMax;
  unsigned int theNbinsVBPt;
  double theVBPtMin;
  double theVBPtMax;
  unsigned int theNbinsVBEta;
  double theVBEtaMin;
  double theVBEtaMax; 
  std::string theRootFileName;

  // The output Root file
  TFile* theFile;

  // Histograms
  TH1F* hMT;
  TH1F* hMET;
  TH1F* hPtNu;
  TH1F* hEtaNu;
  TH2F* hMTvsPtNu;
  TH2F* hMETvsPtNu; 
  TH2F* hMTvsEtaNu;
  TH2F* hMETvsEtaNu;

  TH1F* hCaloMET;
  TH1F* hVBPt;
  TH1F* hVBEta;
  TH2F* hMETvsVBPt;
  TH2F* hMETvsVBEta;
  TH3F* hMETvsVBPtvsVBEta;
  TH2F* hCaloMETvsVBPt;
  TH2F* hCaloMETvsVBEta;
  TH3F* hCaloMETvsVBPtvsVBEta;
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

  theNbinsMT = pset.getUntrackedParameter<unsigned int>("NbinsMT");
  theMTMin = pset.getUntrackedParameter<double>("MTMin");
  theMTMax = pset.getUntrackedParameter<double>("MTMax");
  theNbinsMET = pset.getUntrackedParameter<unsigned int>("NbinsMET");
  theMETMin = pset.getUntrackedParameter<double>("METMin");
  theMETMax = pset.getUntrackedParameter<double>("METMax");
  theNbinsPtNu = pset.getUntrackedParameter<unsigned int>("NbinsPtNu");
  thePtNuMin = pset.getUntrackedParameter<double>("PtNuMin");
  thePtNuMax = pset.getUntrackedParameter<double>("PtNuMax");
  
  theNbinsCaloMET = pset.getUntrackedParameter<unsigned int>("NbinsCaloMET");
  theCaloMETMin = pset.getUntrackedParameter<double>("CaloMETMin");
  theCaloMETMax = pset.getUntrackedParameter<double>("CaloMETMax");
  theNbinsVBPt = pset.getUntrackedParameter<unsigned int>("NbinsVBPt");
  theVBPtMin = pset.getUntrackedParameter<double>("VBPtMin");
  theVBPtMax = pset.getUntrackedParameter<double>("VBPtMax");
  theNbinsVBEta = pset.getUntrackedParameter<unsigned int>("NbinsVBEta");
  theVBEtaMin = pset.getUntrackedParameter<double>("VBEtaMin");
  theVBEtaMax = pset.getUntrackedParameter<double>("VBEtaMax");

  theRootFileName = pset.getUntrackedParameter<string>("RootFileName");
}

/// Destructor
ZMETAnalyzer::~ZMETAnalyzer(){
}

void ZMETAnalyzer::beginJob(const EventSetup& eventSetup){
  // Create the root file
  theFile = new TFile(theRootFileName.c_str(), "RECREATE");
  theFile->cd();

  char chname[256];
  char chtitle[256];

  snprintf(chname, 255, "hMT");
  snprintf(chtitle, 255, "MT (GeV) in Z events");
  hMT = new TH1F(chname, chtitle, theNbinsMT, theMTMin, theMTMax);

  snprintf(chname, 255, "hMET");
  snprintf(chtitle, 255, "MET (GeV) in Z events");
  hMET = new TH1F(chname, chtitle, theNbinsMET, theMETMin, theMETMax);

  snprintf(chname, 255, "hPtNu");
  snprintf(chtitle, 255, "PT?#nu (GeV) in Z events");
  hPtNu = new TH1F(chname, chtitle, theNbinsPtNu, thePtNuMin, thePtNuMax);

  snprintf(chname, 255, "hEtaNu");
  snprintf(chtitle, 255, "#eta?#nu in Z events");
  hEtaNu = new TH1F(chname, chtitle, theNbinsPtNu, 0.0, 2.0);

  snprintf(chname, 255, "hMTvsPtNu");
  snprintf(chtitle, 255, "MT (GeV) vs PT?#nu (GeV) in Z events");
  hMTvsPtNu = new TH2F(chname, chtitle, theNbinsPtNu, thePtNuMin, thePtNuMax
                                      , theNbinsMT, theMTMin, theMTMax);

  snprintf(chname, 255, "hMETvsPtNu");
  snprintf(chtitle, 255, "MET (GeV) vs PT?#nu (GeV) in Z events");
  hMETvsPtNu = new TH2F(chname, chtitle, theNbinsPtNu, thePtNuMin, thePtNuMax
                                      , theNbinsMET, theMETMin, theMETMax);

  snprintf(chname, 255, "hMTvsEtaNu");
  snprintf(chtitle, 255, "MT (GeV) vs #eta?#nu (GeV) in Z events");
  hMTvsEtaNu = new TH2F(chname, chtitle, theNbinsPtNu, 0.0, 2.0
                                      , theNbinsMT, theMTMin, theMTMax);
 
  snprintf(chname, 255, "hMETvsEtaNu");
  snprintf(chtitle, 255, "MET (GeV) vs #eta?#nu (GeV) in Z events");
  hMETvsEtaNu = new TH2F(chname, chtitle, theNbinsPtNu, 0.0, 2.0
                                      , theNbinsMET, theMETMin, theMETMax);

  snprintf(chname, 255, "hCaloMET");
  snprintf(chtitle, 255, "Calo MET (GeV) in Z events");
  hCaloMET = new TH1F(chname, chtitle, theNbinsCaloMET, theCaloMETMin, theCaloMETMax);

  snprintf(chname, 255, "hVBPt");
  snprintf(chtitle, 255, "VB PT (GeV) in Z events");
  hVBPt = new TH1F(chname, chtitle, theNbinsVBPt, theVBPtMin, theVBPtMax);

  snprintf(chname, 255, "hVBEta");
  snprintf(chtitle, 255, "VB Eta in Z events");
  hVBEta = new TH1F(chname, chtitle, theNbinsVBEta, theVBEtaMin, theVBEtaMax);

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

  snprintf(chname, 255, "hCaloMETvsVBPt");
  snprintf(chtitle, 255, "Calo MET (GeV) vs VB PT (GeV) in Z events");
  hCaloMETvsVBPt = new TH2F(chname, chtitle, theNbinsVBPt, theVBPtMin, theVBPtMax
                                      , theNbinsCaloMET, theCaloMETMin, theCaloMETMax);

  snprintf(chname, 255, "hCaloMETvsVBEta");
  snprintf(chtitle, 255, "Calo MET (GeV) vs VB Eta in Z events");
  hCaloMETvsVBEta = new TH2F(chname, chtitle, theNbinsVBEta, theVBEtaMin, theVBEtaMax
                                      , theNbinsCaloMET, theCaloMETMin, theCaloMETMax);
  
  snprintf(chname, 255, "hCaloMETvsVBPtvsVBEta");
  snprintf(chtitle, 255, "Calo MET (GeV) vs VB PT (GeV) vs VB Eta in Z events");
  hCaloMETvsVBPtvsVBEta = new TH3F(chname, chtitle, theNbinsVBPt, theVBPtMin, theVBPtMax
                                      , theNbinsVBEta, theVBEtaMin, theVBEtaMax
                                      , theNbinsCaloMET, theCaloMETMin, theCaloMETMax);
}

void ZMETAnalyzer::endJob(){
  theFile->cd();
  LogInfo("ZMETAnalyzer") << " Number of events used= " << hMT->GetEntries();

  // Write the histos to file
  hMT->Write();
  hMET->Write();
  hPtNu->Write();
  hEtaNu->Write();
  hMTvsPtNu->Write();
  hMETvsPtNu->Write();
  hMTvsEtaNu->Write();
  hMETvsEtaNu->Write();

  hCaloMET->Write();
  hVBPt->Write();
  hVBEta->Write();
  hMETvsVBPt->Write();
  hMETvsVBEta->Write();
  hMETvsVBPtvsVBEta->Write();
  hCaloMETvsVBPt->Write();
  hCaloMETvsVBEta->Write();
  hCaloMETvsVBPtvsVBEta->Write();
  
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
  met_px -= mu->px();
  met_py -= mu->py();	

  const Candidate* nu = myZ.daughter(1);
  double ptnu = nu->pt();
  double etanu = nu->eta();

  Handle<CaloMETCollection> metCollection;
  ev.getByLabel(theMETCollectionLabel, metCollection);
  CaloMETCollection::const_iterator caloMET = metCollection->begin();
  LogTrace("") << ">>> CaloMET_et, CaloMET_py, CaloMET_py= " << caloMET->et() << ", " << caloMET->px() << ", " << caloMET->py();
  met_px += caloMET->px();
  met_py += caloMET->py();
  double met_et = sqrt(met_px*met_px+met_py*met_py);

  double w_et = mu->pt() + met_et;
  double w_px = mu->px() + met_px;
  double w_py = mu->py() + met_py;
  double massT = w_et*w_et - w_px*w_px - w_py*w_py;
  massT = (massT>0) ? sqrt(massT) : 0;
  LogTrace("") << "\t... W_et, W_px, W_py= " << w_et << ", " << w_px << ", " << w_py << " GeV";
  LogTrace("") << "\t... Invariant transverse mass= " << massT << " GeV";

  double fromZtoW = 80.4/91.2;
  hMT->Fill(fromZtoW*massT);
  hMET->Fill(fromZtoW*met_et);
  hPtNu->Fill(fromZtoW*ptnu);
  hEtaNu->Fill(fabs(etanu));
  hMTvsPtNu->Fill(fromZtoW*ptnu, fromZtoW*massT);
  hMETvsPtNu->Fill(fromZtoW*ptnu, fromZtoW*met_et);
  hMTvsEtaNu->Fill(fabs(etanu), fromZtoW*massT);
  hMETvsEtaNu->Fill(fabs(etanu), fromZtoW*met_et);

  hCaloMET->Fill(fromZtoW*caloMET->et());
  hVBPt->Fill(fromZtoW*myZ.pt());
  hVBEta->Fill(fabs(myZ.eta()));
  hMETvsVBPt->Fill(fromZtoW*myZ.pt(), fromZtoW*met_et);
  hMETvsVBEta->Fill(fabs(myZ.eta()), fromZtoW*met_et);
  hMETvsVBPtvsVBEta->Fill(fromZtoW*myZ.pt(), fabs(myZ.eta()), fromZtoW*met_et);
  hCaloMETvsVBPt->Fill(fromZtoW*myZ.pt(), fromZtoW*caloMET->pt());
  hCaloMETvsVBEta->Fill(fabs(myZ.eta()), fromZtoW*caloMET->pt());
  hCaloMETvsVBPtvsVBEta->Fill(fromZtoW*myZ.pt(), fabs(myZ.eta()), fromZtoW*caloMET->pt());

}

DEFINE_FWK_MODULE(ZMETAnalyzer);
