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

void plot(std::string const& selection = "nHFMax_0", int rebin = 1, const char* drawOption = "", bool plotStackVars = false){

   std::vector<std::string> samples;
   samples.push_back("CEP di-jets");
   samples.push_back("SD-plus di-jets");
   samples.push_back("SD-minus di-jets");
   samples.push_back("DPE di-jets");
   samples.push_back("QCD non-diffractive");

   std::vector<TFile*> files;
   std::string dir = "root/selection/" + selection + "/noPU/";
   files.push_back(TFile::Open((dir + "analysisDijetsTTree_histos_CEPDijets_M100_noPU.root").c_str()));
   files.push_back(TFile::Open((dir + "analysisDijetsTTree_histos_SDPlusDijets_Pt30_FastSim.root").c_str()));
   files.push_back(TFile::Open((dir + "analysisDijetsTTree_histos_SDMinusDijets_Pt30_FastSim.root").c_str()));
   files.push_back(TFile::Open((dir + "analysisDijetsTTree_histos_DPEDijets_Pt40_noPU.root").c_str()));
   files.push_back(TFile::Open((dir + "analysisDijetsTTree_histos_QCD100to250-madgraph_FastSim.root").c_str()));
   

   std::vector<std::pair<std::string,TDirectory*> > directories;
   size_t index = 0;
   for(std::vector<TFile*>::const_iterator file = files.begin(),files_end = files.end(); file != files_end; ++file,++index){
      directories.push_back(std::make_pair(samples[index],static_cast<TDirectory*>(*file)));
   }

   std::vector<double> sigmas;
   sigmas.push_back(250.);
   sigmas.push_back(300000./2);
   sigmas.push_back(300000./2);
   sigmas.push_back(4600.);
   sigmas.push_back(15000000.);

   std::vector<double> normFactors(files.size());
   index = 0;
   for(std::vector<TFile*>::const_iterator file = files.begin(),files_end = files.end(); file != files_end; ++file,++index){
      std::string const& name = samples[index];
      double sigma = sigmas[index];

      const TH1F* h_SelEff = static_cast<const TH1F*>((*file)->Get("SelectionEff"));
      const TH1F* h_leadingJetPt_dijetsSelection = static_cast<const TH1F*>((*file)->Get("leadingJetPt"));

      double eff_HLT = h_SelEff->GetBinContent(1);
      double eff_Vtx = h_SelEff->GetBinContent(2);
      double eff_dijetsSelection = h_SelEff->GetBinContent(3);

      std::cout << " Efficiency for HLT selection " << name << ": " << eff_HLT << std::endl;
      std::cout << " Efficiency for Vertex selection " << name << ": " << eff_Vtx << std::endl;
      std::cout << " Efficiency for Di-jets selection " << name << ": " << eff_dijetsSelection << std::endl;

      double nEventsAfterSel = h_leadingJetPt_dijetsSelection->GetEntries();
      double scale = sigma*eff_HLT*eff_Vtx*eff_dijetsSelection/nEventsAfterSel;
      normFactors[index] = scale;
   }

   std::vector<std::string> variables;
   variables.push_back("leadingJetPt");
   variables.push_back("leadingJetEta");
   variables.push_back("leadingJetPhi");
   variables.push_back("secondJetPt");
   variables.push_back("secondJetEta");
   variables.push_back("secondJetPhi");
   variables.push_back("jetsDeltaEta");
   variables.push_back("jetsDeltaPhi");
   variables.push_back("thirdJetPt");
   variables.push_back("thirdJetEta");
   variables.push_back("trackMultiplicity");
   variables.push_back("multiplicityHFPlus");
   variables.push_back("multiplicityHFMinus");
   variables.push_back("xiPlusFromPFCands");
   variables.push_back("xiMinusFromPFCands");
   variables.push_back("massDijets");
   variables.push_back("missingMassFromXi");
   variables.push_back("MxFromJets");
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

   std::vector<std::string> variablesStack;
   variablesStack.push_back("RjjFromJets");
   variablesStack.push_back("RjjFromPFCands");
   variablesStack.push_back("RjjFromJetsAfterSel");
   variablesStack.push_back("RjjFromPFCandsAfterSel");
   variablesStack.push_back("RjjFromJetsAfterSelCustomBin");
   variablesStack.push_back("RjjFromPFCandsAfterSelCustomBin");
   variablesStack.push_back("LogRjjFromJetsAfterSel");
   variablesStack.push_back("LogRjjFromPFCandsAfterSel");

   //Plotter<NumberEntriesNorm> plotter;
   Plotter<DefaultNorm> plotter;
   int colors[] = {kBlack,kMagenta,kOrange,kBlue,kRed};
   std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
   //int linestyles[] = {kSolid,kDashed,kDotted,kDashDotted,kDashDotted};
   int linestyles[] = {1,9,10,3,2};
   std::vector<int> histLineStyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));
   plotter.SetColors(histColors);
   plotter.SetLineStyles(histLineStyles);
   plotter.SetRebin(rebin);
   if(plotStackVars){
      plotter.plotStack(variablesStack,directories,normFactors,drawOption);
   } else{
      plotter.plot(variables,directories,drawOption); 
      //plotter.plot(variables,directories,normFactors,drawOption);
   }
}
