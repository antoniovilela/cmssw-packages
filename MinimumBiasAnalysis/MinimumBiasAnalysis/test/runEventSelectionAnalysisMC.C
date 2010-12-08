#include "TROOT.h"
#include "TChain.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

#include <vector>
#include <string>
#include <map>

void runEventSelectionAnalysis(){
   gROOT->ProcessLine(".L minimumBiasTTreeAnalysis.C+");

   //run_range_t runRange = Data900GeV;
   //run_range_t runRange = Data2360GeV;
   run_range_t runRange = Data7TeV;
   generator_t genType = PYTHIA;
   generator_t genTune = PYTHIAD6T;
   //generator_t genType = PHOJET;
   //generator_t genTune = PHOJET;

   std::string outDir = "root/7TeV/EventSelection/PYTHIA6D6T";

   std::vector<std::string> fileNames(1,"eventSelectionAnalysis_TTree_MinimumBias.root");
    
   bool verbose = false;

   std::vector<std::string> selections;
   selections.push_back("minimumBiasTTreeAnalysisAll");
   selections.push_back("minimumBiasTTreeAnalysisL1CollBscOr");
   selections.push_back("minimumBiasTTreeAnalysishltBscMinBiasORBptxPlusORMinus");
   selections.push_back("minimumBiasTTreeAnalysisVertexFilter");
   selections.push_back("minimumBiasTTreeAnalysisFilterScraping");
   selections.push_back("minimumBiasTTreeAnalysisHBHENoiseFilter");
 
   std::vector<int> processTypes;
   processTypes.push_back(All);
   processTypes.push_back(SD);
   processTypes.push_back(DD);
   processTypes.push_back(Diff);
   processTypes.push_back(Inelastic);

   for(size_t isel = 0; isel < selections.size(); ++isel){
      std::string treeName = selections[isel] + "/data";

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
