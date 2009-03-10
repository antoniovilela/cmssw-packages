/** \class WMuNuAnalyzer
 *  Simple analyzer to make some W->MuNu plots
 *
 *  \author M.I. Josa
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class TFile;
class TH1F;
class TH2F;


class WMuNuAnalyzer : public edm::EDAnalyzer {
public:
  WMuNuAnalyzer(const edm::ParameterSet& pset);
  virtual ~WMuNuAnalyzer();
  virtual void beginJob(const edm::EventSetup& eventSetup);
  virtual void endJob();
  virtual void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);
private:
  edm::InputTag muonTag_;
  edm::InputTag metTag_;
  edm::InputTag jetTag_;
  bool useOnlyGlobalMuons_;
  double ptCut_;
  double etaCut_;
  bool isRelativeIso_;
  double isoCut03_;
  double massTMin_;
  double massTMax_;
  double ptThrForZCount_;
  double acopCut_;
  double eJetMin_;
  int nJetMax_;

// Histograms
  TH1F *hNMu;
  TH1F *hPtMu;
  TH1F *hEtaMu;
  TH1F *hMET;
  TH1F *hTMass;
  TH1F *hAcop;
  TH1F *hNjets;
  TH1F *hPtSum;
  TH1F *hPtSumN;
  TH2F *hTMass_PtSum;

  TH1F *hNMu_sel;
  TH1F *hPtMu_sel;
  TH1F *hEtaMu_sel;
  TH1F *hMET_sel;
  TH1F *hTMass_sel;
  TH1F *hAcop_sel;
  TH1F *hNjets_sel;
  TH1F *hPtSum_sel;
  TH1F *hPtSumN_sel;
  TH2F *hTMass_PtSum_sel;

  unsigned int numberOfEvents;
  unsigned int numberOfMuons;
};

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/GeometryVector/interface/Phi.h"

#include <FWCore/ServiceRegistry/interface/Service.h>
#include <PhysicsTools/UtilAlgos/interface/TFileService.h>

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

#include <map>
#include <vector>

using namespace std;
using namespace edm;
using namespace reco;

/// Constructor
WMuNuAnalyzer::WMuNuAnalyzer(const ParameterSet& pset) :
      muonTag_(pset.getParameter<edm::InputTag> ("MuonTag")),
      metTag_(pset.getParameter<edm::InputTag> ("METTag")),
      jetTag_(pset.getParameter<edm::InputTag> ("JetTag")),
      useOnlyGlobalMuons_(pset.getParameter<bool>("UseOnlyGlobalMuons")),
      ptCut_(pset.getParameter<double>("PtCut")),
      etaCut_(pset.getParameter<double>("EtaCut")),
      isRelativeIso_(pset.getParameter<bool>("IsRelativeIso")),
      isoCut03_(pset.getParameter<double>("IsoCut03")),
      massTMin_(pset.getParameter<double>("MassTMin")),
      massTMax_(pset.getParameter<double>("MassTMax")),
      ptThrForZCount_(pset.getParameter<double>("PtThrForZCount")),
      acopCut_(pset.getParameter<double>("AcopCut")),
      eJetMin_(pset.getParameter<double>("EJetMin")),
      nJetMax_(pset.getParameter<int>("NJetMax"))

{
  LogVerbatim("Analysis") << " WMuNuAnalyzer constructor called";
}

/// Destructor
WMuNuAnalyzer::~WMuNuAnalyzer(){
}

void WMuNuAnalyzer::beginJob(const EventSetup& eventSetup){
  // Book histograms
  edm::Service<TFileService> fs;

  numberOfEvents = 0;
  numberOfMuons = 0;

  hNMu    = fs->make<TH1F>("NMu","Nb. muons in the event",10,0.,10.);
  hPtMu   = fs->make<TH1F>("ptMu","Pt mu",100,0.,100.);
  hEtaMu  = fs->make<TH1F>("etaMu","Eta mu",50,-2.5,2.5);
  hMET    = fs->make<TH1F>("MET","Missing Transverse Energy (GeV)", 100,0.,200.);
  hTMass  = fs->make<TH1F>("TMass","Rec. Transverse Mass (GeV)",150,0.,300.);
  hAcop   = fs->make<TH1F>("Acop","Mu-MET acoplanarity",50,0.,M_PI);
  hNjets  = fs->make<TH1F>("Njets","njets",25,0.,25.);
  hPtSum  = fs->make<TH1F>("ptSum","Sum pT (GeV)",100,0.,50.);
  hPtSumN = fs->make<TH1F>("ptSumN","Sum pT/pT",100,0.,50.);
  hTMass_PtSum = fs->make<TH2F>("TMass_ptSum","Rec. Transverse Mass (GeV) vs Sum pT (GeV)",100,0.,50.,150,0.,300.);

  hNMu_sel    = fs->make<TH1F>("NMu_sel","Nb. selected muons",10,0.,10.);
  hPtMu_sel   = fs->make<TH1F>("ptMu_sel","Pt mu",100,0.,100.);
  hEtaMu_sel  = fs->make<TH1F>("etaMu_sel","Eta mu",50,-2.5,2.5);
  hMET_sel    = fs->make<TH1F>("MET_sel","Missing Transverse Energy (GeV)", 100,0.,200.);
  hTMass_sel  = fs->make<TH1F>("TMass_sel","Rec. Transverse Mass (GeV)",150,0.,300.);
  hAcop_sel   = fs->make<TH1F>("Acop_sel","Mu-MET acoplanarity",50,0.,M_PI);
  hNjets_sel  = fs->make<TH1F>("Njets_sel","njets",25,0.,25.);
  hPtSum_sel  = fs->make<TH1F>("ptSum_sel","Sum pT (GeV)",100,0.,50.);
  hPtSumN_sel = fs->make<TH1F>("ptSumN_sel","Sum pT/pT ",100,0.,2.5);
  hTMass_PtSum_sel = fs->make<TH2F>("TMass_ptSum_sel","Rec. Transverse Mass (GeV) vs Sum pT (GeV)",100,0.,50.,150,0.,300.);

}

void WMuNuAnalyzer::endJob(){
  LogVerbatim("Analysis") << "WMuNuAnalyzer>>> FINAL PRINTOUTS -> BEGIN";
  LogVerbatim("Analysis") << "WMuNuAnalyzer>>> Number of analyzed events= " << numberOfEvents;
  LogVerbatim("Analysis") << "WMuNuAnalyzer>>> Number of analyzed muons= " << numberOfMuons;

  LogVerbatim("Analysis") << "WMuNuAnalyzer>>> FINAL PRINTOUTS -> END";
}
 

void WMuNuAnalyzer::analyze(const Event & event, const EventSetup& eventSetup){
  
   numberOfEvents++;

   double pt_sel[5];
   double eta_sel[5];
   double acop_sel[5];
   double massT_sel[5];
   double iso_sel[5];
   double isoN_sel[5];

   bool event_sel = true;
  
   double met_px = 0.;
   double met_py = 0.;

  // Get the Muon Track collection from the event
   Handle<reco::MuonCollection> muonCollection;
   event.getByLabel(muonTag_, muonCollection);
   if (event.getByLabel(muonTag_, muonCollection)) {
      LogTrace("Analysis") << "Reconstructed Muon tracks: " << muonCollection->size() << endl;
   } else {
      LogTrace("Analysis") << ">>> Muon collection does not exist !!!";
      return;
   }

   numberOfMuons+=muonCollection->size();
  
   for (unsigned int i=0; i<muonCollection->size(); i++) {
      MuonRef mu(muonCollection,i);
      if (useOnlyGlobalMuons_ && !mu->isGlobalMuon()) continue;
      met_px -= mu->px();
      met_py -= mu->py();
   }

  // Get the MET collection from the event
   Handle<CaloMETCollection> metCollection;
   if (!event.getByLabel(metTag_, metCollection)) {
      LogTrace("Analysis") << ">>> MET collection does not exist !!!";
      return;
   }

   CaloMETCollection::const_iterator caloMET = metCollection->begin();
   LogTrace("Analysis") << ">>> CaloMET_et, CaloMET_py, CaloMET_py= " << caloMET->et() << ", " << caloMET->px() << ", " << caloMET->py();;
   met_px += caloMET->px();
   met_py += caloMET->py();
   double met_et = sqrt(met_px*met_px+met_py*met_py);
   hMET->Fill(met_et);

  // Get the Jet collection from the event
   Handle<CaloJetCollection> jetCollection;
   if (!event.getByLabel(jetTag_, jetCollection)) {
      LogTrace("Analysis") << ">>> CALOJET collection does not exist !!!";
      return;
   }

   CaloJetCollection::const_iterator jet = jetCollection->begin();
   int njets = 0;
   for (jet=jetCollection->begin(); jet!=jetCollection->end(); jet++) {
         if (jet->et()>eJetMin_) njets++;
   }
   hNjets->Fill(njets);
   LogTrace("Analysis") << ">>> Number of jets " << jetCollection->size() << "; above " << eJetMin_ << " GeV: " << njets;
   LogTrace("Analysis") << ">>> Number of jets above " << eJetMin_ << " GeV: " << njets;

   if (njets>nJetMax_) event_sel = false;

   unsigned int nmuons = 0;
   unsigned int nmuonsForZ = 0;

   hNMu->Fill(muonCollection->size());
   float max_pt = -9999.;
   int i_max_pt = 0;
   for (unsigned int i=0; i<muonCollection->size(); i++) {
      bool muon_sel = true;

      MuonRef mu(muonCollection,i);
      if (useOnlyGlobalMuons_ && !mu->isGlobalMuon()) continue;
      LogTrace("Analysis") << "> Processing (global) muon number " << i << "...";
// pt
      double pt = mu->pt();
      hPtMu->Fill(pt);
      LogTrace("Analysis") << "\t... pt= " << pt << " GeV";

      if (pt>ptThrForZCount_) nmuonsForZ++;
      if (pt<ptCut_) muon_sel = false;
// eta
      double eta = mu->eta();
      hEtaMu->Fill(eta);
      LogTrace("Analysis") << "\t... eta= " << eta;
      if (fabs(eta)>etaCut_) muon_sel = false;

// acoplanarity
      Geom::Phi<double> deltaphi(mu->phi()-atan2(met_py,met_px));
      double acop = deltaphi.value();
      if (acop<0) acop = - acop;
      acop = M_PI - acop;
      hAcop->Fill(acop);
      LogTrace("Analysis") << "\t... acop= " << acop;
      if (acop>acopCut_) muon_sel = false;

// transverse mass
      double w_et = mu->pt() + met_et;
      double w_px = mu->px() + met_px;
      double w_py = mu->py() + met_py;
      double massT = w_et*w_et - w_px*w_px - w_py*w_py;
      massT = (massT>0) ? sqrt(massT) : 0;
      hTMass->Fill(massT);
      LogTrace("Analysis") << "\t... W_et, W_px, W_py= " << w_et << ", " << w_px << ", " << w_py << " GeV";
      LogTrace("Analysis") << "\t... Invariant transverse mass= " << massT << " GeV";
      if (massT<massTMin_) muon_sel = false;
      if (massT>massTMax_) muon_sel = false;

// Isolation
      double ptsum = mu->isolationR03().sumPt;
      hPtSum->Fill(ptsum);
      hPtSumN->Fill(ptsum/pt);
      hTMass_PtSum->Fill(ptsum,massT);
      LogTrace("Analysis") << "\t... Isol, Track pt= " << mu->pt() << " GeV, " << " ptsum = " << ptsum;
      if (ptsum/pt > isoCut03_) muon_sel = false;

      if (muon_sel) {
        nmuons++;
        if (pt > max_pt) {  //and identify the highest pt muon
           max_pt = pt;
           i_max_pt = nmuons;
        }
        pt_sel[nmuons] = pt;
        eta_sel[nmuons] = eta;
        acop_sel[nmuons] = acop;
        massT_sel[nmuons] = massT;
        iso_sel[nmuons] = ptsum;
        isoN_sel[nmuons] = ptsum/pt;
      }
   }


      LogTrace("Analysis") << "> Muon counts to reject Z= " << nmuonsForZ;
      if (nmuonsForZ>=2) {
            LogTrace("Analysis") << ">>>> Event REJECTED";
            event_sel = false;
      }
      LogTrace("Analysis") << "> Number of muons for W= " << nmuons;
      if (nmuons<1) {
            LogTrace("Analysis") << ">>>> Event REJECTED";
            event_sel = false;
      }

      if (event_sel == true) {
       LogTrace("Analysis") << ">>>> Event SELECTED!!!";

// Fill histograms for selected events
       hNMu_sel->Fill(nmuons);
       hMET_sel->Fill(met_et); 
       hNjets_sel->Fill(njets);

// only the combination with highest pt
       hPtMu_sel->Fill(pt_sel[i_max_pt]);
       hEtaMu_sel->Fill(eta_sel[i_max_pt]);
       hAcop_sel->Fill(acop_sel[i_max_pt]);
       hTMass_sel->Fill(massT_sel[i_max_pt]);
       hPtSum_sel->Fill(iso_sel[i_max_pt]);
       hPtSumN_sel->Fill(isoN_sel[i_max_pt]);
       hTMass_PtSum_sel->Fill(iso_sel[i_max_pt],massT_sel[i_max_pt]);

      }

  
}

DEFINE_FWK_MODULE(WMuNuAnalyzer);
