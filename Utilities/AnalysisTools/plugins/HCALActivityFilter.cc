
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

       double sumETHBMaxPlus_;
       double sumETHBMaxMinus_;
       double sumETHEMaxPlus_;
       double sumETHEMaxMinus_;
       double sumETHFMaxPlus_;
       double sumETHFMaxMinus_; 
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
  sumEHFMaxMinus_(pset.getParameter<double>("SumEMaxHFMinus")),
  sumETHBMaxPlus_(pset.getParameter<double>("SumETMaxHBPlus")),
  sumETHBMaxMinus_(pset.getParameter<double>("SumETMaxHBMinus")),
  sumETHEMaxPlus_(pset.getParameter<double>("SumETMaxHEPlus")),
  sumETHEMaxMinus_(pset.getParameter<double>("SumETMaxHEMinus")),
  sumETHFMaxPlus_(pset.getParameter<double>("SumETMaxHFPlus")),
  sumETHFMaxMinus_(pset.getParameter<double>("SumETMaxHFMinus")) {} 

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
 
  edm::Handle<std::vector<double> > sumETHBplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHBplus"),sumETHBplus);

  edm::Handle<std::vector<double> > sumETHBminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHBminus"),sumETHBminus);

  edm::Handle<std::vector<double> > sumETHEplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHEplus"),sumETHEplus);

  edm::Handle<std::vector<double> > sumETHEminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHEminus"),sumETHEminus);

  edm::Handle<std::vector<double> > sumETHFplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHFplus"),sumETHFplus);

  edm::Handle<std::vector<double> > sumETHFminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHFminus"),sumETHFminus);

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

  double sumETHB_plus = (*sumETHBplus)[indexThresholdHB];
  double sumETHB_minus = (*sumETHBminus)[indexThresholdHB];

  double sumETHE_plus = (*sumETHEplus)[indexThresholdHE];
  double sumETHE_minus = (*sumETHEminus)[indexThresholdHE];

  double sumETHF_plus = (*sumETHFplus)[indexThresholdHF];
  double sumETHF_minus = (*sumETHFminus)[indexThresholdHF];

  bool accept = true;
  if( (nHBTowersMaxPlus_ >= 0 && nHB_plus > nHBTowersMaxPlus_) ||
      (nHBTowersMaxMinus_ >= 0 && nHB_minus > nHBTowersMaxMinus_) ||
      (nHETowersMaxPlus_ >= 0 && nHE_plus > nHETowersMaxPlus_) ||
      (nHETowersMaxMinus_ >= 0 && nHE_minus > nHETowersMaxMinus_) ||
      (nHFTowersMaxPlus_ >= 0 && nHF_plus > nHFTowersMaxPlus_) ||
      (nHFTowersMaxMinus_ >= 0 && nHF_minus > nHFTowersMaxMinus_) ||
      (sumEHBMaxPlus_ >= 0. && sumEHB_plus > sumEHBMaxPlus_) ||
      (sumEHBMaxMinus_ >= 0. && sumEHB_minus > sumEHBMaxMinus_) ||
      (sumEHEMaxPlus_ >= 0. && sumEHE_plus > sumEHEMaxPlus_) ||
      (sumEHEMaxMinus_ >= 0. && sumEHE_minus > sumEHEMaxMinus_) ||
      (sumEHFMaxPlus_ >= 0. && sumEHF_plus > sumEHFMaxPlus_) ||
      (sumEHFMaxMinus_ >= 0. && sumEHF_minus > sumEHFMaxMinus_) ||
      (sumETHBMaxPlus_ >= 0. && sumETHB_plus > sumETHBMaxPlus_) ||
      (sumETHBMaxMinus_ >= 0. && sumETHB_minus > sumETHBMaxMinus_) ||
      (sumETHEMaxPlus_ >= 0. && sumETHE_plus > sumETHEMaxPlus_) ||
      (sumETHEMaxMinus_ >= 0. && sumETHE_minus > sumETHEMaxMinus_) ||
      (sumETHFMaxPlus_ >= 0. && sumETHF_plus > sumETHFMaxPlus_) ||
      (sumETHFMaxMinus_ >= 0. && sumETHF_minus > sumETHFMaxMinus_) ) accept = false;

  return accept;
}

DEFINE_FWK_MODULE(HCALActivityFilter);
