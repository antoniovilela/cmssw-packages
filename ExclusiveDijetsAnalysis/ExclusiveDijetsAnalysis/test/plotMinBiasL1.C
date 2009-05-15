#include "TFile.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TH1F.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>

void plotMinBiasL1(std::vector<std::string>& fileNames){
   TChain chain("HltTree");
   std::cout << ">>> Reading files: " << std::endl;
   for(std::vector<std::string>::const_iterator file = fileNames.begin(); file != fileNames.end(); ++file){
      std::cout << "  " << *file << std::endl;
      chain.Add(file->c_str());
   }

   // Create output file
   TFile* hfile = new TFile("analysisOpenHLT_histos.root","recreate","data histograms");

   std::vector<std::string> triggerBits;
   triggerBits.push_back("L1_SingleJet15");
   triggerBits.push_back("L1_SingleJet30");
   std::vector<std::pair<std::string,TH1F*> > histosTriggerBits;
   for(std::vector<std::string>::const_iterator it = triggerBits.begin(); it != triggerBits.end(); ++it){
      histosTriggerBits.push_back(std::make_pair(*it,new TH1F(it->c_str(),it->c_str(),2,0,2)));
   }

   std::vector<std::pair<std::string,TH1F*> > histos;
   std::vector<std::pair<std::pair<std::string,std::string>,TH1F*> > histosWithCut;

   std::vector<std::string> varNames;
   varNames.push_back("L1HfRing1EtSumNegativeEta");
   varNames.push_back("L1HfRing2EtSumNegativeEta");
   varNames.push_back("L1HfRing1EtSumPositiveEta");
   varNames.push_back("L1HfRing2EtSumPositiveEta");
   for(std::vector<std::string>::const_iterator it = varNames.begin(); it != varNames.end(); ++it){
      histos.push_back(std::make_pair(*it,new TH1F(it->c_str(),it->c_str(),10,0,10)));
      for(std::vector<std::string>::const_iterator cut = triggerBits.begin(); cut != triggerBits.end(); ++cut){
         histosWithCut.push_back(std::make_pair(std::make_pair(*it,*cut),new TH1F((*it + "_" + *cut).c_str(),it->c_str(),10,0,10)));
      }
   }

   TH1F* h_countAll = new TH1F("countAll","countAll",triggerBits.size(),0,triggerBits.size());
   for(size_t k = 0; k < triggerBits.size(); ++k){
      h_countAll->GetXaxis()->SetBinLabel((k + 1),triggerBits[k].c_str());
   }

   int vars[triggerBits.size() + varNames.size()];
   int index = 0;
   std::map<std::string,int> mapNameToIndex;
   for(std::vector<std::string>::const_iterator it = triggerBits.begin(); it != triggerBits.end(); ++it,++index){
      if(mapNameToIndex.find(it->c_str()) != mapNameToIndex.end()) {--index;continue;}
      chain.SetBranchAddress(it->c_str(),vars + index);
      mapNameToIndex.insert(std::make_pair(it->c_str(),index));
   }
   for(std::vector<std::string>::const_iterator it = varNames.begin(); it != varNames.end(); ++it,++index){
      if(mapNameToIndex.find(it->c_str()) != mapNameToIndex.end()) {--index;continue;}
      chain.SetBranchAddress(it->c_str(),vars + index);
      mapNameToIndex.insert(std::make_pair(it->c_str(),index));
   }

   int nEvents = chain.GetEntries();
   for(int entry = 0; entry < nEvents; ++entry){
      chain.GetEntry(entry);
      //size_t index = 0;
      for(std::vector<std::pair<std::string,TH1F*> >::const_iterator it = histosTriggerBits.begin(); it != histosTriggerBits.end(); ++it){
         int index = mapNameToIndex[it->first];
         it->second->Fill(vars[index]);
      }
      for(std::vector<std::pair<std::string,TH1F*> >::const_iterator it = histos.begin(); it != histos.end(); ++it){
         int index = mapNameToIndex[it->first];
         it->second->Fill(vars[index]);
      }

      for(std::vector<std::pair<std::pair<std::string,std::string>,TH1F*> >::const_iterator it = histosWithCut.begin(); it != histosWithCut.end(); ++it){
         int iBit = mapNameToIndex[it->first.second];
         if(vars[iBit] == 1){
            int iVar = mapNameToIndex[it->first.first];
            it->second->Fill(vars[iVar]);
         }
      }

      bool accept = true;
      for(std::vector<std::string>::const_iterator it = varNames.begin(); it != varNames.end(); ++it){
         int index = mapNameToIndex[*it];
         if(vars[index] > 0) accept = false;
      }   

      if(!accept) continue;

      for(size_t k = 0; k < triggerBits.size(); ++k){
         int iBit = mapNameToIndex[triggerBits[k]];
         if(vars[iBit] == 1) h_countAll->Fill(k);
      }       
   }
   h_countAll->Scale(1./nEvents);

   hfile->Write();
   hfile->Close();
}
