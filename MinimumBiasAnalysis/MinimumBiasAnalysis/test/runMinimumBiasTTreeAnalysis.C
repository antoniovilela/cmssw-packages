#include "TROOT.h"
#include "TChain.h"

#include <vector>
#include <string>

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

//#include "fileNames_MinimumBias_Commissioning10-Jun14thReReco_v1_Run132605_minimumBiasAnalysisTTree-v4.C"

void setFileNamesZeroBiasRun132605(std::vector<std::string>& fileNames){
   fileNames.push_back("/data1/antoniov/AnalysisResults/ZeroBias_Commissioning10-Jun14thReReco_v1_RECO/analysisMinBias_ZeroBias_Commissioning10-Jun14thReReco_v1_Run132605_minimumBiasAnalysisTTree-v5.root");
}

void setFileNamesMinimumBias(std::vector<std::string>& fileNames){
   fileNames.push_back("analysisMinBias_TTree_MinimumBias.root");
}

void runMinimumBiasTTreeAnalysis(){
   gROOT->ProcessLine(".L minimumBiasTTreeAnalysis.C+");

   //run_range_t runRange = Data900GeV;
   //run_range_t runRange = Data2360GeV;
   run_range_t runRange = Data7TeV;
   //std::string rootDir = "/tmp/antoniov/root/900GeV";
   //std::string rootDir = "/tmp/antoniov/root/2360GeV";
   //std::string rootDir = "/tmp/antoniov/root/7TeV";
   std::vector<std::string> fileNames(0);
   setFileNamesMinimumBias(fileNames);
   //setFileNamesRun132440(fileNames);
   //setFileNamesRun132605(fileNames);
   //setFileNamesRun132605V2(fileNames);
   //setFileNamesZeroBiasRun132605(fileNames);
 
   //std::string outDir = "root/7TeV/Data/Run132605/minimumBiasAnalysisTTree-v4";
   //std::string outDir = "root/7TeV/ZeroBias/Run132605";
   std::string outDir = "root";
   bool verbose = false;

   /*std::string fileName = rootDir + "/";
   fileName += getTTreeFileName(runRange);*/

   std::vector<std::string> selections;
   selections.push_back("eventSelectionBscMinBiasOR");
   selections.push_back("eventSelectionBscMinBiasOREtaMaxFilter");
   selections.push_back("eventSelectionBscMinBiasOREtaMinFilter");
   /*selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HCALTowerSummaryTag_hcalActivitySummaryScale090_EnergyScaleFactorHCAL_0_9");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HCALTowerSummaryTag_hcalActivitySummaryScale092_EnergyScaleFactorHCAL_0_92");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HCALTowerSummaryTag_hcalActivitySummaryScale095_EnergyScaleFactorHCAL_0_95");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HCALTowerSummaryTag_hcalActivitySummaryScale098_EnergyScaleFactorHCAL_0_98");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HCALTowerSummaryTag_hcalActivitySummaryScale102_EnergyScaleFactorHCAL_1_02");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HCALTowerSummaryTag_hcalActivitySummaryScale105_EnergyScaleFactorHCAL_1_05");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HCALTowerSummaryTag_hcalActivitySummaryScale108_EnergyScaleFactorHCAL_1_08");
   selections.push_back("eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HCALTowerSummaryTag_hcalActivitySummaryScale110_EnergyScaleFactorHCAL_1_1");*/
   /*selections.push_back("eventSelectionBscMinBiasOR_EnergyThresholdHF_3_6");
   selections.push_back("eventSelectionBscMinBiasOR_EnergyThresholdHF_6_0");
   selections.push_back("eventSelectionBscMinBiasOR_EnergyThresholdHF_8_0");
   selections.push_back("eventSelectionBscMinBiasOR_EnergyThresholdHE_1_6_EnergyThresholdHB_1_2");
   selections.push_back("eventSelectionBscMinBiasOR_EnergyThresholdHE_2_4_EnergyThresholdHB_1_8");*/
   /*selections.push_back("eventSelection");
   selections.push_back("eventSelectionL1Tech4");
   selections.push_back("eventSelectionBPTX");
   selections.push_back("eventSelectionBscMinBiasORNoColl");
   selections.push_back("eventSelectionBscMinBiasORBPTXOR");*/
   /*selections.push_back("eventSelectionBscMinBiasORHFVetoPlus");
   selections.push_back("eventSelectionBscMinBiasORHFVetoMinus");
   selections.push_back("eventSelectionBscMinBiasORHEHFVetoPlus");
   selections.push_back("eventSelectionBscMinBiasORHEHFVetoMinus");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFPlus4");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFPlus8");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFPlus12");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFPlus16");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFMinus4");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFMinus8");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFMinus12");
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFMinus16");*/

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
