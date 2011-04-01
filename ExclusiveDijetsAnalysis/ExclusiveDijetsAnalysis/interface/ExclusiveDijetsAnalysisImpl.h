#ifndef ExclusiveDijetsAnalysis_ExclusiveDijetsAnalysisImpl_h
#define ExclusiveDijetsAnalysis_ExclusiveDijetsAnalysisImpl_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include <string>

class JetCorrector;

namespace exclusiveDijetsAnalysis {

struct EventData;

class ExclusiveDijetsAnalysisImpl {
  public:
     explicit ExclusiveDijetsAnalysisImpl(const edm::ParameterSet&);
     ~ExclusiveDijetsAnalysisImpl();

     void setBeginRun(const edm::Run&, const edm::EventSetup&);
     void fillEventData(EventData&, const edm::Event&, const edm::EventSetup&);
  private:
     template <class Coll>
     std::pair<double,double> xi(Coll& partCollection,bool useJetCorr = false);

     template <class JetColl,class PartColl>
     double Rjj(JetColl& jetCollection,PartColl& partCollection);

     unsigned int nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapTreshToiEta, unsigned int thresholdHF, unsigned int ieta);
 
     void setGenInfo(const edm::Event&, const edm::EventSetup&);

     void fillPileUpInfo(EventData&, const edm::Event&, const edm::EventSetup&);
     void fillVertexInfo(EventData&, const edm::Event&, const edm::EventSetup&);
     void fillJetInfo(EventData&, const edm::Event&, const edm::EventSetup&);
     void fillMultiplicities(EventData&, const edm::Event&, const edm::EventSetup&);
     void fillXiInfo(EventData&, const edm::Event&, const edm::EventSetup&);

     edm::InputTag jetTag_;
     edm::InputTag jetNonCorrTag_;
     edm::InputTag particleFlowTag_;
     edm::InputTag vertexTag_;
     edm::InputTag trackMultiplicityTag_;

     bool doBtag_;
     std::string bTagMode_;
     std::string bDiscriminatorName_;
     double bDiscMin_;

     unsigned int thresholdHF_;
     bool useJetCorrection_;
     bool accessPileUpInfo_;
     double Ebeam_;
     bool usePAT_; 
     bool runOnData_;

     math::XYZTLorentzVector genProtonPlus_;
     math::XYZTLorentzVector genProtonMinus_;
     math::XYZTLorentzVector genAllParticles_;

     std::string jetCorrectionService_;
     const JetCorrector* corrector_;
};

} // namespace
#endif 
