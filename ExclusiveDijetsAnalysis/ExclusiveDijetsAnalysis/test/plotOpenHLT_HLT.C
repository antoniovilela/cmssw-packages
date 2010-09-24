#include "TFile.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TH1F.h"
#include "TF1.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <map>

void plotOpenHLT(std::vector<std::string>& fileNames, double crossSection = 1., int maxEvents = -1){
   TChain chain("HltTree");
   std::cout << ">>> Reading files: " << std::endl;
   for(std::vector<std::string>::const_iterator file = fileNames.begin(); file != fileNames.end(); ++file){
      std::cout << "  " << *file << std::endl;
      chain.Add(file->c_str());
   }

   // Create output file
   TFile* hfile = new TFile("analysisOpenHLT_histos_HLT.root","recreate","data histograms");

   TH1F* h_L1HfRing1EtSumNegativeEta = new TH1F("L1HfRing1EtSumNegativeEta","L1HfRing1EtSumNegativeEta",10,0,10);
   TH1F* h_L1HfRing2EtSumNegativeEta = new TH1F("L1HfRing2EtSumNegativeEta","L1HfRing2EtSumNegativeEta",10,0,10);
   TH1F* h_L1HfRing1EtSumPositiveEta = new TH1F("L1HfRing1EtSumPositiveEta","L1HfRing1EtSumPositiveEta",10,0,10);
   TH1F* h_L1HfRing2EtSumPositiveEta = new TH1F("L1HfRing2EtSumPositiveEta","L1HfRing2EtSumPositiveEta",10,0,10);

   TH1F* h_trigBit = new TH1F("trigBit","trigBit",2,0,2);
   TH1F* h_L1CenJetEt = new TH1F("L1CenJetEt","L1CenJetEt",100,0.,100.);
   TH1F* h_L1ForJetEt = new TH1F("L1ForJetEt","L1ForJetEt",100,0.,100.);
   TH1F* h_L1TauEt = new TH1F("L1TauEt","L1TauEt",100,0.,100.);
   /*TH1F* h_L1MetHad = new TH1F("L1MetHad","L1MetHad",100,0.,100.);
   TH1F* h_L1MetTot = new TH1F("L1MetTot","L1MetTot",100,0.,100.);*/
   TH1F* h_L1Met = new TH1F("L1Met","L1Met",100,0.,100.);
   TH1F* h_L1EtHad = new TH1F("L1EtHad","L1EtHad",100,0.,100.);
   TH1F* h_L1EtTot = new TH1F("L1EtTot","L1EtTot",100,0.,100.);

   TH1F* h_L1LeadingJetEt = new TH1F("L1LeadingJetEt","L1LeadingJetEt",100,0.,100.);
   TH1F* h_L1FracEt_HT = new TH1F("L1FracEt_HT","L1FracEt_HT",100,0.,5.);
   TH1F* h_L1FracEt_EtTot = new TH1F("L1FracEt_EtTot","L1FracEt_EtTot",100,0.,5.);
   TH1F* h_L1ResHT = new TH1F("L1ResHT","L1ResHT",100,-5.0,5.0);
   TH1F* h_L1ResEtTot = new TH1F("L1ResEtTot","L1ResEtTot",100,-5.0,5.0);

   TH1F* h_nHFtowers = new TH1F("nHFtowers","nHFtowers",100,0.,600.);
   TH1F* h_hfTwrEnergy = new TH1F("hfTwrEnergy","hfTwrEnergy",500,0.,500.);
   TH1F* h_hfTwrEt = new TH1F("hfTwrEt","hfTwrEt",100,0.,10.);
   TH1F* h_hfTwrEta = new TH1F("hfTwrEta","hfTwrEta",100,-5.5,5.5);
   TH1F* h_hfTwrPhi = new TH1F("hfTwrPhi","hfTwrPhi",100,-1.1*M_PI,1.1*M_PI);

   TH1F* h_sumEHF_plus = new TH1F("sumEHF_plus","sumEHF_plus",500,0.,1000.);
   TH1F* h_sumEHF_minus = new TH1F("sumEHF_minus","sumEHF_minus",500,0.,1000.);

   TH1F* h_fracEt = new TH1F("fracEt","fracEt",500,0.,2.);

   int nThresholds = 100;
   float sumEtMin = 0.;
   float sumEtMax = 500.;

   TH1F* h_NevtsVsThreshold = new TH1F("NevtsVsThreshold","NevtsVsThreshold",nThresholds,sumEtMin,sumEtMax);
   TF1* func_effTrig = new TF1("effTrig","[0]",sumEtMin,sumEtMax);
   TF1* func_effTrigL1 = new TF1("effTrigL1","[0]",sumEtMin,sumEtMax);
   
   std::string trigName = "HLT_Jet30U";
   //std::string trigName = "L1_SingleJet20";

   int runNumber,lumiBlock,bunchCrossing;
   int trigBit;
   int nL1CenJet;
   float l1CenJetEt[4];
   int nL1ForJet;
   float l1ForJetEt[4];
   int nL1Tau;
   float l1TauEt[4];
   /*float l1MetHad;
   float l1MetTot;*/
   float l1Met;
   float l1EtHad;
   float l1EtTot;
   
   int L1HfRing1EtSumNegativeEta;
   int L1HfRing2EtSumNegativeEta;
   int L1HfRing1EtSumPositiveEta;
   int L1HfRing2EtSumPositiveEta;

   int nHFtowers;
   float hfTwrEnergy[1000];
   float hfTwrEt[1000];
   float hfTwrEta[1000];
   float hfTwrPhi[1000];
    
   int nJetsCal;
   float jetCalPt[100];
   float jetCalEta[100];

   chain.SetBranchAddress(trigName.c_str(),&trigBit);
   chain.SetBranchAddress("Run",&runNumber);
   chain.SetBranchAddress("LumiBlock",&lumiBlock);
   chain.SetBranchAddress("Bx",&bunchCrossing);
   chain.SetBranchAddress("NL1CenJet",&nL1CenJet);
   chain.SetBranchAddress("L1CenJetEt",l1CenJetEt);
   chain.SetBranchAddress("NL1ForJet",&nL1ForJet);
   chain.SetBranchAddress("L1ForJetEt",l1ForJetEt); 
   chain.SetBranchAddress("NL1Tau",&nL1Tau);
   chain.SetBranchAddress("L1TauEt",l1TauEt);
   /*chain.SetBranchAddress("L1MetHad",&l1MetHad);
   chain.SetBranchAddress("L1MetTot",&l1MetTot);*/
   chain.SetBranchAddress("L1Met",&l1Met);
   chain.SetBranchAddress("L1EtHad",&l1EtHad);
   chain.SetBranchAddress("L1EtTot",&l1EtTot);
   chain.SetBranchAddress("L1HfRing1EtSumNegativeEta",&L1HfRing1EtSumNegativeEta);
   chain.SetBranchAddress("L1HfRing2EtSumNegativeEta",&L1HfRing2EtSumNegativeEta);
   chain.SetBranchAddress("L1HfRing1EtSumPositiveEta",&L1HfRing1EtSumPositiveEta);
   chain.SetBranchAddress("L1HfRing2EtSumPositiveEta",&L1HfRing2EtSumPositiveEta);
   chain.SetBranchAddress("NrecoTowCal",&nHFtowers);
   chain.SetBranchAddress("recoTowE",hfTwrEnergy);
   chain.SetBranchAddress("recoTowEt",hfTwrEt);
   chain.SetBranchAddress("recoTowEta",hfTwrEta);
   chain.SetBranchAddress("recoTowPhi",hfTwrPhi);
   chain.SetBranchAddress("NrecoJetCal",&nJetsCal);
   chain.SetBranchAddress("recoJetCalPt",jetCalPt); 
   chain.SetBranchAddress("recoJetCalEta",jetCalEta);
   
   int nEvents = chain.GetEntries();
   double etaMin = 3.0;
   double towerThreshold = 4.0; //energy
   int L1EtSumThreshold = 1;
 
   int nPassedTrigBit = 0;
   int nPassedTrigBitAndL1EtSum = 0;
   int countEvts = 0;
   int lumiMin = -1;
   int lumiMax = -1;
   for(int entry = 0; entry < nEvents; ++entry){
      if((maxEvents > 0)&&(entry == maxEvents)) break;
      if(entry%2000 == 0) std::cout << ">>> Analysing " << entry << "th event" << std::endl;

      chain.GetEntry(entry);
      //size_t index = 0;
      ++countEvts;
      if(lumiMin < 0) lumiMin = lumiBlock;
      if(lumiMin >= 0 && lumiBlock < lumiMin) lumiMin = lumiBlock;

      if(lumiMax < 0) lumiMax = lumiBlock;
      if(lumiMax >= 0 && lumiBlock > lumiMax) lumiMax = lumiBlock;

      h_trigBit->Fill(trigBit);

      if(trigBit == 0) continue;

      ++nPassedTrigBit;

      h_L1CenJetEt->Fill(l1CenJetEt[0]);
      h_L1ForJetEt->Fill(l1ForJetEt[0]);
      h_L1TauEt->Fill(l1TauEt[0]);
      /*h_L1MetHad->Fill(l1MetHad);
      h_L1MetTot->Fill(l1MetTot);*/
      h_L1Met->Fill(l1Met);
      h_L1EtHad->Fill(l1EtHad);
      h_L1EtTot->Fill(l1EtTot);

      // Find leading L1 jet Et
      float leadingJetEt = (l1CenJetEt[0] > l1ForJetEt[0]) ? l1CenJetEt[0] : l1ForJetEt[0];
      leadingJetEt = (l1TauEt[0] > leadingJetEt) ? l1TauEt[0] : leadingJetEt;

      h_L1LeadingJetEt->Fill(leadingJetEt);

      // Compute Et fractions
      float l1FracEt_HT = 2*leadingJetEt/l1EtHad;
      float l1FracEt_EtTot = 2*leadingJetEt/l1EtTot;
    
      h_L1FracEt_HT->Fill(l1FracEt_HT); 
      h_L1FracEt_EtTot->Fill(l1FracEt_EtTot);

      h_L1ResHT->Fill((2*leadingJetEt - l1EtHad)/(2*leadingJetEt));
      h_L1ResEtTot->Fill((2*leadingJetEt - l1EtTot)/(2*leadingJetEt));

      h_L1HfRing1EtSumNegativeEta->Fill(L1HfRing1EtSumNegativeEta);
      h_L1HfRing2EtSumNegativeEta->Fill(L1HfRing2EtSumNegativeEta);
      h_L1HfRing1EtSumPositiveEta->Fill(L1HfRing1EtSumPositiveEta);
      h_L1HfRing2EtSumPositiveEta->Fill(L1HfRing2EtSumPositiveEta);

      if((L1HfRing1EtSumNegativeEta > L1EtSumThreshold)||
         (L1HfRing2EtSumNegativeEta > L1EtSumThreshold)||
         (L1HfRing1EtSumPositiveEta > L1EtSumThreshold)||
         (L1HfRing2EtSumPositiveEta > L1EtSumThreshold)) continue;

      ++nPassedTrigBitAndL1EtSum;

      h_nHFtowers->Fill(nHFtowers);
      double sumEHF_plus = 0.;
      double sumEHF_minus = 0.;
      double sumEt = 0.;
      for(int itwr = 0; itwr < nHFtowers; ++itwr){
         double energy = hfTwrEnergy[itwr];
         double et = hfTwrEt[itwr];
         double eta = hfTwrEta[itwr];
  
         if(energy < towerThreshold) continue;
         sumEt += hfTwrEt[itwr];
 
         if(fabs(eta) < etaMin) continue; //for HF
         h_hfTwrEnergy->Fill(energy);
         h_hfTwrEt->Fill(et);		     
         h_hfTwrEta->Fill(eta);
         h_hfTwrPhi->Fill(hfTwrPhi[itwr]);
         if(eta >= 0.) sumEHF_plus += energy;
         else sumEHF_minus += energy;
      }
      h_sumEHF_plus->Fill(sumEHF_plus);
      h_sumEHF_minus->Fill(sumEHF_minus);

      double fracEt = (nJetsCal)?jetCalPt[0]:-1;
      if(nJetsCal >=2 ) fracEt += jetCalPt[1];
      if(sumEt) fracEt /= sumEt;
      else fracEt = -1;

      h_fracEt->Fill(fracEt);

      // Check if passes energy sum cut vs threshold
      for(int i_bin = 0; i_bin < h_NevtsVsThreshold->GetXaxis()->GetNbins(); ++i_bin){
         float threshold = h_NevtsVsThreshold->GetXaxis()->GetBinLowEdge(i_bin + 1);
         float bin_center = h_NevtsVsThreshold->GetXaxis()->GetBinCenter(i_bin + 1);
         // Apply veto
         if((sumEHF_plus < threshold)&&(sumEHF_minus < threshold)) h_NevtsVsThreshold->Fill(bin_center);
      } 
   }
   std::cout << "Analyzed " << countEvts << " events" << std::endl
             << "Min and max lumi section: (" << lumiMin << "," << lumiMax << ")" << std::endl
             << "  Nr. events passing trigger " << trigName << ": " << nPassedTrigBit << std::endl
             << "  Nr. events passing L1 HF Et-sum selection: " << nPassedTrigBitAndL1EtSum << std::endl;
            
   float effTrigBit = (float)nPassedTrigBit/countEvts;
   func_effTrig->SetParameter(0,effTrigBit);
   
   float effTrigBitL1 = (float)nPassedTrigBitAndL1EtSum/countEvts;
   func_effTrigL1->SetParameter(0,effTrigBitL1); 

   TH1F* h_EffVsThreshold = static_cast<TH1F*>(h_NevtsVsThreshold->Clone("EffVsThreshold"));
   h_EffVsThreshold->Scale(1./countEvts);

   TH1F* h_RateVsThreshold = static_cast<TH1F*>(h_NevtsVsThreshold->Clone("RateVsThreshold"));
   double lumiLength = 23.3; //s
   double deltaLumi = lumiMax - lumiMin;
   h_RateVsThreshold->Scale(1./(deltaLumi*lumiLength));

   TH1F* h_RateXSectionVsThreshold = static_cast<TH1F*>(h_NevtsVsThreshold->Clone("RateXSectionVsThreshold"));
   double Lum = 1.;//10^31cms-2s-1
   h_RateXSectionVsThreshold->Scale(Lum*crossSection*10000.);

   func_effTrig->Write();
   func_effTrigL1->Write();
   hfile->Write();
   hfile->Close();
}
