#include "TROOT.h"
#include "TChain.h"

#include <vector>
#include <string>

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

//#include "fileNames_MinimumBias_Commissioning10-Jun14thReReco_v1_Run132605_minimumBiasAnalysisTTree-v4.C"

void setFileNamesZeroBiasRun132605(std::vector<std::string>& fileNames){
   fileNames.push_back("/data1/antoniov/AnalysisResults/ZeroBias_Commissioning10-Jun14thReReco_v1_RECO/analysisMinBias_ZeroBias_Commissioning10-Jun14thReReco_v1_Run132605_minimumBiasAnalysisTTree-v5.root");
}

void setFileNamesMinimumBiasRun135528(std::vector<std::string>& fileNames){
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinimumBias_Commissioning10-May19ReReco-v1/Run135528/minimumBiasAnalysisTTree-v2/mergedTTree/analysisMinBias_TTree_MinimumBias_merged_0.root");
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
   //setFileNamesMinimumBias(fileNames);
   setFileNamesMinimumBiasRun135528(fileNames);
 
   std::string outDir = "root/7TeV/Data/Run135528/minimumBiasAnalysisTTree-v2";
   //std::string outDir = "root/7TeV/ZeroBias/Run132605";
   //std::string outDir = "root";
   bool verbose = false;

   /*std::string fileName = rootDir + "/";
   fileName += getTTreeFileName(runRange);*/

   std::vector<std::string> selections;
   selections.push_back("eventSelectionBscMinBiasOR");
   selections.push_back("eventSelectionBscMinBiasOREtaMaxFilter");
   selections.push_back("eventSelectionBscMinBiasOREtaMinFilter");
   selections.push_back("eventSelectionBscMinBiasOREtaMinFilterCastorVeto");
   selections.push_back("eventSelectionBscMinBiasOREtaMinFilterCastorTag");

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
