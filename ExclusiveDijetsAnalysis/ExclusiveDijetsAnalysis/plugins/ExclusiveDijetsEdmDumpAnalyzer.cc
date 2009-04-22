
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TTree;
class TH1F;
class TH2F;

class ExclusiveDijetsEdmDumpAnalyzer: public edm::EDAnalyzer
{
  public: 
    explicit ExclusiveDijetsEdmDumpAnalyzer(const edm::ParameterSet&);
    ~ExclusiveDijetsEdmDumpAnalyzer();

    virtual void beginJob(const edm::EventSetup&);
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
  private:
    edm::InputTag jetTag_;

    double ptJetMin_;
    double etaJetMax_;
    double deltaEtaMax_;
    double deltaPhiMax_;
    unsigned int nTracksMax_;
    unsigned int nHFPlusMax_;
    unsigned int nHFMinusMax_;

    unsigned int thresholdHF_;
    double Ebeam_;
 
    struct {
      TH1F* h_leadingJetPt_;
      TH1F* h_leadingJetEta_;
      TH1F* h_leadingJetPhi_;

      TH1F* h_secondJetPt_;
      TH1F* h_secondJetEta_;
      TH1F* h_secondJetPhi_;

      TH1F* h_jetsDeltaEta_;
      TH1F* h_jetsDeltaPhi_;

      TH1F* h_trackMultiplicity_;
      TH1F* h_multiplicityHFPlus_;
      TH1F* h_multiplicityHFMinus_;

      TH1F* h_xiGenPlus_;
      TH1F* h_xiGenMinus_;
      TH1F* h_xiPlus_;
      TH1F* h_xiMinus_;
      TH2F* h_xiTowerVsxiGenPlus_;
      TH2F* h_xiTowerVsxiGenMinus_;

      TH1F* h_massDijets_;
      TH1F* h_missingMassFromXi_;
      TH1F* h_MxFromJets_;
      TH1F* h_RjjFromJets_;

      TH1F* h_EnergyVsEta_;

      TH1F* h_xiPlusAfterSel_;
      TH1F* h_xiMinusAfterSel_;
      TH1F* h_RjjAfterSel_;
    } histos_;
};

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/JetReco/interface/Jet.h"
//#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace reco;

ExclusiveDijetsEdmDumpAnalyzer::ExclusiveDijetsEdmDumpAnalyzer(const edm::ParameterSet& pset):
  jetTag_(pset.getParameter<edm::InputTag>("JetTag")),
  ptJetMin_(pset.getParameter<double>("PtMinJet")),
  etaJetMax_(pset.getParameter<double>("EtaMaxJet")),
  deltaEtaMax_(pset.getParameter<double>("DeltaEtaMax")),
  deltaPhiMax_(pset.getParameter<double>("DeltaPhiMax")),
  nTracksMax_(pset.getParameter<unsigned int>("NTracksMax")),
  nHFPlusMax_(pset.getParameter<unsigned int>("NHFPlusMax")),
  nHFMinusMax_(pset.getParameter<unsigned int>("NHFMinusMax")),
  thresholdHF_(pset.getParameter<unsigned int>("HFThresholdIndex")),
  Ebeam_(5000.)
{}

ExclusiveDijetsEdmDumpAnalyzer::~ExclusiveDijetsEdmDumpAnalyzer() {}

void ExclusiveDijetsEdmDumpAnalyzer::beginJob(const edm::EventSetup& setup){
  edm::Service<TFileService> fs;

  histos_.h_leadingJetPt_ = fs->make<TH1F>("leadingJetPt","leadingJetPt",100,0.,100.);
  histos_.h_leadingJetEta_ = fs->make<TH1F>("leadingJetEta","leadingJetEta",100,-5.,5.);
  histos_.h_leadingJetPhi_ = fs->make<TH1F>("leadingJetPhi","leadingJetPhi",100,-1.1*M_PI,1.1*M_PI);

  histos_.h_secondJetPt_ = fs->make<TH1F>("secondJetPt","secondJetPt",100,0.,100.);
  histos_.h_secondJetEta_ = fs->make<TH1F>("secondJetEta","secondJetEta",100,-5.,5.);
  histos_.h_secondJetPhi_ = fs->make<TH1F>("secondJetPhi","secondJetPhi",100,-1.1*M_PI,1.1*M_PI);

  histos_.h_jetsDeltaEta_ = fs->make<TH1F>("jetsDeltaEta","jetsDeltaEta",100,-5.,5.);
  histos_.h_jetsDeltaPhi_ = fs->make<TH1F>("jetsDeltaPhi","jetsDeltaPhi",100,0.,1.1*M_PI);

  histos_.h_trackMultiplicity_ = fs->make<TH1F>("trackMultiplicity","trackMultiplicity",20,0,20);
  histos_.h_multiplicityHFPlus_ = fs->make<TH1F>("multiplicityHFPlus","multiplicityHFPlus",20,0,20);
  histos_.h_multiplicityHFMinus_ = fs->make<TH1F>("multiplicityHFMinus","multiplicityHFMinus",20,0,20);

  histos_.h_xiGenPlus_ = fs->make<TH1F>("xiGenPlus","xiGenPlus",200,0.,1.);
  histos_.h_xiGenMinus_ = fs->make<TH1F>("xiGenMinus","xiGenMinus",200,0.,1.);
  histos_.h_xiPlus_ = fs->make<TH1F>("xiPlus","xiPlus",200,0.,1.);
  histos_.h_xiMinus_ = fs->make<TH1F>("xiMinus","xiMinus",200,0.,1.);
  histos_.h_xiTowerVsxiGenPlus_ = fs->make<TH2F>("xiTowerVsxiGenPlus","xiTowerVsxiGenPlus",100,0.,1.,100,0.,1.);
  histos_.h_xiTowerVsxiGenMinus_ = fs->make<TH2F>("xiTowerVsxiGenMinus","xiTowerVsxiGenMinus",100,0.,1.,100,0.,1.);

  histos_.h_massDijets_ = fs->make<TH1F>("massDijets","massDijets",200,-10.,200.);
  histos_.h_missingMassFromXi_ = fs->make<TH1F>("missingMassFromXi","missingMassFromXi",200,-10.,200.);
  histos_.h_MxFromJets_ = fs->make<TH1F>("MxFromJets","MxFromJets",200,-10.,200.);
  histos_.h_RjjFromJets_ = fs->make<TH1F>("RjjFromJets","RjjFromJets",200,-0.1,1.5);

  histos_.h_EnergyVsEta_ = fs->make<TH1F>("EnergyVsEta","EnergyVsEta",300,-15.,15.);

  histos_.h_xiPlusAfterSel_ = fs->make<TH1F>("xiPlusAfterSel","xiPlusAfterSel",200,0.,1.);
  histos_.h_xiMinusAfterSel_ = fs->make<TH1F>("xiMinusAfterSel","xiMinusAfterSel",200,0.,1.);
  histos_.h_RjjAfterSel_ = fs->make<TH1F>("RjjAfterSel","RjjAfterSel",200,-0.1,1.5);

}

void ExclusiveDijetsEdmDumpAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);

  const reco::Jet& jet1 = (*jetCollectionH)[0];// they come out ordered right?
  const reco::Jet& jet2 = (*jetCollectionH)[1];

  histos_.h_leadingJetPt_->Fill(jet1.pt());
  histos_.h_secondJetPt_->Fill(jet2.pt());
  if(jet1.pt() < ptJetMin_) return;
  if(jet2.pt() < ptJetMin_) return;

  histos_.h_leadingJetEta_->Fill(jet1.eta());
  histos_.h_secondJetEta_->Fill(jet2.eta());
  if(fabs(jet1.eta()) > etaJetMax_) return;
  if(fabs(jet2.eta()) > etaJetMax_) return; 

  histos_.h_leadingJetPhi_->Fill(jet1.phi());
  histos_.h_secondJetPhi_->Fill(jet2.phi());

  histos_.h_jetsDeltaEta_->Fill(jet1.eta() - jet2.eta());
  histos_.h_jetsDeltaPhi_->Fill(M_PI - fabs(jet1.phi() - jet2.phi()));
 
  if(fabs(jet1.eta() - jet2.eta()) > deltaEtaMax_) return;
  if((M_PI - fabs(jet1.phi() - jet2.phi())) > deltaPhiMax_) return;

  math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
  dijetSystem += jet1.p4();
  dijetSystem += jet2.p4();
  histos_.h_massDijets_->Fill(dijetSystem.M());

  math::XYZTLorentzVector allJets(0.,0.,0.,0.);
  for(edm::View<Jet>::const_iterator jet = jetCollectionH->begin();
                                     jet != jetCollectionH->end(); ++jet) allJets += jet->p4();

  histos_.h_MxFromJets_->Fill(allJets.M());

  double RjjFromJets = dijetSystem.M()/allJets.M();
  histos_.h_RjjFromJets_->Fill(RjjFromJets);
     
  // Gen particles
  edm::Handle<edm::View<GenParticle> > genParticlesCollectionH;
  event.getByLabel("genParticles",genParticlesCollectionH);

  edm::View<GenParticle>::const_iterator proton1 = genParticlesCollectionH->end();
  edm::View<GenParticle>::const_iterator proton2 = genParticlesCollectionH->end();
  for(edm::View<GenParticle>::const_iterator genpart = genParticlesCollectionH->begin();
                                             genpart != genParticlesCollectionH->end(); ++genpart){
    if(genpart->status() != 1) continue;
    histos_.h_EnergyVsEta_->Fill(genpart->eta(),genpart->energy());	
		
    double pz = genpart->pz();
    if((proton1 == genParticlesCollectionH->end())&&(genpart->pdgId() == 2212)&&(pz > 0.75*Ebeam_)) proton1 = genpart;
    else if((proton2 == genParticlesCollectionH->end())&&(genpart->pdgId() == 2212)&&(pz < -0.75*Ebeam_)) proton2 = genpart;
  }

  double xigen_plus = -1;
  double xigen_minus = -1;
  if((proton1 != genParticlesCollectionH->end())&&(proton2 != genParticlesCollectionH->end())){
    LogTrace("Analysis") << "Proton (z-plus): " << proton1->pt() << "  " << proton1->eta() << "  " << proton1->phi() << std::endl;
    LogTrace("Analysis") << "Proton (z-minus): " << proton2->pt() << "  " << proton2->eta() << "  " << proton2->phi() << std::endl;
    xigen_plus = 1 - proton1->pz()/Ebeam_;
    xigen_minus = 1 + proton2->pz()/Ebeam_;

    histos_.h_xiGenPlus_->Fill(xigen_plus);
    histos_.h_xiGenMinus_->Fill(xigen_minus); 
  }

  // Access multiplicities
  edm::Handle<unsigned int> trackMultiplicity; 
  event.getByLabel("trackMultiplicityAssociatedToPV","trackMultiplicity",trackMultiplicity);

  edm::Handle<std::vector<unsigned int> > nHFPlus;
  event.getByLabel("hfTower","nHFplus",nHFPlus);
  
  edm::Handle<std::vector<unsigned int> > nHFMinus;
  event.getByLabel("hfTower","nHFminus",nHFMinus);

  edm::Handle<double> xiTowerPlus;
  event.getByLabel("xiTower","xiTowerplus",xiTowerPlus);

  edm::Handle<double> xiTowerMinus;
  event.getByLabel("xiTower","xiTowerminus",xiTowerMinus);

  unsigned int nTracks = *trackMultiplicity;

  unsigned int nHF_plus = (*nHFPlus)[thresholdHF_];
  unsigned int nHF_minus = (*nHFMinus)[thresholdHF_];

  double xiTower_plus = *xiTowerPlus;
  double xiTower_minus = *xiTowerMinus;
 
  histos_.h_trackMultiplicity_->Fill(nTracks);
  histos_.h_multiplicityHFPlus_->Fill(nHF_plus);
  histos_.h_multiplicityHFMinus_->Fill(nHF_minus);     

  histos_.h_xiPlus_->Fill(xiTower_plus);
  histos_.h_xiMinus_->Fill(xiTower_minus);
  if((proton1 != genParticlesCollectionH->end())&&(proton2 != genParticlesCollectionH->end())){
    histos_.h_xiTowerVsxiGenPlus_->Fill(xigen_plus,xiTower_plus);
    histos_.h_xiTowerVsxiGenMinus_->Fill(xigen_minus,xiTower_minus);
  }

  double missingMass = 2*Ebeam_*sqrt(xiTower_plus*xiTower_minus);
  histos_.h_missingMassFromXi_->Fill(missingMass);

  // Selection
  if(nTracks > nTracksMax_) return;
  if(nHF_plus > nHFPlusMax_) return;
  if(nHF_minus > nHFMinusMax_) return;

  histos_.h_xiPlusAfterSel_->Fill(xiTower_plus);
  histos_.h_xiMinusAfterSel_->Fill(xiTower_minus);
  histos_.h_RjjAfterSel_->Fill(RjjFromJets);

}

DEFINE_FWK_MODULE(ExclusiveDijetsEdmDumpAnalyzer);
