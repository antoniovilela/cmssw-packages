#ifndef MinimumBiasAnalysis_EventData_h
#define MinimumBiasAnalysis_EventData_h

class TTree;

namespace minimumBiasAnalysis {
  // Plain struct for event data
  struct EventData {
     int runNumber_;
     int eventNumber_;
     int lumiSection_;
     int nVertex_;
     double posXPrimVtx_;
     double posYPrimVtx_;
     double posZPrimVtx_;
     double leadingJetPt_;
     double leadingJetEta_;
     double leadingJetPhi_;
     int trackMultiplicity_;
     int trackMultiplicityAssociatedToPV_;
     int multiplicityHFPlus_;
     int multiplicityHFMinus_;
     double sumEnergyHFPlus_;
     double sumEnergyHFMinus_;
     // 1-13 HF rings, 2.866 < |eta| < 5.205
     int multiplicityHFPlusVsiEta_[13];
     int multiplicityHFMinusVsiEta_[13];
     double xiGenPlus_;
     double xiGenMinus_;
     double xiPlusFromTowers_;
     double xiMinusFromTowers_;
     //double xiPlusFromJets_;
     //double xiMinusFromJets_;
     double xiPlusFromPFCands_;
     double xiMinusFromPFCands_;
     double missingMassFromXiFromTowers_;
     //double missingMassFromXiFromJets_;
     double missingMassFromXiFromPFCands_;
     double EPlusPzFromTowers_;
     double EMinusPzFromTowers_;
     //double EPlusPzFromJets_;
     //double EMinusPzFromJets_;
     double EPlusPzFromPFCands_;
     double EMinusPzFromPFCands_;
     double MxGen_;
     double MxFromTowers_;
     //double MxFromJets_;
     double MxFromPFCands_;
     double sumET_;
     double MEx_;
     double MEy_;
     double MET_;
     int BeamHaloLooseId_;
     int BeamHaloTightId_;
     int LooseNoiseFilter_;
     int TightNoiseFilter_;
  };

  void addTTreeBranches(TTree& data, EventData& eventData);
  void setTTreeBranches(TTree& data, EventData& eventData);
}

#endif
