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

void plotOpenHLT(std::vector<std::string>& fileNames, double crossSection = 1., int maxEvents = -1, std::string const& outputFileName = "analysisOpenHLT_histos_HLT.root"){
   TChain chain("HltTree");
   std::cout << ">>> Reading files: " << std::endl;
   for(std::vector<std::string>::const_iterator file = fileNames.begin(); file != fileNames.end(); ++file){
      std::cout << "  " << *file << std::endl;
      chain.Add(file->c_str());
   }

   // Create output file
   TFile* hfile = new TFile(outputFileName.c_str(),"recreate","data histograms");

   TH1::SetDefaultSumw2(true);

   TH1F* h_L1HfRing1EtSumNegativeEta = new TH1F("L1HfRing1EtSumNegativeEta","L1HfRing1EtSumNegativeEta",10,0,10);
   TH1F* h_L1HfRing2EtSumNegativeEta = new TH1F("L1HfRing2EtSumNegativeEta","L1HfRing2EtSumNegativeEta",10,0,10);
   TH1F* h_L1HfRing1EtSumPositiveEta = new TH1F("L1HfRing1EtSumPositiveEta","L1HfRing1EtSumPositiveEta",10,0,10);
   TH1F* h_L1HfRing2EtSumPositiveEta = new TH1F("L1HfRing2EtSumPositiveEta","L1HfRing2EtSumPositiveEta",10,0,10);

   TH1F* h_L1HfTowerCountPositiveEtaRing1 = new TH1F("L1HfTowerCountPositiveEtaRing1","L1HfTowerCountPositiveEtaRing1",10,0,10);
   TH1F* h_L1HfTowerCountNegativeEtaRing1 = new TH1F("L1HfTowerCountNegativeEtaRing1","L1HfTowerCountNegativeEtaRing1",10,0,10);
   TH1F* h_L1HfTowerCountPositiveEtaRing2 = new TH1F("L1HfTowerCountPositiveEtaRing2","L1HfTowerCountPositiveEtaRing2",10,0,10);
   TH1F* h_L1HfTowerCountNegativeEtaRing2 = new TH1F("L1HfTowerCountNegativeEtaRing2","L1HfTowerCountNegativeEtaRing2",10,0,10);

   TH1F* h_trigBitL1 = new TH1F("trigBitL1","trigBitL1",2,0,2);
   TH1F* h_trigBitHLT = new TH1F("trigBitHLT","trigBitHLT",2,0,2);
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

   TH1F* h_nJetsCor = new TH1F("nJetsCor","nJetsCor",20,0,20);
   TH1F* h_leadingJetCorPt = new TH1F("leadingJetCorPt","leadingJetCorPt",200,0.,200.);
   TH1F* h_leadingJetCorEta = new TH1F("leadingJetCorEta","leadingJetCorEta",100,-5.,5.);
   TH1F* h_secondJetCorPt = new TH1F("secondJetCorPt","secondJetCorPt",200,0.,200.);
   TH1F* h_secondJetCorEta = new TH1F("secondJetCorEta","secondJetCorEta",100,-5.,5.);
   TH1F* h_nHFtowers = new TH1F("nHFtowers","nHFtowers",100,0.,600.);
   TH1F* h_hfTwrEnergy = new TH1F("hfTwrEnergy","hfTwrEnergy",500,0.,500.);
   TH1F* h_hfTwrEt = new TH1F("hfTwrEt","hfTwrEt",100,0.,10.);
   TH1F* h_hfTwrEta = new TH1F("hfTwrEta","hfTwrEta",100,-5.5,5.5);
   TH1F* h_hfTwrPhi = new TH1F("hfTwrPhi","hfTwrPhi",100,-1.1*M_PI,1.1*M_PI);

   TH1F* h_sumEHF_plus = new TH1F("sumEHF_plus","sumEHF_plus",500,0.,1000.);
   TH1F* h_sumEHF_minus = new TH1F("sumEHF_minus","sumEHF_minus",500,0.,1000.);

   TH1F* h_fracEt = new TH1F("fracEt","fracEt",500,0.,2.);

   int nThresholds = 100;
   float sumEMin = 0.;
   float sumEMax = 500.;
   int lumiBinMax = 5000;

   TH1F* h_NevtsVsThreshold = new TH1F("NevtsVsThreshold","NevtsVsThreshold",nThresholds,sumEMin,sumEMax);
   TH1F* h_NevtsL1BitVsLumi = new TH1F("NevtsL1BitVsLumi","NevtsL1BitVsLumi",lumiBinMax,0,lumiBinMax);
   TH1F* h_NevtsL1VsLumi = new TH1F("NevtsL1VsLumi","NevtsL1VsLumi",lumiBinMax,0,lumiBinMax);
   TH1F* h_NevtsHLTBitVsLumi = new TH1F("NevtsHLTBitVsLumi","NevtsHLTBitVsLumi",lumiBinMax,0,lumiBinMax);
   TH1F* h_NevtsHLTVsLumi = new TH1F("NevtsHLTVsLumi","NevtsHLTVsLumi",lumiBinMax,0,lumiBinMax);
   TF1* func_effTrigBitL1 = new TF1("effTrigBitL1","[0]",sumEMin,sumEMax);
   TF1* func_effTrigL1 = new TF1("effTrigL1","[0]",sumEMin,sumEMax);
   TF1* func_effTrigBitHLT = new TF1("effTrigBitHLT","[0]",sumEMin,sumEMax);
   TF1* func_effTrigHLT = new TF1("effTrigHLT","[0]",sumEMin,sumEMax);   

   //std::string trigNameL1 = "L1_SingleJet36";
   std::string trigNameL1 = "L1_SingleJet16";
   //std::string trigNameHLT = "HLT_Jet60_v1"; 
   std::string trigNameHLT = "HLT_Jet30_v1"; 

   int runNumber,lumiBlock,bunchCrossing;
   int trigBitL1;
   int trigBitHLT;
   int trigBitL1Prescl;
   int trigBitHLTPrescl; 
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

   int L1HfTowerCountPositiveEtaRing1;
   int L1HfTowerCountNegativeEtaRing1;
   int L1HfTowerCountPositiveEtaRing2;
   int L1HfTowerCountNegativeEtaRing2;

   int nHFtowers;
   float hfTwrEnergy[1000];
   float hfTwrEt[1000];
   float hfTwrEta[1000];
   float hfTwrPhi[1000];
    
   int nJetsCor;
   float jetCorPt[100];
   float jetCorEta[100];

   if(trigNameL1 != ""){
      chain.SetBranchAddress(trigNameL1.c_str(),&trigBitL1);
      chain.SetBranchAddress((trigNameL1 + "_Prescl").c_str(),&trigBitL1Prescl);
   } else{
      trigBitL1Prescl = 1;  
   }
   if(trigNameHLT != ""){
      chain.SetBranchAddress(trigNameHLT.c_str(),&trigBitHLT);
      chain.SetBranchAddress((trigNameHLT + "_Prescl").c_str(),&trigBitHLTPrescl);
   } else{
      trigBitHLTPrescl = 1;
   }
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
   chain.SetBranchAddress("L1HfTowerCountPositiveEtaRing1",&L1HfTowerCountPositiveEtaRing1);
   chain.SetBranchAddress("L1HfTowerCountNegativeEtaRing1",&L1HfTowerCountNegativeEtaRing1);
   chain.SetBranchAddress("L1HfTowerCountPositiveEtaRing2",&L1HfTowerCountPositiveEtaRing2);
   chain.SetBranchAddress("L1HfTowerCountNegativeEtaRing2",&L1HfTowerCountNegativeEtaRing2);
   chain.SetBranchAddress("NrecoTowCal",&nHFtowers);
   chain.SetBranchAddress("recoTowE",hfTwrEnergy);
   chain.SetBranchAddress("recoTowEt",hfTwrEt);
   chain.SetBranchAddress("recoTowEta",hfTwrEta);
   chain.SetBranchAddress("recoTowPhi",hfTwrPhi);
   chain.SetBranchAddress("NrecoJetCorCal",&nJetsCor);
   chain.SetBranchAddress("recoJetCorCalPt",jetCorPt); 
   chain.SetBranchAddress("recoJetCorCalEta",jetCorEta);
   
   int nEvents = chain.GetEntries();
   double etaMin = 3.0;
   double towerThreshold = 4.0; //energy
   int L1EtSumThreshold = 999;
   int L1TowerCountThreshold = 5;
   double hltJetPtMin = 50.; 
   double sumEHFThreshold = 200.; 
 
   int nPassedTrigBitL1 = 0;
   int nPassedTrigBitL1AndL1EtSum = 0;
   int nPassedTrigBitHLT = 0;
   int nPassedTrigBitHLTAndSumEHF = 0;
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
      
      if(trigNameL1 != ""){
         h_trigBitL1->Fill(trigBitL1);
         if(trigBitL1 == 0) continue;
      }
      ++nPassedTrigBitL1;
      h_NevtsL1BitVsLumi->Fill(lumiBlock);

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

      h_L1HfTowerCountPositiveEtaRing1->Fill(L1HfTowerCountPositiveEtaRing1);
      h_L1HfTowerCountNegativeEtaRing1->Fill(L1HfTowerCountNegativeEtaRing1);
      h_L1HfTowerCountPositiveEtaRing2->Fill(L1HfTowerCountPositiveEtaRing2);
      h_L1HfTowerCountNegativeEtaRing2->Fill(L1HfTowerCountNegativeEtaRing2);

      if((L1HfRing1EtSumNegativeEta > L1EtSumThreshold)||
         (L1HfRing2EtSumNegativeEta > L1EtSumThreshold)||
         (L1HfRing1EtSumPositiveEta > L1EtSumThreshold)||
         (L1HfRing2EtSumPositiveEta > L1EtSumThreshold)) continue;

      if( (L1HfTowerCountPositiveEtaRing1 > L1TowerCountThreshold) || 
          (L1HfTowerCountNegativeEtaRing1 > L1TowerCountThreshold) ||
          (L1HfTowerCountPositiveEtaRing2 > L1TowerCountThreshold) ||
          (L1HfTowerCountPositiveEtaRing2 > L1TowerCountThreshold) ) continue;     

      ++nPassedTrigBitL1AndL1EtSum;
      h_NevtsL1VsLumi->Fill(lumiBlock);

      if(trigNameHLT != ""){
         h_trigBitHLT->Fill(trigBitHLT);
         if(trigBitHLT == 0) continue; 
      }
      h_nJetsCor->Fill(nJetsCor);
      bool acceptJetPt = true;
      if(nJetsCor >= 1){
         h_leadingJetCorPt->Fill(jetCorPt[0]);
         h_leadingJetCorEta->Fill(jetCorEta[0]); 
         if(jetCorPt[0] < hltJetPtMin) acceptJetPt = false;
      }
      if(nJetsCor >= 2){
         h_secondJetCorPt->Fill(jetCorPt[1]);
         h_secondJetCorEta->Fill(jetCorEta[1]);
         if(jetCorPt[1] < hltJetPtMin) acceptJetPt = false;
      }
      if(!acceptJetPt) continue;
 
      ++nPassedTrigBitHLT;
      h_NevtsHLTBitVsLumi->Fill(lumiBlock);

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

      double fracEt = (nJetsCor)?jetCorPt[0]:-1;
      if(nJetsCor >=2 ) fracEt += jetCorPt[1];
      if(sumEt) fracEt /= sumEt;
      else fracEt = -1;

      h_fracEt->Fill(fracEt);

      // Check if passes energy sum cut vs threshold
      for(int i_bin = 0; i_bin < h_NevtsVsThreshold->GetXaxis()->GetNbins(); ++i_bin){
         float threshold = h_NevtsVsThreshold->GetXaxis()->GetBinLowEdge(i_bin + 1);
         float bin_center = h_NevtsVsThreshold->GetXaxis()->GetBinCenter(i_bin + 1);
         // Apply veto
         if((sumEHF_plus < threshold)&&(sumEHF_minus < threshold)) h_NevtsVsThreshold->Fill(bin_center,(trigBitL1Prescl*trigBitHLTPrescl));
      }

      if((sumEHF_plus < sumEHFThreshold)&&(sumEHF_minus < sumEHFThreshold)){
         ++nPassedTrigBitHLTAndSumEHF;
         h_NevtsHLTVsLumi->Fill(lumiBlock); 
      }
   }
   std::cout << "Analyzed " << countEvts << " events" << std::endl
             << "Min and max lumi section: (" << lumiMin << "," << lumiMax << ")" << std::endl
             << "  Nr. events passing L1 bit " << trigNameL1 << ": " << nPassedTrigBitL1 << std::endl
             << "  Nr. events passing L1 HF Et-sum selection: " << nPassedTrigBitL1AndL1EtSum << std::endl
             << "  Nr. events passing HLT bit " << trigNameHLT << ": " << nPassedTrigBitHLT << std::endl
             << "  Nr. events passing HLT HF selection: " << nPassedTrigBitHLTAndSumEHF << std::endl;
            
   float effTrigBitL1 = (float)nPassedTrigBitL1/countEvts;
   func_effTrigBitL1->SetParameter(0,effTrigBitL1);
   
   float effTrigL1 = (float)nPassedTrigBitL1AndL1EtSum/countEvts;
   func_effTrigL1->SetParameter(0,effTrigL1);

   float effTrigBitHLT = (float)nPassedTrigBitHLT/countEvts;
   func_effTrigBitHLT->SetParameter(0,effTrigBitHLT); 

   float effTrigHLT = (float)nPassedTrigBitHLTAndSumEHF/countEvts;
   func_effTrigHLT->SetParameter(0,effTrigHLT);

   TH1F* h_EffVsThreshold = static_cast<TH1F*>(h_NevtsVsThreshold->Clone("EffVsThreshold"));
   h_EffVsThreshold->Scale(1./countEvts);

   double lumiLength = 23.3; //s
   double deltaLumi = lumiMax - lumiMin;
   TH1F* h_RateVsThreshold = static_cast<TH1F*>(h_NevtsVsThreshold->Clone("RateVsThreshold"));
   h_RateVsThreshold->Scale(1./(deltaLumi*lumiLength));

   TH1F* h_RateXSectionVsThreshold = static_cast<TH1F*>(h_NevtsVsThreshold->Clone("RateXSectionVsThreshold"));
   double Lum = 1.;//10^31cms-2s-1
   h_RateXSectionVsThreshold->Scale(Lum*crossSection*10000.);

   TH1F* h_RateL1BitVsLumi = static_cast<TH1F*>(h_NevtsL1BitVsLumi->Clone("RateL1BitVsLumi"));
   h_RateL1BitVsLumi->Scale(1./lumiLength);

   TH1F* h_RateL1VsLumi = static_cast<TH1F*>(h_NevtsL1VsLumi->Clone("RateL1VsLumi"));
   h_RateL1VsLumi->Scale(1./lumiLength);

   TH1F* h_RateHLTBitVsLumi = static_cast<TH1F*>(h_NevtsHLTBitVsLumi->Clone("RateHLTBitVsLumi"));
   h_RateHLTBitVsLumi->Scale(1./lumiLength);

   TH1F* h_RateHLTVsLumi = static_cast<TH1F*>(h_NevtsHLTVsLumi->Clone("RateHLTVsLumi"));
   h_RateHLTVsLumi->Scale(1./lumiLength);

   func_effTrigBitL1->Write();
   func_effTrigL1->Write();
   func_effTrigBitHLT->Write();
   func_effTrigHLT->Write();
   hfile->Write();
   hfile->Close();
}
