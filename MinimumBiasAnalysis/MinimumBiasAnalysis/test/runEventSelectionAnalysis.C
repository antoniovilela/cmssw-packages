#include "TROOT.h"
#include "TChain.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

#include <vector>
#include <string>
#include <map>

#include "fileNames_MinimumBias_Commissioning10-Jun14thReReco_v1_Run132605_eventSelection-v4.C"

void runEventSelectionAnalysis(){
   gROOT->ProcessLine(".L minimumBiasTTreeAnalysis.C+");

   //run_range_t runRange = Data900GeV;
   //run_range_t runRange = Data2360GeV;
   run_range_t runRange = Data7TeV;

   //std::string outDir = "root/7TeV/Data/Run132605/eventSelection";
   //std::string outDir = "root/7TeV/Data/Run132605/eventSelection/doHcalNoiseSelection";
   std::string outDir = "root/7TeV/Data/Run132605/eventSelection/doHcalNoiseSelection/doXiPlusSelection";
  
   std::vector<std::string> fileNames;
   setFileNamesRun132605(fileNames);
 
   bool verbose = false;

   std::vector<std::string> selections;
   /*selections.push_back("minimumBiasTTreeAnalysisAll");
   selections.push_back("minimumBiasTTreeAnalysishltBscMinBiasORBptxPlusORMinus");
   selections.push_back("minimumBiasTTreeAnalysisBPTX");
   selections.push_back("minimumBiasTTreeAnalysisBscOr");
   selections.push_back("minimumBiasTTreeAnalysisVertexFilter");
   selections.push_back("minimumBiasTTreeAnalysisBeamHaloVeto");
   selections.push_back("minimumBiasTTreeAnalysisFilterScraping");*/
   selections.push_back("minimumBiasTTreeAnalysisHBHENoiseFilter");
 
   for(size_t isel = 0; isel < selections.size(); ++isel){
      std::string treeName = selections[isel] + "/data";

      TChain* chain = new TChain(treeName.c_str());
      for(size_t ifile = 0; ifile < fileNames.size(); ++ifile) chain->Add(fileNames[ifile].c_str());
 
      std::string histosFileName = outDir + "/";
      histosFileName += getHistosFileName(runRange,selections[isel]);

      //minimumBiasTTreeAnalysis(fileName,treeName,histosFileName,false,false,-1,-1,-1,verbose);
      minimumBiasTTreeAnalysis(chain,histosFileName,false,false,-1,-1,-1,verbose);
   }
}
