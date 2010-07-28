
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class HCALActivityFilter : public edm::EDFilter {
    public:
       explicit HCALActivityFilter( edm::ParameterSet const& );
       ~HCALActivityFilter();

       virtual void beginJob();
       virtual bool filter( edm::Event&, edm::EventSetup const& );
   private:
       edm::InputTag hcalTowerSummaryTag_;

       double energyThresholdHB_;
       double energyThresholdHE_;
       double energyThresholdHF_;

       int nHBTowersMaxPlus_;
       int nHBTowersMaxMinus_;
       int nHETowersMaxPlus_;
       int nHETowersMaxMinus_;
       int nHFTowersMaxPlus_;
       int nHFTowersMaxMinus_;

       double sumEHBMaxPlus_;
       double sumEHBMaxMinus_; 
       double sumEHEMaxPlus_;
       double sumEHEMaxMinus_; 
       double sumEHFMaxPlus_;
       double sumEHFMaxMinus_;
};

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

HCALActivityFilter::HCALActivityFilter(edm::ParameterSet const& pset):
  hcalTowerSummaryTag_(pset.getParameter<edm::InputTag>("HCALTowerSummaryTag")),
  energyThresholdHB_(pset.getParameter<double>("EnergyThresholdHB")),
  energyThresholdHE_(pset.getParameter<double>("EnergyThresholdHE")),
  energyThresholdHF_(pset.getParameter<double>("EnergyThresholdHF")),
  nHBTowersMaxPlus_(pset.getParameter<int>("NTowersMaxHBPlus")),
  nHBTowersMaxMinus_(pset.getParameter<int>("NTowersMaxHBMinus")),
  nHETowersMaxPlus_(pset.getParameter<int>("NTowersMaxHEPlus")), 
  nHETowersMaxMinus_(pset.getParameter<int>("NTowersMaxHEMinus")),
  nHFTowersMaxPlus_(pset.getParameter<int>("NTowersMaxHFPlus")), 
  nHFTowersMaxMinus_(pset.getParameter<int>("NTowersMaxHFMinus")),
  sumEHBMaxPlus_(pset.getParameter<double>("SumEMaxHBPlus")),
  sumEHBMaxMinus_(pset.getParameter<double>("SumEMaxHBMinus")),
  sumEHEMaxPlus_(pset.getParameter<double>("SumEMaxHEPlus")), 
  sumEHEMaxMinus_(pset.getParameter<double>("SumEMaxHEMinus")),
  sumEHFMaxPlus_(pset.getParameter<double>("SumEMaxHFPlus")), 
  sumEHFMaxMinus_(pset.getParameter<double>("SumEMaxHFMinus")) {} 

HCALActivityFilter::~HCALActivityFilter() {}

void HCALActivityFilter::beginJob() {}

bool HCALActivityFilter::filter(edm::Event& event, edm::EventSetup const& setup){

  // Find thresholds FIXME: This should be a Run product
  edm::Handle<std::vector<double> > thresholdsHB;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"thresholdsHB"),thresholdsHB);
  size_t indexThresholdHB = std::lower_bound((*thresholdsHB).begin(),(*thresholdsHB).end(),energyThresholdHB_) - (*thresholdsHB).begin();

  edm::Handle<std::vector<double> > thresholdsHE;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"thresholdsHE"),thresholdsHE);
  size_t indexThresholdHE = std::lower_bound((*thresholdsHE).begin(),(*thresholdsHE).end(),energyThresholdHE_) - (*thresholdsHE).begin();

  edm::Handle<std::vector<double> > thresholdsHF;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"thresholdsHF"),thresholdsHF);
  size_t indexThresholdHF = std::lower_bound((*thresholdsHF).begin(),(*thresholdsHF).end(),energyThresholdHF_) - (*thresholdsHF).begin();

  edm::Handle<std::vector<unsigned int> > nHBPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHBplus"),nHBPlus);

  edm::Handle<std::vector<unsigned int> > nHBMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHBminus"),nHBMinus);
 
  edm::Handle<std::vector<unsigned int> > nHEPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHEplus"),nHEPlus);

  edm::Handle<std::vector<unsigned int> > nHEMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHEminus"),nHEMinus);

  edm::Handle<std::vector<unsigned int> > nHFPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHFplus"),nHFPlus);
  
  edm::Handle<std::vector<unsigned int> > nHFMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHFminus"),nHFMinus);

  edm::Handle<std::vector<double> > sumEHBplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHBplus"),sumEHBplus);

  edm::Handle<std::vector<double> > sumEHBminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHBminus"),sumEHBminus);

  edm::Handle<std::vector<double> > sumEHEplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHEplus"),sumEHEplus);

  edm::Handle<std::vector<double> > sumEHEminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHEminus"),sumEHEminus);

  edm::Handle<std::vector<double> > sumEHFplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHFplus"),sumEHFplus);

  edm::Handle<std::vector<double> > sumEHFminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHFminus"),sumEHFminus);

  int nHB_plus = (*nHBPlus)[indexThresholdHB];
  int nHB_minus = (*nHBMinus)[indexThresholdHB];

  int nHE_plus = (*nHEPlus)[indexThresholdHE];
  int nHE_minus = (*nHEMinus)[indexThresholdHE];

  int nHF_plus = (*nHFPlus)[indexThresholdHF];
  int nHF_minus = (*nHFMinus)[indexThresholdHF];

  double sumEHB_plus = (*sumEHBplus)[indexThresholdHB];
  double sumEHB_minus = (*sumEHBminus)[indexThresholdHB];

  double sumEHE_plus = (*sumEHEplus)[indexThresholdHE];
  double sumEHE_minus = (*sumEHEminus)[indexThresholdHE];

  double sumEHF_plus = (*sumEHFplus)[indexThresholdHF];
  double sumEHF_minus = (*sumEHFminus)[indexThresholdHF];

  bool accept = true;
  if(nHBTowersMaxPlus_ >= 0 && nHB_plus > nHBTowersMaxPlus_) accept = false;
  if(nHBTowersMaxMinus_ >= 0 && nHB_minus > nHBTowersMaxMinus_) accept = false;
  if(nHETowersMaxPlus_ >= 0 && nHE_plus > nHETowersMaxPlus_) accept = false;
  if(nHETowersMaxMinus_ >= 0 && nHE_minus > nHETowersMaxMinus_) accept = false;
  if(nHFTowersMaxPlus_ >= 0 && nHF_plus > nHFTowersMaxPlus_) accept = false;
  if(nHFTowersMaxMinus_ >= 0 && nHF_minus > nHFTowersMaxMinus_) accept = false; 

  if(sumEHBMaxPlus_ >= 0. && sumEHB_plus > sumEHBMaxPlus_) accept = false;
  if(sumEHBMaxMinus_ >= 0. && sumEHB_minus > sumEHBMaxMinus_) accept = false; 
  if(sumEHEMaxPlus_ >= 0. && sumEHE_plus > sumEHEMaxPlus_) accept = false;
  if(sumEHEMaxMinus_ >= 0. && sumEHE_minus > sumEHEMaxMinus_) accept = false;
  if(sumEHFMaxPlus_ >= 0. && sumEHF_plus > sumEHFMaxPlus_) accept = false;
  if(sumEHFMaxMinus_ >= 0. && sumEHF_minus > sumEHFMaxMinus_) accept = false;

  return accept;
}

DEFINE_FWK_MODULE(HCALActivityFilter);
