#ifndef MinimumBiasAnalysis_MinimumBiasAnalysis_h
#define MinimumBiasAnalysis_MinimumBiasAnalysis_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
//#include "DataFormats/Math/interface/LorentzVector.h"

#include <vector>
#include <string>
#include <map>

struct MinimumBiasEventData;

namespace minimumBiasAnalysis {

class MinimumBiasAnalysis {
  public:
     explicit MinimumBiasAnalysis(const edm::ParameterSet&);
     ~MinimumBiasAnalysis();

     void servicesBeginRun(const edm::Run&, const edm::EventSetup&);
     void fillEventData(MinimumBiasEventData&, const edm::Event&, const edm::EventSetup&);
  private:
     void fillEventInfo(MinimumBiasEventData&, const edm::Event&, const edm::EventSetup&);
     void fillNoiseInfo(MinimumBiasEventData&, const edm::Event&, const edm::EventSetup&);
     void fillTriggerInfo(MinimumBiasEventData&, const edm::Event&, const edm::EventSetup&);
     void fillVertexInfo(MinimumBiasEventData&, const edm::Event&, const edm::EventSetup&);
     void fillTrackInfo(MinimumBiasEventData&, const edm::Event&, const edm::EventSetup&);
     void fillMETInfo(MinimumBiasEventData&, const edm::Event&, const edm::EventSetup&);
     void fillJetInfo(MinimumBiasEventData&, const edm::Event&, const edm::EventSetup&);
     void fillMultiplicities(MinimumBiasEventData&, const edm::Event&, const edm::EventSetup&);
     void fillEventVariables(MinimumBiasEventData&, const edm::Event&, const edm::EventSetup&);

     void resetPFThresholds(std::map<int,std::pair<double,double> >&);
     void setPFThresholds(std::map<int,std::pair<double,double> >&, edm::ParameterSet const&);

     edm::InputTag vertexTag_;
     edm::InputTag trackTag_;
     edm::InputTag metTag_;
     edm::InputTag jetTag_;
     edm::InputTag caloTowerTag_;
     edm::InputTag castorRecHitTag_;
     edm::InputTag particleFlowTag_;
     edm::InputTag genChargedTag_;
     edm::InputTag triggerResultsTag_;
     edm::InputTag hcalTowerSummaryTag_;

     double energyThresholdHB_;
     double energyThresholdHE_;
     double energyThresholdHF_;
     double Ebeam_;
     bool applyEnergyScaleHCAL_;
     double energyScaleHCAL_;
     bool accessMCInfo_;
     //std::vector<std::string> > hltPathNames_;
     std::string hltPathName_; 
     int ttBit_; 

     std::map<int,std::pair<double,double> > thresholdsPFlowBarrel_;
     std::map<int,std::pair<double,double> > thresholdsPFlowEndcap_;
     std::map<int,std::pair<double,double> > thresholdsPFlowTransition_;
     std::map<int,std::pair<double,double> > thresholdsPFlowForward_;
     std::map<int,std::map<int,std::pair<double,double> > > thresholdsPFlow_;

     /*math::XYZTLorentzVector genAllParticles_;
     math::XYZTLorentzVector genAllParticlesHEPlus_;
     math::XYZTLorentzVector genAllParticlesHEMinus_;
     math::XYZTLorentzVector genAllParticlesHFPlus_;
     math::XYZTLorentzVector genAllParticlesHFMinus_;
     math::XYZTLorentzVector genProtonPlus_;
     math::XYZTLorentzVector genProtonMinus_;*/
};

} // namespace
#endif 
