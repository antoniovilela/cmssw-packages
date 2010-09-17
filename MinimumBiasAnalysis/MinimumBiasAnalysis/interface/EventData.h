#ifndef MinimumBiasAnalysis_EventData_h
#define MinimumBiasAnalysis_EventData_h

class TTree;

namespace minimumBiasAnalysis {
  // Plain struct for event data
  struct EventData {
     int processId_;
     int runNumber_;
     int eventNumber_;
     int lumiSection_;
     int bunchCrossing_;
     //int HLT_MinBiasBSCOR_;
     //int HLT_MinBiasPixel_;
     int HLTPath_;
     int SelectionPath_;
     int nVertex_;
     double posXPrimVtx_;
     double posYPrimVtx_;
     double posZPrimVtx_;
     int multiplicityTracks_;
     double sumPtTracks_;
     double leadingJetPt_;
     double leadingJetEta_;
     double leadingJetPhi_;
     /*int trackMultiplicity_;
     int trackMultiplicityAssociatedToPV_;*/
     int multiplicityHEPlus_;
     int multiplicityHEMinus_;
     double sumEnergyHEPlus_;
     double sumEnergyHEMinus_;
     double sumETHEPlus_;
     double sumETHEMinus_;
     int multiplicityHFPlus_;
     int multiplicityHFMinus_;
     double sumEnergyHFPlus_;
     double sumEnergyHFMinus_;
     double sumETHFPlus_;
     double sumETHFMinus_;
     // 1-13 HF rings, 2.866 < |eta| < 5.205
     int multiplicityHFPlusVsiEta_[13];
     int multiplicityHFMinusVsiEta_[13];
     double sumEHFPlusVsiEta_[13];
     double sumEHFMinusVsiEta_[13];
     double sumETHFPlusVsiEta_[13];
     double sumETHFMinusVsiEta_[13];
     double sumEnergyHEPlusGen_;
     double sumEnergyHEMinusGen_;
     double sumEnergyHFPlusGen_;
     double sumEnergyHFMinusGen_; 
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
