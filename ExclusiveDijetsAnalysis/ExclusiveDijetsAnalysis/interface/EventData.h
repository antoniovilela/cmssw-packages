#ifndef ExclusiveDijetsAnalysis_EventData_h
#define ExclusiveDijetsAnalysis_EventData_h

class TTree;

namespace exclusiveDijetsAnalysis {
  // Plain struct for event data
  struct EventData {
     int nPileUpBx0_;
     int nVertex_;
     double leadingJetPt_;
     double leadingJetEta_;
     double leadingJetPhi_;
     double leadingJetBDiscriminator_;
     double secondJetPt_;
     double secondJetEta_;
     double secondJetPhi_;
     double secondJetBDiscriminator_;
     double thirdJetPt_;
     double thirdJetEta_;
     double jetsAveEta_; //redundant
     double jetsDeltaEta_; //redundant
     double jetsDeltaPhi_; //redundant
     double jetsDeltaPt_; //redundant
     int trackMultiplicity_;
     int multiplicityHFPlus_;
     int multiplicityHFMinus_;
     double sumEnergyHFPlus_;
     double sumEnergyHFMinus_;
     // 1-13 HF rings, 2.866 < |eta| < 5.205
     int multiplicityHFPlusVsiEta_[13];
     int multiplicityHFMinusVsiEta_[13];
     double xiGenPlus_;
     double xiGenMinus_;
     double xiTowerPlus_;
     double xiTowerMinus_;
     double xiPlusFromJets_;
     double xiMinusFromJets_;
     double xiPlusFromPFCands_;
     double xiMinusFromPFCands_;
     double missingMassFromXi_; //redundant
     double massDijets_;
     double massDijetsGen_;
     double MxFromJets_;
     double MxFromPFCands_;
     double MxGen_;
     double RjjFromJets_;
     double RjjFromPFCands_;
     double RjjGen_;
     /*double xiPlusFromJetsAfterSel_;
     double xiMinusFromJetsAfterSel_;
     double xiPlusFromPFCandsAfterSel_;
     double xiMinusFromPFCandsAfterSel_;
     double RjjFromJetsAfterSel_;
     double RjjFromPFCandsAfterSel_;*/
  };

  void addTTreeBranches(TTree& data, EventData& eventData);
  void setTTreeBranches(TTree& data, EventData& eventData);
}

#endif
