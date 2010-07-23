#ifndef MinimumBiasAnalysis_RootTools_h
#define MinimumBiasAnalysis_RootTools_h

#include <string>
#include <vector>

#include "Utilities/PlottingTools/interface/PlottingTools.h"

enum generator_t {PYTHIA,PHOJET,PYTHIADW,PYTHIAP0,PYTHIAProQ20,PYTHIACW,PYTHIA8};
enum process_category_t {All,SD,NonSD,Inelastic,DD,Diff};
enum run_range_t {Data900GeV,Data2360GeV,Data7TeV};

std::string getProcessCategoryName(int processCategory);
void getProcessIdsPYTHIA(std::vector<int>& processIDs, std::vector<std::string>& processNames);
void getProcessIdsPHOJET(std::vector<int>& processIDs, std::vector<std::string>& processNames);
void getSelectedProcIdsPYTHIA(int processCategory,std::vector<int>& selectedProcIds);
void getSelectedProcIdsPHOJET(int processCategory,std::vector<int>& selectedProcIds);

std::string getDataLabel(int runRange);
std::string getDataLabel(int genType, int runRange);
std::string getTTreeFileName(int runRange);
std::string getTTreeFileName(int genType, int runRange);
std::string getHistosFileName(int runRange, std::string const& selection);
std::string getHistosFileName(int genType, int runRange, int processType, std::string const& selection);

//==========================================================================================

std::string getDataLabel(int runRange){
   std::string label;
   if(runRange == Data900GeV) label = "MinimumBias_900GeV";
   else if(runRange == Data2360GeV) label = "MinimumBias_2360GeV";
   else if(runRange == Data7TeV) label = "MinimumBias_7TeV";
   else throw RootException("ERROR: Invalid option");

   return label;
}

std::string getDataLabel(int genType, int runRange){
   std::string label;
   if(genType == PYTHIA){
      if(runRange == Data900GeV) label = "PYTHIA_MinBias_900GeV";
      else if(runRange == Data2360GeV) label = "PYTHIA_MinBias_2360GeV";
      else if(runRange == Data7TeV) label = "PYTHIA_MinBias_7TeV"; 
      else throw RootException("ERROR: Invalid option");
   } else if(genType == PHOJET){
      if(runRange == Data900GeV) label = "PHOJET_MinBias_900GeV";
      else if(runRange == Data2360GeV) label = "PHOJET_MinBias_2360GeV";
      else if(runRange == Data7TeV) label = "PHOJET_MinBias_7TeV"; 
      else throw RootException("ERROR: Invalid option");
   } else if(genType == PYTHIADW){
      if(runRange == Data900GeV) label = "PYTHIADW_MinBias_900GeV";
      else if(runRange == Data2360GeV) label = "PYTHIADW_MinBias_2360GeV";
      else if(runRange == Data7TeV) label = "PYTHIADW_MinBias_7TeV"; 
      else throw RootException("ERROR: Invalid option");
   } else if(genType == PYTHIAP0){
      if(runRange == Data900GeV) label = "PYTHIAP0_MinBias_900GeV";
      else if(runRange == Data2360GeV) label = "PYTHIAP0_MinBias_2360GeV";
      else if(runRange == Data7TeV) label = "PYTHIAP0_MinBias_7TeV";
      else throw RootException("ERROR: Invalid option");
   } else if(genType == PYTHIAProQ20){
      if(runRange == Data900GeV) label = "PYTHIAProQ20_MinBias_900GeV";
      else if(runRange == Data2360GeV) label = "PYTHIAProQ20_MinBias_2360GeV";
      else if(runRange == Data7TeV) label = "PYTHIAProQ20_MinBias_7TeV";
      else throw RootException("ERROR: Invalid option");
   } else if(genType == PYTHIACW){
      if(runRange == Data900GeV) label = "PYTHIACW_MinBias_900GeV";
      else if(runRange == Data2360GeV) label = "PYTHIACW_MinBias_2360GeV";
      else if(runRange == Data7TeV) label = "PYTHIACW_MinBias_7TeV";
      else throw RootException("ERROR: Invalid option");
   } else if(genType == PYTHIA8){
      if(runRange == Data900GeV) label = "PYTHIA8_MinBias_900GeV";
      else if(runRange == Data2360GeV) label = "PYTHIA8_MinBias_2360GeV";
      else if(runRange == Data7TeV) label = "PYTHIA8_MinBias_7TeV";
      else throw RootException("ERROR: Invalid option"); 
   } else throw RootException("ERROR: Invalid option");

   return label;
}

std::string getTTreeFileName(int runRange){
   std::string fileName;
   if(runRange == Data900GeV) fileName = "analysisMinBias_TTree_MinimumBias_900GeV.root";
   else if(runRange == Data2360GeV) fileName = "analysisMinBias_TTree_MinimumBias_900GeV.root";
   else if(runRange == Data7TeV) fileName = "analysisMinBias_TTree_MinimumBias_7TeV.root";
   else throw RootException("ERROR: Invalid option");

   return fileName;
}

std::string getTTreeFileName(int genType, int runRange){
   std::string fileName;
   if(genType == PYTHIA){
      if(runRange == Data900GeV) fileName = "analysisMinBias_TTree_PYTHIA_MinBias_STARTUP3X_V8P_900GeV.root";
      else if(runRange == Data2360GeV) fileName = "analysisMinBias_TTree_PYTHIA_MinBias_STARTUP3X_V8O_2360GeV.root";
      else throw RootException("ERROR: Invalid option");
   } else if(genType == PHOJET){ 
      if(runRange == Data900GeV) fileName = "analysisMinBias_TTree_PHOJET_MinBias_STARTUP3X_V8P_900GeV.root";
      else if(runRange == Data2360GeV) fileName = "analysisMinBias_TTree_PHOJET_MinBias_STARTUP3X_V8O_2360GeV.root";
      else throw RootException("ERROR: Invalid option");
   } else if(genType == PYTHIADW){
      if(runRange == Data900GeV) fileName = "analysisMinBias_TTree_PYTHIADW_MinBias_STARTUP3X_V8P_900GeV.root";
      else if(runRange == Data2360GeV) fileName = "analysisMinBias_TTree_PYTHIADW_MinBias_STARTUP3X_V8O_2360GeV.root";
      else throw RootException("ERROR: Invalid option");
   } else if(genType == PYTHIAP0){
      if(runRange == Data900GeV) fileName = "analysisMinBias_TTree_PYTHIAP0_MinBias_STARTUP3X_V8P_900GeV.root";
      else if(runRange == Data2360GeV) fileName = "analysisMinBias_TTree_PYTHIAP0_MinBias_STARTUP3X_V8O_2360GeV.root";
      else throw RootException("ERROR: Invalid option");
   } else if(genType == PYTHIAProQ20){
      if(runRange == Data900GeV) fileName = "analysisMinBias_TTree_PYTHIAProQ20_MinBias_STARTUP3X_V8P_900GeV.root";
      else if(runRange == Data2360GeV) fileName = "analysisMinBias_TTree_PYTHIAProQ20_MinBias_STARTUP3X_V8O_2360GeV.root";
      else throw RootException("ERROR: Invalid option");
   } else if(genType == PYTHIACW){
      if(runRange == Data900GeV) fileName = "analysisMinBias_TTree_PYTHIACW_MinBias_STARTUP3X_V8P_900GeV.root";
      else if(runRange == Data2360GeV) fileName = "analysisMinBias_TTree_PYTHIACW_MinBias_STARTUP3X_V8O_2360GeV.root";
      else throw RootException("ERROR: Invalid option");
   } else throw RootException("ERROR: Invalid option");

   return fileName;
}

std::string getHistosFileName(int runRange, std::string const& selection){
   std::string histosFileName = "analysisMinBiasTTree_"
                                + getDataLabel(runRange) + "_"
                                + selection + "_histos.root";

   return histosFileName;
}

std::string getHistosFileName(int genType, int runRange, int processType, std::string const& selection){
   std::string histosFileName = "analysisMinBiasTTree_";
   histosFileName += getDataLabel(genType,runRange);
   histosFileName += "_" + selection + "_histos_";
   histosFileName += getProcessCategoryName(processType) + ".root";

   return histosFileName;
}

//==========================================================================================

std::string getProcessCategoryName(int processCategory){
   std::string processName;
   switch (processCategory){
      case All: processName = "All";break;
      case SD: processName = "SD";break;
      case NonSD: processName = "NonSD";break;
      case Inelastic: processName = "Inelastic";break;
      case DD: processName = "DD";break;
      case Diff: processName = "Diff";break;
   }
   return processName;
}

void getProcessIdsPYTHIA(std::vector<int>& processIDs, std::vector<std::string>& processNames){
   processIDs.push_back(11); //f_i f_j -> f_i f_j (QCD)
   processNames.push_back("f_{i} f_{j} #rightarrow f_{i} f_{j}");
   processIDs.push_back(12); //f_i f_ibar -> f_k f_kbar
   processNames.push_back("f_{i} #bar{f_{i}} #rightarrow f_{k} #bar{f_{k}}");
   processIDs.push_back(13); //f_i f_ibar -> g g
   processNames.push_back("f_{i} #bar{f_{i}} #rightarrow g g");
   processIDs.push_back(28); //f_i g -> f_i g
   processNames.push_back("f_{i} g #rightarrow f_{i} g");
   processIDs.push_back(53); //g g -> f_k f_kbar
   processNames.push_back("g g #rightarrow f_{k} #bar{f_{k}}");
   processIDs.push_back(68); //g g -> g g
   processNames.push_back("g g #rightarrow g g");
   processIDs.push_back(92); //SD AB->XB
   processNames.push_back("SD AB #rightarrow XB");
   processIDs.push_back(93); //SD AB->AX
   processNames.push_back("SD AB #rightarrow AX");
   processIDs.push_back(94); //DD 
   processNames.push_back("DD");
   processIDs.push_back(95); //low-p_T production 
   processNames.push_back("low-p_{T}");
}

void getProcessIdsPHOJET(std::vector<int>& processIDs, std::vector<std::string>& processNames){
   processIDs.push_back(1);
   processNames.push_back("Inelastic");
   processIDs.push_back(2);
   processNames.push_back("Elastic");
   processIDs.push_back(3);
   processNames.push_back("Quasi-elastic VM");
   processIDs.push_back(4);
   processNames.push_back("Central diffraction");
   processIDs.push_back(5);
   processNames.push_back("Single diffraction Part. 1");
   processIDs.push_back(6);
   processNames.push_back("Single diffraction Part. 2");
   processIDs.push_back(7);
   processNames.push_back("Double diffraction");
}

void getSelectedProcIdsPYTHIA(int processCategory,std::vector<int>& selectedProcIds){
   switch (processCategory){
      case SD:
         selectedProcIds.push_back(92); //SD AB->XB
         selectedProcIds.push_back(93); //SD AB->AX
         break;
      case NonSD:
         selectedProcIds.push_back(11); //f_i f_j -> f_i f_j (QCD)
         selectedProcIds.push_back(12); //f_i f_ibar -> f_k f_kbar
         selectedProcIds.push_back(13); //f_i f_ibar -> g g
         selectedProcIds.push_back(28); //f_i g -> f_i g
         selectedProcIds.push_back(53); //g g -> f_k f_kbar
         selectedProcIds.push_back(68); //g g -> g g
         selectedProcIds.push_back(95); //low-p_T production
         selectedProcIds.push_back(94); //DD
         break;
      case Inelastic:
         selectedProcIds.push_back(11); //f_i f_j -> f_i f_j (QCD)
         selectedProcIds.push_back(12); //f_i f_ibar -> f_k f_kbar
         selectedProcIds.push_back(13); //f_i f_ibar -> g g
         selectedProcIds.push_back(28); //f_i g -> f_i g
         selectedProcIds.push_back(53); //g g -> f_k f_kbar
         selectedProcIds.push_back(68); //g g -> g g
         selectedProcIds.push_back(95); //low-p_T production
         break;
      case DD:
         selectedProcIds.push_back(94); //DD
         break;
      case Diff:
         selectedProcIds.push_back(92); //SD AB->XB
         selectedProcIds.push_back(93); //SD AB->AX
         selectedProcIds.push_back(94); //DD
         break;
   }  
}

void getSelectedProcIdsPHOJET(int processCategory,std::vector<int>& selectedProcIds){
   switch (processCategory){
      case SD:
         selectedProcIds.push_back(5);
         selectedProcIds.push_back(6);
         break;
      case NonSD:
         selectedProcIds.push_back(1);
         selectedProcIds.push_back(3);
         selectedProcIds.push_back(4);
         selectedProcIds.push_back(7);
         break;
      case Inelastic:
         selectedProcIds.push_back(1);
         break;
      case DD:
         selectedProcIds.push_back(7);
         break;
      case Diff:
         selectedProcIds.push_back(4);
         selectedProcIds.push_back(5);
         selectedProcIds.push_back(6);
         selectedProcIds.push_back(7);
         break;
   }
}

#endif
