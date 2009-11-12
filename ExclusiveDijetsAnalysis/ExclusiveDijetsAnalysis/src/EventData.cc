
#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/EventData.h"
#include "TTree.h"

namespace exclusiveDijetsAnalysis {
  void addTTreeBranches(TTree& data, EventData& eventData){
     data.Branch("nPileUpBx0",&eventData.nPileUpBx0_,"nPileUpBx0/I");
     data.Branch("nVertex",&eventData.nVertex_,"nVertex/I");
     data.Branch("leadingJetPt",&eventData.leadingJetPt_,"leadingJetPt/D");
     data.Branch("leadingJetEta",&eventData.leadingJetEta_,"leadingJetEta/D");
     data.Branch("leadingJetPhi",&eventData.leadingJetPhi_,"leadingJetPhi/D");
     data.Branch("leadingJetBDiscriminator",&eventData.leadingJetBDiscriminator_,"leadingJetBDiscriminator/D");
     data.Branch("secondJetPt",&eventData.secondJetPt_,"secondJetPt/D");
     data.Branch("secondJetEta",&eventData.secondJetEta_,"secondJetEta/D");
     data.Branch("secondJetPhi",&eventData.secondJetPhi_,"secondJetPhi/D");
     data.Branch("secondJetBDiscriminator",&eventData.secondJetBDiscriminator_,"secondJetBDiscriminator/D");
     data.Branch("thirdJetPt",&eventData.thirdJetPt_,"thirdJetPt/D");
     data.Branch("thirdJetEta",&eventData.thirdJetEta_,"thirdJetEta/D");
     data.Branch("jetsAveEta",&eventData.jetsAveEta_,"jetsAveEta/D");
     data.Branch("jetsDeltaEta",&eventData.jetsDeltaEta_,"jetsDeltaEta/D");
     data.Branch("jetsDeltaPhi",&eventData.jetsDeltaPhi_,"jetsDeltaPhi/D");
     data.Branch("jetsDeltaPt",&eventData.jetsDeltaPt_,"jetsDeltaPt/D");
     data.Branch("trackMultiplicity",&eventData.trackMultiplicity_,"trackMultiplicity/I");
     data.Branch("multiplicityHFPlus",&eventData.multiplicityHFPlus_,"multiplicityHFPlus/I");
     data.Branch("multiplicityHFMinus",&eventData.multiplicityHFMinus_,"multiplicityHFMinus/I");
     data.Branch("sumWeightsHFPlus",&eventData.sumWeightsHFPlus_,"sumWeightsHFPlus/D");
     data.Branch("sumWeightsHFMinus",&eventData.sumWeightsHFMinus_,"sumWeightsHFMinus/D");
     data.Branch("sumEnergyHFPlus",&eventData.sumEnergyHFPlus_,"sumEnergyHFPlus/D");
     data.Branch("sumEnergyHFMinus",&eventData.sumEnergyHFMinus_,"sumEnergyHFMinus/D");
     data.Branch("sumEnergyWeightedHFPlus",&eventData.sumEnergyWeightedHFPlus_,"sumEnergyWeightedHFPlus/D");
     data.Branch("sumEnergyWeightedHFMinus",&eventData.sumEnergyWeightedHFMinus_,"sumEnergyWeightedHFMinus/D");
     // 1-13 HF rings, 2.866 < |eta| < 5.205, 29 <= ieta <= 41
     // FIXME 
     data.Branch("multiplicityHFPlusVsiEta",eventData.multiplicityHFPlusVsiEta_,"multiplicityHFPlusVsiEta[13]/I");
     data.Branch("multiplicityHFMinusVsiEta",eventData.multiplicityHFMinusVsiEta_,"multiplicityHFMinusVsiEta[13]/I");
     data.Branch("xiGenPlus",&eventData.xiGenPlus_,"xiGenPlus/D");
     data.Branch("xiGenMinus",&eventData.xiGenMinus_,"xiGenMinus/D");
     data.Branch("xiTowerPlus",&eventData.xiTowerPlus_,"xiTowerPlus/D");
     data.Branch("xiTowerMinus",&eventData.xiTowerMinus_,"xiTowerMinus/D");
     data.Branch("xiPlusFromJets",&eventData.xiPlusFromJets_,"xiPlusFromJets/D");
     data.Branch("xiMinusFromJets",&eventData.xiMinusFromJets_,"xiMinusFromJets/D");
     data.Branch("xiPlusFromPFCands",&eventData.xiPlusFromPFCands_,"xiPlusFromPFCands/D");
     data.Branch("xiMinusFromPFCands",&eventData.xiMinusFromPFCands_,"xiMinusFromPFCands/D");
     data.Branch("missingMassFromXi",&eventData.missingMassFromXi_,"missingMassFromXi/D");
     data.Branch("massDijets",&eventData.massDijets_,"massDijets/D");
     data.Branch("massDijetsGen",&eventData.massDijetsGen_,"massDijetsGen/D");
     data.Branch("MxFromJets",&eventData.MxFromJets_,"MxFromJets/D");
     data.Branch("MxFromPFCands",&eventData.MxFromPFCands_,"MxFromPFCands/D");
     data.Branch("MxGen",&eventData.MxGen_,"MxGen/D");
     data.Branch("RjjFromJets",&eventData.RjjFromJets_,"RjjFromJets/D");
     data.Branch("RjjFromPFCands",&eventData.RjjFromPFCands_,"RjjFromPFCands/D");
     data.Branch("RjjGen",&eventData.RjjGen_,"RjjGen/D");
  }

  void setTTreeBranches(TTree& data, EventData& eventData){
     data.SetBranchAddress("nPileUpBx0",&eventData.nPileUpBx0_);
     data.SetBranchAddress("nVertex",&eventData.nVertex_);
     data.SetBranchAddress("leadingJetPt",&eventData.leadingJetPt_);
     data.SetBranchAddress("leadingJetEta",&eventData.leadingJetEta_);
     data.SetBranchAddress("leadingJetPhi",&eventData.leadingJetPhi_);
     data.SetBranchAddress("leadingJetBDiscriminator",&eventData.leadingJetBDiscriminator_);
     data.SetBranchAddress("secondJetPt",&eventData.secondJetPt_);
     data.SetBranchAddress("secondJetEta",&eventData.secondJetEta_);
     data.SetBranchAddress("secondJetPhi",&eventData.secondJetPhi_);
     data.SetBranchAddress("secondJetBDiscriminator",&eventData.secondJetBDiscriminator_);
     data.SetBranchAddress("thirdJetPt",&eventData.thirdJetPt_);
     data.SetBranchAddress("thirdJetEta",&eventData.thirdJetEta_);
     data.SetBranchAddress("jetsAveEta",&eventData.jetsAveEta_);
     data.SetBranchAddress("jetsDeltaEta",&eventData.jetsDeltaEta_);
     data.SetBranchAddress("jetsDeltaPhi",&eventData.jetsDeltaPhi_);
     data.SetBranchAddress("jetsDeltaPt",&eventData.jetsDeltaPt_);
     data.SetBranchAddress("trackMultiplicity",&eventData.trackMultiplicity_);
     data.SetBranchAddress("multiplicityHFPlus",&eventData.multiplicityHFPlus_);
     data.SetBranchAddress("multiplicityHFMinus",&eventData.multiplicityHFMinus_);
     data.SetBranchAddress("sumWeightsHFPlus",&eventData.sumWeightsHFPlus_);
     data.SetBranchAddress("sumWeightsHFMinus",&eventData.sumWeightsHFMinus_);
     data.SetBranchAddress("sumEnergyHFPlus",&eventData.sumEnergyHFPlus_);
     data.SetBranchAddress("sumEnergyHFMinus",&eventData.sumEnergyHFMinus_);
     data.SetBranchAddress("sumEnergyWeightedHFPlus",&eventData.sumEnergyWeightedHFPlus_);
     data.SetBranchAddress("sumEnergyWeightedHFMinus",&eventData.sumEnergyWeightedHFMinus_);
     // 1-13 HF rings, 2.866 < |eta| < 5.205
     // FIXME 
     data.SetBranchAddress("multiplicityHFPlusVsiEta",eventData.multiplicityHFPlusVsiEta_);
     data.SetBranchAddress("multiplicityHFMinusVsiEta",eventData.multiplicityHFMinusVsiEta_);
     data.SetBranchAddress("xiGenPlus",&eventData.xiGenPlus_);
     data.SetBranchAddress("xiGenMinus",&eventData.xiGenMinus_);
     data.SetBranchAddress("xiTowerPlus",&eventData.xiTowerPlus_);
     data.SetBranchAddress("xiTowerMinus",&eventData.xiTowerMinus_);
     data.SetBranchAddress("xiPlusFromJets",&eventData.xiPlusFromJets_);
     data.SetBranchAddress("xiMinusFromJets",&eventData.xiMinusFromJets_);
     data.SetBranchAddress("xiPlusFromPFCands",&eventData.xiPlusFromPFCands_);
     data.SetBranchAddress("xiMinusFromPFCands",&eventData.xiMinusFromPFCands_);
     data.SetBranchAddress("missingMassFromXi",&eventData.missingMassFromXi_);
     data.SetBranchAddress("massDijets",&eventData.massDijets_);
     data.SetBranchAddress("massDijetsGen",&eventData.massDijetsGen_);
     data.SetBranchAddress("MxFromJets",&eventData.MxFromJets_);
     data.SetBranchAddress("MxFromPFCands",&eventData.MxFromPFCands_);
     data.SetBranchAddress("MxGen",&eventData.MxGen_);
     data.SetBranchAddress("RjjFromJets",&eventData.RjjFromJets_);
     data.SetBranchAddress("RjjFromPFCands",&eventData.RjjFromPFCands_);
     data.SetBranchAddress("RjjGen",&eventData.RjjGen_);
  }
}
