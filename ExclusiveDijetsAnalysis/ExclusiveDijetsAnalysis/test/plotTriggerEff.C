#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TLegend.h>
#include "TDirectory.h"
#include "TH1F.h"

#include "plotTools.h"

#include <iostream>
#include <vector>
#include <map>

/*TH1F* getHisto(TFile*, const string&);
TH1F* getHisto(TDirectory*, const string&);
void scaleHisto(TH1F* histo, double scale, int line, int color, int rebin);

void plot(std::map<std::string,std::vector<std::string> >& variablesMap, TDirectory* dir, bool Norm);
void plot(std::vector<std::string>& variables, std::vector<TDirectory*>& directories, bool Norm);*/

//void plot(std::map<std::string,std::vector<std::string> >& variablesMap, TDirectory* dir, bool Norm);

void plot(bool Norm = false){
   TFile* file = TFile::Open("root/analysisOpenHLT_MinBias_histos_L1.root");

   std::map<std::string,std::vector<std::string> > varMap;

   std::vector<std::string> varNames;
   varNames.push_back("L1HfRing1EtSumNegativeEta");
   varNames.push_back("L1HfRing2EtSumNegativeEta");
   varNames.push_back("L1HfRing1EtSumPositiveEta");
   varNames.push_back("L1HfRing2EtSumPositiveEta");

   std::vector<std::string> triggerBits;
   //triggerBits.push_back("L1_SingleJet15");
   triggerBits.push_back("L1_SingleJet30");
   triggerBits.push_back("HLT_DiJetAve30");
   //triggerBits.push_back("L1_SingleEG5");
   //triggerBits.push_back("L1_SingleEG8");
   //triggerBits.push_back("L1_DoubleEG5");
   //triggerBits.push_back("L1_DoubleMuOpen");
   //triggerBits.push_back("L1_DoubleMu3");
   //triggerBits.push_back("HLT_DoubleMu3");

   std::vector<std::string> def;
   for(std::vector<std::string>::const_iterator var = varNames.begin(); var != varNames.end(); ++var){
      varMap[*var] = def;
      std::vector<std::string>& variables = varMap[*var];
      variables.push_back(*var);
      for(std::vector<std::string>::const_iterator trigBit = triggerBits.begin(); trigBit != triggerBits.end(); ++trigBit){
         variables.push_back(*var + "_" + *trigBit);
      }
   }

   varMap["Rates"] = def;
   varMap["TrigBitVsThreshold"] = def;
   for(std::vector<std::string>::const_iterator trigBit = triggerBits.begin(); trigBit != triggerBits.end(); ++trigBit){
      varMap["Rates"].push_back(*trigBit + "_Rate");
      varMap["TrigBitVsThreshold"].push_back(*trigBit + "_VsThreshold");
   }

   plot(varMap,file,Norm);
}
