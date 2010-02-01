#ifndef MinimumBiasAnalysis_MinimumBiasAnalysis_h
#define MinimumBiasAnalysis_MinimumBiasAnalysis_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Math/interface/LorentzVector.h"

namespace minimumBiasAnalysis {

struct EventData;

class MinimumBiasAnalysis {
  public:
     explicit MinimumBiasAnalysis(const edm::ParameterSet&);
     ~MinimumBiasAnalysis();

     void servicesBeginRun(const edm::Run&, const edm::EventSetup&);
     void fillEventData(EventData&, const edm::Event&, const edm::EventSetup&);
  private:
     void fillNoiseInfo(EventData&, const edm::Event&, const edm::EventSetup&);
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

     unsigned int thresholdHF_;
     double energyThresholdHBHE_;
     double energyThresholdHF_;
     double Ebeam_;
     bool accessMCInfo_;

     std::map<int,std::pair<double,double> > thresholdsPFlow_;

     math::XYZTLorentzVector genAllParticles_;
     math::XYZTLorentzVector genProtonPlus_;
     math::XYZTLorentzVector genProtonMinus_;
};

} // namespace
#endif 
