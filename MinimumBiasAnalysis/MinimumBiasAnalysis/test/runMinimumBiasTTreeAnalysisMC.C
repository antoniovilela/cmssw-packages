#include "TROOT.h"
#include "TChain.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

#include <vector>
#include <string>
#include <map>

void setFileNamesPythia8Tune4C(std::vector<std::string>& fileNames){
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinBias_Tune4C_7TeV-pythia8_Summer11-NoPU_START42_V11-v1/minimumBiasAnalysisTTree-v5/mergedTTree//analysisMinBias_TTree_MinBias_0.root");
   //fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinBias_Tune4C_7TeV-pythia8_Summer11-NoPU_START42_V11-v1/minimumBiasAnalysisTTree-v5/mergedTTree//analysisMinBias_TTree_MinBias_1.root");
   //fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinBias_Tune4C_7TeV-pythia8_Summer11-NoPU_START42_V11-v1/minimumBiasAnalysisTTree-v5/mergedTTree//analysisMinBias_TTree_MinBias_2.root");
   //fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinBias_Tune4C_7TeV-pythia8_Summer11-NoPU_START42_V11-v1/minimumBiasAnalysisTTree-v5/mergedTTree//analysisMinBias_TTree_MinBias_3.root");
}

void setFileNamesPythia8MBR(std::vector<std::string>& fileNames){
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/Pythia8MBR_reco423patch3/minimumBiasTTreeAnalysis-v4/analysisMinBias_TTree_MinBias_Pythia8MBR-reco423patch3.root");
}

void setFileNamesPhojet(std::vector<std::string>& fileNames){
}

void setFileNamesPythia6D6T(std::vector<std::string>& fileNames){
}

void setFileNamesPythia6DW(std::vector<std::string>& fileNames){
}

void setFileNamesPythia6CW(std::vector<std::string>& fileNames){
}

void setFileNamesPythia6P0(std::vector<std::string>& fileNames){
}

void setFileNamesPythia6Z1(std::vector<std::string>& fileNames){
}

void setFileNamesMinBias(std::vector<std::string>& fileNames){
}

void runMinimumBiasTTreeAnalysisMC(){
   gROOT->ProcessLine(".L minimumBiasTTreeAnalysis.C+");

   //run_range_t runRange = Data900GeV;
   //run_range_t runRange = Data2360GeV;
   run_range_t runRange = Data7TeV;
   generator_t genType = PYTHIA8;
   generator_t genTune = PYTHIA8;
   //generator_t genType = PHOJET;
   //generator_t genTune = PHOJET;

   //std::string outDir = "root/7TeV/";
   //std::string outDir = "root/7TeV/Pythia8Tune4C/minimumBiasTTreeAnalysis-v5/LogXiGenPlusMax-5.5";
   std::string outDir = "root/7TeV/Pythia8Tune4C/minimumBiasTTreeAnalysis-v5";

   std::vector<std::string> fileNames(0);
   setFileNamesPythia8Tune4C(fileNames);

   bool verbose = false;

   // CINT is having some problems with operator+
   //std::string fileName = rootDir + "/";
   //fileName += getTTreeFileName(genTune,runRange);

   std::vector<std::string> selections;
   selections.push_back("eventSelection");
   selections.push_back("eventSelectionBeamHaloVeto");
   selections.push_back("eventSelectionBscMinBiasOR");
   selections.push_back("eventSelectionBscMinBiasOREtaMaxFilter");
   selections.push_back("eventSelectionBscMinBiasOREtaMinFilter");
   selections.push_back("eventSelectionBscMinBiasOREtaMinFilterCastorVeto");
   selections.push_back("eventSelectionBscMinBiasOREtaMinFilterCastorTag");

   std::vector<int> processTypes;
   processTypes.push_back(All);
   processTypes.push_back(SDPlus);
   processTypes.push_back(SDMinus);
   processTypes.push_back(DD);
   //processTypes.push_back(Diff);
   processTypes.push_back(Inelastic);

   for(size_t isel = 0; isel < selections.size(); ++isel){
      std::string treeName = "minimumBiasTTreeAnalysis_" + selections[isel] + "/data";

      TChain* chain = new TChain(treeName.c_str());
      for(size_t ifile = 0; ifile < fileNames.size(); ++ifile){
         std::cout << "Adding file " << fileNames[ifile] << std::endl;
         chain->Add(fileNames[ifile].c_str());
      }
 
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
