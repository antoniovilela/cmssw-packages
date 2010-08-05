#include "TROOT.h"
#include "TChain.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

#include <vector>
#include <string>
#include <map>

void setFileNamesPythia8(std::vector<std::string>& fileNames){
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_10_1_Xcl.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_11_1_Ofx.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_12_1_e0t.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_13_1_8b7.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_14_1_dL9.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_15_1_qsl.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_16_1_8bh.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_17_1_ZKN.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_18_1_iov.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_19_1_Dph.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_1_1_Hhb.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_20_1_Nng.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_2_1_63S.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_3_1_4pi.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_4_1_cHn.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_5_1_GBy.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_6_2_yHy.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_7_1_dyw.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_8_1_6sP.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_7TeV-pythia8_Spring10-START3X_V26B-v1/minimumBiasAnalysisTTree-v1/analysisMinBias_TTree_MinBias_9_1_qgP.root");
}

void setFileNamesPythia6(std::vector<std::string>& fileNames){
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_10_1_Ndv.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_11_1_uf1.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_12_1_MDZ.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_13_1_P8A.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_15_1_BCq.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_17_1_7z8.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_18_1_VNd.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_19_1_0Ml.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_1_1_cnP.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_20_1_pEq.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_2_1_jIM.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_3_1_wWE.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_4_1_mzR.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_5_1_biP.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_7_1_XTN.root");
   fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/crab_output/AnalysisResults/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2/minimumBiasAnalysisTTree-v2/analysisMinBias_TTree_MinBias_9_1_GRr.root");
}

void runMinimumBiasTTreeAnalysisMC(){
   gROOT->ProcessLine(".L minimumBiasTTreeAnalysis.C+");

   //run_range_t runRange = Data900GeV;
   //run_range_t runRange = Data2360GeV;
   run_range_t runRange = Data7TeV;
   generator_t genType = PYTHIA;
   generator_t genTune = PYTHIA;
   //generator_t genType = PHOJET;
   //generator_t genTune = PHOJET;
   //std::string rootDir = "/tmp/antoniov/root/900GeV";
   //std::string rootDir = "/tmp/antoniov/root/2360GeV";

   //std::string outDir = "root/900GeV/NoSel";
   //std::string outDir = "root/900GeV/SumEnergyMaxHFPlus_8_0";
   //std::string outDir = "root/900GeV/SumEnergyMaxHFMinus_8_0";
   //std::string outDir = "root/900GeV/SumEnergyMaxHFPlus_8_0_HFMinus_8_0";
   //std::string outDir = "root/2360GeV/NoSel";
   //std::string outDir = "root/2360GeV/SumEnergyMaxHFPlus_8_0";
   //std::string outDir = "root/2360GeV/SumEnergyMaxHFMinus_8_0";
   std::string outDir = "root/7TeV/Pythia6";
   //std::string outDir = "root/7TeV/Pythia6";

   std::vector<std::string> fileNames(0);
   //setFileNamesPythia8(fileNames);
   setFileNamesPythia6(fileNames);

   bool verbose = false;

   // CINT is having some problems with operator+
   //std::string fileName = rootDir + "/";
   //fileName += getTTreeFileName(genTune,runRange);

   std::vector<std::string> selections;
   selections.push_back("eventSelectionBscMinBiasOR");
   /*selections.push_back("processIdPythia6_SD_eventSelectionBscMinBiasOR");
   selections.push_back("processIdPythia6_DD_eventSelectionBscMinBiasOR");
   selections.push_back("processIdPythia6_Diff_eventSelectionBscMinBiasOR");*/
   selections.push_back("eventSelectionBscMinBiasORHFVetoPlus");
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
   selections.push_back("eventSelectionBscMinBiasORSumEMaxHFMinus16");   

   std::vector<int> processTypes;
   processTypes.push_back(All);
   processTypes.push_back(SD);
   processTypes.push_back(DD);
   processTypes.push_back(Diff);
   processTypes.push_back(Inelastic);

   for(size_t isel = 0; isel < selections.size(); ++isel){
      std::string treeName = "minimumBiasTTreeAnalysis_" + selections[isel] + "/data";

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
