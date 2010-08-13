#include "TROOT.h"
#include "TChain.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

#include <vector>
#include <string>
#include <map>

void setFileNamesPythia8(std::vector<std::string>& fileNames){
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_10_1_wQS.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_11_1_bR9.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_12_1_ZSZ.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_13_1_w1k.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_14_1_ESr.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_15_1_a5u.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_16_1_fDp.root");
   //fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_17_1_kMn.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_18_1_QB9.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_19_1_Spy.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_1_1_aMI.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_20_1_X0X.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_2_1_P6l.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_3_1_F7s.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_4_1_Rzi.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_5_1_4wQ.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_6_1_s4z.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_7_1_YwC.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_8_1_vVs.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_9_1_uP5.root");
}

void setFileNamesPhojet(std::vector<std::string>& fileNames){
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_10_1_DSF.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_11_1_mYX.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_12_1_upI.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_13_1_XAk.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_14_1_h9i.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_15_1_BrR.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_16_1_UfO.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_17_1_4Dg.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_18_1_vS6.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_19_1_Hxv.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_1_1_BHP.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_20_1_zgI.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_2_1_XOv.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_3_1_kbS.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_4_1_A2m.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_5_1_LrE.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_6_1_xWV.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_7_1_AS9.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_8_1_tEb.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_9_1_PkY.root");
}

void setFileNamesPythia6D6T(std::vector<std::string>& fileNames){
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_10_1_VvD.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_11_1_j5D.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_12_1_BkD.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_13_1_cAK.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_14_1_PWq.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_15_1_cWy.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_16_1_T0x.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_17_1_mVb.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_18_1_IXf.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_19_1_dji.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_1_1_bY6.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_20_1_nsZ.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_2_1_LTs.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_3_1_481.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_4_1_Ec1.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_5_1_dcg.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_6_1_J5g.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_7_1_Zbp.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_8_1_RJY.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_9_1_yhE.root");
}

void setFileNamesPythia6DW(std::vector<std::string>& fileNames){
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_10_1_OBJ.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_11_1_rrX.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_12_1_Lv0.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_13_1_abd.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_14_1_h2F.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_15_1_Rju.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_16_1_aIF.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_17_1_DYW.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_18_1_TeK.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_19_1_FmU.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_1_1_Qfw.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_20_1_N2M.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_21_1_4Tf.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_2_1_xoG.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_3_1_27T.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_4_1_YuQ.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_5_1_KBu.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_6_1_T3x.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_7_1_KSB.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_8_1_qDV.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_9_1_N4X.root");
}

void setFileNamesPythia6CW(std::vector<std::string>& fileNames){
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_10_1_Stc.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_11_1_LlL.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_12_1_mP2.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_13_1_4V2.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_14_1_If2.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_1_1_N0Z.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_2_1_m69.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_3_1_EjG.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_4_1_JUO.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_5_1_exe.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_6_1_wLi.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_7_1_Xax.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_8_1_dmj.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_9_1_Cyq.root");
}

void setFileNamesPythia6P0(std::vector<std::string>& fileNames){
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_10_1_Cck.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_11_1_33C.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_12_1_9ZR.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_13_1_wtD.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_14_1_dF1.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_1_1_cwI.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_2_1_Wvr.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_3_1_i8h.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_4_1_S2Z.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_5_1_R5A.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_6_1_uLa.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_7_1_KId.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_8_1_PyB.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_9_1_anL.root");
}

void setFileNamesPythia6Z1(std::vector<std::string>& fileNames){
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_10_1_TGE.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_11_1_F9K.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_12_1_7Xj.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_13_1_jcw.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_14_1_qB1.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_1_1_Sqb.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_2_1_80O.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_3_1_DCa.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_4_1_UKW.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_5_1_RNR.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_6_1_1QN.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_7_1_5PB.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_8_1_i1c.root");
   fileNames.push_back("rfio://castorcms//castor/cern.ch//user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_9_1_nEc.root");
}

void runMinimumBiasTTreeAnalysisMC(){
   gROOT->ProcessLine(".L minimumBiasTTreeAnalysis.C+");

   //run_range_t runRange = Data900GeV;
   //run_range_t runRange = Data2360GeV;
   run_range_t runRange = Data7TeV;
   generator_t genType = PYTHIA;
   generator_t genTune = PYTHIAZ1;
   //generator_t genType = PHOJET;
   //generator_t genTune = PHOJET;

   //std::string outDir = "root/7TeV/Pythia8/SumEnergyMaxHFPlus_8_0";
   //std::string outDir = "root/7TeV/Phojet";
   //std::string outDir = "root/7TeV/Pythia6D6T/SumEnergyMaxHFPlus_8_0";
   //std::string outDir = "root/7TeV/Pythia6DW/SumEnergyMaxHFPlus_8_0";
   //std::string outDir = "root/7TeV/Pythia6CW/SumEnergyMaxHFPlus_8_0";
   //std::string outDir = "root/7TeV/Pythia6P0/SumEnergyMaxHFPlus_8_0";
   std::string outDir = "root/7TeV/Pythia6Z1";

   std::vector<std::string> fileNames(0);
   //setFileNamesPythia8(fileNames);
   //setFileNamesPhojet(fileNames);
   //setFileNamesPythia6D6T(fileNames);
   //setFileNamesPythia6DW(fileNames);
   //setFileNamesPythia6CW(fileNames);
   //setFileNamesPythia6P0(fileNames);
   setFileNamesPythia6Z1(fileNames);

   bool verbose = false;

   // CINT is having some problems with operator+
   //std::string fileName = rootDir + "/";
   //fileName += getTTreeFileName(genTune,runRange);

   std::vector<std::string> selections;
   selections.push_back("eventSelectionBscMinBiasOR");
   /*selections.push_back("processIdPythia6_SD_eventSelectionBscMinBiasOR");
   selections.push_back("processIdPythia6_DD_eventSelectionBscMinBiasOR");
   selections.push_back("processIdPythia6_Diff_eventSelectionBscMinBiasOR");*/
   /*selections.push_back("eventSelectionBscMinBiasORHFVetoPlus");
   selections.push_back("eventSelectionBscMinBiasORHFVetoMinus");
   selections.push_back("eventSelectionBscMinBiasORHEHFVetoPlus");
   selections.push_back("eventSelectionBscMinBiasORHEHFVetoMinus");*/
   /*selections.push_back("eventSelectionBscMinBiasORSumEMaxHFPlus4");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFPlus8");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFPlus12");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFPlus16");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFMinus4");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFMinus8");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFMinus12");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFMinus16");*/   

   std::vector<int> processTypes;
   processTypes.push_back(All);
   processTypes.push_back(SD);
   processTypes.push_back(DD);
   processTypes.push_back(Diff);
   processTypes.push_back(Inelastic);

   for(size_t isel = 0; isel < selections.size(); ++isel){
      std::string treeName = "minimumBiasTTreeAnalysis_" + selections[isel] + "/data";

      TChain* chain = new TChain(treeName.c_str());
      for(size_t ifile = 0; ifile < fileNames.size(); ++ifile) chain->Add(fileNames[ifile].c_str());
 
      for(size_t iproc = 0; iproc < processTypes.size(); ++iproc){
         std::string histosFileName = outDir + "/";
         histosFileName += getHistosFileName(genTune,runRange,processTypes[iproc],selections[isel]);

         /*minimumBiasTTreeAnalysis(fileName,
                                  treeName,
                                  histosFileName,
                                  false,true,
                                  genType,
                                  processTypes[iproc],-1,verbose);*/
         minimumBiasTTreeAnalysis(chain,
                                  histosFileName,
                                  false,true,
                                  genType,
                                  processTypes[iproc],-1,verbose); 

         //std::cout << fileName << "  " << treeName << "  " << histosFileName << "  " << genType << "  " << processTypes[iproc] << std::endl;
      }
   }
}
