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
#include "TProfile.h"

class WMETAnalyzer: public edm::EDAnalyzer {
public:
  /// Constructor
  WMETAnalyzer(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~WMETAnalyzer();

  // Operations

  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  virtual void beginJob(const edm::EventSetup& eventSetup) ;
  virtual void endJob() ;

private:
  // Input from cfg file
  edm::InputTag theGeneratorLabel;
  edm::InputTag theMuonCollectionLabel;
  edm::InputTag theMETCollectionLabel;

  std::string theInputRootFileName;
  std::string theOutputRootFileName;

  // The input/output Root file
  TFile* theInputFile;
  TFile* theOutputFile;

  // Histograms
  TH1F* hMTInput;
  TH1F* hMETInput;
  TH1F* hPtNuInput;
  TH2F* hMTvsPtNuInput;
  TH2F* hMETvsPtNuInput;

  TH1F* hMT;
  TH1F* hMET;
  TH1F* hPtNu;
  TH2F* hMTvsPtNu;
  TH2F* hMETvsPtNu;

  TH1F* hPtNuRew;
  TH1F* hMTCorr;
  TH1F* hMTCorr2;
  TH1F* hMETCorr;
  TH1F* hMETCorr2;	
  TH2F* hMTvsPtNuCorr;
  TH2F* hMTvsPtNuCorr2;
  TH2F* hMETvsPtNuCorr;
  TH2F* hMETvsPtNuCorr2;	

  TH1F* hCaloMETInput;
  TH1F* hVBPtInput;
  TH2F* hCaloMETvsVBPtInput;
  TH2F* hMETvsVBPtInput;

  TH1F* hCaloMET;
  TH1F* hVBPt;
  TH2F* hCaloMETvsVBPt;
  TH2F* hMETvsVBPt;

  TH1F* hVBPtRew;
  TH1F* hCaloMETCorr;
  TH1F* hMETCorr3;
  TH2F* hCaloMETvsVBPtCorr;
  TH2F* hMETvsVBPtCorr;

  TH1F* hMETfromZmumu;
  TH1F* hMTfromZmumu;
  TH2F* hMETfromZmumuvsVBPt;

  //Random number engine/generator
  CLHEP::HepRandomEngine* fRandomEngine;
  CLHEP::RandGauss*       fRandomGenerator;  
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
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

using namespace std;
using namespace edm;
using namespace reco;

/// Constructor
WMETAnalyzer::WMETAnalyzer(const ParameterSet& pset)
{
  theGeneratorLabel = pset.getUntrackedParameter<InputTag>("GeneratorCollectionLabel");
  theMuonCollectionLabel = pset.getUntrackedParameter<InputTag>("MuonCollectionLabel");
  theMuonCollectionLabel = edm::InputTag(theMuonCollectionLabel.label(),"WMuons");
  theMETCollectionLabel = pset.getUntrackedParameter<InputTag>("METCollectionLabel");

  theInputRootFileName = pset.getUntrackedParameter<string>("InputRootFileName");
  theOutputRootFileName = pset.getUntrackedParameter<string>("OutputRootFileName");
  LogTrace("") << ">>> GENERATOR LABEL: " << theGeneratorLabel.label();
}

/// Destructor
WMETAnalyzer::~WMETAnalyzer(){
}

void WMETAnalyzer::beginJob(const EventSetup& eventSetup){
  // Create the root file
  theInputFile = new TFile(theInputRootFileName.c_str(), "READONLY");
  theInputFile->cd();

  hMTInput = (TH1F*)theInputFile->Get("hMT");
  hMETInput = (TH1F*)theInputFile->Get("hMET"); 
  hPtNuInput = (TH1F*)theInputFile->Get("hPtNu"); 
  hMTvsPtNuInput = (TH2F*)theInputFile->Get("hMTvsPtNu");
  hMETvsPtNuInput = (TH2F*)theInputFile->Get("hMETvsPtNu"); 
  hMTInput->SetName("hMTInput");
  hMETInput->SetName("hMETInput");
  hPtNuInput->SetName("hPtNuInput");
  hMTvsPtNuInput->SetName("hMTvsPtNuInput");
  hMETvsPtNuInput->SetName("hMETvsPtNuInput");

  hMT = (TH1F*)hMTInput->Clone("hMT");
  hMET = (TH1F*)hMETInput->Clone("hMET"); 
  hPtNu = (TH1F*)hPtNuInput->Clone("hPtNu"); 
  hMTvsPtNu = (TH2F*)hMTvsPtNuInput->Clone("hMTvsPtNu");
  hMETvsPtNu = (TH2F*)hMETvsPtNuInput->Clone("hMETvsPtNu"); 
  hMT->Reset();
  hMET->Reset();
  hPtNu->Reset();
  hMTvsPtNu->Reset();
  hMETvsPtNu->Reset();

  hPtNuRew = (TH1F*)hPtNuInput->Clone("hPtNuRew"); 
  hMTCorr = (TH1F*)hMTInput->Clone("hMTCorr");
  hMTCorr2 = (TH1F*)hMTInput->Clone("hMTCorr2");
  hMETCorr = (TH1F*)hMETInput->Clone("hMETCorr");
  hMETCorr2 = (TH1F*)hMETInput->Clone("hMETCorr2"); 
  hMTvsPtNuCorr = (TH2F*)hMTvsPtNuInput->Clone("hMTvsPtNuCorr");
  hMTvsPtNuCorr2 = (TH2F*)hMTvsPtNuInput->Clone("hMTvsPtNuCorr2"); 
  hMETvsPtNuCorr = (TH2F*)hMETvsPtNuInput->Clone("hMETvsPtNuCorr");
  hMETvsPtNuCorr2 = (TH2F*)hMETvsPtNuInput->Clone("hMETvsPtNuCorr2");
  hPtNuRew->Reset();
  hMTCorr->Reset();
  hMTCorr2->Reset();
  hMETCorr->Reset();
  hMETCorr2->Reset();
  hMTvsPtNuCorr->Reset();
  hMTvsPtNuCorr2->Reset();
  hMETvsPtNuCorr->Reset();
  hMETvsPtNuCorr2->Reset();

  hCaloMETInput = (TH1F*)theInputFile->Get("hCaloMET");
  hVBPtInput = (TH1F*)theInputFile->Get("hVBPt");
  hCaloMETvsVBPtInput = (TH2F*)theInputFile->Get("hCaloMETvsVBPt");
  hMETvsVBPtInput = (TH2F*)theInputFile->Get("hMETvsVBPt");
  hCaloMETInput->SetName("hCaloMETInput");
  hVBPtInput->SetName("hVBPtInput");
  hCaloMETvsVBPtInput->SetName("hCaloMETvsVBPtInput");
  hMETvsVBPtInput->SetName("hMETvsVBPtInput");

  hCaloMET = (TH1F*)hCaloMETInput->Clone("hCaloMET");
  hVBPt = (TH1F*)hVBPtInput->Clone("hVBPt");
  hCaloMETvsVBPt = (TH2F*)hCaloMETvsVBPtInput->Clone("hCaloMETvsVBPt");
  hMETvsVBPt = (TH2F*)hMETvsVBPtInput->Clone("hMETvsVBPt");
  hCaloMET->Reset();
  hVBPt->Reset();
  hCaloMETvsVBPt->Reset();
  hMETvsVBPt->Reset();

  hVBPtRew = (TH1F*)hVBPtInput->Clone("hVBPtRew");
  hCaloMETCorr = (TH1F*)hCaloMETInput->Clone("hCaloMETCorr");
  hMETCorr3 = (TH1F*)hMETInput->Clone("hMETCorr3");
  hCaloMETvsVBPtCorr = (TH2F*)hCaloMETvsVBPtInput->Clone("hCaloMETvsVBPtCorr");
  hMETvsVBPtCorr = (TH2F*)hMETvsVBPtInput->Clone("hMETvsVBPtCorr");
  hVBPtRew->Reset();
  hCaloMETCorr->Reset();
  hMETCorr3->Reset();
  hCaloMETvsVBPtCorr->Reset();
  hMETvsVBPtCorr->Reset();

  hMTfromZmumu = (TH1F*)hMTInput->Clone("hMTfromZmumu");
  hMETfromZmumu = (TH1F*)hMETInput->Clone("hMETfromZmumu");
  hMETfromZmumuvsVBPt = (TH2F*)hMETvsVBPtInput->Clone("hMETfromZmumuvsVBPt");
  hMTfromZmumu->Reset();
  hMETfromZmumu->Reset();
  hMETfromZmumuvsVBPt->Reset();

  theOutputFile = new TFile(theOutputRootFileName.c_str(), "RECREATE");
  theOutputFile->cd();

  //Initialize random generator
  Service<RandomNumberGenerator> rng;
  long seed = (long)(rng->mySeed());
  LogTrace("") << " seed= " << seed;
  fRandomEngine = new CLHEP::HepJamesRandom(seed);
  fRandomGenerator = new CLHEP::RandGauss(fRandomEngine);
  LogTrace("") << "Random Generator Initialized";
  
}

void WMETAnalyzer::endJob(){
  theOutputFile->cd();
  LogInfo("WMETAnalyzer") << " Number of events used= " << hMT->GetEntries();

  // Write the histos to file
  /*hMTInput->Write();
  hMETInput->Write();
  hPtNuInput->Write();
  hMTvsPtNuInput->Write();
  hMETvsPtNuInput->Write();*/
  hMT->Write();
  hMET->Write();
  hPtNu->Write();
  hMTvsPtNu->Write();
  hMETvsPtNu->Write();

  /*hCaloMETInput->Write();
  hVBPtInput->Write(); 
  hCaloMETvsVBPtInput->Write();
  hMETvsVBPtInput->Write();*/

  hCaloMET->Write();
  hVBPt->Write();
  hCaloMETvsVBPt->Write();
  hMETvsVBPt->Write();

  hMTfromZmumu->Write();
  hMETfromZmumu->Write();
  hMETfromZmumuvsVBPt->Write();

  double contZ = hPtNuInput->Integral(0,hPtNuInput->GetNbinsX()+1);
  double contW = hPtNu->Integral(0,hPtNu->GetNbinsX()+1);
  if (contZ>0) hPtNuInput->Scale(1./contZ);
  if (contW>0) hPtNu->Scale(1./contW);
  hPtNuRew = (TH1F*)hPtNu->Clone("hPtNuRew");
  hPtNuRew->Divide(hPtNuInput);

  for (int j=0; j<=hMTvsPtNu->GetNbinsY()+1; j++) {
    double entriesj = 0.;
    for (int i=0; i<=hMTvsPtNu->GetNbinsX()+1; i++) {
      double entries = hMTvsPtNu->GetBinContent(i,j);
      entries *= hPtNuRew->GetBinContent(i);
      hMTvsPtNuCorr->SetBinContent(i,j,entries);
      entriesj += hMTvsPtNuCorr->GetBinContent(i,j);
    }
    hMTCorr->SetBinContent(j,entriesj);
  }
  for (int j=0; j<=hMETvsPtNu->GetNbinsY()+1; j++) {
    double entriesj = 0.;
    for (int i=0; i<=hMETvsPtNu->GetNbinsX()+1; i++) {
      double entries = hMETvsPtNu->GetBinContent(i,j);
      entries *= hPtNuRew->GetBinContent(i);
      hMETvsPtNuCorr->SetBinContent(i,j,entries);
      entriesj += hMETvsPtNuCorr->GetBinContent(i,j);
    }
    hMETCorr->SetBinContent(j,entriesj);
  }

  for (int j=0; j<=hMTvsPtNuInput->GetNbinsY()+1; j++) {
    double entriesj = 0.;
    for (int i=0; i<=hMTvsPtNuInput->GetNbinsX()+1; i++) {
      double entries = hMTvsPtNuInput->GetBinContent(i,j);
      entries *= hPtNuRew->GetBinContent(i);
      hMTvsPtNuCorr2->SetBinContent(i,j,entries);
      entriesj += hMTvsPtNuCorr2->GetBinContent(i,j);
    }
    hMTCorr2->SetBinContent(j,entriesj);
  }
  for (int j=0; j<=hMETvsPtNuInput->GetNbinsY()+1; j++) {
    double entriesj = 0.;
    for (int i=0; i<=hMETvsPtNuInput->GetNbinsX()+1; i++) {
      double entries = hMETvsPtNuInput->GetBinContent(i,j);
      entries *= hPtNuRew->GetBinContent(i);
      hMETvsPtNuCorr2->SetBinContent(i,j,entries);
      entriesj += hMETvsPtNuCorr2->GetBinContent(i,j);
    }
    hMETCorr2->SetBinContent(j,entriesj);
  }

  hPtNuRew->Write();
  hMTCorr->Write();
  hMTCorr2->Write();
  hMETCorr->Write();
  hMETCorr2->Write();
  hMTvsPtNuCorr->Write();
  hMTvsPtNuCorr2->Write();
  hMETvsPtNuCorr->Write();
  hMETvsPtNuCorr2->Write();


  double contZ2 = hVBPtInput->Integral(0,hVBPtInput->GetNbinsX()+1);
  double contW2 = hVBPt->Integral(0,hVBPt->GetNbinsX()+1);
  if (contZ2>0) hVBPtInput->Scale(1./contZ2);
  if (contW2>0) hVBPt->Scale(1./contW2);
  hVBPtRew = (TH1F*)hVBPt->Clone("hVBPtRew");
  hVBPtRew->Divide(hVBPtInput);

  for (int j=0; j<=hMETvsVBPtInput->GetNbinsY()+1; j++) {
    double entriesj = 0.;
    for (int i=0; i<=hMETvsVBPtInput->GetNbinsX()+1; i++) {
      double entries = hMETvsVBPtInput->GetBinContent(i,j);
      entries *= hVBPtRew->GetBinContent(i);
      hMETvsVBPtCorr->SetBinContent(i,j,entries);
      entriesj += hMETvsVBPtCorr->GetBinContent(i,j);
    }
    hMETCorr3->SetBinContent(j,entriesj);
  }

  for (int j=0; j<=hCaloMETvsVBPtInput->GetNbinsY()+1; j++) {
    double entriesj = 0.;
    for (int i=0; i<=hCaloMETvsVBPtInput->GetNbinsX()+1; i++) {
      double entries = hCaloMETvsVBPtInput->GetBinContent(i,j);
      entries *= hVBPtRew->GetBinContent(i);
      hCaloMETvsVBPtCorr->SetBinContent(i,j,entries);
      entriesj += hCaloMETvsVBPtCorr->GetBinContent(i,j);
    }
    hCaloMETCorr->SetBinContent(j,entriesj);
  }
  
  hVBPtRew->Write();
  hCaloMETCorr->Write();
  hMETCorr3->Write();
  hCaloMETvsVBPtCorr->Write();
  hMETvsVBPtCorr->Write();

  theOutputFile->Close();
}

void WMETAnalyzer::analyze(const Event & ev, const EventSetup&){
  
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
  met_px -= mu->px();
  met_py -= mu->py();
  
  // PT of neutrino
  double ptnu = -1.;
  double pxnu,pynu;
  //const Candidate* myW = 0;
  /*const double Zmuonptcut_ = 20.;
  const double Zmuonetacut_ = 2.;*/ 
  // Get the generator information from the event
  int myMuindex = -1;
  int myNuindex = -1;
  Candidate::LorentzVector myMup4;
  Candidate::LorentzVector myNup4;
  if (theGeneratorLabel.label()!="NONE") {
    Handle<CandidateCollection> genParticles;
    ev.getByLabel(theGeneratorLabel, genParticles);
    int pid_ids[2] = {13,14};
    for(int j = 0; j < 2; j++){	
    	for( unsigned int i = 0; i < genParticles->size(); ++ i ) {
      		const Candidate & genpart = (*genParticles)[i];
      		//LogTrace("") << ">>>>>>> pid,status,px,py,px,e= "  << genpart.pdgId() << " , " << genpart.status() << " , " << genpart.px() << " , " << genpart.py() << " , " << genpart.pz() << " , " << genpart.energy();	
      		int id1 = genpart.pdgId();
      		/*if (abs(id1)!=14) continue;
      		const Candidate* mother = genpart.mother();
      		int idmother = mother->pdgId();
      		if (fabs(idmother)!=24 && idmother!=24) continue;*/

      		//if (genpart.pt()<Zmuonptcut_) continue;
      		//if (fabs(genpart.eta())>Zmuonetacut_) continue;	

		if (abs(id1)!=pid_ids[j]) continue; 
      		//ptnu = genpart.pt();
      		//myW = mother;
		if(pid_ids[j] == 13) myMuindex = i;
		else if(pid_ids[j] == 14) myNuindex = i;

      		break;
    	}
    }	
    if (myNuindex<0) {LogTrace("") << ">>>>>>> No generated neutrino found...skipping event";return;}
    if (myMuindex<0) {LogTrace("") << ">>>>>>> No generated muon found...skipping event";return;}
    const Candidate & genmuon = (*genParticles)[myMuindex];
    const Candidate & genneutrino = (*genParticles)[myNuindex];
    myMup4 = genmuon.p4();
    myNup4 = genneutrino.p4();
    LogTrace("") << ">>>> Gen neutrino px,py,pz,e= " << myNup4.px() << " , " << myNup4.py() << " , " <<	myNup4.pz() << " , " << myNup4.energy();
    LogTrace("") << ">>>> Gen muon px,py,pz,e= " << myMup4.px() << " , " << myMup4.py() << " , " << myMup4.pz() << " , " << myMup4.energy();	 
    ptnu = genneutrino.pt();
    pxnu = genneutrino.px();
    pynu = genneutrino.py();	 
  // No generator information is used: assume PT(W) = 0
  } else {
      ptnu = mu->pt();
  }
  //assert(ptnu >= 0.);
  //if(theGeneratorLabel.label()!="NONE") assert(myW != 0); 

  Handle<CaloMETCollection> metCollection;
  ev.getByLabel(theMETCollectionLabel, metCollection);
  CaloMETCollection::const_iterator caloMET = metCollection->begin();
  LogTrace("") << ">>> CaloMET_et, CaloMET_py, CaloMET_py= " << caloMET->et() << ", " << caloMET->px() << ", " << caloMET->py();;
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

  hMT->Fill(massT);
  hMET->Fill(met_et);
  hPtNu->Fill(ptnu);
  hMTvsPtNu->Fill(ptnu, massT);
  hMETvsPtNu->Fill(ptnu, met_et);

  hCaloMET->Fill(caloMET->pt());
  if (theGeneratorLabel.label()!="NONE") {
	/*double pxW = myMup4.px() + myNup4.px();
	double pyW = myMup4.py() + myNup4.py();
	double ptW = sqrt(pxW*pxW + pyW*pyW);*/
	double pxW = mu->px() + pxnu;
	double pyW = mu->py() + pynu;
	double ptW = sqrt(pxW*pxW + pyW*pyW);

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
	double proj_mean = histCaloMET_binWpt->GetMean();
	double proj_sigma = histCaloMET_binWpt->GetRMS();
	double dataMET = fRandomGenerator->fire(proj_mean,proj_sigma);
	LogTrace("") << "\t... W pt, bin in CaloMET dist.= " << ptW << " , " << binWpt;
	LogTrace("") << "\t... CaloMET from Z events= " << dataMET;

	double scale = dataMET/caloMET->pt();
	double dataMEx = scale*caloMET->px();
	dataMEx -= mu->px();
	double dataMEy = scale*caloMET->py();
        dataMEy -= mu->py();
	dataMET = sqrt(dataMEx*dataMEx + dataMEy*dataMEy);	
	double w_et = mu->pt() + dataMET;
        double w_px = mu->px() + dataMEx;
        double w_py = mu->py() + dataMEy;

	double massT = w_et*w_et - w_px*w_px - w_py*w_py;
	massT = (massT>0) ? sqrt(massT) : 0;
	LogTrace("") << "\t... W_et, W_px, W_py= " << w_et << ", " << w_px << ", " << w_py << " GeV";
	LogTrace("") << "\t... Invariant transverse mass= " << massT << " GeV";

	//ptW = sqrt(w_px*w_px + w_py*w_py);
	hVBPt->Fill(ptW);
        hCaloMETvsVBPt->Fill(ptW, caloMET->pt());
        hMETvsVBPt->Fill(ptW, met_et);

	hMTfromZmumu->Fill(massT);
  	hMETfromZmumu->Fill(dataMET);
  	hMETfromZmumuvsVBPt->Fill(ptW, dataMET);
  } 
}

DEFINE_FWK_MODULE(WMETAnalyzer);
