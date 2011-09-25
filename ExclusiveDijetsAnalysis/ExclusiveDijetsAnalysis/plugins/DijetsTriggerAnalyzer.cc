
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFBitCounts.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCollections.h"

#include <vector>
#include <string>
#include <map>
#include <cmath>

class TH1F;
class TH2F;

class DijetsTriggerAnalyzer: public edm::EDAnalyzer
{
  public: 
    explicit DijetsTriggerAnalyzer(const edm::ParameterSet&);
    ~DijetsTriggerAnalyzer();

    virtual void beginJob();
    virtual void endJob();
    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
  private:
    bool acceptHFRingEtSum(std::vector<TH1F*>&, const L1GctHFRingEtSumsCollection*);
    bool acceptHFRingEtSum(std::vector<TH1F*>&, const L1GctHFBitCountsCollection*);
    bool acceptHFRingEtSum(std::vector<TH1F*>&, const std::vector<L1GctJetCounts>*);

    class Correlation{
       public:
          Correlation():sumEvt_(0.),sumX_(0.),sumX2_(0.),sumY_(0.),sumY2_(0.),sumXY_(0.) {}
          void Fill(double x, double y) {++sumEvt_;sumX_ += x;sumX2_ += x*x;sumY_ += y;sumY2_ += y*y;sumXY_ += x*y;}
          double Value() {
             double covxy = sumXY_/sumEvt_ - (sumX_/sumEvt_)*(sumY_/sumEvt_);
             double sigx = sqrt(fabs(sumX2_/sumEvt_ - (sumX_/sumEvt_)*(sumX_/sumEvt_)));
             double sigy = sqrt(fabs(sumY2_/sumEvt_ - (sumY_/sumEvt_)*(sumY_/sumEvt_)));
             
             return ((sigx == 0.)||(sigy == 0.)) ? 0. : covxy/(sigx*sigy);
          }
       private:
          double sumEvt_;
          double sumX_;
          double sumX2_;
          double sumY_;
          double sumY2_;
          double sumXY_;
    };

    edm::InputTag gtDigisTag_;
    edm::InputTag l1GtObjectMapTag_; 
    edm::InputTag gctDigisTag_;

    L1GtUtils l1GtUtils_;

    //unsigned int thresholdHFRingEtSum_;
    unsigned int thresholdHFRingSum_;
    std::string hfRingSumType_;
    bool accessL1GctHFRingEtSums_;

    std::vector<std::string> ringNames_;

    TH1F* h_summaryL1_;
    std::vector<TH1F*> histosCountAll_;
    std::vector<std::vector<TH1F*> > histosRingSum_;

    std::vector<std::string> l1TriggerNames_;

    TH2F* h_correlations_;
    std::map<std::pair<int,int>,Correlation> correlations_;

};


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

//#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h"
//#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFBitCounts.h"
//#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"
#include "TH2F.h"

#include <sstream>

DijetsTriggerAnalyzer::DijetsTriggerAnalyzer(const edm::ParameterSet& pset):
     //gtDigisTag_(pset.getParameter<edm::InputTag>("gtDigisTag")),
     //l1GtObjectMapTag_(pset.getParameter<edm::InputTag>("l1GTObjectMapTag")),
     gctDigisTag_(pset.getParameter<edm::InputTag>("gctDigisTag")),
     thresholdHFRingSum_(pset.getParameter<unsigned int>("hfRingThreshold")),
     hfRingSumType_(pset.getParameter<std::string>("hfRingSumType")),
     accessL1GctHFRingEtSums_(false),
     l1TriggerNames_(pset.getParameter<std::vector<std::string> >("l1TriggerNames")) {

     //FIXME
     if      (hfRingSumType_ == "HFRingEtSums") accessL1GctHFRingEtSums_ = true;
     else if (hfRingSumType_ == "HFBitCounts")  accessL1GctHFRingEtSums_ = false;
     else throw cms::Exception("Configuration") << "Option " << hfRingSumType_ << " not valid (available: HFRingEtSums, HFBitCounts)\n";
}

DijetsTriggerAnalyzer::~DijetsTriggerAnalyzer() {}

void DijetsTriggerAnalyzer::beginJob(){
  edm::Service<TFileService> fs;

  ringNames_.push_back("Ring 1 HF-plus");
  ringNames_.push_back("Ring 1 HF-minus");
  ringNames_.push_back("Ring 2 HF-plus");
  ringNames_.push_back("Ring 2 HF-minus");
  
  std::ostringstream oss;
  h_summaryL1_ = fs->make<TH1F>("summaryL1","summaryL1",l1TriggerNames_.size()+1,0,l1TriggerNames_.size()+1);  
  h_summaryL1_->GetXaxis()->SetBinLabel(1,"All");
  histosCountAll_.resize(l1TriggerNames_.size());
  histosRingSum_.resize(l1TriggerNames_.size());
  for(size_t k = 0; k < histosCountAll_.size(); ++k){
     oss << "Creating histograms for L1 reference trigger " << l1TriggerNames_[k] << std::endl;

     h_summaryL1_->GetXaxis()->SetBinLabel(k+2,l1TriggerNames_[k].c_str());
   
     histosCountAll_[k] = fs->make<TH1F>((l1TriggerNames_[k] + "_count").c_str(),(l1TriggerNames_[k] + "_count").c_str(),3,0,3);
     histosCountAll_[k]->GetXaxis()->SetBinLabel(1,"All");
     histosCountAll_[k]->GetXaxis()->SetBinLabel(2,l1TriggerNames_[k].c_str());
     histosCountAll_[k]->GetXaxis()->SetBinLabel(3, accessL1GctHFRingEtSums_ ? "L1HFRingEtSum" : "L1HFBitCount");

     histosRingSum_[k].resize(ringNames_.size());
     for(size_t ihisto = 0; ihisto < histosRingSum_[k].size(); ++ihisto){
        std::string hname = ringNames_[ihisto] + "_" + l1TriggerNames_[k];
        histosRingSum_[k][ihisto] = fs->make<TH1F>(hname.c_str(),hname.c_str(),8,0,8);
     }
  }

  size_t nTriggers = l1TriggerNames_.size();
  h_correlations_ = fs->make<TH2F>("correlations","correlations",nTriggers,0,nTriggers,nTriggers,0,nTriggers);
  for(size_t i = 0; i < nTriggers; ++i){
     for(size_t j = 0; j < nTriggers; ++j){
        h_correlations_->GetXaxis()->SetBinLabel(i+1,l1TriggerNames_[i].c_str());
        h_correlations_->GetYaxis()->SetBinLabel(j+1,l1TriggerNames_[j].c_str());
        correlations_.insert(std::make_pair(std::make_pair(i,j),Correlation()));
     }
  }
  edm::LogInfo("Analysis") << oss.str();
}

void DijetsTriggerAnalyzer::endJob(){
   std::ostringstream oss;
   for(size_t k = 0; k < histosCountAll_.size(); ++k){
     float nall = histosCountAll_[k]->GetBinContent(1);
     float ntrig = histosCountAll_[k]->GetBinContent(2); 
     float nHFRingSum = histosCountAll_[k]->GetBinContent(3);

     oss << "Efficiency L1 trigger " << l1TriggerNames_[k] << " = " 
                                     << ( (nall) ? (ntrig/nall) : -1 ) << std::endl
         << "Efficiency L1 HF Rings = " << ( (nall) ? (nHFRingSum/nall) : -1 ) << std::endl
         << "              relative = " << ( (ntrig) ? (nHFRingSum/ntrig) : -1 ) << std::endl;
   }
   edm::LogInfo("Analysis") << oss.str();

   for(size_t i = 0; i < l1TriggerNames_.size(); ++i){
      for(size_t j = 0; j < l1TriggerNames_.size(); ++j){
         std::pair<int,int> index(i,j);
         h_correlations_->SetBinContent(i+1,j+1,correlations_[index].Value());
      }
   }
}

void DijetsTriggerAnalyzer::beginRun(const edm::Run& run, const edm::EventSetup& setup){
  //l1GtUtils_.getL1GtRunCache(run, setup, true, false);
}

void DijetsTriggerAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){

  // Access L1 bits
  /*edm::Handle<L1GlobalTriggerReadoutRecord> l1GTReadoutRcdH;
  event.getByLabel(gtDigisTag_, l1GTReadoutRcdH);

  edm::Handle<L1GlobalTriggerObjectMapRecord> l1GTObjMapRcdH;
  event.getByLabel(l1GtObjectMapTag_,l1GTObjMapRcdH);
  if( !l1GTReadoutRcdH.isValid() ) { edm::LogWarning("L1 GT Readout record not found, skipping.");return;}
  if( !l1GTObjMapRcdH.isValid() )  { edm::LogWarning("L1 GT Object Map not found, skipping.");return;}  

  const L1GlobalTriggerReadoutRecord* gtReadoutRecord = l1GTReadoutRcdH.product();
  const L1GlobalTriggerObjectMapRecord* gtObjectMapRecord = l1GTObjMapRcdH.product();

  DecisionWord gtDecisionWord = gtReadoutRecord->decisionWord();
  const unsigned int numberTriggerBits(gtDecisionWord.size());

  const std::vector<L1GlobalTriggerObjectMap>& objMapVec = gtObjectMapRecord->gtObjectMap();

  std::vector<std::string> passedL1;
  for(std::vector<L1GlobalTriggerObjectMap>::const_iterator itMap = objMapVec.begin();
                                                            itMap != objMapVec.end(); ++itMap) {
    // Get trigger bits
    int itrig = (*itMap).algoBitNumber();
    // Get trigger names
    //std::string trigName = (*itMap).algoName();

    if(gtDecisionWord[itrig]) passedL1.push_back( (*itMap).algoName() );
  }*/
 
  l1GtUtils_.retrieveL1EventSetup(setup);
  /*edm::ESHandle<L1GtTriggerMenu> l1GtMenuRcd;
  setup.get<L1GtTriggerMenuRcd>().get(l1GtMenuRcd);
  const L1GtTriggerMenu* l1GtMenu = l1GtMenuRcd.product();
  // Get L1 menu from event setup
  for(CItAlgo algo = l1GtMenu->gtAlgorithmMap().begin(); algo != l1GtMenu->gtAlgorithmMap().end(); ++algo) {
     //if (_Debug) std::cout << "Name: " << (algo->second).algoName() << " Alias: " << (algo->second).algoAlias() << std::endl;
     int itrig = (algo->second).algoBitNumber();
     algoBitToName[itrig] = (algo->second).algoName();
  }*/
  std::vector<std::string> passedL1;
  std::vector<int> prescalesL1;
  for(size_t k = 0; k < l1TriggerNames_.size(); ++k){
     int ierror = -1;
     bool decision = l1GtUtils_.decisionBeforeMask(event,l1TriggerNames_[k],ierror);
     if(ierror == 0 && decision){
        passedL1.push_back( l1TriggerNames_[k] );
        int prescale = l1GtUtils_.prescaleFactor(event,l1TriggerNames_[k],ierror);
        prescalesL1.push_back(prescale);
     }
  }

  for(size_t k = 0; k < passedL1.size(); ++k)
     LogTrace("Analysis") << "Passed L1 trigger " << passedL1[k] << " prescale " << prescalesL1[k];

  // Check if event satisfied pre-defined triggers and get correlations
  for(size_t i = 0; i < l1TriggerNames_.size(); ++i){
     for(size_t j = 0; j < l1TriggerNames_.size(); ++j){
        std::pair<int,int> index(i,j);
        Correlation& corr = correlations_[index];
        double x = 0.;
        double y = 0.; 
        if(find(passedL1.begin(),passedL1.end(),l1TriggerNames_[i]) != passedL1.end()) x = 1.;
        if(find(passedL1.begin(),passedL1.end(),l1TriggerNames_[j]) != passedL1.end()) y = 1.; 
             
        corr.Fill(x,y); 
     }
  }

  const L1GctHFRingEtSumsCollection* gctHFRingEtSumsCollection = 0;
  const L1GctHFBitCountsCollection* gctHFBitCountsCollection = 0;
  //const std::vector<L1GctJetCounts>* jetCounts = 0;
  if(accessL1GctHFRingEtSums_){
     edm::Handle<L1GctHFRingEtSumsCollection> gctHFRingEtSumsH;
     event.getByLabel(gctDigisTag_, gctHFRingEtSumsH);
     if( !gctHFRingEtSumsH.isValid() ){
        edm::LogWarning("L1 GCT HF Ring Et sums collection not found, skipping.");
        return;
     }
     gctHFRingEtSumsCollection = gctHFRingEtSumsH.product();
  } else{
     edm::Handle<L1GctHFBitCountsCollection> gctHFBitCountsH;
     event.getByLabel(gctDigisTag_, gctHFBitCountsH);
     if( !gctHFBitCountsH.isValid() ){
        edm::LogWarning("L1 GCT HF bit counts collection not found, skipping.");
        return;
     }
     gctHFBitCountsCollection = gctHFBitCountsH.product();  
  }

  h_summaryL1_->Fill(0); 
  for(size_t k = 0; k < l1TriggerNames_.size(); ++k){
    histosCountAll_[k]->Fill(0);
    if( find(passedL1.begin(),passedL1.end(),l1TriggerNames_[k]) != passedL1.end() ){ //Found trigger
       h_summaryL1_->Fill(k+1);
 
       histosCountAll_[k]->Fill(1);

       std::vector<TH1F*>& histos = histosRingSum_[k];
       bool acceptRingSum = (accessL1GctHFRingEtSums_) ? acceptHFRingEtSum(histos,gctHFRingEtSumsCollection) : 
                                                         acceptHFRingEtSum(histos,gctHFBitCountsCollection);
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
      LogTrace("Analysis") << "Is empty: " << ringSumItr->empty();  
      for(size_t k = 0; k < 4; ++k){ 
         LogTrace("Analysis") << "ET Sum for " << ringNames_[k] << ": " << ringSumItr->etSum(k);
         histosRingSum[k]->Fill(ringSumItr->etSum(k));
         if(ringSumItr->etSum(k) > thresholdHFRingSum_) accept = false;
      }
   }

   return accept;
}

bool DijetsTriggerAnalyzer::acceptHFRingEtSum(std::vector<TH1F*>& histosRingSum, const L1GctHFBitCountsCollection* ringSumCollection){
   bool accept = true;

   L1GctHFBitCountsCollection::const_iterator ringSumItr = ringSumCollection->begin();
   // Require bit count threshold for all rings
   for(; ringSumItr->bx()==0 && ringSumItr != ringSumCollection->end(); ++ringSumItr){
      LogTrace("Analysis") << "Is empty: " << ringSumItr->empty();  
      for(size_t k = 0; k < 4; ++k){ 
         LogTrace("Analysis") << "Bit count for " << ringNames_[k] << ": " << ringSumItr->bitCount(k);
         histosRingSum[k]->Fill(ringSumItr->bitCount(k));
         if(ringSumItr->bitCount(k) > thresholdHFRingSum_) accept = false;
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
         LogTrace("Analysis") << "ET Sum index " << icount << ": " << jetCountItr->count(icount);
         histosRingSum[iring]->Fill(jetCountItr->count(icount));
         if(jetCountItr->count(icount) > thresholdHFRingSum_) accept = false; 
      }
   }

  return accept;
}

DEFINE_FWK_MODULE(DijetsTriggerAnalyzer);
