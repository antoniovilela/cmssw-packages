
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TTree;

class SimpleDijetsAnalyzer: public edm::EDAnalyzer
{
  public: 
    explicit SimpleDijetsAnalyzer(const edm::ParameterSet&);
    ~SimpleDijetsAnalyzer();

    virtual void beginJob(const edm::EventSetup&);
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
  private:
    edm::InputTag jetTag_;

    TTree* data_;

    struct {
      double jet1Pt_;
      double jet1Phi_;
      double jet1Eta_;
      double jet2Pt_;
      double jet2Phi_;
      double jet2Eta_; 
    } eventData_;
};


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"

using namespace reco;

SimpleDijetsAnalyzer::SimpleDijetsAnalyzer(const edm::ParameterSet& pset):
  jetTag_(pset.getParameter<edm::InputTag>("JetTag"))
{
}

SimpleDijetsAnalyzer::~SimpleDijetsAnalyzer() {}

void SimpleDijetsAnalyzer::beginJob(const edm::EventSetup& setup){
  edm::Service<TFileService> fs;

  data_ = fs->make<TTree>("data","data");
  data_->Branch("jet1Pt",&eventData_.jet1Pt_,"jet1Pt/D");
  data_->Branch("jet1Phi",&eventData_.jet1Phi_,"jet1Phi/D");
  data_->Branch("jet1Eta",&eventData_.jet1Eta_,"jet1Eta/D");
  data_->Branch("jet2Pt",&eventData_.jet2Pt_,"jet2Pt/D");
  data_->Branch("jet2Phi",&eventData_.jet2Phi_,"jet2Phi/D");
  data_->Branch("jet2Eta",&eventData_.jet2Eta_,"jet2Eta/D");
}

void SimpleDijetsAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);

  if(jetCollectionH->size() < 2) return;

  const Jet& jet1 = (*jetCollectionH.product())[0];
  const Jet& jet2 = (*jetCollectionH.product())[1];

  eventData_.jet1Pt_ = jet1.pt();
  eventData_.jet1Phi_ = jet1.phi();
  eventData_.jet1Eta_ = jet1.eta();

  eventData_.jet2Pt_ = jet2.pt();
  eventData_.jet2Phi_ = jet2.phi();
  eventData_.jet2Eta_ = jet2.eta();

  data_->Fill();
}
