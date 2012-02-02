#include "TROOT.h"
#include "TChain.h"

#include <vector>
#include <string>

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

void setFileNamesZeroBiasRun132605(std::vector<std::string>& fileNames){
   fileNames.push_back();
}

void setFileNamesZeroBiasRun135528(std::vector<std::string>& fileNames){
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/ZeroBias_Commissioning10-May19ReReco-v1/Run135528/minimumBiasAnalysisTTree-v4/mergedTTree/analysisMinBias_TTree_MinimumBias_merged_0.root");
}

void setFileNamesMinimumBiasRun135528(std::vector<std::string>& fileNames){
   //fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinimumBias_Commissioning10-May19ReReco-v1/Run135528/minimumBiasAnalysisTTree-v2/mergedTTree/analysisMinBias_TTree_MinimumBias_merged_0.root");
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinimumBias_Commissioning10-May19ReReco-v1/Run135528/minimumBiasAnalysisTTree-v8/mergedTTree/analysisMinBias_TTree_MinimumBias_merged_0.root");
   /*fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinimumBias_Commissioning10-May19ReReco-v1/Run135528/minimumBiasAnalysisTTree-v8/mergedTTree/analysisMinBias_TTree_MinimumBias_1.root");
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinimumBias_Commissioning10-May19ReReco-v1/Run135528/minimumBiasAnalysisTTree-v8/mergedTTree/analysisMinBias_TTree_MinimumBias_2.root");*/
}

void setFileNamesMinimumBiasRun132605(std::vector<std::string>& fileNames){
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinimumBias_Commissioning10-May19ReReco-v1/Run132605/minimumBiasAnalysisTTree-v2/mergedTTree/analysisMinBias_TTree_MinimumBias_merged_0.root");
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
   setFileNamesMinimumBiasRun132605(fileNames);
   //setFileNamesMinimumBiasRun135528(fileNames);
   //setFileNamesZeroBiasRun135528(fileNames);
 
   //std::string outDir = "root";
   std::string outDir = "root/7TeV/Data/Run132605/minimumBiasTTreeAnalysis-v2";
   //std::string outDir = "root/7TeV/Data/Run135528/minimumBiasTTreeAnalysis-v8";
   //std::string outDir = "root/7TeV/Data/Run135528/minimumBiasTTreeAnalysis-v8/SingleLogXiBin";
   //std::string outDir = "root/7TeV/ZeroBias/Run135528/minimumBiasTTreeAnalysis-v4";
   bool verbose = false;

   /*std::string fileName = rootDir + "/";
   fileName += getTTreeFileName(runRange);*/

   std::vector<std::string> selections;
   /*selections.push_back("eventSelectionBPTX");
   selections.push_back("eventSelectionL1Tech4");
   selections.push_back("eventSelectionL1Tech4BeamHaloVeto");
   selections.push_back("eventSelectionL1Tech4BscOr");
   selections.push_back("eventSelectionL1Tech4HLTBscMinBiasOR");*/
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
