#include "TROOT.h"
#include "TChain.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

#include <vector>
#include <string>
#include <map>

//#include "fileNames_MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1_eventSelection-v4.C"
//#include "fileNames_MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1_eventSelection-v5.C"
#include "fileNames_MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1_eventSelection-v6.C"

//#include "fileNames_MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1-eventSelection-v2.C"

void runEventSelectionAnalysisMC(){
   gROOT->ProcessLine(".L minimumBiasTTreeAnalysis.C+");

   //run_range_t runRange = Data900GeV;
   //run_range_t runRange = Data2360GeV;
   run_range_t runRange = Data7TeV;
   generator_t genType = PYTHIA8;
   generator_t genTune = PYTHIA8;
   //generator_t genType = PHOJET;
   //generator_t genTune = PHOJET;

   std::string outDir;
   outDir = "root/7TeV/Pythia8/eventSelection-v6";
   //outDir = "root/7TeV/Pythia8/eventSelection-v6/doEtaMaxSelection_etaMax1";
   //outDir = "root/7TeV/Pythia8/eventSelection-v6/doEtaMinSelection_etaMin-1";
   //outDir = "root/7TeV/Pythia6Z1/eventSelection-v2";

   std::vector<std::string> fileNames;
   setFileNamesPythia8(fileNames);
   //setFileNamesPythia6(fileNames);
 
   bool verbose = false;

   std::vector<std::string> selections;
   //selections.push_back("minimumBiasTTreeAnalysisAll");
   selections.push_back("minimumBiasTTreeAnalysishltBscMinBiasORBptxPlusORMinus");
   //selections.push_back("minimumBiasTTreeAnalysisBPTX");
   selections.push_back("minimumBiasTTreeAnalysisBscOr");
   selections.push_back("minimumBiasTTreeAnalysisVertexFilter");
   //selections.push_back("minimumBiasTTreeAnalysisBeamHaloVeto");
   //selections.push_back("minimumBiasTTreeAnalysisFilterScraping");
   selections.push_back("minimumBiasTTreeAnalysisHcalNoiseFilter");
   selections.push_back("minimumBiasTTreeAnalysisEtaMaxFilter");
 
   std::vector<int> processTypes;
   //processTypes.push_back(All);
   //processTypes.push_back(SD);
   processTypes.push_back(DD);
   //processTypes.push_back(Diff);
   //processTypes.push_back(Inelastic);
   processTypes.push_back(SDPlus);
   processTypes.push_back(SDMinus);

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
