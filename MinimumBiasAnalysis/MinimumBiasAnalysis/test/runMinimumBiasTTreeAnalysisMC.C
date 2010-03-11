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
   generator_t genTune = PYTHIADW;
   //generator_t genType = PHOJET;
   //generator_t genTune = PHOJET;
   std::string rootDir = "root/900GeV";
   //std::string outDir = "root/900GeV/NoSel";
   //std::string outDir = "root/900GeV/SumEnergyMaxHFPlus_8_0";
   std::string outDir = "root/900GeV/SumEnergyMaxHFMinus_8_0";
   //std::string outDir = "root/900GeV/SumEnergyMaxHFPlus_8_0_HFMinus_8_0";
   bool verbose = false;

   // CINT is having some problems with operator+
   std::string fileName = rootDir + "/";
   fileName += getTTreeFileName(genTune,runRange);

   std::vector<std::string> selections;
   selections.push_back("eventSelectionMinBiasBSCOR");
   selections.push_back("eventSelectionMinBiasPixel");

   std::vector<int> processTypes;
   processTypes.push_back(All);
   processTypes.push_back(SD);
   processTypes.push_back(DD);
   processTypes.push_back(Diff);
   processTypes.push_back(Inelastic);

   for(size_t isel = 0; isel < selections.size(); ++isel){
      std::string treeName = "minimumBiasTTreeAnalysis_" + selections[isel] + "/data";

      for(size_t iproc = 0; iproc < processTypes.size(); ++iproc){
         std::string histosFileName = outDir + "/";
         histosFileName += getHistosFileName(genTune,runRange,processTypes[iproc],selections[isel]);

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
