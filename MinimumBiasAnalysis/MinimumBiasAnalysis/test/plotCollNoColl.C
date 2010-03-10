#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"
#include "Utilities/PlottingTools/interface/Plotter.h"

#include <iostream>
#include <vector>

void plot(const char* drawOption = "", int rebin = 1){
   std::vector<std::string> variables;
   variables.push_back("nVertex");
   variables.push_back("posZPrimVtx");
   variables.push_back("trackMultiplicityAssociatedToPV");
   variables.push_back("multiplicityHFPlus");
   variables.push_back("multiplicityHFMinus");
   variables.push_back("sumEnergyHFPlus");
   variables.push_back("sumEnergyHFMinus");
   variables.push_back("xiPlusFromTowers");
   variables.push_back("xiMinusFromTowers");
   variables.push_back("xiPlusFromPFCands");
   variables.push_back("xiMinusFromPFCands");
   variables.push_back("missingMassFromXiFromPFCands");
   variables.push_back("missingMassFromXiFromTowers");
   variables.push_back("EPlusPzFromTowers");
   variables.push_back("EMinusPzFromTowers");
   variables.push_back("EPlusPzFromPFCands");
   variables.push_back("EMinusPzFromPFCands");
   variables.push_back("MxFromTowers");
   variables.push_back("MxFromPFCands");
   variables.push_back("sumET");
 
   TFile* fileColl = TFile::Open("root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_1_5_histos.root");
   TFile* fileNoColl = TFile::Open("root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCORNoColl_histos.root");

   std::vector<std::pair<std::string,TDirectory*> > dirs;
   dirs.push_back(std::make_pair("Colliding Bx's",fileColl)); 
   dirs.push_back(std::make_pair("Non-colliding Bx's",fileNoColl));
   std::vector<double> normFactors;
   normFactors.push_back(1.);
   normFactors.push_back(0.75);

   //Plotter<NumberEntriesNorm> plotter;
   Plotter<DefaultNorm> plotter;
   int colors[] = {kBlack,kRed,kBlue};
   std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
   int linestyles[] = {kSolid,kDashed,kDashDotted};
   std::vector<int> histLineStyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));
   plotter.SetColors(histColors);
   plotter.SetLineStyles(histLineStyles);
   plotter.SetRebin(rebin);
   plotter.plot(variables,dirs,normFactors,drawOption);
   //plotter.plot(variables,dirs,drawOption);
}

