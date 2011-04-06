#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TLegend.h>
#include "TDirectory.h"
#include "TH1F.h"

#include "Utilities/PlottingTools/interface/Plotter.h"
#include "Utilities/PlottingTools/interface/PlottingTools.h"

#include <iostream>
#include <vector>
#include <map>

void plot(std::string const& fileName){
   TFile* file = TFile::Open( fileName.c_str() );

   std::map<std::string,std::vector<std::string> > varMap;
   std::map<std::string,std::vector<std::string> > varMapRates;

   std::vector<std::string> varNames;
   /*varNames.push_back("L1HfRing1EtSumNegativeEta");
   varNames.push_back("L1HfRing2EtSumNegativeEta");
   varNames.push_back("L1HfRing1EtSumPositiveEta");
   varNames.push_back("L1HfRing2EtSumPositiveEta");*/
   varNames.push_back("L1HfTowerCountPositiveEtaRing1");
   varNames.push_back("L1HfTowerCountNegativeEtaRing1");
   varNames.push_back("L1HfTowerCountPositiveEtaRing2");
   varNames.push_back("L1HfTowerCountNegativeEtaRing2");

   std::vector<std::string> triggerBits;
   //triggerBits.push_back("L1_SingleJet16");
   triggerBits.push_back("L1_SingleJet36");
   triggerBits.push_back("L1_SingleEG12");
   triggerBits.push_back("L1_DoubleEG3");

   std::vector<std::string> def;
   for(std::vector<std::string>::const_iterator var = varNames.begin(); var != varNames.end(); ++var){
      varMap[*var] = def;
      std::vector<std::string>& variables = varMap[*var];
      variables.push_back(*var);
      for(std::vector<std::string>::const_iterator trigBit = triggerBits.begin(); trigBit != triggerBits.end(); ++trigBit){
         variables.push_back(*var + "_" + *trigBit);
      }
   }

   varMapRates["Rates"] = def;
   varMapRates["TrigBitVsThreshold"] = def;
   for(std::vector<std::string>::const_iterator trigBit = triggerBits.begin(); trigBit != triggerBits.end(); ++trigBit){
      varMapRates["Rates"].push_back(*trigBit + "_Rate");
      varMapRates["TrigBitVsThreshold"].push_back(*trigBit + "_VsThreshold");
   }

   Plotter<NumberEntriesNorm> plotter;
   int colors[] = {kBlack,kRed,kBlue,kMagenta,kOrange};
   int linestyles[] = {1,2,3,9,10,12};
   std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
   std::vector<int> histLineStyles(linestyles,linestyles+sizeof(linestyles)/sizeof(int));
   plotter.SetLineColors(histColors);
   plotter.SetLineStyles(histLineStyles);
   plotter.SetFillColors(std::vector<int>(1,0));
   plotter.SetFillStyles(std::vector<int>(1,0));
   plotter.SetStats(false);

   plotter.plot(varMap,file,"HISTOE1");

   Plotter<DefaultNorm> plotterRates;
   histColors.erase(histColors.begin());
   histLineStyles.erase(histLineStyles.begin());
   plotterRates.SetLineColors(histColors);
   plotterRates.SetLineStyles(histLineStyles);
   plotterRates.SetFillColors(std::vector<int>(1,0));
   plotterRates.SetFillStyles(std::vector<int>(1,0));
   plotterRates.SetStats(false);  

   plotterRates.plot(varMapRates,file,"HISTOE1");
}
