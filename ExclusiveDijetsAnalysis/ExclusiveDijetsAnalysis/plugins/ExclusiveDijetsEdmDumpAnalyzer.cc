
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TTree;
class TH1F;
class TH2F;

class JetCorrector;

class ExclusiveDijetsEdmDumpAnalyzer: public edm::EDAnalyzer
{
  public: 
    explicit ExclusiveDijetsEdmDumpAnalyzer(const edm::ParameterSet&);
    ~ExclusiveDijetsEdmDumpAnalyzer();

    virtual void beginJob(const edm::EventSetup&);
    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
  private:
    template <class Coll>
    std::pair<double,double> xi(Coll& partCollection,bool useJetCorr = false);

    template <class JetColl,class PartColl>
    double Rjj(JetColl& jetCollection,PartColl& partCollection);

    edm::InputTag jetTag_;
    edm::InputTag particleFlowTag_;

    double ptJetMin_;
    double etaJetMax_;
    double deltaEtaMax_;
    double deltaPhiMax_;
    double deltaPtMax_;
    unsigned int nTracksMax_;
    unsigned int nHFPlusMax_;
    unsigned int nHFMinusMax_;

    unsigned int thresholdHF_;
    bool useJetCorrection_;
    double Ebeam_;
    bool usePAT_; 

    struct {
      TH1F* h_leadingJetPt_;
      TH1F* h_leadingJetEta_;
      TH1F* h_leadingJetPhi_;

      TH1F* h_secondJetPt_;
      TH1F* h_secondJetEta_;
      TH1F* h_secondJetPhi_;

      TH1F* h_jetsDeltaEta_;
      TH1F* h_jetsDeltaPhi_;
      TH1F* h_jetsDeltaPt_;

      TH1F* h_trackMultiplicity_;
      TH1F* h_multiplicityHFPlus_;
      TH1F* h_multiplicityHFMinus_;

      TH1F* h_xiGenPlus_;
      TH1F* h_xiGenMinus_;
      TH1F* h_xiPlus_;
      TH1F* h_xiMinus_;
      TH1F* h_xiPlusFromJets_;
      TH1F* h_xiMinusFromJets_;
      TH1F* h_ResXiPlus_;
      TH1F* h_ResXiMinus_;
      TH1F* h_ResXiPlusFromJets_;
      TH1F* h_ResXiMinusFromJets_;
      TH2F* h_xiPlusVsxiGenPlus_;
      TH2F* h_xiMinusVsxiGenMinus_;
      
      TH1F* h_massDijets_;
      TH1F* h_missingMassFromXi_;
      TH1F* h_MxFromJets_;
      TH1F* h_RjjFromJets_;
      TH1F* h_RjjFromPFCands_;
      TH1F* h_ResMassDijets_;

      TH1F* h_EnergyVsEta_;

      TH1F* h_xiPlusAfterSel_;
      TH1F* h_xiMinusAfterSel_;
      TH1F* h_RjjAfterSel_;
    } histos_;

    std::string jetCorrectionService_;
    const JetCorrector* corrector_;
};

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/JetReco/interface/Jet.h"
//#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

ExclusiveDijetsEdmDumpAnalyzer::ExclusiveDijetsEdmDumpAnalyzer(const edm::ParameterSet& pset):
  jetTag_(pset.getParameter<edm::InputTag>("JetTag")),
  particleFlowTag_(pset.getParameter<edm::InputTag>("ParticleFlowTag")),
  ptJetMin_(pset.getParameter<double>("PtMinJet")),
  etaJetMax_(pset.getParameter<double>("EtaMaxJet")),
  deltaEtaMax_(pset.getParameter<double>("DeltaEtaMax")),
  deltaPhiMax_(pset.getParameter<double>("DeltaPhiMax")),
  deltaPtMax_(pset.getParameter<double>("DeltaPtMax")),
  nTracksMax_(pset.getParameter<unsigned int>("NTracksMax")),
  nHFPlusMax_(pset.getParameter<unsigned int>("NHFPlusMax")),
  nHFMinusMax_(pset.getParameter<unsigned int>("NHFMinusMax")),
  thresholdHF_(pset.getParameter<unsigned int>("HFThresholdIndex")),
  useJetCorrection_(pset.getParameter<bool>("UseJetCorrection")),
  Ebeam_(pset.getUntrackedParameter<double>("EBeam",5000.)),
  usePAT_(pset.getUntrackedParameter<bool>("UsePAT",true))
{
  if(useJetCorrection_) jetCorrectionService_ = pset.getParameter<std::string>("JetCorrectionService");
}

ExclusiveDijetsEdmDumpAnalyzer::~ExclusiveDijetsEdmDumpAnalyzer(){}

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
  histos_.h_jetsDeltaPt_ = fs->make<TH1F>("jetsDeltaPt","jetsDeltaPt",100,0.,10.);

  histos_.h_trackMultiplicity_ = fs->make<TH1F>("trackMultiplicity","trackMultiplicity",20,0,20);
  histos_.h_multiplicityHFPlus_ = fs->make<TH1F>("multiplicityHFPlus","multiplicityHFPlus",20,0,20);
  histos_.h_multiplicityHFMinus_ = fs->make<TH1F>("multiplicityHFMinus","multiplicityHFMinus",20,0,20);

  histos_.h_xiGenPlus_ = fs->make<TH1F>("xiGenPlus","xiGenPlus",200,0.,1.);
  histos_.h_xiGenMinus_ = fs->make<TH1F>("xiGenMinus","xiGenMinus",200,0.,1.);
  histos_.h_xiPlus_ = fs->make<TH1F>("xiPlus","xiPlus",200,0.,1.);
  histos_.h_xiMinus_ = fs->make<TH1F>("xiMinus","xiMinus",200,0.,1.);
  histos_.h_xiPlusFromJets_ = fs->make<TH1F>("xiPlusFromJets","xiPlusFromJets",200,0.,1.);
  histos_.h_xiMinusFromJets_ = fs->make<TH1F>("xiMinusFromJets","xiMinusFromJets",200,0.,1.);
  histos_.h_ResXiPlus_ = fs->make<TH1F>("ResXiPlus","ResXiPlus",100,-1.,1.);
  histos_.h_ResXiMinus_ = fs->make<TH1F>("ResXiMinus","ResXiMinus",100,-1.,1.);
  histos_.h_ResXiPlusFromJets_ = fs->make<TH1F>("ResXiPlusFromJets","ResXiPlusFromJets",100,-1.,1.);
  histos_.h_ResXiMinusFromJets_ = fs->make<TH1F>("ResXiMinusFromJets","ResXiMinusFromJets",100,-1.,1.);
  histos_.h_xiPlusVsxiGenPlus_ = fs->make<TH2F>("xiPlusVsxiGenPlus","xiPlusVsxiGenPlus",100,0.,1.,100,0.,1.);
  histos_.h_xiMinusVsxiGenMinus_ = fs->make<TH2F>("xiMinusVsxiGenMinus","xiMinusVsxiGenMinus",100,0.,1.,100,0.,1.);

  histos_.h_massDijets_ = fs->make<TH1F>("massDijets","massDijets",200,-10.,400.);
  histos_.h_missingMassFromXi_ = fs->make<TH1F>("missingMassFromXi","missingMassFromXi",200,-10.,400.);
  histos_.h_MxFromJets_ = fs->make<TH1F>("MxFromJets","MxFromJets",200,-10.,400.);
  histos_.h_RjjFromJets_ = fs->make<TH1F>("RjjFromJets","RjjFromJets",200,-0.1,1.5);
  histos_.h_RjjFromPFCands_ = fs->make<TH1F>("RjjFromPFCands","RjjFromPFCands",200,-0.1,1.5);
  histos_.h_ResMassDijets_ = fs->make<TH1F>("ResMassDijets","ResMassDijets",100,-1.,1.);

  histos_.h_EnergyVsEta_ = fs->make<TH1F>("EnergyVsEta","EnergyVsEta",300,-15.,15.);

  histos_.h_xiPlusAfterSel_ = fs->make<TH1F>("xiPlusAfterSel","xiPlusAfterSel",200,0.,1.);
  histos_.h_xiMinusAfterSel_ = fs->make<TH1F>("xiMinusAfterSel","xiMinusAfterSel",200,0.,1.);
  histos_.h_RjjAfterSel_ = fs->make<TH1F>("RjjAfterSel","RjjAfterSel",200,-0.1,1.5);

}

void ExclusiveDijetsEdmDumpAnalyzer::beginRun(const edm::Run& run, const edm::EventSetup& setup){
  if(useJetCorrection_) corrector_ = JetCorrector::getJetCorrector(jetCorrectionService_,setup);
}

void ExclusiveDijetsEdmDumpAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);

  const reco::Jet& jet1 = (*jetCollectionH)[0];// they come out ordered right?
  const reco::Jet& jet2 = (*jetCollectionH)[1];

  /*edm::Handle<edm::View<pat::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);

  const pat::Jet& jet1 = (*jetCollectionH)[0];// they come out ordered right?
  const pat::Jet& jet2 = (*jetCollectionH)[1];*/

  edm::Handle<edm::View<reco::PFCandidate> > particleFlowCollectionH;
  event.getByLabel("particleFlow",particleFlowCollectionH);

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
  histos_.h_jetsDeltaPt_->Fill(fabs(jet1.pt() - jet2.pt())); 

  if(fabs(jet1.eta() - jet2.eta()) > deltaEtaMax_) return;
  if((M_PI - fabs(jet1.phi() - jet2.phi())) > deltaPhiMax_) return;
  if(fabs(jet1.pt() - jet2.pt()) > deltaPtMax_) return;

  math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
  dijetSystem += jet1.p4();
  dijetSystem += jet2.p4();
  histos_.h_massDijets_->Fill(dijetSystem.M());

  /*math::XYZTLorentzVector allJets(0.,0.,0.,0.);
  for(edm::View<reco::Jet>::const_iterator jet = jetCollectionH->begin();
                                           jet != jetCollectionH->end(); ++jet) allJets += jet->p4();

  histos_.h_MxFromJets_->Fill(allJets.M());

  double RjjFromJets = dijetSystem.M()/allJets.M();
  histos_.h_RjjFromJets_->Fill(RjjFromJets);*/
    
  double RjjFromJets = Rjj(*jetCollectionH,*jetCollectionH);
  histos_.h_RjjFromJets_->Fill(RjjFromJets);
  
  edm::Handle<edm::View<reco::Jet> > jetCollectionNonCorrH;
  event.getByLabel("sisCone7PFJets",jetCollectionNonCorrH);
  histos_.h_RjjFromPFCands_->Fill(Rjj(*jetCollectionNonCorrH,*particleFlowCollectionH));

  if(usePAT_){
    const pat::Jet* patJet1 = dynamic_cast<const pat::Jet*>(&jet1);
    const pat::Jet* patJet2 = dynamic_cast<const pat::Jet*>(&jet2);
    if(!patJet1 || !patJet2) throw edm::Exception(edm::errors::Configuration) << "Expecting PATJet's as input";

    const reco::GenJet* genJet1 = patJet1->genJet();
    const reco::GenJet* genJet2 = patJet2->genJet();
    /*const reco::GenJet* genJet1 = jet1.genJet();
    const reco::GenJet* genJet2 = jet2.genJet();*/
    if(genJet1&&genJet2){
      math::XYZTLorentzVector dijetGenSystem(0.,0.,0.,0.);
      dijetGenSystem += genJet1->p4();
      dijetGenSystem += genJet2->p4();
      double massGen = dijetGenSystem.M();
      histos_.h_ResMassDijets_->Fill((dijetSystem.M() - massGen)/massGen);
    }
  }

  // Gen particles
  edm::Handle<edm::View<reco::GenParticle> > genParticlesCollectionH;
  event.getByLabel("genParticles",genParticlesCollectionH);

  edm::View<reco::GenParticle>::const_iterator proton1 = genParticlesCollectionH->end();
  edm::View<reco::GenParticle>::const_iterator proton2 = genParticlesCollectionH->end();
  for(edm::View<reco::GenParticle>::const_iterator genpart = genParticlesCollectionH->begin();
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
  event.getByLabel("trackMultiplicityTransverseRegion","trackMultiplicity",trackMultiplicity);

  edm::Handle<std::vector<unsigned int> > nHFPlus;
  event.getByLabel("hfTower","nHFplus",nHFPlus);
  
  edm::Handle<std::vector<unsigned int> > nHFMinus;
  event.getByLabel("hfTower","nHFminus",nHFMinus);

  /*edm::Handle<double> xiTowerPlus;
  event.getByLabel("xiTower","xiTowerplus",xiTowerPlus);

  edm::Handle<double> xiTowerMinus;
  event.getByLabel("xiTower","xiTowerminus",xiTowerMinus);*/

  unsigned int nTracks = *trackMultiplicity;

  unsigned int nHF_plus = (*nHFPlus)[thresholdHF_];
  unsigned int nHF_minus = (*nHFMinus)[thresholdHF_];

  /*double xi_plus = *xiTowerPlus;
  double xi_minus = *xiTowerMinus;*/
 
  std::pair<double,double> xiFromJets = xi(*jetCollectionH);
  double xiFromJets_plus = xiFromJets.first;
  double xiFromJets_minus = xiFromJets.second;

  std::pair<double,double> xiFromPFCands = xi(*particleFlowCollectionH,useJetCorrection_);
  double xi_plus = xiFromPFCands.first;
  double xi_minus = xiFromPFCands.second;

  histos_.h_trackMultiplicity_->Fill(nTracks);
  histos_.h_multiplicityHFPlus_->Fill(nHF_plus);
  histos_.h_multiplicityHFMinus_->Fill(nHF_minus);     

  histos_.h_xiPlus_->Fill(xi_plus);
  histos_.h_xiMinus_->Fill(xi_minus);
  histos_.h_xiPlusFromJets_->Fill(xiFromJets_plus);
  histos_.h_xiMinusFromJets_->Fill(xiFromJets_minus);
  if((proton1 != genParticlesCollectionH->end())&&(proton2 != genParticlesCollectionH->end())){
    histos_.h_ResXiPlus_->Fill((xi_plus - xigen_plus)/xigen_plus);
    histos_.h_ResXiMinus_->Fill((xi_minus - xigen_minus)/xigen_minus);
    histos_.h_ResXiPlusFromJets_->Fill((xiFromJets_plus - xigen_plus)/xigen_plus);
    histos_.h_ResXiMinusFromJets_->Fill((xiFromJets_minus - xigen_minus)/xigen_minus);
 
    histos_.h_xiPlusVsxiGenPlus_->Fill(xigen_plus,xi_plus);
    histos_.h_xiMinusVsxiGenMinus_->Fill(xigen_minus,xi_minus);
  }

  double missingMass = 2*Ebeam_*sqrt(xi_plus*xi_minus);
  histos_.h_missingMassFromXi_->Fill(missingMass);

  // Selection
  if(nTracks > nTracksMax_) return;
  if(nHF_plus > nHFPlusMax_) return;
  if(nHF_minus > nHFMinusMax_) return;

  histos_.h_xiPlusAfterSel_->Fill(xi_plus);
  histos_.h_xiMinusAfterSel_->Fill(xi_minus);
  histos_.h_RjjAfterSel_->Fill(RjjFromJets);

}

template <class Coll>
std::pair<double,double> ExclusiveDijetsEdmDumpAnalyzer::xi(Coll& partCollection, bool useJetCorr){

  double xi_towers_plus = 0.;
  double xi_towers_minus = 0.;
  double pt_min = 10.;   
  for(typename Coll::const_iterator part = partCollection.begin(); part != partCollection.end(); ++part){
    double correction = (useJetCorr&&corrector_&&(part->pt() >= pt_min))?(corrector_->correction(part->p4())):1.;
    xi_towers_plus += correction*part->et()*exp(part->eta());
    xi_towers_minus += correction*part->et()*exp(-part->eta());
  }

  xi_towers_plus /= 2*Ebeam_;
  xi_towers_minus /= 2*Ebeam_;
   
  return std::make_pair(xi_towers_plus,xi_towers_minus);
}

template <class JetColl,class PartColl>
double ExclusiveDijetsEdmDumpAnalyzer::Rjj(JetColl& jetCollection,PartColl& partCollection){
   math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
   dijetSystem += (jetCollection[0]).p4();
   dijetSystem += (jetCollection[1]).p4(); 

   math::XYZTLorentzVector allCands(0.,0.,0.,0.);
   for(typename PartColl::const_iterator part = partCollection.begin();
                                         part != partCollection.end(); ++part) allCands += part->p4();

   return (dijetSystem.M()/allCands.M());
}

DEFINE_FWK_MODULE(ExclusiveDijetsEdmDumpAnalyzer);
