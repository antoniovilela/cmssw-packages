#ifndef MinimumBiasAnalysis_MinimumBiasAnalysis_h
#define MinimumBiasAnalysis_MinimumBiasAnalysis_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include <vector>
#include <string>
#include <map>

namespace minimumBiasAnalysis {

struct EventData;

class MinimumBiasAnalysis {
  public:
     explicit MinimumBiasAnalysis(const edm::ParameterSet&);
     ~MinimumBiasAnalysis();

     void servicesBeginRun(const edm::Run&, const edm::EventSetup&);
     void fillEventData(EventData&, const edm::Event&, const edm::EventSetup&);
  private:
     void fillEventInfo(EventData&, const edm::Event&, const edm::EventSetup&);
     void fillNoiseInfo(EventData&, const edm::Event&, const edm::EventSetup&);
     void fillTriggerInfo(EventData&, const edm::Event&, const edm::EventSetup&);
     void fillVertexInfo(EventData&, const edm::Event&, const edm::EventSetup&);
     void fillMETInfo(EventData&, const edm::Event&, const edm::EventSetup&);
     void fillJetInfo(EventData&, const edm::Event&, const edm::EventSetup&);
     void fillMultiplicities(EventData&, const edm::Event&, const edm::EventSetup&);
     void fillEventVariables(EventData&, const edm::Event&, const edm::EventSetup&);

     edm::InputTag vertexTag_;
     edm::InputTag metTag_;
     edm::InputTag jetTag_;
     edm::InputTag caloTowerTag_;
     edm::InputTag particleFlowTag_;
     edm::InputTag triggerResultsTag_;
     //edm::InputTag hfTowerSummaryTag_;
     edm::InputTag hcalTowerSummaryTag_;

     //unsigned int thresholdHF_;
     double energyThresholdHB_;
     double energyThresholdHE_;
     double energyThresholdHF_;
     double Ebeam_;
     bool applyEnergyScaleHCAL_;
     double energyScaleHCAL_;
     bool accessMCInfo_;
     //std::vector<std::string> > hltPathNames_;
     std::string hltPathName_; 
     
     std::map<int,std::pair<double,double> > thresholdsPFlow_;

     math::XYZTLorentzVector genAllParticles_;
     math::XYZTLorentzVector genAllParticlesHEPlus_;
     math::XYZTLorentzVector genAllParticlesHEMinus_;
     math::XYZTLorentzVector genAllParticlesHFPlus_;
     math::XYZTLorentzVector genAllParticlesHFMinus_;
     math::XYZTLorentzVector genProtonPlus_;
     math::XYZTLorentzVector genProtonMinus_;
};

} // namespace
#endif 
