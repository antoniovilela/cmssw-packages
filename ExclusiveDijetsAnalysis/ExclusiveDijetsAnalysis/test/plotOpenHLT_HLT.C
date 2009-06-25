#include "TFile.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TH1F.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <map>

void plotOpenHLT(std::vector<std::string>& fileNames, int maxEvents = -1){
   TChain chain("HltTree");
   std::cout << ">>> Reading files: " << std::endl;
   for(std::vector<std::string>::const_iterator file = fileNames.begin(); file != fileNames.end(); ++file){
      std::cout << "  " << *file << std::endl;
      chain.Add(file->c_str());
   }

   // Create output file
   TFile* hfile = new TFile("analysisOpenHLT_histos.root","recreate","data histograms");

   TH1F* h_nHFtowers = new TH1F("nHFtowers","nHFtowers",100,0.,600.);
   TH1F* h_hfTwrEnergy = new TH1F("hfTwrEnergy","hfTwrEnergy",500,0.,500.);
   TH1F* h_hfTwrEt = new TH1F("hfTwrEt","hfTwrEt",100,0.,10.);
   TH1F* h_hfTwrEta = new TH1F("hfTwrEta","hfTwrEta",100,-5.5,5.5);
   TH1F* h_hfTwrPhi = new TH1F("hfTwrPhi","hfTwrPhi",100,-1.1*M_PI,1.1*M_PI);

   TH1F* h_sumEHF_plus = new TH1F("sumEHF_plus","sumEHF_plus",500,0.,1000.);
   TH1F* h_sumEHF_minus = new TH1F("sumEHF_minus","sumEHF_minus",500,0.,1000.);

   std::string trigName = "HLT_DiJetAve30";

   int trigBit;
   int nHFtowers;
   float hfTwrEnergy[1000];
   float hfTwrEt[1000];
   float hfTwrEta[1000];
   float hfTwrPhi[1000];
    
   chain.SetBranchAddress(trigName.c_str(),&trigBit);
   chain.SetBranchAddress("NrecoTowCal",&nHFtowers);
   chain.SetBranchAddress("recoTowE",hfTwrEnergy);
   chain.SetBranchAddress("recoTowEt",hfTwrEt);
   chain.SetBranchAddress("recoTowEta",hfTwrEta);
   chain.SetBranchAddress("recoTowPhi",hfTwrPhi);

   int nEvents = chain.GetEntries();
   double etaMin = 3.0;
   double towerThreshold = 2.5; //energy
   for(int entry = 0; entry < nEvents; ++entry){
      if((maxEvents > 0)&&(nEvents == maxEvents)) break;
      if(entry%2000 == 0) std::cout << ">>> Analysing " << entry << "th event" << std::endl;

      chain.GetEntry(entry);
      //size_t index = 0;

      if(trigBit == 0) continue;

      h_nHFtowers->Fill(nHFtowers);
      double sumEHF_plus = 0.;
      double sumEHF_minus = 0.;
      for(int itwr = 0; itwr < nHFtowers; ++itwr){
         double energy = hfTwrEnergy[itwr];
         double et = hfTwrEt[itwr];
         double eta = hfTwrEta[itwr];
  
         if(fabs(eta) < etaMin) continue;
         if(energy < towerThreshold) continue;  

         h_hfTwrEnergy->Fill(energy);
         h_hfTwrEt->Fill(et);		     
         h_hfTwrEta->Fill(eta);
         h_hfTwrPhi->Fill(hfTwrPhi[itwr]);
         if(eta >= 0.) sumEHF_plus += energy;
         else sumEHF_minus += energy;
      }
      h_sumEHF_plus->Fill(sumEHF_plus);
      h_sumEHF_minus->Fill(sumEHF_minus); 
   }

   hfile->Write();
   hfile->Close();
}
