#ifndef CaloTowerNoiseAnalyzer_h
#define CaloTowerNoiseAnalyzer_h
  
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <map>
#include <string> 
#include <memory>

#include "TH1F.h"

namespace CLHEP{
 class RandGaussQ;
}

class TH2F;

class CaloTowerNoiseAnalyzer : public edm::EDAnalyzer
{
 public:
  explicit CaloTowerNoiseAnalyzer(const edm::ParameterSet& conf);
  virtual ~CaloTowerNoiseAnalyzer();
  
  virtual void beginJob();
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
 private:
  edm::InputTag calotowersTag_;
  edm::InputTag hfRecHitsTag_;
  bool accessRecHits_;

  bool excludeHotTowers_;
  unsigned int nThresholdIter_;
  double eThresholdHBMin_;
  double eThresholdHBMax_;
  double eThresholdHEMin_;
  double eThresholdHEMax_; 
  double eThresholdHFMin_;
  double eThresholdHFMax_;

  int nBinsHB_;
  int nBinsHE_;
  int nBinsHF_;

  bool reweightHFTower_;
  std::vector<std::string> reweightHistoName_;
  TH1F reweightHisto_;

  bool applyEnergyOffset_;
  std::auto_ptr<CLHEP::RandGaussQ> gauss_;
  /*double sigmaShort_;
  double sigmaLong_;
  int nIncrements_;*/
  double meanHFEnergy_;
  double sigmaHFEnergy_;

  std::vector<std::pair<int,int> > excludeList_;
  std::vector<double> eThresholdHB_;
  std::vector<double> eThresholdHE_;
  std::vector<double> eThresholdHF_;

  std::map<std::string,TH1F*> histosTH1F_;
  std::map<std::string,TH2F*> histosTH2F_;
  std::vector<TH1F*> histosNHBPlus_;
  std::vector<TH1F*> histosNHBMinus_;
  std::vector<TH1F*> histosSumEHBPlus_;
  std::vector<TH1F*> histosSumEHBMinus_;
  std::vector<TH1F*> histosNHEPlus_;
  std::vector<TH1F*> histosNHEMinus_;
  std::vector<TH1F*> histosSumEHEPlus_;
  std::vector<TH1F*> histosSumEHEMinus_; 
  std::vector<TH1F*> histosNHFPlus_;
  std::vector<TH1F*> histosNHFMinus_;
  std::vector<TH1F*> histosSumEHFPlus_;
  std::vector<TH1F*> histosSumEHFMinus_;
  //std::vector<TH1F*> histosNHFLow_;
  //std::vector<TH1F*> histosNHFHigh_;
};
  
#endif
