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

std::vector<std::pair<std::string,TFile*> > getSignalSamples(std::string const& selection){
   std::vector<std::pair<std::string,TFile*> > files;
   //std::string selection = "nHFMax_0";
   std::string dir = "root/selection/" + selection + "/";
   files.push_back(std::make_pair("<N_{PU}> = 2.1",TFile::Open((dir + "AvePU/analysisDijetsTTree_histos_CEPDijets_M100.root").c_str())));
   files.push_back(std::make_pair("N_{PU} = 0",TFile::Open((dir + "0PU/analysisDijetsTTree_histos_CEPDijets_M100.root").c_str())));
   files.push_back(std::make_pair("N_{PU} = 1",TFile::Open((dir + "1PU/analysisDijetsTTree_histos_CEPDijets_M100.root").c_str())));
   files.push_back(std::make_pair("N_{PU} = 2",TFile::Open((dir + "2PU/analysisDijetsTTree_histos_CEPDijets_M100.root").c_str())));
   files.push_back(std::make_pair("N_{PU} = 3",TFile::Open((dir + "3PU/analysisDijetsTTree_histos_CEPDijets_M100.root").c_str())));
   files.push_back(std::make_pair("N_{PU} = 4",TFile::Open((dir + "4PU/analysisDijetsTTree_histos_CEPDijets_M100.root").c_str())));

   return files;
}

void plot(std::string const& selection = "nHFMax_0", int rebin = 1){
   double sigma = 250.; //pb

   std::vector<string> variables;
   variables.push_back("trackMultiplicity");
   variables.push_back("multiplicityHFPlus");
   variables.push_back("multiplicityHFMinus");
   variables.push_back("xiPlusFromPFCands");
   variables.push_back("xiMinusFromPFCands");
   variables.push_back("massDijets");
   variables.push_back("missingMassFromXi");
   variables.push_back("MxFromJets");
   variables.push_back("MxFromPFCands");
   variables.push_back("xiPlusFromPFCandsAfterSel");
   variables.push_back("xiMinusFromPFCandsAfterSel");
   variables.push_back("RjjFromJets");
   variables.push_back("RjjFromPFCands");
   variables.push_back("RjjFromJetsAfterSel");
   variables.push_back("RjjFromPFCandsAfterSel");
   variables.push_back("RjjFromJetsAfterSelCustomBin");
   variables.push_back("RjjFromPFCandsAfterSelCustomBin");
   variables.push_back("LogRjjFromJetsAfterSel");
   variables.push_back("LogRjjFromPFCandsAfterSel");

   std::vector<std::pair<std::string,TFile*> > files = getSignalSamples(selection);

   std::vector<std::pair<std::string,TDirectory*> > dirs;
   for(std::vector<std::pair<std::string,TFile*> >::const_iterator it = files.begin(); it != files.end(); ++it){
      dirs.push_back(std::make_pair(it->first,static_cast<TDirectory*>(it->second)));
   }

   std::string refVar = "leadingJetPt";
   TH1F const* histRef = getHisto(dirs[0].second,refVar);
   double nEventsRef = histRef->GetSumOfWeights();

   std::vector<double> normFactors(dirs.size(),1./nEventsRef);

   int colors[] = {kRed,kBlue,kMagenta,kYellow,kOrange};
   std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));

   Plotter<DefaultNorm> plotter;
   //Plotter<NumberEntriesNorm> plotter;
   //Plotter<ConstNorm> plotter;
   plotter.SetRebin(rebin);
   plotter.SetVerbose(true);
   //plotter.SetColors(histColors);
   //plotter.plot(variables,dirs);
   plotter.plotComponents(variables,dirs,normFactors);
}
