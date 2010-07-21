#include "TROOT.h"
#include "TChain.h"

#include <vector>
#include <string>

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

void runMinimumBiasTTreeAnalysis(){
   gROOT->ProcessLine(".L minimumBiasTTreeAnalysis.C+");

   //run_range_t runRange = Data900GeV;
   //run_range_t runRange = Data2360GeV;
   run_range_t runRange = Data7TeV;
   //std::string rootDir = "/tmp/antoniov/root/900GeV";
   //std::string rootDir = "/tmp/antoniov/root/2360GeV";
   //std::string rootDir = "/tmp/antoniov/root/7TeV";
   std::vector<std::string> fileNames;
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinimumBias_Commissioning10-GOODCOLL-Jun14thSkim/Run132440/minimumBiasAnalysisTTree-v3/analysisMinBias_TTree_MinimumBias_1_1_88z.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinimumBias_Commissioning10-GOODCOLL-Jun14thSkim/Run132440/minimumBiasAnalysisTTree-v3/analysisMinBias_TTree_MinimumBias_2_1_iHQ.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinimumBias_Commissioning10-GOODCOLL-Jun14thSkim/Run132440/minimumBiasAnalysisTTree-v3/analysisMinBias_TTree_MinimumBias_3_1_fs5.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinimumBias_Commissioning10-GOODCOLL-Jun14thSkim/Run132440/minimumBiasAnalysisTTree-v3/analysisMinBias_TTree_MinimumBias_4_1_ueb.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinimumBias_Commissioning10-GOODCOLL-Jun14thSkim/Run132440/minimumBiasAnalysisTTree-v3/analysisMinBias_TTree_MinimumBias_5_1_zd8.root");

   //std::string outDir = "root/900GeV/NoSel";
   //std::string outDir = "root/900GeV/SumEnergyMaxHFPlus_8_0";
   //std::string outDir = "root/900GeV/SumEnergyMaxHFMinus_8_0";
   //std::string outDir = "root/900GeV/SumEnergyMaxHFPlus_8_0_HFMinus_8_0";
   //std::string outDir = "root/2360GeV/NoSel";
   //std::string outDir = "root/2360GeV/SumEnergyMaxHFPlus_8_0";
   //std::string outDir = "root/2360GeV/SumEnergyMaxHFMinus_8_0";
   std::string outDir = "root/7TeV/NoSel";
   bool verbose = false;

   /*std::string fileName = rootDir + "/";
   fileName += getTTreeFileName(runRange);*/

   std::vector<std::string> selections;
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_False");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale090_EnergyScaleFactorHCAL_0_9");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale092_EnergyScaleFactorHCAL_0_92");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale095_EnergyScaleFactorHCAL_0_95");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale098_EnergyScaleFactorHCAL_0_98");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale102_EnergyScaleFactorHCAL_1_02");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale105_EnergyScaleFactorHCAL_1_05");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale108_EnergyScaleFactorHCAL_1_08");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale110_EnergyScaleFactorHCAL_1_1");
   selections.push_back("eventSelectionBscMinBiasOR_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_3_0");
   selections.push_back("eventSelectionBscMinBiasOR_HFThresholdIndex_30_EnergyThresholdHF_6_0_EnergyThresholdHBHE_3_0");
   selections.push_back("eventSelectionBscMinBiasOR_HFThresholdIndex_40_EnergyThresholdHF_8_0_EnergyThresholdHBHE_3_0");
   selections.push_back("eventSelectionBscMinBiasOR_HFThresholdIndex_20_EnergyThresholdHF_4_0_EnergyThresholdHBHE_2_0");
   selections.push_back("eventSelection");
   selections.push_back("eventSelectionBscMinBiasORNoColl");
   selections.push_back("eventSelectionBscMinBiasORBPTXOR");

   for(size_t k = 0; k < selections.size(); ++k){
      std::string treeName = "minimumBiasTTreeAnalysis_" + selections[k] + "/data";
      std::string histosFileName = outDir + "/";
      histosFileName += getHistosFileName(runRange,selections[k]);
 
      TChain* chain = new TChain(treeName.c_str()); 
      for(size_t ifile = 0; ifile < fileNames.size(); ++ifile) chain->Add(fileNames[ifile].c_str());

      //minimumBiasTTreeAnalysis(fileName,treeName,histosFileName,false,false,-1,-1,-1,verbose);
      minimumBiasTTreeAnalysis(chain,histosFileName,false,false,-1,-1,-1,verbose);

      //std::cout << fileName << "  " << treeName << "  " << histosFileName << std::endl;
   }
}
