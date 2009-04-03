
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <string>

class TH1F;

class DijetsTriggerAnalyzer: public edm::EDAnalyzer
{
  public: 
    explicit DijetsTriggerAnalyzer(const edm::ParameterSet&);
    ~DijetsTriggerAnalyzer();

    virtual void beginJob(const edm::EventSetup&);
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
  private:
    std::vector<std::string> ringNames_;

    std::vector<TH1F*> histosRingSum_;
};


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"

typedef std::vector<L1GctHFRingEtSums> L1GctHFRingEtSumsCollection;

DijetsTriggerAnalyzer::DijetsTriggerAnalyzer(const edm::ParameterSet& pset){}

DijetsTriggerAnalyzer::~DijetsTriggerAnalyzer() {}

void DijetsTriggerAnalyzer::beginJob(const edm::EventSetup& setup){
  edm::Service<TFileService> fs;

  ringNames_.push_back("Ring 1 HF-plus");
  ringNames_.push_back("Ring 1 HF-minus");
  ringNames_.push_back("Ring 2 HF-plus");
  ringNames_.push_back("Ring 2 HF-minus");

  histosRingSum_.resize(ringNames_.size());
  assert(histosRingSum_.size() == ringNames_.size());
  for(size_t k = 0; k < histosRingSum_.size(); ++k) histosRingSum_[k] = fs->make<TH1F>(ringNames_[k].c_str(),ringNames_[k].c_str(),8,0,8); 
}

void DijetsTriggerAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){

  edm::Handle<L1GctHFRingEtSumsCollection> ringSumsH;
  event.getByLabel("hltGctDigis", ringSumsH);
  L1GctHFRingEtSumsCollection::const_iterator ringSumItr;
  
  for(ringSumItr=ringSumsH->begin(); ringSumItr->bx()==0 && ringSumItr != ringSumsH->end(); ++ringSumItr){
    edm::LogVerbatim("") << "Is empty: " << ringSumItr->empty();  
    for(size_t k = 0; k < 4; ++k){ 
      edm::LogVerbatim("") << "ET Sum for " << ringNames_[k] << ": " << ringSumItr->etSum(k);
      histosRingSum_[k]->Fill(ringSumItr->etSum(k));
    }
  }
}

DEFINE_FWK_MODULE(DijetsTriggerAnalyzer);
