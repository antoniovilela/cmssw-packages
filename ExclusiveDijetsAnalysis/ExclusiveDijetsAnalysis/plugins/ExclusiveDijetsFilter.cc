
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>

class TH1F;
class TH2F;

class ExclusiveDijetsFilter: public edm::EDFilter
{
  public: 
    explicit ExclusiveDijetsFilter(const edm::ParameterSet&);
    ~ExclusiveDijetsFilter();

    virtual void beginJob(const edm::EventSetup&);
    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    virtual bool filter(edm::Event&, const edm::EventSetup&);
  private:
    edm::InputTag jetTag_;

    double ptJetMin_;
    double etaJetMax_;
    double deltaEtaMax_;
    double deltaPhiMax_;
    double deltaPtMax_;
    double thirdJetPtMax_;
    unsigned int nTracksMax_;
    unsigned int nHFPlusMax_;
    unsigned int nHFMinusMax_;

    bool doBtag_;
    std::string bTagMode_;
    std::string bDiscriminatorName_;
    double bDiscMin_;

    unsigned int thresholdHF_;
    bool usePAT_; 

    struct {
      TH1F* h_leadingJetPt_;
      TH1F* h_leadingJetEta_;
      TH1F* h_leadingJetPhi_;
      TH1F* h_leadingJetBDiscriminator_;
   
      TH1F* h_secondJetPt_;
      TH1F* h_secondJetEta_;
      TH1F* h_secondJetPhi_;
      TH1F* h_secondJetBDiscriminator_;

      TH1F* h_thirdJetPt_;
      TH1F* h_thirdJetEta_;

      TH1F* h_jetsAveEta_;
      TH1F* h_jetsDeltaEta_;
      TH1F* h_jetsDeltaPhi_;
      TH1F* h_jetsDeltaPt_;

      TH1F* h_trackMultiplicity_;
      TH1F* h_multiplicityHFPlus_;
      TH1F* h_multiplicityHFMinus_;
    } histos_;

};

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"
#include "TH2F.h"

ExclusiveDijetsFilter::ExclusiveDijetsFilter(const edm::ParameterSet& pset):
  jetTag_(pset.getParameter<edm::InputTag>("JetTag")),
  ptJetMin_(pset.getParameter<double>("PtMinJet")),
  etaJetMax_(pset.getParameter<double>("EtaMaxJet")),
  deltaEtaMax_(pset.getParameter<double>("DeltaEtaMax")),
  deltaPhiMax_(pset.getParameter<double>("DeltaPhiMax")),
  deltaPtMax_(pset.getParameter<double>("DeltaPtMax")),
  thirdJetPtMax_(pset.getParameter<double>("ThirdJetPtMax")),
  nTracksMax_(pset.getParameter<unsigned int>("NTracksMax")),
  nHFPlusMax_(pset.getParameter<unsigned int>("NHFPlusMax")),
  nHFMinusMax_(pset.getParameter<unsigned int>("NHFMinusMax")),
  doBtag_(pset.getParameter<bool>("DoBTag")),
  thresholdHF_(pset.getParameter<unsigned int>("HFThresholdIndex")),
  usePAT_(pset.getUntrackedParameter<bool>("UsePAT",true))
{
  if(doBtag_){
     bTagMode_ = pset.getParameter<std::string>("BTagMode");  
     bDiscriminatorName_ = pset.getParameter<std::string>("BDiscriminatorName");
     bDiscMin_ = pset.getParameter<double>("BDiscMin");
  }  
}

ExclusiveDijetsFilter::~ExclusiveDijetsFilter(){}

void ExclusiveDijetsFilter::beginJob(const edm::EventSetup& setup){
  edm::Service<TFileService> fs;

  histos_.h_leadingJetPt_ = fs->make<TH1F>("leadingJetPt","leadingJetPt",100,0.,100.);
  histos_.h_leadingJetEta_ = fs->make<TH1F>("leadingJetEta","leadingJetEta",100,-5.,5.);
  histos_.h_leadingJetPhi_ = fs->make<TH1F>("leadingJetPhi","leadingJetPhi",100,-1.1*M_PI,1.1*M_PI);
  histos_.h_leadingJetBDiscriminator_ = fs->make<TH1F>("leadingJetBDiscriminator","leadingJetBDiscriminator",100,-10.,30.);
 
  histos_.h_secondJetPt_ = fs->make<TH1F>("secondJetPt","secondJetPt",100,0.,100.);
  histos_.h_secondJetEta_ = fs->make<TH1F>("secondJetEta","secondJetEta",100,-5.,5.);
  histos_.h_secondJetPhi_ = fs->make<TH1F>("secondJetPhi","secondJetPhi",100,-1.1*M_PI,1.1*M_PI);
  histos_.h_secondJetBDiscriminator_ = fs->make<TH1F>("secondJetBDiscriminator","secondJetBDiscriminator",100,-10.,30.);

  histos_.h_thirdJetPt_ = fs->make<TH1F>("thirdJetPt","thirdJetPt",100,0.,80.);
  histos_.h_thirdJetEta_ = fs->make<TH1F>("thirdJetEta","thirdJetEta",100,-5.,5.);

  histos_.h_jetsAveEta_ = fs->make<TH1F>("jetsAveEta","jetsAveEta",100,-5.,5.);
  histos_.h_jetsDeltaEta_ = fs->make<TH1F>("jetsDeltaEta","jetsDeltaEta",100,-5.,5.);
  histos_.h_jetsDeltaPhi_ = fs->make<TH1F>("jetsDeltaPhi","jetsDeltaPhi",100,0.,1.1*M_PI);
  histos_.h_jetsDeltaPt_ = fs->make<TH1F>("jetsDeltaPt","jetsDeltaPt",100,0.,10.);

  histos_.h_trackMultiplicity_ = fs->make<TH1F>("trackMultiplicity","trackMultiplicity",20,0,20);
  histos_.h_multiplicityHFPlus_ = fs->make<TH1F>("multiplicityHFPlus","multiplicityHFPlus",20,0,20);
  histos_.h_multiplicityHFMinus_ = fs->make<TH1F>("multiplicityHFMinus","multiplicityHFMinus",20,0,20);
}

void ExclusiveDijetsFilter::beginRun(const edm::Run& run, const edm::EventSetup& setup){}

bool ExclusiveDijetsFilter::filter(edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);

  const reco::Jet& jet1 = (*jetCollectionH)[0];// they come out ordered right?
  const reco::Jet& jet2 = (*jetCollectionH)[1];

  histos_.h_leadingJetPt_->Fill(jet1.pt());
  histos_.h_secondJetPt_->Fill(jet2.pt());
  if(jet1.pt() < ptJetMin_) return false;
  if(jet2.pt() < ptJetMin_) return false;

  histos_.h_leadingJetEta_->Fill(jet1.eta());
  histos_.h_secondJetEta_->Fill(jet2.eta());
  if(fabs(jet1.eta()) > etaJetMax_) return false;
  if(fabs(jet2.eta()) > etaJetMax_) return false; 

  histos_.h_leadingJetPhi_->Fill(jet1.phi());
  histos_.h_secondJetPhi_->Fill(jet2.phi());

  histos_.h_jetsAveEta_->Fill((jet1.eta() + jet2.eta())/2);
  histos_.h_jetsDeltaEta_->Fill(jet1.eta() - jet2.eta());
  histos_.h_jetsDeltaPhi_->Fill(M_PI - fabs(jet1.phi() - jet2.phi()));
  histos_.h_jetsDeltaPt_->Fill(fabs(jet1.pt() - jet2.pt())); 

  if(fabs(jet1.eta() - jet2.eta()) > deltaEtaMax_) return false;
  if((M_PI - fabs(jet1.phi() - jet2.phi())) > deltaPhiMax_) return false;
  if(fabs(jet1.pt() - jet2.pt()) > deltaPtMax_) return false;

  if(jetCollectionH->size() > 2){
     const reco::Jet& jet3 = (*jetCollectionH)[2];
     histos_.h_thirdJetPt_->Fill(jet3.pt());
     histos_.h_thirdJetEta_->Fill(jet3.eta());
  }

  if(usePAT_){
    const pat::Jet* patJet1 = dynamic_cast<const pat::Jet*>(&jet1);
    const pat::Jet* patJet2 = dynamic_cast<const pat::Jet*>(&jet2);
    if(!patJet1 || !patJet2) throw edm::Exception(edm::errors::Configuration) << "Expecting PATJet's as input";

    // B-tagging
    if(doBtag_){
       double bDiscJet1 = patJet1->bDiscriminator(bDiscriminatorName_);
       double bDiscJet2 = patJet2->bDiscriminator(bDiscriminatorName_);

       histos_.h_leadingJetBDiscriminator_->Fill(bDiscJet1);
       histos_.h_secondJetBDiscriminator_->Fill(bDiscJet2);

       // Selection
       bool singleBTag = ((bDiscJet1 > bDiscMin_)||(bDiscJet2 > bDiscMin_));
       bool doubleBTag = ((bDiscJet1 > bDiscMin_)&&(bDiscJet2 > bDiscMin_));
       if((bTagMode_ == "1Btag")||(bTagMode_ == "1BTag")||(bTagMode_ == "single")){
	  if(!singleBTag) return false;
       } else if(((bTagMode_ == "2Btag")||(bTagMode_ == "2BTag")||(bTagMode_ == "double"))){
	  if(!doubleBTag) return false;
       } 
    }
  } else {
    // access info from outside PAT here
  }

  // Access multiplicities
  edm::Handle<unsigned int> trackMultiplicity; 
  event.getByLabel("trackMultiplicityTransverseRegion","trackMultiplicity",trackMultiplicity);

  edm::Handle<std::vector<unsigned int> > nHFPlus;
  event.getByLabel("hfTower","nHFplus",nHFPlus);
  
  edm::Handle<std::vector<unsigned int> > nHFMinus;
  event.getByLabel("hfTower","nHFminus",nHFMinus);

  unsigned int nTracks = *trackMultiplicity;

  unsigned int nHF_plus = (*nHFPlus)[thresholdHF_];
  unsigned int nHF_minus = (*nHFMinus)[thresholdHF_];

  histos_.h_trackMultiplicity_->Fill(nTracks);
  histos_.h_multiplicityHFPlus_->Fill(nHF_plus);
  histos_.h_multiplicityHFMinus_->Fill(nHF_minus);     

  double thirdJetPt = (jetCollectionH->size() > 2)?(*jetCollectionH)[2].pt():0.;

  // Selection
  if(thirdJetPt > thirdJetPtMax_) return false;
  if(nTracks > nTracksMax_) return false;
  if(nHF_plus > nHFPlusMax_) return false;
  if(nHF_minus > nHFMinusMax_) return false;

  return true;
}

DEFINE_FWK_MODULE(ExclusiveDijetsFilter);
