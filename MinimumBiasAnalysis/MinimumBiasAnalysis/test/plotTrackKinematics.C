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
#include <string>

void setDirsDataMC(std::string const&, std::vector<std::pair<std::string,TDirectory*> >&, std::vector<double>&);
void setDirsDataMCDiff(std::string const&, std::vector<std::pair<std::string,TDirectory*> >&, std::vector<double>&);
void setDirsDataMCTunes(std::string const&, std::vector<std::pair<std::string,TDirectory*> >&, std::vector<double>&);
void setDirsDataMCMatBudget(std::string const&, std::vector<std::pair<std::string,TDirectory*> >&, std::vector<double>&);
void setDirsDataVsSumEHF(std::vector<std::pair<std::string,TDirectory*> >&, std::vector<double>&);

void plot(std::string const& eventSelection = "", std::string const& mode = "setDirsDataMC", const char* drawOption = "", int rebin = 1){

   std::vector<std::string> variables;
   variables.push_back("TrackEta");
   variables.push_back("TrackPt");
   variables.push_back("NTracks");
   variables.push_back("TrackPtSum");
 
   /*std::string fileNameEvtSel_data = "root/7TeV/Data/Run132440/analysisMinBiasTTree_MinimumBias_7TeV_";
   fileNameEvtSel_data += eventSelection; fileNameEvtSel_data += "_histos.root";
   TFile* fileEvtSel_data = TFile::Open(fileNameEvtSel_data.c_str());
   TH1F* h_EventSelection_data = static_cast<TH1F*>(fileEvtSel_data->Get("EventSelection"));
   double nEvents_data = h_EventSelection_data->GetBinContent(h_EventSelection_data->GetNbinsX());

   std::string fileNameEvtSel_mc = "root/7TeV/Pythia6/analysisMinBiasTTree_PYTHIA_MinBias_7TeV_";
   fileNameEvtSel_mc += eventSelection; fileNameEvtSel_mc += "_histos_All.root";
   TFile* fileEvtSel_mc = TFile::Open(fileNameEvtSel_mc.c_str());
   TH1F* h_EventSelection_mc = static_cast<TH1F*>(fileEvtSel_mc->Get("EventSelection"));
   double nEvents_mc = h_EventSelection_mc->GetBinContent(h_EventSelection_mc->GetNbinsX());*/

   std::vector<std::pair<std::string,TDirectory*> > dirs;
   std::vector<double> normFactors;
   if(mode == "setDirsDataMC") setDirsDataMC(eventSelection,dirs,normFactors);
   else if(mode == "setDirsDataMCDiff") setDirsDataMCDiff(eventSelection,dirs,normFactors);
   else if(mode == "setDirsDataMCTunes") setDirsDataMCTunes(eventSelection,dirs,normFactors);
   else if(mode == "setDirsDataMCMatBudget") setDirsDataMCMatBudget(eventSelection,dirs,normFactors);
   else if(mode == "setDirsDataVsSumEHF") setDirsDataVsSumEHF(dirs,normFactors);

   //Plotter<NumberEntriesNorm> plotter;
   Plotter<DefaultNorm> plotter;
   int colors[] = {1,2,6,12,4,43,38,57};
   std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
   int linestyles[] = {1,1,2,3,4,9,10,2};
   std::vector<int> histLineStyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));
   int markerstyles[] = {20,1,1,1,1,1,1,1};
   std::vector<int> histMarkerStyles(markerstyles,markerstyles + sizeof(markerstyles)/sizeof(int));
   plotter.SetLineColors(histColors);
   plotter.SetLineStyles(histLineStyles);
   plotter.SetFillColors(std::vector<int>(1,0));
   plotter.SetFillStyles(std::vector<int>(1,0));
   plotter.SetMarkerColors(histColors);
   plotter.SetMarkerStyles(histMarkerStyles);
   plotter.SetMarkerSizes(std::vector<float>(1,1.));
   plotter.SetRebin(rebin);
   plotter.SetVerbose(true);
   plotter.SetStats(false);
   plotter.plot(variables,dirs,normFactors,drawOption);
   //plotter.plot(variables,dirs,drawOption);
   //plotter.plotComponents(variables,dirs,normFactors,"NOSTACK");
   //plotter.plotComponents(variables,dirs,normFactors,drawOption);
}

void setDirsDataMC(std::string const& eventSelection,std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){

   TFile* file_data = TFile::Open("root/7TeV/Data/Run132605/trackAnalysis/analysisTracks_MinimumBias_Commissioning10-GOODCOLL-Jun14thSkim_Run132605_trackAnalysis-v3.root");
   TFile* file_mc = TFile::Open("root/7TeV/Pythia8/trackAnalysis/analysisTracks_MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1-trackAnalysis-v1.root");

   /*std::string labelAll  = "trackHistoAnalyzer_";                       labelAll  += eventSelection;
   std::string labelSD   = "trackHistoAnalyzer_processIdPythia6_SD_";   labelSD   += eventSelection;
   std::string labelDD   = "trackHistoAnalyzer_processIdPythia6_DD_";   labelDD   += eventSelection;
   std::string labelDiff = "trackHistoAnalyzer_processIdPythia6_Diff_"; labelDiff += eventSelection;
   std::string labelND   = "trackHistoAnalyzer_processIdPythia6_ND_";   labelND   += eventSelection;*/

   std::string labelAll  = "trackHistoAnalyzer_";                       labelAll  += eventSelection;
   std::string labelSD   = "trackHistoAnalyzer_processIdPythia8_SD_";   labelSD   += eventSelection;
   std::string labelDD   = "trackHistoAnalyzer_processIdPythia8_DD_";   labelDD   += eventSelection;
   std::string labelDiff = "trackHistoAnalyzer_processIdPythia8_Diff_"; labelDiff += eventSelection;
   std::string labelND   = "trackHistoAnalyzer_processIdPythia8_ND_";   labelND   += eventSelection;

   std::string hNTracksPath = labelAll + "/NTracks";
   TH1F* hNTracks_data = static_cast<TH1F*>(file_data->Get(hNTracksPath.c_str()));
   double nEvents_data = hNTracks_data->GetEntries();
   TH1F* hNTracks_mc = static_cast<TH1F*>(file_mc->Get(hNTracksPath.c_str()));
   double nEvents_mc = hNTracks_mc->GetEntries();
   std::cout << "Number of events in Data: " << nEvents_data << std::endl;
   std::cout << "Number of events in MC: " << nEvents_mc << std::endl;

   dirs.push_back(std::make_pair("p+p (7 TeV)",file_data->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("MC All",file_mc->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("SD",file_mc->GetDirectory(labelSD.c_str())));
   dirs.push_back(std::make_pair("DD",file_mc->GetDirectory(labelDD.c_str())));
   //dirs.push_back(std::make_pair("Diff",file_mc->GetDirectory(labelDiff.c_str()))); 
   dirs.push_back(std::make_pair("ND",file_mc->GetDirectory(labelND.c_str())));

   normFactors.resize(5);
   normFactors[0] = 1./nEvents_data;
   normFactors[1] = 1./nEvents_mc;
   normFactors[2] = 1./nEvents_mc;
   normFactors[3] = 1./nEvents_mc;
   normFactors[4] = 1./nEvents_mc;
   //normFactors[5] = 1./nEvents_mc;
}

void setDirsDataMCDiff(std::string const& eventSelection,std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   
   TFile* file_data = TFile::Open("root/7TeV/Data/Run132605/trackAnalysis/analysisTracks_MinimumBias_Commissioning10-GOODCOLL-Jun14thSkim_Run132605_trackAnalysis-v3.root");
   TFile* file_mc_0 = TFile::Open("root/7TeV/Pythia6D6T/trackAnalysis/analysisTracks_MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1-trackAnalysis-v1.root");
   TFile* file_mc_1 = TFile::Open("root/7TeV/Pythia8/trackAnalysis/analysisTracks_MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1-trackAnalysis-v1.root");
   TFile* file_mc_2 = TFile::Open("root/7TeV/Phojet/trackAnalysis/analysisTracks_MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1-trackAnalysis-v1.root");

   std::string labelAll    = "trackHistoAnalyzer_";                       labelAll    += eventSelection;

   std::string labelDiff_0 = "trackHistoAnalyzer_processIdPythia6_Diff_"; labelDiff_0 += eventSelection;
   std::string labelND_0   = "trackHistoAnalyzer_processIdPythia6_ND_";   labelND_0   += eventSelection;

   std::string labelDiff_1 = "trackHistoAnalyzer_processIdPythia8_Diff_"; labelDiff_1 += eventSelection;
   std::string labelND_1   = "trackHistoAnalyzer_processIdPythia8_ND_";   labelND_1   += eventSelection;

   std::string labelDiff_2 = "trackHistoAnalyzer_processIdPhojet_Diff_";  labelDiff_2 += eventSelection;
   std::string labelND_2   = "trackHistoAnalyzer_processIdPhojet_ND_";    labelND_2   += eventSelection;

   std::string hNTracksPath = labelAll + "/NTracks";
   TH1F* hNTracks_data = static_cast<TH1F*>(file_data->Get(hNTracksPath.c_str()));
   double nEvents_data = hNTracks_data->GetEntries();
   TH1F* hNTracks_mc_0 = static_cast<TH1F*>(file_mc_0->Get(hNTracksPath.c_str()));
   double nEvents_mc_0 = hNTracks_mc_0->GetEntries();
   TH1F* hNTracks_mc_1 = static_cast<TH1F*>(file_mc_1->Get(hNTracksPath.c_str()));
   double nEvents_mc_1 = hNTracks_mc_1->GetEntries();
   TH1F* hNTracks_mc_2 = static_cast<TH1F*>(file_mc_2->Get(hNTracksPath.c_str()));
   double nEvents_mc_2 = hNTracks_mc_2->GetEntries();

   dirs.push_back(std::make_pair("p+p (7 TeV)",file_data->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PYTHIA D6T All",file_mc_0->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PYTHIA D6T Diffractive",file_mc_0->GetDirectory(labelDiff_0.c_str()))); 
   dirs.push_back(std::make_pair("PYTHIA 8 All",file_mc_1->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PYTHIA 8 Diffractive",file_mc_1->GetDirectory(labelDiff_1.c_str())));
   dirs.push_back(std::make_pair("PHOJET All",file_mc_2->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PHOJET Diffractive",file_mc_2->GetDirectory(labelDiff_2.c_str())));

   normFactors.resize(7);
   normFactors[0] = 1./nEvents_data;
   normFactors[1] = 1./nEvents_mc_0;
   normFactors[2] = 1./nEvents_mc_0;
   normFactors[3] = 1./nEvents_mc_1;
   normFactors[4] = 1./nEvents_mc_1;
   normFactors[5] = 1./nEvents_mc_2;
   normFactors[6] = 1./nEvents_mc_2;
}

void setDirsDataMCTunes(std::string const& eventSelection,std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   TFile* file_data = TFile::Open("root/7TeV/Data/Run132605/trackAnalysis/analysisTracks_MinimumBias_Commissioning10-GOODCOLL-Jun14thSkim_Run132605_trackAnalysis-v3.root");

   std::vector<TFile*> filesMC;
   filesMC.resize(7);
   filesMC[0] = TFile::Open("root/7TeV/Pythia6D6T/trackAnalysis/analysisTracks_MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1-trackAnalysis-v1.root");
   filesMC[1] = TFile::Open("root/7TeV/Pythia6DW/trackAnalysis/analysisTracks_MinBias_TuneDW_7TeV-pythia6_Summer10-START36_V10-v1-trackAnalysis-v1.root");
   filesMC[2] = TFile::Open("root/7TeV/Pythia6CW/trackAnalysis/analysisTracks_MinBias_TuneCW_7TeV-pythia6_Summer10-START36_V10-v1-trackAnalysis-v1.root"); 
   filesMC[3] = TFile::Open("root/7TeV/Pythia6P0/trackAnalysis/analysisTracks_MinBias_TuneP0_7TeV-pythia6_Summer10-START36_V10_SP10-v1-trackAnalysis-v1.root");
   filesMC[4] = TFile::Open("root/7TeV/Pythia6Z1/trackAnalysis/analysisTracks_MinBias_TuneZ1_7TeV-pythia6_Summer10-START36_V10_TP-v1-trackAnalysis-v1.root");
   filesMC[5] = TFile::Open("root/7TeV/Pythia8/trackAnalysis/analysisTracks_MinBias_7TeV-pythia8_Summer10-START36_V10_SP10-v1-trackAnalysis-v1.root");
   filesMC[6] = TFile::Open("root/7TeV/Phojet/trackAnalysis/analysisTracks_MinBias_7TeV-phojet_START36_V10_362-HLT-v1_antoniov-PrivateProd-START36_V10_362_RECO-v1-trackAnalysis-v1.root");

   std::string labelAll    = "trackHistoAnalyzer_";                       labelAll    += eventSelection;

   std::string labelDiffPythia6 = "trackHistoAnalyzer_processIdPythia6_Diff_"; labelDiffPythia6 += eventSelection;
   std::string labelNDPythia6   = "trackHistoAnalyzer_processIdPythia6_ND_";   labelNDPythia6   += eventSelection;

   std::string labelDiffPythia8 = "trackHistoAnalyzer_processIdPythia8_Diff_"; labelDiffPythia8 += eventSelection;
   std::string labelNDPythia8   = "trackHistoAnalyzer_processIdPythia8_ND_";   labelNDPythia8   += eventSelection;

   std::string labelDiffPhojet = "trackHistoAnalyzer_processIdPhojet_Diff_"; labelDiffPhojet += eventSelection;
   std::string labelNDPhojet   = "trackHistoAnalyzer_processIdPhojet_ND_";   labelNDPhojet   += eventSelection;

   dirs.push_back(std::make_pair("p+p (7 TeV)",file_data->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PYTHIA D6T",filesMC[0]->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PYTHIA DW",filesMC[1]->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PYTHIA CW",filesMC[2]->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PYTHIA P0",filesMC[3]->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PYTHIA Z1",filesMC[4]->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PYTHIA 8",filesMC[5]->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PHOJET",filesMC[6]->GetDirectory(labelAll.c_str())));

   std::string hNTracksPath = labelAll + "/NTracks";
   TH1F* hNTracks_data = static_cast<TH1F*>(file_data->Get(hNTracksPath.c_str()));
   double nEvents_data = hNTracks_data->GetEntries();
   normFactors.push_back(1./nEvents_data); 
   std::vector<TH1F*> hNTracksMC;
   std::vector<double> nEventsMC;
   std::vector<TFile*>::const_iterator it_file = filesMC.begin();
   std::vector<TFile*>::const_iterator filesMC_end = filesMC.end();
   for(; it_file != filesMC_end; ++it_file){
      hNTracksMC.push_back( static_cast<TH1F*>( (*it_file)->Get( hNTracksPath.c_str() ) ) );
      double nEvents_mc = hNTracksMC.back()->GetEntries();
      nEventsMC.push_back( nEvents_mc );
      normFactors.push_back( 1./nEvents_mc );

      std::cout << (*it_file)->GetName() << ":  " << nEvents_mc << std::endl; 
   }
}
 
void setDirsDataMCMatBudget(std::string const& eventSelection,std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   TFile* file_data = TFile::Open("root/7TeV/Data/Run132605/trackAnalysis/analysisTracks_MinimumBias_Commissioning10-GOODCOLL-Jun14thSkim_Run132605_trackAnalysis-v3.root");

   std::vector<TFile*> filesMC;
   filesMC.resize(3);
   filesMC[0] = TFile::Open("root/7TeV/Pythia6D6T/trackAnalysis/analysisTracks_MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10_SP10-v1-trackAnalysis-v1.root");
   filesMC[1] = TFile::Open("root/7TeV/Pythia6D6T_X0MAX/trackAnalysis/analysisTracks_MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10-X0MAX-v1-trackAnalysis-v1.root");
   filesMC[2] = TFile::Open("root/7TeV/Pythia6D6T_LiMAX/trackAnalysis/analysisTracks_MinBias_TuneD6T_7TeV-pythia6_Summer10-START36_V10-LiMAX-v1-trackAnalysis-v1.root");

   std::string labelAll    = "trackHistoAnalyzer_";                       labelAll    += eventSelection; 

   std::string labelDiffPythia6 = "trackHistoAnalyzer_processIdPythia6_Diff_"; labelDiffPythia6 += eventSelection;
   std::string labelNDPythia6   = "trackHistoAnalyzer_processIdPythia6_ND_";   labelNDPythia6   += eventSelection;

   dirs.push_back(std::make_pair("p+p (7 TeV)",file_data->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PYTHIA D6T",filesMC[0]->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PYTHIA D6T - X0Max",filesMC[1]->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("PYTHIA D6T - LiMax",filesMC[2]->GetDirectory(labelAll.c_str())));

   std::string hNTracksPath = labelAll + "/NTracks";
   TH1F* hNTracks_data = static_cast<TH1F*>(file_data->Get(hNTracksPath.c_str()));
   double nEvents_data = hNTracks_data->GetEntries();
   normFactors.push_back(1./nEvents_data);
   std::cout << file_data->GetName() << ":  " << nEvents_data << std::endl;
   std::vector<TH1F*> hNTracksMC;
   std::vector<double> nEventsMC;
   std::vector<TFile*>::const_iterator it_file = filesMC.begin();
   std::vector<TFile*>::const_iterator filesMC_end = filesMC.end();
   for(; it_file != filesMC_end; ++it_file){
      hNTracksMC.push_back( static_cast<TH1F*>( (*it_file)->Get( hNTracksPath.c_str() ) ) );
      double nEvents_mc = hNTracksMC.back()->GetEntries();
      nEventsMC.push_back( nEvents_mc );
      normFactors.push_back( 1./nEvents_mc );

      std::cout << (*it_file)->GetName() << ":  " << nEvents_mc << std::endl;
   }
}

void setDirsDataVsSumEHF(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){

   TFile* file_data = TFile::Open("root/7TeV/Data/Run132605/trackAnalysis/analysisTracks_MinimumBias_Commissioning10-GOODCOLL-Jun14thSkim_Run132605_trackAnalysis-v3.root");

   std::string labelAll              = "trackHistoAnalyzer_eventSelectionBscMinBiasOR";
   std::string labelSumEMaxHFPlus4   = "trackHistoAnalyzer_eventSelectionBscMinBiasORSumEMaxHFPlus4";
   std::string labelSumEMaxHFPlus8   = "trackHistoAnalyzer_eventSelectionBscMinBiasORSumEMaxHFPlus8";
   std::string labelSumEMaxHFPlus12  = "trackHistoAnalyzer_eventSelectionBscMinBiasORSumEMaxHFPlus12";
   std::string labelSumEMaxHFPlus16  = "trackHistoAnalyzer_eventSelectionBscMinBiasORSumEMaxHFPlus16";

   dirs.push_back(std::make_pair("p+p (7 TeV)",file_data->GetDirectory(labelAll.c_str())));
   dirs.push_back(std::make_pair("p+p (7 TeV) E(HF+) < 16 GeV",file_data->GetDirectory(labelSumEMaxHFPlus16.c_str())));
   dirs.push_back(std::make_pair("p+p (7 TeV) E(HF+) < 12 GeV",file_data->GetDirectory(labelSumEMaxHFPlus12.c_str())));
   dirs.push_back(std::make_pair("p+p (7 TeV) E(HF+) < 8 GeV",file_data->GetDirectory(labelSumEMaxHFPlus8.c_str())));
   dirs.push_back(std::make_pair("p+p (7 TeV) E(HF+) < 4 GeV",file_data->GetDirectory(labelSumEMaxHFPlus4.c_str())));

   /*std::string hNTracksPath = labelAll + "/NTracks";
   TH1F* hNTracks_data = static_cast<TH1F*>(file_data->Get(hNTracksPath.c_str()));
   double nEvents_data = hNTracks_data->GetEntries();*/

   normFactors = std::vector<double>(5,1.);       
}
