#include "TROOT.h"
#include "TChain.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

#include <vector>
#include <string>
#include <map>

void setFileNamesPythia8Tune4C(std::vector<std::string>& fileNames){
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinBias_Tune4C_7TeV-pythia8_Summer11-NoPU_START42_V11-v1/eventSelectionAnalysis-v1/mergedTTree/eventSelectionAnalysis_TTree_MinBias_merged_0.root");
   /*fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinBias_Tune4C_7TeV-pythia8_Summer11-NoPU_START42_V11-v1/eventSelectionAnalysis-v1/mergedTTree/eventSelectionAnalysis_TTree_MinBias_merged_1.root");
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinBias_Tune4C_7TeV-pythia8_Summer11-NoPU_START42_V11-v1/eventSelectionAnalysis-v1/mergedTTree/eventSelectionAnalysis_TTree_MinBias_merged_2.root");
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinBias_Tune4C_7TeV-pythia8_Summer11-NoPU_START42_V11-v1/eventSelectionAnalysis-v1/mergedTTree/eventSelectionAnalysis_TTree_MinBias_merged_3.root");
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinBias_Tune4C_7TeV-pythia8_Summer11-NoPU_START42_V11-v1/eventSelectionAnalysis-v1/mergedTTree/eventSelectionAnalysis_TTree_MinBias_merged_4.root");
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinBias_Tune4C_7TeV-pythia8_Summer11-NoPU_START42_V11-v1/eventSelectionAnalysis-v1/mergedTTree/eventSelectionAnalysis_TTree_MinBias_merged_5.root");
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinBias_Tune4C_7TeV-pythia8_Summer11-NoPU_START42_V11-v1/eventSelectionAnalysis-v1/mergedTTree/eventSelectionAnalysis_TTree_MinBias_merged_6.root");
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/MinBias_Tune4C_7TeV-pythia8_Summer11-NoPU_START42_V11-v1/eventSelectionAnalysis-v1/mergedTTree/eventSelectionAnalysis_TTree_MinBias_merged_7.root");*/   
}

void setFileNamesPythia8MBR(std::vector<std::string>& fileNames){
   fileNames.push_back("/storage2/antoniov/data1/AnalysisResults/Pythia8MBR_reco423patch3/eventSelection-v1/eventSelectionAnalysis_TTree_MinimumBias_Pythia8MBR-reco423patch3.root");
}

void setFileNamesMinBias(std::vector<std::string>& fileNames){
}

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
   //outDir = "root/7TeV/Pythia8MBR/eventSelection-v1";
   //outDir = "root/7TeV/Pythia8MBR/eventSelection-v1/LogXiGenPlusMax-5.5"; 
   //std::string outDir = "root/7TeV/Pythia8Tune4C/eventSelection-v1";
   //std::string outDir = "root/7TeV/Pythia8Tune4C/eventSelection-v1/LogXiGenPlusMax-5.5";
   std::string outDir = "root/7TeV/Pythia8Tune4C/eventSelection-v1/LogXiGenPlusMax-6.5";

   std::vector<std::string> fileNames;
   setFileNamesPythia8Tune4C(fileNames);
 
   bool verbose = false;

   std::vector<std::string> selections;
   //selections.push_back("minimumBiasTTreeAnalysisAll");
   selections.push_back("minimumBiasTTreeAnalysishltBscMinBiasORBptxPlusORMinus");
   //selections.push_back("minimumBiasTTreeAnalysisBPTX");
   //selections.push_back("minimumBiasTTreeAnalysisBscOr");
   //selections.push_back("minimumBiasTTreeAnalysisVertexFilter");
   //selections.push_back("minimumBiasTTreeAnalysisBeamHaloVeto");
   //selections.push_back("minimumBiasTTreeAnalysisFilterScraping");
   //selections.push_back("minimumBiasTTreeAnalysisHcalNoiseFilter");
   selections.push_back("minimumBiasTTreeAnalysisMultipleVertexVeto");
   selections.push_back("minimumBiasTTreeAnalysisEtaMinFilter");
   selections.push_back("minimumBiasTTreeAnalysisCastorVeto");
 
   std::vector<int> processTypes;
   processTypes.push_back(All);
   //processTypes.push_back(SD);
   processTypes.push_back(DD);
   //processTypes.push_back(Diff);
   processTypes.push_back(Inelastic);
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
