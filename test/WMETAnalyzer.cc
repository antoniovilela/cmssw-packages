/** \class WMETAnalyzer
 *
 *  \author J. Alcaraz
 */

////////// Header section /////////////////////////////////////////////
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

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
  TH1F* hMETCorr;	
  TH2F* hMTvsPtNuCorr;
  TH2F* hMETvsPtNuCorr;	

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
  hMET = (TH1F*)hMTInput->Clone("hMET"); 
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
  hMETCorr = (TH1F*)hMETInput->Clone("hMETCorr"); 
  hMTvsPtNuCorr = (TH2F*)hMTvsPtNuInput->Clone("hMTvsPtNuCorr"); 
  hMETvsPtNuCorr = (TH2F*)hMETvsPtNuInput->Clone("hMETvsPtNuCorr");
  hPtNuRew->Reset();
  hMTCorr->Reset();
  hMETCorr->Reset();
  hMTvsPtNuCorr->Reset();
  hMETvsPtNuCorr->Reset();

  theOutputFile = new TFile(theOutputRootFileName.c_str(), "RECREATE");
  theOutputFile->cd();

}

void WMETAnalyzer::endJob(){
  theOutputFile->cd();
  LogInfo("WMETAnalyzer") << " Number of events used= " << hMT->GetEntries();

  // Write the histos to file
  hMTInput->Write();
  hMETInput->Write();
  hPtNuInput->Write();
  hMTvsPtNuInput->Write();
  hMETvsPtNuInput->Write();
  hMT->Write();
  hMET->Write();
  hPtNu->Write();
  hMTvsPtNu->Write();
  hMETvsPtNu->Write();

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

  hPtNuRew->Write();
  hMTCorr->Write();
  hMETCorr->Write();
  hMTvsPtNuCorr->Write();
  hMETvsPtNuCorr->Write();

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
  // Get the generator information from the event
  if (theGeneratorLabel.label()!="NONE") {
    Handle<CandidateCollection> genParticles;
    ev.getByLabel(theGeneratorLabel, genParticles);
    for( unsigned int i = 0; i < genParticles->size(); ++ i ) {
      const Candidate & genpart = (*genParticles)[i];
      int id1 = genpart.pdgId();
      if (abs(id1)!=14) continue;
      const Candidate* mother = genpart.mother();
      int idmother = mother->pdgId();
      if (fabs(idmother)!=24 && idmother!=24) continue;
      ptnu = genpart.pt();
      break;
    }
    if (ptnu<0) return;

  // No generator information is used: assume PT(W) = 0
  } else {
      ptnu = mu->pt();
  }

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

}

DEFINE_FWK_MODULE(WMETAnalyzer);
