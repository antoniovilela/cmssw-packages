#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TLegend.h>
#include "TDirectory.h"
#include "TH1F.h"

//#include "PlottingTools.h"
#include "Plotter.h"

#include <iostream>
#include <vector>
#include <map>

void plot(){
   TFile* file = TFile::Open("analysisPAT_histos.root");

   std::map<std::string,std::vector<std::string> > varMap;

   std::vector<std::string> jetColls;
   jetColls.push_back("selectedLayer1JetsSC7PF");
   //jetColls.push_back("selectedLayer1JetsSC7Calo");
   jetColls.push_back("selectedLayer1JetsSC5PF");
   //jetColls.push_back("selectedLayer1JetsSC5Calo");
   //jetColls.push_back("selectedLayer1JetsKT6PF");
   //jetColls.push_back("selectedLayer1JetsKT6Calo");

   std::vector<std::string> refVars;
   refVars.push_back("RjjFromJets");
   refVars.push_back("ResXiPlusFromJets");
   refVars.push_back("ResXiMinusFromJets");
   refVars.push_back("ResMassDijets");

   std::vector<std::string> def;
   for(std::vector<std::string>::const_iterator var = refVars.begin(); var != refVars.end(); ++var){
      varMap[*var] = def;
      std::vector<std::string>& variables = varMap[*var];
      for(std::vector<std::string>::const_iterator coll = jetColls.begin(); coll != jetColls.end(); ++coll){
         variables.push_back(*var + "_" + *coll);
      }
   }

   std::map<std::string,std::vector<std::string> > varMapCands;
   varMapCands["Rjj"] = def;
   varMapCands["Rjj"].push_back("RjjFromJets");
   varMapCands["Rjj"].push_back("RjjFromPFCands");
   varMapCands["ResXiPlus"] = def;
   varMapCands["ResXiPlus"].push_back("ResXiPlus");
   varMapCands["ResXiPlus"].push_back("ResXiPlusFromJets");
   varMapCands["ResXiPlus"].push_back("ResXiPlusFromPFCands"); 
   varMapCands["ResXiMinus"] = def;
   varMapCands["ResXiMinus"].push_back("ResXiMinus");
   varMapCands["ResXiMinus"].push_back("ResXiMinusFromJets");
   varMapCands["ResXiMinus"].push_back("ResXiMinusFromPFCands");

   Plotter<DefaultNorm> plotter;
   plotter.plot(varMap,file); 
   plotter.plot(varMapCands,file);
   //plot(varMap,file);
   //plot(varMapCands,file);
}
