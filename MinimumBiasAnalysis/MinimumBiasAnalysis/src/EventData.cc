
#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/EventData.h"
#include "TTree.h"

namespace minimumBiasAnalysis {
  void addTTreeBranches(TTree& data, EventData& eventData){
     data.Branch("nVertex",&eventData.nVertex_,"nVertex/I");
     data.Branch("posXPrimVtx",&eventData.posXPrimVtx_,"posXPrimVtx/D");
     data.Branch("posYPrimVtx",&eventData.posYPrimVtx_,"posYPrimVtx/D");
     data.Branch("posZPrimVtx",&eventData.posZPrimVtx_,"posZPrimVtx/D");
     data.Branch("leadingJetPt",&eventData.leadingJetPt_,"leadingJetPt/D");
     data.Branch("leadingJetEta",&eventData.leadingJetEta_,"leadingJetEta/D");
     data.Branch("leadingJetPhi",&eventData.leadingJetPhi_,"leadingJetPhi/D");
     data.Branch("trackMultiplicity",&eventData.trackMultiplicity_,"trackMultiplicity/I");
     data.Branch("trackMultiplicityAssociatedToPV",&eventData.trackMultiplicityAssociatedToPV_,"trackMultiplicityAssociatedToPV/I");
     data.Branch("multiplicityHFPlus",&eventData.multiplicityHFPlus_,"multiplicityHFPlus/I");
     data.Branch("multiplicityHFMinus",&eventData.multiplicityHFMinus_,"multiplicityHFMinus/I");
     data.Branch("sumEnergyHFPlus",&eventData.sumEnergyHFPlus_,"sumEnergyHFPlus/D");
     data.Branch("sumEnergyHFMinus",&eventData.sumEnergyHFMinus_,"sumEnergyHFMinus/D");
     // 1-13 HF rings, 2.866 < |eta| < 5.205
     data.Branch("multiplicityHFPlusVsiEta",eventData.multiplicityHFPlusVsiEta_,"multiplicityHFPlusVsiEta[13]/I");
     data.Branch("multiplicityHFMinusVsiEta",eventData.multiplicityHFMinusVsiEta_,"multiplicityHFMinusVsiEta[13]/I");
     data.Branch("xiGenPlus",&eventData.xiGenPlus_,"xiGenPlus/D");
     data.Branch("xiGenMinus",&eventData.xiGenMinus_,"xiGenMinus/D");
     data.Branch("xiPlusFromTowers",&eventData.xiPlusFromTowers_,"xiPlusFromTowers/D");
     data.Branch("xiMinusFromTowers",&eventData.xiMinusFromTowers_,"xiMinusFromTowers/D");
     //data.Branch("xiPlusFromJets",&eventData.xiPlusFromJets_,"xiPlusFromJets/D");
     //data.Branch("xiMinusFromJets",&eventData.xiMinusFromJets_,"xiMinusFromJets/D");
     data.Branch("xiPlusFromPFCands",&eventData.xiPlusFromPFCands_,"xiPlusFromPFCands/D");
     data.Branch("xiMinusFromPFCands",&eventData.xiMinusFromPFCands_,"xiMinusFromPFCands/D");
     data.Branch("missingMassFromXiFromTowers",&eventData.missingMassFromXiFromTowers_,"missingMassFromXiFromTowers/D");
     //data.Branch("missingMassFromXiFromJets",&eventData.missingMassFromXiFromJets_,"missingMassFromXiFromJets/D");
     data.Branch("missingMassFromXiFromPFCands",&eventData.missingMassFromXiFromPFCands_,"missingMassFromXiFromPFCands/D");
     data.Branch("EPlusPzFromTowers",&eventData.EPlusPzFromTowers_,"EPlusPzFromTowers/D");
     data.Branch("EMinusPzFromTowers",&eventData.EMinusPzFromTowers_,"EMinusPzFromTowers/D");
     //data.Branch("EPlusPzFromJets",&eventData.EPlusPzFromJets_,"EPlusPzFromJets/D");
     //data.Branch("EMinusPzFromJets",&eventData.EMinusPzFromJets_,"EMinusPzFromJets/D");
     data.Branch("EPlusPzFromPFCands",&eventData.EPlusPzFromPFCands_,"EPlusPzFromPFCands/D");
     data.Branch("EMinusPzFromPFCands",&eventData.EMinusPzFromPFCands_,"EMinusPzFromPFCands/D");
     data.Branch("MxGen",&eventData.MxGen_,"MxGen/D");
     data.Branch("MxFromTowers",&eventData.MxFromTowers_,"MxFromTowers/D");
     //data.Branch("MxFromJets",&eventData.MxFromJets_,"MxFromJets/D");
     data.Branch("MxFromPFCands",&eventData.MxFromPFCands_,"MxFromPFCands/D");
     data.Branch("sumET",&eventData.sumET_,"sumET/D");
     data.Branch("MEx",&eventData.MEx_,"MEx/D");
     data.Branch("MEy",&eventData.MEy_,"/D");     
     data.Branch("MET",&eventData.MET_,"/D");
     data.Branch("BeamHaloLooseId",&eventData.BeamHaloLooseId_,"BeamHaloLooseId/I");
     data.Branch("BeamHaloTightId",&eventData.BeamHaloTightId_,"BeamHaloTightId/I");
     data.Branch("LooseNoiseFilter",&eventData.LooseNoiseFilter_,"LooseNoiseFilter/I");
     data.Branch("TightNoiseFilter",&eventData.TightNoiseFilter_,"TightNoiseFilter/I");
  }

  void setTTreeBranches(TTree& data, EventData& eventData){
     data.SetBranchAddress("nVertex",&eventData.nVertex_);
     data.SetBranchAddress("posXPrimVtx",&eventData.posXPrimVtx_);
     data.SetBranchAddress("posYPrimVtx",&eventData.posYPrimVtx_);
     data.SetBranchAddress("posZPrimVtx",&eventData.posZPrimVtx_);
     data.SetBranchAddress("leadingJetPt",&eventData.leadingJetPt_);
     data.SetBranchAddress("leadingJetEta",&eventData.leadingJetEta_);
     data.SetBranchAddress("leadingJetPhi",&eventData.leadingJetPhi_);
     data.SetBranchAddress("trackMultiplicity",&eventData.trackMultiplicity_);
     data.SetBranchAddress("trackMultiplicityAssociatedToPV",&eventData.trackMultiplicityAssociatedToPV_);
     data.SetBranchAddress("multiplicityHFPlus",&eventData.multiplicityHFPlus_);
     data.SetBranchAddress("multiplicityHFMinus",&eventData.multiplicityHFMinus_);
     data.SetBranchAddress("sumEnergyHFPlus",&eventData.sumEnergyHFPlus_);
     data.SetBranchAddress("sumEnergyHFMinus",&eventData.sumEnergyHFMinus_);
     // 1-13 HF rings, 2.866 < |eta| < 5.205
     data.SetBranchAddress("multiplicityHFPlusVsiEta",eventData.multiplicityHFPlusVsiEta_);
     data.SetBranchAddress("multiplicityHFMinusVsiEta",eventData.multiplicityHFMinusVsiEta_);
     data.SetBranchAddress("xiGenPlus",&eventData.xiGenPlus_);
     data.SetBranchAddress("xiGenMinus",&eventData.xiGenMinus_);
     data.SetBranchAddress("xiPlusFromTowers",&eventData.xiPlusFromTowers_);
     data.SetBranchAddress("xiMinusFromTowers",&eventData.xiMinusFromTowers_);
     //data.SetBranchAddress("xiPlusFromJets",&eventData.xiPlusFromJets_);
     //data.SetBranchAddress("xiMinusFromJets",&eventData.xiMinusFromJets_);
     data.SetBranchAddress("xiPlusFromPFCands",&eventData.xiPlusFromPFCands_);
     data.SetBranchAddress("xiMinusFromPFCands",&eventData.xiMinusFromPFCands_);
     data.SetBranchAddress("missingMassFromXiFromTowers",&eventData.missingMassFromXiFromTowers_);
     //data.SetBranchAddress("missingMassFromXiFromJets",&eventData.missingMassFromXiFromJets_);
     data.SetBranchAddress("missingMassFromXiFromPFCands",&eventData.missingMassFromXiFromPFCands_);
     data.SetBranchAddress("EPlusPzFromTowers",&eventData.EPlusPzFromTowers_);
     data.SetBranchAddress("EMinusPzFromTowers",&eventData.EMinusPzFromTowers_);
     //data.SetBranchAddress("EPlusPzFromJets",&eventData.EPlusPzFromJets_);
     //data.SetBranchAddress("EMinusPzFromJets",&eventData.EMinusPzFromJets_);
     data.SetBranchAddress("EPlusPzFromPFCands",&eventData.EPlusPzFromPFCands_);
     data.SetBranchAddress("EMinusPzFromPFCands",&eventData.EMinusPzFromPFCands_);
     data.SetBranchAddress("MxGen",&eventData.MxGen_);
     data.SetBranchAddress("MxFromTowers",&eventData.MxFromTowers_);
     //data.SetBranchAddress("MxFromJets",&eventData.MxFromJets_);
     data.SetBranchAddress("MxFromPFCands",&eventData.MxFromPFCands_);
     data.SetBranchAddress("sumET",&eventData.sumET_);
     data.SetBranchAddress("MEx",&eventData.MEx_);
     data.SetBranchAddress("MEy",&eventData.MEy_);
     data.SetBranchAddress("MET",&eventData.MET_);
     data.SetBranchAddress("BeamHaloLooseId",&eventData.BeamHaloLooseId_);
     data.SetBranchAddress("BeamHaloTightId",&eventData.BeamHaloTightId_);
     data.SetBranchAddress("LooseNoiseFilter",&eventData.LooseNoiseFilter_);
     data.SetBranchAddress("TightNoiseFilter",&eventData.TightNoiseFilter_);
  }
}
