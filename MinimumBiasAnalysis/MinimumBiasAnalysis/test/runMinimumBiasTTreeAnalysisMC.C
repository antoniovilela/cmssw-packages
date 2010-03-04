#include "TROOT.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

#include <vector>
#include <string>
#include <map>

enum run_range_t {Data900GeV,Data2360GeV};

void runMinimumBiasTTreeAnalysisMC(){
   gROOT->ProcessLine(".L minimumBiasTTreeAnalysis.C+");

   run_range_t runRange = Data900GeV;
   //run_range_t runRange = Data2360GeV;
   generator_t genType = PYTHIA;
   //generator_t genType = PHOJET;
   std::string rootDir = "root/900GeV";
   std::string outDir = "root/900GeV/SumEnergyMaxHFPlus_8_0";
   bool verbose = false;

   std::string fileName,type;
   // FIXME
   if(genType == PYTHIA){
      if(runRange == Data900GeV){
         fileName = rootDir + "/" + "analysisMinBias_TTree_PYTHIA_MinBias_STARTUP3X_V8P_900GeV.root";
         type = "PYTHIA_MinBias_900GeV";
      } else if(runRange == Data2360GeV){
         fileName = rootDir + "/" + "analysisMinBias_TTree_PYTHIA_MinBias_STARTUP3X_V8O_2360GeV.root";
         type = "PYTHIA_MinBias_2360GeV";
      } else{
         std::cout << "ERROR: Invalid option" << std::endl;
         return;
      }
   } else if(genType == PHOJET){
      if(runRange == Data900GeV){
         fileName = rootDir + "/" + "analysisMinBias_TTree_PHOJET_MinBias_STARTUP3X_V8P_900GeV.root";
         type = "PHOJET_MinBias_900GeV";
      } else if(runRange == Data2360GeV){
         fileName = rootDir + "/" + "analysisMinBias_TTree_PHOJET_MinBias_STARTUP3X_V8O_2360GeV.root";
         type = "PHOJET_MinBias_2360GeV";
      } else{
         std::cout << "ERROR: Invalid option" << std::endl;
         return;
      }
   } else{
      std::cout << "ERROR: Invalid option" << std::endl;
      return;
   }

   std::vector<std::string> selections;
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_15_EnergyThresholdHF_3_0_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasBSCOR_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasPixel_HFThresholdIndex_15_EnergyThresholdHF_3_0_EnergyThresholdHBHE_1_5");
   selections.push_back("eventSelectionMinBiasPixel_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_1_5");

   std::vector<int> processTypes;
   processTypes.push_back(All);
   processTypes.push_back(SD);
   processTypes.push_back(DD);
   processTypes.push_back(Inelastic);

   for(size_t isel = 0; isel < selections.size(); ++isel){
      std::string treeName = "minimumBiasTTreeAnalysis_" + selections[isel] + "/data";

      for(size_t iproc = 0; iproc < processTypes.size(); ++iproc){
         std::string processName = getProcessCategoryName(processTypes[iproc]);
         std::string histosFileName = outDir + "/" + "analysisMinBiasTTree_" + type + "_" + selections[isel] + "_histos_" + processName + ".root";
         minimumBiasTTreeAnalysis(fileName,
                                  treeName,
                                  histosFileName,
                                  false,true,
                                  genType,
                                  processTypes[iproc],-1,verbose);

         //std::cout << fileName << "  " << treeName << "  " << histosFileName << "  " << genType << "  " << processTypes[iproc] << std::endl;
      }
   }
}
