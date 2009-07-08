#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TLegend.h>
#include "TDirectory.h"
#include "TH1F.h"

//#include "plotTools.h"
#include "PlottingTools.h"
#include "Plotter.h"

#include <iostream>
#include <vector>
#include <map>

void plot(bool doNorm = false){
   TFile* file = TFile::Open("root/analysisDijetsTrigger_CEPGG_M100_histos.root");

   std::vector<std::string> varNames;
   varNames.push_back("Ring 1 HF-plus");
   varNames.push_back("Ring 1 HF-minus");
   varNames.push_back("Ring 2 HF-plus");
   varNames.push_back("Ring 2 HF-minus");

   std::vector<std::string> triggerBits;
   triggerBits.push_back("L1_SingleJet30");

   std::vector<std::string> variables;
   for(std::vector<std::string>::const_iterator var = varNames.begin(); var != varNames.end(); ++var){
      for(std::vector<std::string>::const_iterator trig = triggerBits.begin(); trig != triggerBits.end(); ++trig){
         variables.push_back(*var + "_" + *trig);
      }
   }

   //std::map<std::string,std::vector<std::string> > varMap = buildVarMap(varNames,triggerBits);

   std::vector<std::pair<std::string,TDirectory*> > dirs;
   dirs.push_back(std::make_pair("<N_{PU}> = 2.1",file->GetDirectory("triggerAnalysis")));
   dirs.push_back(std::make_pair("N_{PU} = 0",file->GetDirectory("triggerAnalysis_filter0PU")));
   dirs.push_back(std::make_pair("N_{PU} = 1",file->GetDirectory("triggerAnalysis_filter1PU")));
   dirs.push_back(std::make_pair("N_{PU} = 2",file->GetDirectory("triggerAnalysis_filter2PU")));
   dirs.push_back(std::make_pair("N_{PU} = 3",file->GetDirectory("triggerAnalysis_filter3PU")));
   dirs.push_back(std::make_pair("N_{PU} = 4",file->GetDirectory("triggerAnalysis_filter4PU")));

   /*DefaultNorm* defNorm = new DefaultNorm(); 
   ConstNorm* constNorm = new ConstNorm(1.);

   BaseNorm* myNorm = 0;
   if(doNorm) myNorm = constNorm;
   else myNorm = defNorm;

   plot(variables,dirs,myNorm);*/

   Plotter<DefaultNorm> defPlotter;
   Plotter<ConstNorm> constPlotter;
   if(doNorm) constPlotter.plot(variables,dirs);
   else defPlotter.plot(variables,dirs);

   std::vector<std::string> counts;
   counts.push_back("L1_SingleJet30_count");

   //plot(counts,dirs,myNorm);
 
   if(doNorm) constPlotter.plot(counts,dirs);
   else defPlotter.plot(counts,dirs);

   /*delete defNorm;
   delete constNorm;*/
}
