#include "TROOT.h"

#include <vector>
#include <string>

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

void runMinimumBiasTTreeAnalysis(){
   gROOT->ProcessLine(".L minimumBiasTTreeAnalysis.C+");

   run_range_t runRange = Data900GeV;
   std::string rootDir = "root/900GeV";
   //std::string outDir = "root/900GeV/NoSel";
   //std::string outDir = "root/900GeV/SumEnergyMaxHFPlus_8_0";
   //std::string outDir = "root/900GeV/SumEnergyMaxHFMinus_8_0";
   std::string outDir = "root/900GeV/SumEnergyMaxHFPlus_8_0_HFMinus_8_0";
   bool verbose = false;

   std::string fileName = rootDir + "/";
   fileName += getTTreeFileName(runRange);

   std::vector<std::string> selections;
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_15_EnergyThresholdHF_3_0_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_16_EnergyThresholdHF_3_2_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_17_EnergyThresholdHF_3_4_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_19_EnergyThresholdHF_3_8_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_20_EnergyThresholdHF_4_0_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_1_0");
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_2_0");

   selections.push_back("eventSelectionMinBiasPixel_HFThresholdIndex_15_EnergyThresholdHF_3_0_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasPixel_HFThresholdIndex_16_EnergyThresholdHF_3_2_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasPixel_HFThresholdIndex_17_EnergyThresholdHF_3_4_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasPixel_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasPixel_HFThresholdIndex_19_EnergyThresholdHF_3_8_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasPixel_HFThresholdIndex_20_EnergyThresholdHF_4_0_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasPixel_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_1_0");
   selections.push_back("eventSelectionMinBiasPixel_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_2_0");

   selections.push_back("eventSelectionMinBiasBSCORNoColl");
   selections.push_back("eventSelectionMinBiasPixelNoColl"); 

   for(size_t k = 0; k < selections.size(); ++k){
      std::string treeName = "minimumBiasTTreeAnalysis_" + selections[k] + "/data";
      std::string histosFileName = outDir + "/";
      histosFileName += getHistosFileName(runRange,selections[k]);

      minimumBiasTTreeAnalysis(fileName,treeName,histosFileName,false,false,-1,-1,-1,verbose);

      //std::cout << fileName << "  " << treeName << "  " << histosFileName << std::endl;
   }
}
