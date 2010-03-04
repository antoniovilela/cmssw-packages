#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"

#include <iostream>
#include <string>
#include <vector>

void fracNoColl(const char* variable, double xlow, double xhigh){

   TFile* fileColl = TFile::Open("root/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasPixel_histos.root");
   TFile* fileNoColl = TFile::Open("root/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasPixelNoColl_histos.root");

   TH1F* histVarColl = static_cast<const TH1F*>(fileColl->Get(variable));
   TH1F* histVarNoColl = static_cast<const TH1F*>(fileNoColl->Get(variable));

   std::cout << "Range: " << xlow << " - " << xhigh << std::endl;

   int binlow = histVarColl->FindBin(xlow);
   int binhigh = histVarColl->FindBin(xhigh);
   std::cout << "Bin range: " << binlow << " - " << binhigh << std::endl;

   double sumColl = histVarColl->Integral(binlow,binhigh);
   std::cout << "N events in range Coll.: " << sumColl << std::endl;
   
   binlow = histVarNoColl->FindBin(xlow); 
   binhigh = histVarNoColl->FindBin(xhigh);
   std::cout << "Bin range: " << binlow << " - " << binhigh << std::endl;
   double sumNoColl = histVarNoColl->Integral(binlow,binhigh);
   std::cout << "N events in range No Coll.: " << sumNoColl << std::endl;
}
