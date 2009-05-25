
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h>
#include <DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h>

#include <vector>
#include <string>

class TH1F;

class DijetsTriggerAnalyzer: public edm::EDAnalyzer
{
  public: 
    typedef std::vector<L1GctHFRingEtSums> L1GctHFRingEtSumsCollection;

    explicit DijetsTriggerAnalyzer(const edm::ParameterSet&);
    ~DijetsTriggerAnalyzer();

    virtual void beginJob(const edm::EventSetup&);
    virtual void endJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
  private:
    bool acceptHFRingEtSum(std::vector<TH1F*>&, const L1GctHFRingEtSumsCollection*);
    bool acceptHFRingEtSum(std::vector<TH1F*>&, const std::vector<L1GctJetCounts>*);

    edm::InputTag gtDigisTag_;
    edm::InputTag gctDigisTag_;
    edm::InputTag l1GtObjectMapTag_; 

    int thresholdHFRingEtSum_;
    bool accessL1GctHFRingEtSums_;

    std::vector<std::string> ringNames_;

    TH1F* h_summaryL1_;
    std::vector<TH1F*> histosCountAll_;
    std::vector<std::vector<TH1F*> > histosRingSum_;

    std::vector<std::string> l1TriggerNames_;
};


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#include <DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h>
#include <DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h>
#include <DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h>
//#include <DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"

//typedef std::vector<L1GctHFRingEtSums> L1GctHFRingEtSumsCollection;

DijetsTriggerAnalyzer::DijetsTriggerAnalyzer(const edm::ParameterSet& pset):
     gtDigisTag_(pset.getParameter<edm::InputTag>("GTDigisTag")),
     gctDigisTag_(pset.getParameter<edm::InputTag>("GCTDigisTag")),
     l1GtObjectMapTag_(pset.getParameter<edm::InputTag>("L1GTObjectMapTag")),
     thresholdHFRingEtSum_(pset.getParameter<int>("HFRingETSumThreshold")),
     accessL1GctHFRingEtSums_(pset.getUntrackedParameter<bool>("AccessL1GctHFRingEtSums",true)),
     l1TriggerNames_(pset.getParameter<std::vector<std::string> >("L1TriggerNames")){}

DijetsTriggerAnalyzer::~DijetsTriggerAnalyzer() {}

void DijetsTriggerAnalyzer::beginJob(const edm::EventSetup& setup){
  edm::Service<TFileService> fs;

  /*h_countAll_ = fs->make<TH1F>("countAll","countAll",3,0,3);
  h_countAll_->GetXaxis()->SetBinLabel(1,"All");
  h_countAll_->GetXaxis()->SetBinLabel(2,"L1Jets");
  h_countAll_->GetXaxis()->SetBinLabel(3,"L1HFRingEtSum");*/
 
  ringNames_.push_back("Ring 1 HF-plus");
  ringNames_.push_back("Ring 1 HF-minus");
  ringNames_.push_back("Ring 2 HF-plus");
  ringNames_.push_back("Ring 2 HF-minus");

  h_summaryL1_ = fs->make<TH1F>("summaryL1","summaryL1",l1TriggerNames_.size()+1,0,l1TriggerNames_.size()+1);  
  h_summaryL1_->GetXaxis()->SetBinLabel(1,"All");
  histosCountAll_.resize(l1TriggerNames_.size());
  histosRingSum_.resize(l1TriggerNames_.size());
  for(size_t k = 0; k < histosCountAll_.size(); ++k){
     h_summaryL1_->GetXaxis()->SetBinLabel(k+2,l1TriggerNames_[k].c_str());
   
     histosCountAll_[k] = fs->make<TH1F>((l1TriggerNames_[k] + "_count").c_str(),(l1TriggerNames_[k] + "_count").c_str(),3,0,3);
     histosCountAll_[k]->GetXaxis()->SetBinLabel(1,"All");
     histosCountAll_[k]->GetXaxis()->SetBinLabel(2,l1TriggerNames_[k].c_str());
     histosCountAll_[k]->GetXaxis()->SetBinLabel(3,"L1HFRingEtSum"); 

     histosRingSum_[k].resize(ringNames_.size());
     for(size_t ihisto = 0; ihisto < histosRingSum_[k].size(); ++ihisto){
        std::string hname = ringNames_[ihisto] + "_" + l1TriggerNames_[k];
        histosRingSum_[k][ihisto] = fs->make<TH1F>(hname.c_str(),hname.c_str(),8,0,8);
     }
  }
}

void DijetsTriggerAnalyzer::endJob(){
   for(size_t k = 0; k < histosCountAll_.size(); ++k){
     float nall = histosCountAll_[k]->GetBinContent(1);
     float ntrig = histosCountAll_[k]->GetBinContent(2); 
     float nHFRingETSum = histosCountAll_[k]->GetBinContent(3);

     edm::LogInfo("") << "Efficiency " << l1TriggerNames_[k] << " = " << ((nall)?(ntrig/nall):-1) << "\n"
                      << "Efficiency L1 HF Rings ET sum = " << ((nall)?(nHFRingETSum/nall):-1)
                                                            << " relative = " << ((ntrig)?(nHFRingETSum/ntrig):-1);
   }
}

void DijetsTriggerAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){

  // Access L1 bits
  edm::Handle<L1GlobalTriggerReadoutRecord> l1GTReadoutRcdH;
  event.getByLabel(gtDigisTag_, l1GTReadoutRcdH);

  edm::Handle<L1GlobalTriggerObjectMapRecord> l1GTObjMapRcdH;
  event.getByLabel(l1GtObjectMapTag_,l1GTObjMapRcdH);
   TString Tname;
   // int Tbits[20];
  if(!l1GTReadoutRcdH.isValid() || !l1GTObjMapRcdH.isValid()) return;  

  DecisionWord gtDecisionWord = l1GTReadoutRcdH->decisionWord();

  const std::vector<L1GlobalTriggerObjectMap>& objMapVec = l1GTObjMapRcdH->gtObjectMap();
  std::vector<std::string> passedL1;
  for(std::vector<L1GlobalTriggerObjectMap>::const_iterator itMap = objMapVec.begin();
                                                            itMap != objMapVec.end(); ++itMap) {
    // Get trigger bits
    int itrig = (*itMap).algoBitNumber();
    // Get trigger names
    //std::string trigName = (*itMap).algoName();

    if(gtDecisionWord[itrig]) passedL1.push_back((*itMap).algoName());
  }
 
  for(std::vector<std::string>::const_iterator itPassedL1 = passedL1.begin(); itPassedL1 != passedL1.end(); ++itPassedL1)
             LogTrace("") << "Passed L1 trigger " << *itPassedL1;

  // Check if event satisfied pre-defined triggers
  //bool acceptL1Jets = false;
  /*for(std::vector<std::string>::const_iterator itTrigL1 = l1TriggerNames_.begin();
                                               itTrigL1 != l1TriggerNames_.end(); ++itTrigL1,++itrig){
    if(find(passedL1.begin(),passedL1.end(),*itTrigL1) != passedL1.end()) {acceptL1Jets = true;break;}*/

  const L1GctHFRingEtSumsCollection* ringSumCollection = 0;
  const std::vector<L1GctJetCounts>* jetCounts = 0;
  if(accessL1GctHFRingEtSums_){
     edm::Handle<L1GctHFRingEtSumsCollection> ringSumsH;
     event.getByLabel(gctDigisTag_, ringSumsH);
     ringSumCollection = &*ringSumsH;
  } else{
     edm::Handle<std::vector<L1GctJetCounts> > jetCountsH;
     event.getByLabel(gctDigisTag_, jetCountsH);
     jetCounts = &*jetCountsH;
  } 
  for(size_t k = 0; k < l1TriggerNames_.size(); ++k){
    histosCountAll_[k]->Fill(0);
    h_summaryL1_->Fill(0);
    if(find(passedL1.begin(),passedL1.end(),l1TriggerNames_[k]) != passedL1.end()){ //Found trigger
       h_summaryL1_->Fill(k+1);
 
       histosCountAll_[k]->Fill(1);

       std::vector<TH1F*>& histos = histosRingSum_[k];
       bool acceptRingSum = (accessL1GctHFRingEtSums_)?acceptHFRingEtSum(histos,ringSumCollection):acceptHFRingEtSum(histos,jetCounts);
       if(!acceptRingSum) continue;

       histosCountAll_[k]->Fill(2); 
    }
  }
  
}

bool DijetsTriggerAnalyzer::acceptHFRingEtSum(std::vector<TH1F*>& histosRingSum, const L1GctHFRingEtSumsCollection* ringSumCollection){
   bool accept = true;
   L1GctHFRingEtSumsCollection::const_iterator ringSumItr = ringSumCollection->begin();
  
   // Require sum(ET) threshold for all rings
   for(; ringSumItr->bx()==0 && ringSumItr != ringSumCollection->end(); ++ringSumItr){
      LogTrace("") << "Is empty: " << ringSumItr->empty();  
      for(size_t k = 0; k < 4; ++k){ 
         LogTrace("") << "ET Sum for " << ringNames_[k] << ": " << ringSumItr->etSum(k);
         histosRingSum[k]->Fill(ringSumItr->etSum(k));
         if(ringSumItr->etSum(k) > thresholdHFRingEtSum_) accept = false;
      }
   }

   return accept;
}

bool DijetsTriggerAnalyzer::acceptHFRingEtSum(std::vector<TH1F*>& histosRingSum, const std::vector<L1GctJetCounts>* jetCounts){
   bool accept = true;
   for(std::vector<L1GctJetCounts>::const_iterator jetCountItr = jetCounts->begin();
                                                   jetCountItr != jetCounts->end(); ++jetCountItr){
      int iring = 0;
      for(int icount = 8; icount <= 11; ++icount,++iring){
         LogTrace("") << "ET Sum index " << icount << ": " << jetCountItr->count(icount);
         histosRingSum[iring]->Fill(jetCountItr->count(icount));
         if(jetCountItr->count(icount) > thresholdHFRingEtSum_) accept = false; 
      }
   }

  return accept;
}

DEFINE_FWK_MODULE(DijetsTriggerAnalyzer);
