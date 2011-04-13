#include "TFile.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TH1F.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>

void plotOpenHLT(std::vector<std::string>& fileNames, double norm = 1., int maxEvents = -1){
   TChain chain("HltTree");
   std::cout << ">>> Reading files: " << std::endl;
   for(std::vector<std::string>::const_iterator file = fileNames.begin(); file != fileNames.end(); ++file){
      std::cout << "  " << *file << std::endl;
      chain.Add(file->c_str());
   }

   // Create output file
   TFile* hfile = new TFile("analysisOpenHLT_histos.root","recreate","data histograms");

   // Reference trigger
   //std::string refTriggerName = "HLT_Physics_v1";
   //std::string refTriggerName = "HLT_ZeroBias_v1";
   std::string refTriggerName = "HLT_L1SingleJet36_v1";
   //std::string refTriggerName = "HLT_ExclDiJet60_HFAND_v1";

   std::vector<std::string> triggerBits;
   //triggerBits.push_back("L1_BptxPlus_NotBptxMinus");
   //triggerBits.push_back("L1_BptxMinus_NotBptxPlus"); 
   //triggerBits.push_back("L1_SingleJet16");
   triggerBits.push_back("L1_SingleJet36");
   //triggerBits.push_back("L1_SingleEG12");
   //triggerBits.push_back("L1_DoubleEG3");
   
   std::vector<std::string> varNames;
   /*varNames.push_back("L1HfRing1EtSumNegativeEta");
   varNames.push_back("L1HfRing2EtSumNegativeEta");
   varNames.push_back("L1HfRing1EtSumPositiveEta");
   varNames.push_back("L1HfRing2EtSumPositiveEta");*/
   varNames.push_back("L1HfTowerCountPositiveEtaRing1");
   varNames.push_back("L1HfTowerCountNegativeEtaRing1");
   varNames.push_back("L1HfTowerCountPositiveEtaRing2");
   varNames.push_back("L1HfTowerCountNegativeEtaRing2"); 

   TH1::SetDefaultSumw2(true);

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

   std::vector<std::pair<std::string,TH1F*> > histos;
   std::vector<std::pair<std::pair<std::string,std::string>,TH1F*> > histosWithCut;
   for(std::vector<std::string>::const_iterator it = varNames.begin(); it != varNames.end(); ++it){
      histos.push_back(std::make_pair(*it,new TH1F(it->c_str(),it->c_str(),10,0,10)));
      for(std::vector<std::string>::const_iterator cut = triggerBits.begin(); cut != triggerBits.end(); ++cut){
         histosWithCut.push_back(std::make_pair(std::make_pair(*it,*cut),new TH1F((*it + "_" + *cut).c_str(),it->c_str(),10,0,10)));
      }
   }

   // Reference trigger
   int refTrigger;
   if(refTriggerName != ""){
      std::cout << ">>> Setting reference trigger " << refTriggerName << std::endl; 
      chain.SetBranchAddress(refTriggerName.c_str(),&refTrigger);
   }
   // Trigger bits, prescales, variables
   int vars[triggerBits.size() + triggerBits.size() + varNames.size()];
   int index = 0;
   std::map<std::string,int> mapNameToIndex;
   for(std::vector<std::string>::const_iterator it = triggerBits.begin(); it != triggerBits.end(); ++it,++index){
      if(mapNameToIndex.find(it->c_str()) != mapNameToIndex.end()) {--index;continue;}
      chain.SetBranchAddress(it->c_str(),vars + index);
      mapNameToIndex.insert(std::make_pair(it->c_str(),index));
   }
   // Prescales
   for(std::vector<std::string>::const_iterator it = triggerBits.begin(); it != triggerBits.end(); ++it,++index){
      std::string name(*it);
      name += "_Prescl";
      if(mapNameToIndex.find(name) != mapNameToIndex.end()) {--index;continue;}
      chain.SetBranchAddress(name.c_str(),vars + index);
      mapNameToIndex.insert(std::make_pair(name,index));
   }
   for(std::vector<std::string>::const_iterator it = varNames.begin(); it != varNames.end(); ++it,++index){
      if(mapNameToIndex.find(it->c_str()) != mapNameToIndex.end()) {--index;continue;}
      chain.SetBranchAddress(it->c_str(),vars + index);
      mapNameToIndex.insert(std::make_pair(it->c_str(),index));
   }

   int nEvents = chain.GetEntries();
   int countEvts = 0;
   std::vector<double> sumWeights(triggerBits.size(),0.);
   for(int entry = 0; entry < nEvents; ++entry){
      if((maxEvents > 0)&&(entry == maxEvents)) break;
      if(entry%2000 == 0) std::cout << ">>> Analysing " << entry << "th event" << std::endl;
      chain.GetEntry(entry);

      // Reference trigger
      //if(refTriggerName != "" && refTrigger == 0) continue;
      if(refTriggerName != ""){
         bool refTrigAccept = (refTrigger == 1);
         //std::cout << refTriggerName << ": " << refTrigAccept << std::endl;
         if(!refTrigAccept) continue;
      }

      ++countEvts;

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
         int iBitPrescl = mapNameToIndex[(*itBit + "_Prescl")];
         bool trigAccept = (vars[iBit] == 1);
         int prescale = vars[iBitPrescl];
         if(!trigAccept) continue;

         double weight = prescale;
         sumWeights[trigBit] += weight;

         std::vector<int> accept(nThresholds,1);
         for(std::vector<std::string>::const_iterator itVar = varNames.begin(); itVar != varNames.end(); ++itVar){
            int iVar = mapNameToIndex[*itVar]; 
            for(int iThreshold = 0; iThreshold < nThresholds; ++iThreshold){
               if(vars[iVar] > iThreshold) accept[iThreshold] = 0;
            }
         } 
         if(accept[0] == 1) h_countAll->Fill(trigBit);
         for(int iThreshold = 0; iThreshold < nThresholds; ++iThreshold){
            if(accept[iThreshold] == 1) histosEffVsThreshold[trigBit].second->Fill(iThreshold,prescale);
         }
      }       
   }
   h_countAll->Scale(1./countEvts);
  
   std::vector<TH1F*> histosRateVsThreshold;
   //double crossSection = 80.;//mb
   //double Lum = 1.;//10^31cms-2s-2
   size_t idx_bit = 0;
   for(std::vector<std::pair<std::string,TH1F*> >::const_iterator it = histosEffVsThreshold.begin();
                                                                  it != histosEffVsThreshold.end();
                                                                  ++it,++idx_bit){
      //it->second->Scale(1./countEvts);
      std::string hname = it->first + "_Rate";
      histosRateVsThreshold.push_back(static_cast<TH1F*>(it->second->Clone(hname.c_str())));
      histosRateVsThreshold.back()->Scale(norm);
      double sum_weights = sumWeights[idx_bit];
      it->second->Scale(1./sum_weights);
   }

   hfile->Write();
   hfile->Close();
}
