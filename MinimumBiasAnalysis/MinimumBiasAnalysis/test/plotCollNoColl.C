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
   variables.push_back("multiplicityTracks");
   /*variables.push_back("multiplicityHFPlus");
   variables.push_back("multiplicityHFMinus");
   variables.push_back("sumEnergyHFPlus");
   variables.push_back("sumEnergyHFMinus");
   variables.push_back("EPlusPzFromTowers");
   variables.push_back("EMinusPzFromTowers");*/
   variables.push_back("EPlusPzFromTowersVarBin_dist_newBinning");
   variables.push_back("EMinusPzFromTowersVarBin_dist_newBinning");
   variables.push_back("sumEnergyHFPlusVarBin_dist_newBinning");
   variables.push_back("sumEnergyHFMinusVarBin_dist_newBinning");
   variables.push_back("multiplicityHFPlusVarBin_dist");
   variables.push_back("multiplicityHFMinusVarBin_dist");
   variables.push_back("sumET");
 
   TFile* fileColl = TFile::Open("root/7TeV/Data/Run132605/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOR_histos.root");
   TFile* fileNoColl = TFile::Open("root/7TeV/Data/Run132605/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasORNoColl_histos.root");

   std::vector<std::pair<std::string,TDirectory*> > dirs;
   dirs.push_back(std::make_pair("Colliding Bx's",fileColl)); 
   dirs.push_back(std::make_pair("Non-colliding Bx's",fileNoColl));
   std::vector<double> normFactors;
   normFactors.push_back(1.);
   normFactors.push_back(1.);

   //Plotter<NumberEntriesNorm> plotter;
   Plotter<DefaultNorm> plotter;
   int colors[] = {kBlack,kRed,kBlue};
   std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
   int linestyles[] = {kSolid,kDashed,kDashDotted};
   std::vector<int> histLineStyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));
   int fillcolors[] = {0,0,0};
   std::vector<int> histFillColors(fillcolors,fillcolors + sizeof(fillcolors)/sizeof(int));
   plotter.SetColors(histColors);
   plotter.SetLineStyles(histLineStyles);
   plotter.SetFillColors(histFillColors);
   plotter.SetRebin(rebin);
   plotter.plot(variables,dirs,normFactors,drawOption);
   //plotter.plot(variables,dirs,drawOption);
}

