#include "TROOT.h"

#include <vector>
#include <string>

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

void runMinimumBiasTTreeAnalysis(){
   gROOT->ProcessLine(".L minimumBiasTTreeAnalysis.C+");

   //run_range_t runRange = Data900GeV;
   run_range_t runRange = Data2360GeV;
   //std::string rootDir = "/tmp/antoniov/root/900GeV";
   std::string rootDir = "/tmp/antoniov/root/2360GeV";

   //std::string outDir = "root/900GeV/NoSel";
   //std::string outDir = "root/900GeV/SumEnergyMaxHFPlus_8_0";
   //std::string outDir = "root/900GeV/SumEnergyMaxHFMinus_8_0";
   //std::string outDir = "root/900GeV/SumEnergyMaxHFPlus_8_0_HFMinus_8_0";
   std::string outDir = "root/2360GeV/NoSel";
   //std::string outDir = "root/2360GeV/SumEnergyMaxHFPlus_8_0";
   //std::string outDir = "root/2360GeV/SumEnergyMaxHFMinus_8_0";
   bool verbose = false;

   std::string fileName = rootDir + "/";
   fileName += getTTreeFileName(runRange);

   std::vector<std::string> selections;
   selections.push_back("eventSelectionMinBiasBSCOR_ApplyEnergyScaleHCAL_False");
   selections.push_back("eventSelectionMinBiasBSCOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale09_EnergyScaleFactorHCAL_0_9");
   selections.push_back("eventSelectionMinBiasBSCOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale11_EnergyScaleFactorHCAL_1_1");
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_3_0");
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_30_EnergyThresholdHF_6_0_EnergyThresholdHBHE_3_0");
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_40_EnergyThresholdHF_8_0_EnergyThresholdHBHE_3_0");
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_20_EnergyThresholdHF_4_0_EnergyThresholdHBHE_2_0");
   selections.push_back("eventSelectionMinBiasBSCORNoColl");

   for(size_t k = 0; k < selections.size(); ++k){
      std::string treeName = "minimumBiasTTreeAnalysis_" + selections[k] + "/data";
      std::string histosFileName = outDir + "/";
      histosFileName += getHistosFileName(runRange,selections[k]);

      minimumBiasTTreeAnalysis(fileName,treeName,histosFileName,false,false,-1,-1,-1,verbose);

      //std::cout << fileName << "  " << treeName << "  " << histosFileName << std::endl;
   }
}
