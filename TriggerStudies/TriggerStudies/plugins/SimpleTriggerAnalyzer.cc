
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include <vector>
#include <string>
#include <cmath>

class TFileService;
class TH1F;
//class TH2F;

class SimpleTriggerAnalyzer: public edm::EDAnalyzer
{
  public:
    typedef std::map<std::string,TH1F*> HistoMapTH1F;
 
    explicit SimpleTriggerAnalyzer(edm::ParameterSet const&);
    ~SimpleTriggerAnalyzer();

    void beginJob();
    void analyze(edm::Event const&, edm::EventSetup const&);
  private:
    void bookHistos(HistoMapTH1F&, edm::Service<TFileService> const&);    

    L1GtUtils l1GtUtils_;

    edm::InputTag gtDigisTag_;
    edm::InputTag l1ExtraParticlesTag_;
    edm::InputTag hltJetTag_;

    std::vector<std::string> l1TriggerNames_;
    double l1SingleJetEtMin_;
    double l1DoubleJetEtMin_;
    double minPt_;
    double maxPt_;
    unsigned int nBinsPt_;
    double minEta_;
    double maxEta_;
    unsigned int nBinsEta_;
    double minPhi_;
    double maxPhi_;
    unsigned int nBinsPhi_;

    HistoMapTH1F histosTH1F_;
    //HistoMapTH2F histosTH2F_;
};

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

SimpleTriggerAnalyzer::SimpleTriggerAnalyzer(edm::ParameterSet const& pset):
  gtDigisTag_(pset.getParameter<edm::InputTag>("gtDigisTag")),
  l1ExtraParticlesTag_(pset.getParameter<edm::InputTag>("l1ExtraParticlesTag")),
  hltJetTag_(pset.getParameter<edm::InputTag>("hltJetTag")),
  l1TriggerNames_(pset.getParameter<std::vector<std::string> >("l1TriggerNames")),
  l1SingleJetEtMin_(pset.getParameter<double>("l1SingleJetEtMin")),
  l1DoubleJetEtMin_(pset.getParameter<double>("l1DoubleJetEtMin")),
  minPt_(pset.getParameter<double>("minPt")),
  maxPt_(pset.getParameter<double>("maxPt")),  
  nBinsPt_(pset.getParameter<unsigned int>("nBinsPt")),
  minEta_(pset.getParameter<double>("minEta")),          
  maxEta_(pset.getParameter<double>("maxEta")),
  nBinsEta_(pset.getParameter<unsigned int>("nBinsEta")),
  minPhi_(pset.getParameter<double>("minPhi")),          
  maxPhi_(pset.getParameter<double>("maxPhi")),
  nBinsPhi_(pset.getParameter<unsigned int>("nBinsPhi")) {}

SimpleTriggerAnalyzer::~SimpleTriggerAnalyzer(){}

void SimpleTriggerAnalyzer::beginJob(){
  edm::Service<TFileService> fs;

  bool sumw2 = TH1::GetDefaultSumw2();
  TH1::SetDefaultSumw2(false);

  bookHistos(histosTH1F_,fs);
  //bookHistos(histosTH2F_,fs);
  for(HistoMapTH1F::const_iterator it = histosTH1F_.begin(); it != histosTH1F_.end(); ++it) it->second->Sumw2();

  TH1::SetDefaultSumw2(sumw2);
}

void SimpleTriggerAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){
  //l1GtUtils_.retrieveL1EventSetup(setup);
  bool useL1EventSetup = true;
  bool useL1GtTriggerMenuLite = false;
  l1GtUtils_.getL1GtRunCache(event,setup,useL1EventSetup,useL1GtTriggerMenuLite);

  std::vector<std::string> passedL1;
  std::vector<int> prescalesL1;
  for(size_t k = 0; k < l1TriggerNames_.size(); ++k){
     edm::LogVerbatim("Analysis") << "Accessing L1 trigger " << l1TriggerNames_[k];

     int ierror = -1;
     int ierr_pr = -1;
     bool decision = false;
     if(gtDigisTag_.label() != "")
	decision = l1GtUtils_.decisionBeforeMask(event,gtDigisTag_,gtDigisTag_,l1TriggerNames_[k],ierror);
     else
	decision = l1GtUtils_.decisionBeforeMask(event,l1TriggerNames_[k],ierror);

     //int l1decision = -1;
     //if(ierror == 0) l1decision = decision ? 1 : 0;

     std::stringstream ss_acc;
     ss_acc << "l1Accept_" << l1TriggerNames_[k];
     histosTH1F_[ ss_acc.str() ]->Fill( decision ? 1 : 0 );

     int prescale = -1;
     if(ierror == 0){
	if(gtDigisTag_.label() != "")
	   prescale = l1GtUtils_.prescaleFactor(event,gtDigisTag_,gtDigisTag_,l1TriggerNames_[k],ierr_pr);
	else 
	   prescale = l1GtUtils_.prescaleFactor(event,l1TriggerNames_[k],ierr_pr);
     }
    
     std::stringstream ss_pcl;
     ss_pcl << "l1Prescale_" << l1TriggerNames_[k];
     histosTH1F_[ ss_pcl.str() ]->Fill( prescale );

     if(ierror == 0 && decision){
        passedL1.push_back( l1TriggerNames_[k] );
        prescalesL1.push_back(prescale);
     }
  }

  for(size_t k = 0; k < passedL1.size(); ++k)
     edm::LogVerbatim("Analysis") << "Passed L1 trigger " << passedL1[k] << " prescale " << prescalesL1[k];

  bool acceptL1Trigger = false;
  for(size_t ibit = 0; ibit < l1TriggerNames_.size(); ++ibit){
     if( find(passedL1.begin(),passedL1.end(),l1TriggerNames_[ibit]) != passedL1.end() ){
        acceptL1Trigger = true;
        break; 
     }
  }

  //============
  edm::Handle<l1extra::L1JetParticleCollection> l1ExtraJetCollectionCentralH;
  event.getByLabel( edm::InputTag(l1ExtraParticlesTag_.label(),"Central"), l1ExtraJetCollectionCentralH);
  const l1extra::L1JetParticleCollection& l1ExtraJetCollCentral = *l1ExtraJetCollectionCentralH;

  edm::LogVerbatim("Analysis") << "L1 Extra jet particle collection size: " << l1ExtraJetCollCentral.size();

  l1extra::L1JetParticleCollection::const_iterator l1jet = l1ExtraJetCollCentral.begin();
  l1extra::L1JetParticleCollection::const_iterator l1jets_end = l1ExtraJetCollCentral.end();
  for(; l1jet != l1jets_end; ++l1jet){
     histosTH1F_["l1JetCentralPt"]->Fill( l1jet->pt() );
     histosTH1F_["l1JetCentralEta"]->Fill( l1jet->eta() );
     histosTH1F_["l1JetCentralPhi"]->Fill( l1jet->phi() );
  }

  //============
  edm::Handle<reco::CaloJetCollection> hltJetCollectionH;
  event.getByLabel(hltJetTag_,hltJetCollectionH);
  const reco::CaloJetCollection& hltJetColl = *hltJetCollectionH;
     
  reco::CaloJetCollection::const_iterator jet = hltJetColl.begin();
  reco::CaloJetCollection::const_iterator jets_end = hltJetColl.end();
  for(; jet != jets_end; ++jet){
     // Any selection here

     histosTH1F_["hltJetPt"]->Fill( jet->pt() );
     histosTH1F_["hltJetEta"]->Fill( jet->eta() );
     histosTH1F_["hltJetPhi"]->Fill( jet->phi() );
  
     if( acceptL1Trigger ){
	histosTH1F_["hltJetPtL1Trigger"]->Fill( jet->pt() );
	histosTH1F_["hltJetEtaL1Trigger"]->Fill( jet->eta() );
	histosTH1F_["hltJetPhiL1Trigger"]->Fill( jet->phi() );
     }
  }
  if( hltJetColl.size() >= 1){
     reco::CaloJet const& leadingJet = hltJetColl[0];
     histosTH1F_["hltLeadingJetPt"]->Fill( leadingJet.pt() );
     histosTH1F_["hltLeadingJetEta"]->Fill( leadingJet.eta() );
     histosTH1F_["hltLeadingJetPhi"]->Fill( leadingJet.phi() );
     if( acceptL1Trigger ){
	histosTH1F_["hltLeadingJetPtL1Trigger"]->Fill( leadingJet.pt() );
	histosTH1F_["hltLeadingJetEtaL1Trigger"]->Fill( leadingJet.eta() );
	histosTH1F_["hltLeadingJetPhiL1Trigger"]->Fill( leadingJet.phi() );
     }
     if( hltJetColl.size() >= 2){
	reco::CaloJet const& secondJet = hltJetColl[1];
	histosTH1F_["hltSecondJetPt"]->Fill( secondJet.pt() );
	histosTH1F_["hltSecondJetEta"]->Fill( secondJet.eta() );
	histosTH1F_["hltSecondJetPhi"]->Fill( secondJet.phi() );
	if( acceptL1Trigger ){
	   histosTH1F_["hltSecondJetPtL1Trigger"]->Fill( secondJet.pt() );
	   histosTH1F_["hltSecondJetEtaL1Trigger"]->Fill( secondJet.eta() );
	   histosTH1F_["hltSecondJetPhiL1Trigger"]->Fill( secondJet.phi() );
	}
	if( leadingJet.pt() >= 60. ){
	   histosTH1F_["hltSecondJetPtLeadingJetPt60"]->Fill( secondJet.pt() );
	   histosTH1F_["hltSecondJetEtaLeadingJetPt60"]->Fill( secondJet.eta() );
	   histosTH1F_["hltSecondJetPhiLeadingJetPt60"]->Fill( secondJet.phi() );
	}
	if( acceptL1Trigger && leadingJet.pt() >= 60. ){
	   histosTH1F_["hltSecondJetPtL1TriggerLeadingJetPt60"]->Fill( secondJet.pt() );
	   histosTH1F_["hltSecondJetEtaL1TriggerLeadingJetPt60"]->Fill( secondJet.eta() );
	   histosTH1F_["hltSecondJetPhiL1TriggerLeadingJetPt60"]->Fill( secondJet.phi() );
	}
     }
  }
}

void SimpleTriggerAnalyzer::bookHistos(HistoMapTH1F& histos, edm::Service<TFileService> const& fs){
  for(size_t k = 0; k < l1TriggerNames_.size(); ++k){
     std::stringstream ss_acc;
     ss_acc << "l1Accept_" << l1TriggerNames_[k];
     histos[ ss_acc.str() ] = fs->make<TH1F>(ss_acc.str().c_str(),"accept",2,0,2);

     std::stringstream ss_pcl;
     ss_pcl << "l1Prescale_" << l1TriggerNames_[k];
     histos[ ss_pcl.str() ] = fs->make<TH1F>(ss_pcl.str().c_str(),"prescale",1000,0,1000);
  }

  histos["l1JetCentralEta"] = fs->make<TH1F>("l1JetCentralEta","eta",50,-5.2,5.2);
  histos["l1JetCentralPt"] = fs->make<TH1F>("l1JetCentralPt","pt",50,0.,100.);
  histos["l1JetCentralEta"] = fs->make<TH1F>("l1JetCentralEta","eta",50,-5.2,5.2);
  histos["l1JetCentralPhi"] = fs->make<TH1F>("l1JetCentralPhi","phi",50,-M_PI,M_PI);
  histos["l1JetForwardPt"] = fs->make<TH1F>("l1JetForwardPt","pt",50,0.,100.);
  histos["l1JetForwardEta"] = fs->make<TH1F>("l1JetForwardEta","eta",50,-5.2,5.2);
  histos["l1JetForwardPhi"] = fs->make<TH1F>("l1JetForwardPhi","phi",50,-M_PI,M_PI);
  histos["l1JetTauPt"] = fs->make<TH1F>("l1JetTauPt","pt",50,0.,100.);
  histos["l1JetTauEta"] = fs->make<TH1F>("l1JetTauEta","eta",50,-5.2,5.2);
  histos["l1JetTauPhi"] = fs->make<TH1F>("l1JetTauPhi","phi",50,-M_PI,M_PI);

  histos["hltJetPt"] = fs->make<TH1F>("hltJetPt","pt",nBinsPt_,minPt_,maxPt_);
  histos["hltJetEta"] = fs->make<TH1F>("hltJetEta","eta",nBinsEta_,minEta_,maxEta_);
  histos["hltJetPhi"] = fs->make<TH1F>("hltJetPhi","phi",nBinsPhi_,minPhi_,maxPhi_);
  histos["hltJetPtL1Trigger"] = fs->make<TH1F>("hltJetPtL1Trigger","pt",nBinsPt_,minPt_,maxPt_);
  histos["hltJetEtaL1Trigger"] = fs->make<TH1F>("hltJetEtaL1Trigger","eta",nBinsEta_,minEta_,maxEta_);
  histos["hltJetPhiL1Trigger"] = fs->make<TH1F>("hltJetPhiL1Trigger","phi",nBinsPhi_,minPhi_,maxPhi_);
  histos["hltLeadingJetPt"] = fs->make<TH1F>("hltLeadingJetPt","pt",nBinsPt_,minPt_,maxPt_);
  histos["hltLeadingJetEta"] = fs->make<TH1F>("hltLeadingJetEta","eta",nBinsEta_,minEta_,maxEta_);
  histos["hltLeadingJetPhi"] = fs->make<TH1F>("hltLeadingJetPhi","phi",nBinsPhi_,minPhi_,maxPhi_);
  histos["hltLeadingJetPtL1Trigger"] = fs->make<TH1F>("hltLeadingJetPtL1Trigger","pt",nBinsPt_,minPt_,maxPt_);
  histos["hltLeadingJetEtaL1Trigger"] = fs->make<TH1F>("hltLeadingJetEtaL1Trigger","eta",nBinsEta_,minEta_,maxEta_);
  histos["hltLeadingJetPhiL1Trigger"] = fs->make<TH1F>("hltLeadingJetPhiL1Trigger","phi",nBinsPhi_,minPhi_,maxPhi_);
  histos["hltSecondJetPt"] = fs->make<TH1F>("hltSecondJetPt","pt",nBinsPt_,minPt_,maxPt_);
  histos["hltSecondJetEta"] = fs->make<TH1F>("hltSecondJetEta","eta",nBinsEta_,minEta_,maxEta_);
  histos["hltSecondJetPhi"] = fs->make<TH1F>("hltSecondJetPhi","phi",nBinsPhi_,minPhi_,maxPhi_);
  histos["hltSecondJetPtL1Trigger"] = fs->make<TH1F>("hltSecondJetPtL1Trigger","pt",nBinsPt_,minPt_,maxPt_);
  histos["hltSecondJetEtaL1Trigger"] = fs->make<TH1F>("hltSecondJetEtaL1Trigger","eta",nBinsEta_,minEta_,maxEta_);
  histos["hltSecondJetPhiL1Trigger"] = fs->make<TH1F>("hltSecondJetPhiL1Trigger","phi",nBinsPhi_,minPhi_,maxPhi_);
  histos["hltSecondJetPtLeadingJetPt60"] = fs->make<TH1F>("hltSecondJetPtLeadingJetPt60","pt",nBinsPt_,minPt_,maxPt_);
  histos["hltSecondJetEtaLeadingJetPt60"] = fs->make<TH1F>("hltSecondJetEtaLeadingJetPt60","eta",nBinsEta_,minEta_,maxEta_);
  histos["hltSecondJetPhiLeadingJetPt60"] = fs->make<TH1F>("hltSecondJetPhiLeadingJetPt60","phi",nBinsPhi_,minPhi_,maxPhi_);
  histos["hltSecondJetPtL1TriggerLeadingJetPt60"] = fs->make<TH1F>("hltSecondJetPtL1TriggerLeadingJetPt60","pt",nBinsPt_,minPt_,maxPt_);
  histos["hltSecondJetEtaL1TriggerLeadingJetPt60"] = fs->make<TH1F>("hltSecondJetEtaL1TriggerLeadingJetPt60","eta",nBinsEta_,minEta_,maxEta_);
  histos["hltSecondJetPhiL1TriggerLeadingJetPt60"] = fs->make<TH1F>("hltSecondJetPhiL1TriggerLeadingJetPt60","phi",nBinsPhi_,minPhi_,maxPhi_);
}

DEFINE_FWK_MODULE(SimpleTriggerAnalyzer);
