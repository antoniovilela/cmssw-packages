#ifndef MinimumBiasAnalysis_RootTools_h
#define MinimumBiasAnalysis_RootTools_h

#include <string>
#include <vector>

enum generator_t {PYTHIA,PHOJET};
enum process_category_t {All,SD,NonSD,Inelastic,DD,Diff};

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
