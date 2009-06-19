#include "TFile.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TH1F.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>

void plotMinBiasL1(std::vector<std::string>& fileNames, double crossSection = 1.){
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
   triggerBits.push_back("HLT_DiJetAve30");
   triggerBits.push_back("L1_SingleEG2");
   triggerBits.push_back("L1_SingleEG5");
   triggerBits.push_back("L1_SingleEG8");
   triggerBits.push_back("L1_SingleIsoEG5");
   triggerBits.push_back("L1_SingleIsoEG8");
   triggerBits.push_back("L1_DoubleEG1");
   triggerBits.push_back("L1_DoubleEG5");
   triggerBits.push_back("L1_DoubleMuOpen");
   triggerBits.push_back("L1_DoubleMu3");
   triggerBits.push_back("HLT_DoubleMu3");

   std::vector<std::pair<std::string,TH1F*> > histosTriggerBits(triggerBits.size());
   std::vector<std::pair<std::string,TH1F*> > histosEffVsThreshold(triggerBits.size());
   TH1F* h_countAll = new TH1F("countAll","countAll",triggerBits.size(),0,triggerBits.size());
   int nThresholds = 8;
   size_t trigBit = 0;
   for(std::vector<std::string>::const_iterator it = triggerBits.begin(); it != triggerBits.end(); ++it,++trigBit){
      histosTriggerBits[trigBit] = std::make_pair(*it,new TH1F(it->c_str(),it->c_str(),2,0,2));
      histosEffVsThreshold[trigBit] = std::make_pair(*it,new TH1F((*it + "_VsThreshold").c_str(),it->c_str(),nThresholds,0,nThresholds));
      h_countAll->GetXaxis()->SetBinLabel((trigBit + 1),it->c_str());
   }

   std::vector<std::string> varNames;
   varNames.push_back("L1HfRing1EtSumNegativeEta");
   varNames.push_back("L1HfRing2EtSumNegativeEta");
   varNames.push_back("L1HfRing1EtSumPositiveEta");
   varNames.push_back("L1HfRing2EtSumPositiveEta");

   std::vector<std::pair<std::string,TH1F*> > histos;
   std::vector<std::pair<std::pair<std::string,std::string>,TH1F*> > histosWithCut;
   for(std::vector<std::string>::const_iterator it = varNames.begin(); it != varNames.end(); ++it){
      histos.push_back(std::make_pair(*it,new TH1F(it->c_str(),it->c_str(),10,0,10)));
      for(std::vector<std::string>::const_iterator cut = triggerBits.begin(); cut != triggerBits.end(); ++cut){
         histosWithCut.push_back(std::make_pair(std::make_pair(*it,*cut),new TH1F((*it + "_" + *cut).c_str(),it->c_str(),10,0,10)));
      }
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
      if(entry%2000 == 0) std::cout << ">>> Analysing " << entry << "th event" << std::endl;
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

      /*bool accept = true;
      for(std::vector<std::string>::const_iterator it = varNames.begin(); it != varNames.end(); ++it){
         int index = mapNameToIndex[*it];
         if(vars[index] > 0) accept = false;
      }   

      if(!accept) continue;

      for(size_t k = 0; k < triggerBits.size(); ++k){
         int iBit = mapNameToIndex[triggerBits[k]];
         if(vars[iBit] == 1) h_countAll->Fill(k);
      }*/

      size_t trigBit = 0;
      for(std::vector<std::string>::const_iterator itBit = triggerBits.begin(); itBit != triggerBits.end(); ++itBit,++trigBit){
         int iBit = mapNameToIndex[*itBit];
         bool trigAccept = (vars[iBit] == 1);
         if(!trigAccept) continue;

         std::vector<int> accept(nThresholds,1);
         for(std::vector<std::string>::const_iterator itVar = varNames.begin(); itVar != varNames.end(); ++itVar){
            int iVar = mapNameToIndex[*itVar]; 
            for(int iThreshold = 0; iThreshold < nThresholds; ++iThreshold){
               if(vars[iVar] > iThreshold) accept[iThreshold] = 0;
            }
         } 
         if(accept[0] == 1) h_countAll->Fill(trigBit);
         for(int iThreshold = 0; iThreshold < nThresholds; ++iThreshold){
            if(accept[iThreshold] == 1) histosEffVsThreshold[trigBit].second->Fill(iThreshold);
         }
      }       
   }
   h_countAll->Scale(1./nEvents);
  
   std::vector<TH1F*> histosRateVsThreshold;
   //double crossSection = 80.;//mb
   double Lum = 1.;//10^31cms-2s-2
   for(std::vector<std::pair<std::string,TH1F*> >::const_iterator it = histosEffVsThreshold.begin();
                                                                  it != histosEffVsThreshold.end(); ++it){
      it->second->Scale(1./nEvents);
      std::string hname = it->first + "_Rate";
      histosRateVsThreshold.push_back(static_cast<TH1F*>(it->second->Clone(hname.c_str())));
      histosRateVsThreshold.back()->Scale(Lum*crossSection*10000.);
   }

   hfile->Write();
   hfile->Close();
}
