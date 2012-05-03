#ifndef Utilities_AnalysisTools_PFlowNoiseAnalyzer_h
#define Utilities_AnalysisTools_PFlowNoiseAnalyzer_h
 
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h" 

#include <string>
#include <map>

class TH1F;
class TH2F;

class PFlowNoiseAnalyzer : public edm::EDAnalyzer
{
 public:
  explicit PFlowNoiseAnalyzer(const edm::ParameterSet&);
  virtual ~PFlowNoiseAnalyzer();
  
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
 private:
  double corrEnergyHF(double, double); 
 
  edm::InputTag particleFlowTag_;
  bool applyHFEnergyCorrection_;

  std::map<std::string,TH1F*> histosTH1F_;
  std::map<std::string,TH2F*> histosTH2F_;
};
  
#endif
