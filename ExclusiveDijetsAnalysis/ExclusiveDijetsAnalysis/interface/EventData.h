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
     double massDijets_;
     double massDijetsGen_;
     double missingMassFromXi_;
     double MxFromJets_;
     double RjjFromJets_;
     double RjjFromPFCands_;
     double xiPlusAfterSel_;
     double xiMinusAfterSel_;
     double RjjAfterSel_;
     double RjjFromPFAfterSel_;
  };

  void addTTreeBranches(TTree& data, EventData& eventData);
  void setTTreeBranches(TTree& data, EventData& eventData);
}

#endif
