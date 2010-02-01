
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/EventData.h"
//#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/Histos.h"
#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/MinimumBiasAnalysis.h"

class TTree;
class TH1F;
class TH2F;

class MinimumBiasTTreeAnalyzer: public edm::EDAnalyzer
{
  public: 
    explicit MinimumBiasTTreeAnalyzer(const edm::ParameterSet&);
    ~MinimumBiasTTreeAnalyzer();

    virtual void beginJob(const edm::EventSetup&);
    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
  private:
    minimumBiasAnalysis::MinimumBiasAnalysis minimumBiasAnalysis_;
    bool saveTTree_;

    TTree* data_;
    minimumBiasAnalysis::EventData eventData_;
    //minimumBiasAnalysis::HistoMapTH1F histosTH1F_;
    //minimumBiasAnalysis::HistoMapTH2F histosTH2F_;
};

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace minimumBiasAnalysis;

MinimumBiasTTreeAnalyzer::MinimumBiasTTreeAnalyzer(const edm::ParameterSet& pset):
  minimumBiasAnalysis_(pset),  
  saveTTree_(pset.getUntrackedParameter<bool>("SaveROOTTree",true)){}

MinimumBiasTTreeAnalyzer::~MinimumBiasTTreeAnalyzer(){}

void MinimumBiasTTreeAnalyzer::beginJob(const edm::EventSetup& setup){
  edm::Service<TFileService> fs;

  if(saveTTree_){
    data_ = fs->make<TTree>("data","data");
    addTTreeBranches(*data_,eventData_);
  }

  //bookHistos(histosTH1F_,*fs);
  //bookHistos(histosTH2F_,*fs);
}

void MinimumBiasTTreeAnalyzer::beginRun(const edm::Run& run, const edm::EventSetup& setup){
  minimumBiasAnalysis_.servicesBeginRun(run,setup);
}

void MinimumBiasTTreeAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){

  minimumBiasAnalysis_.fillEventData(eventData_,event,setup);

  // Fill TTree
  if(saveTTree_) data_->Fill();
}

DEFINE_FWK_MODULE(MinimumBiasTTreeAnalyzer);
