#ifndef CaloTowerAnalyzer_h
#define CaloTowerAnalyzer_h
  
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

class CaloTowerAnalyzer : public edm::EDAnalyzer
{
 public:
  
  explicit CaloTowerAnalyzer(const edm::ParameterSet& conf);
  
  virtual ~CaloTowerAnalyzer();
  
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

  struct {
     TH1F *histNEvents_;
     TH1F *histenergyHF_;
     TH1F *histenergyHFplus_;
     TH1F *histenergyHFminus_;
     TH1F *histemEnergyHF_;
     TH1F *histhadEnergyHF_;
     TH1F *histemFractionHF_; 
     TH1F *histenergyvseta_;
     TH2F *histetavsphi_;
     TH2F *histietavsiphi_;
     TH2F *histetavsphiweighted_;
     TH2F *histietavsiphiweighted_;
     TH1F *histenergyvsetaHF_;
     TH2F *histetavsphiHF_;
     TH2F *histetavsphiHFweighted_;
     TH1F *histenergyvsetaHFMax_;
     TH2F *histetavsphiHFMax_;
     TH1F *histhbtiming_;
     TH1F *histhfhitenergy_;
     TH1F *histhflongenergy_;
     TH1F *histhfshortenergy_;
     TH1F *histhfhitenergyplus_;
     TH1F *histhflongenergyplus_;
     TH1F *histhfshortenergyplus_;
     TH1F *histhfhitenergyminus_;
     TH1F *histhflongenergyminus_;
     TH1F *histhfshortenergyminus_;
     TH1F *histhfenergyvstime_;
     TH1F *histhflongenergyFromTwr_;
     TH1F *histhfshortenergyFromTwr_;
  } histos_;
};
  
#endif
