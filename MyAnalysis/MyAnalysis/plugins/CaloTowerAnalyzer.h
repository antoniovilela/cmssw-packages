#ifndef CaloTowerAnalyzer_h
#define CaloTowerAnalyzer_h
  
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
 
class TH1F;
class TH2F;

class CaloTowerAnalyzer : public edm::EDAnalyzer
{
 public:
  
  explicit CaloTowerAnalyzer(const edm::ParameterSet& conf);
  
  virtual ~CaloTowerAnalyzer();
  
  virtual void beginJob(edm::EventSetup const& iSetup);
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  
 private:

  std::vector<TH1F*> histosnhfplus_;
  std::vector<TH1F*> histosnhfminus_; 	 
  std::vector<TH1F*> histosnhflow_;
  std::vector<TH1F*> histosnhfhigh_;

  TH1F *histenergyHF_;
  TH1F *histenergyHFplus_;
  TH1F *histenergyHFminus_;	
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

  edm::InputTag calotowersTag_;
  bool accessRecHits_;

  bool excludeHotTowers_;
  std::vector<double> eThreshold_;
  unsigned int nThresholdIter_;	
  double eThresholdHFMin_;
  double eThresholdHFMax_;	

  int nBinsHF_;

  std::vector<std::pair<int,int> > excludeList_;
};
  
#endif
 


