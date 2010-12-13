#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

//#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/test/PlottingTools.h"
//#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/test/Plotter.h"
#include "Utilities/PlottingTools/interface/PlottingTools.h"
#include "Utilities/PlottingTools/interface/Plotter.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

#include <iostream>
#include <vector>

//std::string getDataFile(int runRange);
//std::string getMCFile(int genType,int runRange);

void setDirsMCComponents(std::string const& selection,std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsPYTHIAPHOJET(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsDataMC(std::string const& selection,std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsDataMCComponents(std::string const& selection,std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsCompareData(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);

void plot(const char* drawOption = "", std::string const& selection = "NoSel", int rebin = 1){
   std::vector<std::string> variables;
   /*variables.push_back("nVertex");
   variables.push_back("posXPrimVtx");
   variables.push_back("posYPrimVtx");
   variables.push_back("posZPrimVtx");*/
   variables.push_back("multiplicityTracks");
   variables.push_back("multiplicityHFPlus");
   variables.push_back("multiplicityHFMinus");
   variables.push_back("sumEnergyHFPlus");
   variables.push_back("sumEnergyHFMinus");
   variables.push_back("xiPlusFromPFCands");
   variables.push_back("xiMinusFromPFCands");
   variables.push_back("EPlusPzFromPFCands");
   variables.push_back("EMinusPzFromPFCands");
   variables.push_back("MxFromPFCands");
   variables.push_back("etaMaxFromPFCands");
   variables.push_back("etaMinFromPFCands");
 
   std::vector<std::pair<std::string,TDirectory*> > dirs;
   std::vector<double> normFactors;

   //setDirsPYTHIAPHOJET(dirs,normFactors);
   //setDirsMCComponents(selection,dirs,normFactors);
   //setDirsDataMC(selection,dirs,normFactors);
   setDirsDataMCComponents(selection,dirs,normFactors);
   //setDirsCompareData(dirs,normFactors);

   //Plotter<NumberEntriesNorm> plotter;
   Plotter<DefaultNorm> plotter;
   //int colors[] = {kBlack,kRed,kBlue};
   //int colors[] = {kBlack,kRed,kOrange,kMagenta,kYellow,kBlue};
   //int colors[] = {kRed,kBlue};
   int colors[] = {kBlue,kOrange,kRed};
   std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
   //int linestyles[] = {kSolid,kDashed,kDashDotted,kSolid,kDashed,kDashDotted};
   //int linestyles[] = {1,2,3,9,10,2};
   int linestyles[] = {1,9,10};
   std::vector<int> histLineStyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));
   plotter.SetColors(histColors);
   plotter.SetLineStyles(histLineStyles);
   plotter.SetStats(false);
   plotter.SetRebin(rebin);
   //plotter.plot(variables,dirs,drawOption);
   //plotter.plot(variables,dirs,normFactors,drawOption);
   plotter.plotComponents(variables,dirs,normFactors,drawOption);
}

void setDirsMCComponents(std::string const& selection, std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   run_range_t runRange = Data7TeV;
   generator_t genType = PYTHIAD6T;
   std::string eventSelection = "minimumBiasTTreeAnalysisHBHENoiseFilterHcalNoiseSelection";
   std::string dir = "root/7TeV/Pythia6D6T/eventSelection";   

   TFile* fileMC_All = TFile::Open((dir + "/" + getHistosFileName(genType,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelection_All = static_cast<TH1F*>(fileMC_All->Get("EventSelection"));
   double nEvents_All = h_EventSelection_All->GetBinContent(1);

   TFile* fileMC_SD = TFile::Open((dir + "/" + getHistosFileName(genType,runRange,SD,eventSelection)).c_str());
   TH1F* h_EventSelection_SD = static_cast<TH1F*>(fileMC_SD->Get("EventSelection"));
   double nEvents_SD = h_EventSelection_SD->GetBinContent(1);

   TFile* fileMC_DD = TFile::Open((dir + "/" + getHistosFileName(genType,runRange,DD,eventSelection)).c_str());
   TH1F* h_EventSelection_DD = static_cast<TH1F*>(fileMC_DD->Get("EventSelection"));
   double nEvents_DD = h_EventSelection_DD->GetBinContent(1);

   TFile* fileMC_QCD = TFile::Open((dir + "/" + getHistosFileName(genType,runRange,Inelastic,eventSelection)).c_str());
   TH1F* h_EventSelection_QCD = static_cast<TH1F*>(fileMC_QCD->Get("EventSelection"));
   double nEvents_QCD = h_EventSelection_QCD->GetBinContent(1);

   dirs.push_back(std::make_pair("PYTHIA-6 D6T",fileMC_All));
   dirs.push_back(std::make_pair("Single-diffractive",fileMC_SD));
   dirs.push_back(std::make_pair("Double-diffractive",fileMC_DD));
   dirs.push_back(std::make_pair("Inel. non-diffractive",fileMC_QCD));
   normFactors.push_back(1./nEvents_All);
   normFactors.push_back(1./nEvents_SD);
   normFactors.push_back(1./nEvents_DD);
   normFactors.push_back(1./nEvents_QCD);
}

void setDirsPYTHIAPHOJET(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   TFile* filePYTHIA = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
   TH1F* h_EventSelectionPYTHIA = static_cast<TH1F*>(filePYTHIA->Get("EventSelection"));
   double nEventsPreSelPYTHIA = h_EventSelectionPYTHIA->GetBinContent(7);

   TFile* filePHOJET = TFile::Open("analysisMinBiasFWLite_histos_PHOJET_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
   TH1F* h_EventSelectionPHOJET = static_cast<TH1F*>(filePHOJET->Get("EventSelection"));
   double nEventsPreSelPHOJET = h_EventSelectionPHOJET->GetBinContent(7);

   dirs.push_back(std::make_pair("MinBias PYTHIA",filePYTHIA));
   dirs.push_back(std::make_pair("MinBias PHOJET",filePHOJET));
   normFactors.push_back(1./nEventsPreSelPYTHIA);
   normFactors.push_back(1./nEventsPreSelPHOJET);
}

void setDirsDataMC(std::string const& selection, std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   run_range_t runRange = Data900GeV;
   std::string eventSelection = "eventSelectionMinBiasBSCOR";
   std::string dir = "root/900GeV/" + selection;
   
   //TFile* fileData = TFile::Open((dir + "/" + getHistosFileName(runRange,eventSelection)).c_str());
   TFile* fileDataRef = TFile::Open((dir + "/" + getHistosFileName(runRange,eventSelection)).c_str());
   //TFile* fileData = TFile::Open("histosErrorBand_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_EnergyThresholdHFVar.root");
   TFile* fileData = fileDataRef; 
   //TH1F* h_EventSelectionData = static_cast<TH1F*>(fileData->Get("EventSelection"));
   TH1F* h_EventSelectionData = static_cast<TH1F*>(fileDataRef->Get("EventSelection"));
   double nEventsDataFullSel = h_EventSelectionData->GetBinContent(11);

   TFile* fileMC_PYTHIA = TFile::Open((dir + "/" + getHistosFileName(PYTHIA,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelectionMC_PYTHIA = static_cast<TH1F*>(fileMC_PYTHIA->Get("EventSelection"));
   double nEventsMCFullSel_PYTHIA = h_EventSelectionMC_PYTHIA->GetBinContent(11);

   TFile* fileMC_PHOJET = TFile::Open((dir + "/" + getHistosFileName(PHOJET,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelectionMC_PHOJET = static_cast<TH1F*>(fileMC_PHOJET->Get("EventSelection"));
   double nEventsMCFullSel_PHOJET = h_EventSelectionMC_PHOJET->GetBinContent(11);

   TFile* fileMC_PYTHIADW = TFile::Open((dir + "/" + getHistosFileName(PYTHIADW,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelectionMC_PYTHIADW = static_cast<TH1F*>(fileMC_PYTHIADW->Get("EventSelection"));
   double nEventsMCFullSel_PYTHIADW = h_EventSelectionMC_PYTHIADW->GetBinContent(11);

   TFile* fileMC_PYTHIAP0 = TFile::Open((dir + "/" + getHistosFileName(PYTHIAP0,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelectionMC_PYTHIAP0 = static_cast<TH1F*>(fileMC_PYTHIAP0->Get("EventSelection"));
   double nEventsMCFullSel_PYTHIAP0 = h_EventSelectionMC_PYTHIAP0->GetBinContent(11);

   TFile* fileMC_PYTHIAProQ20 = TFile::Open((dir + "/" + getHistosFileName(PYTHIAProQ20,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelectionMC_PYTHIAProQ20 = static_cast<TH1F*>(fileMC_PYTHIAProQ20->Get("EventSelection"));
   double nEventsMCFullSel_PYTHIAProQ20 = h_EventSelectionMC_PYTHIAProQ20->GetBinContent(11);

   dirs.push_back(std::make_pair("Data 900 GeV",fileData));
   dirs.push_back(std::make_pair("MinBias PYTHIA D6T 900 GeV",fileMC_PYTHIA));
   dirs.push_back(std::make_pair("MinBias PYTHIA DW 900 GeV",fileMC_PYTHIADW));
   dirs.push_back(std::make_pair("MinBias PYTHIA P0 900 GeV",fileMC_PYTHIAP0));
   dirs.push_back(std::make_pair("MinBias PYTHIA ProQ20 900 GeV",fileMC_PYTHIAProQ20)); 
   dirs.push_back(std::make_pair("MinBias PHOJET 900 GeV",fileMC_PHOJET));
   normFactors.push_back(1.);
   normFactors.push_back(nEventsDataFullSel/nEventsMCFullSel_PYTHIA);
   normFactors.push_back(nEventsDataFullSel/nEventsMCFullSel_PYTHIADW);
   normFactors.push_back(nEventsDataFullSel/nEventsMCFullSel_PYTHIAP0);
   normFactors.push_back(nEventsDataFullSel/nEventsMCFullSel_PYTHIAProQ20);
   normFactors.push_back(nEventsDataFullSel/nEventsMCFullSel_PHOJET);
}

void setDirsDataMCComponents(std::string const& selection, std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   run_range_t runRange = Data7TeV;
   //generator_t genType = PYTHIAD6T;
   generator_t genType = PHOJET;
   std::string eventSelection = "minimumBiasTTreeAnalysisHBHENoiseFilterHcalNoiseSelection";
   std::string dirData = "root/7TeV/Data/Run132605/eventSelection";
   //std::string dirMC = "root/7TeV/Pythia6D6T/eventSelection";
   std::string dirMC = "root/7TeV/Phojet/eventSelection";

   TFile* file_Data = TFile::Open((dirData + "/" + getHistosFileName(runRange,eventSelection)).c_str());
   TH1F* h_EventSelection_Data = static_cast<TH1F*>(file_Data->Get("EventSelection"));
   double nEventsFullSel_Data = h_EventSelection_Data->GetBinContent(11);

   TFile* fileMC_All = TFile::Open((dirMC + "/" + getHistosFileName(genType,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelection_All = static_cast<TH1F*>(fileMC_All->Get("EventSelection"));
   double nEvents_All = h_EventSelection_All->GetBinContent(1);
   double nEventsFullSel_All = h_EventSelection_All->GetBinContent(11);

   TFile* fileMC_SD = TFile::Open((dirMC + "/" + getHistosFileName(genType,runRange,SD,eventSelection)).c_str());
   TH1F* h_EventSelection_SD = static_cast<TH1F*>(fileMC_SD->Get("EventSelection"));
   double nEvents_SD = h_EventSelection_SD->GetBinContent(1);

   TFile* fileMC_DD = TFile::Open((dirMC + "/" + getHistosFileName(genType,runRange,DD,eventSelection)).c_str());
   TH1F* h_EventSelection_DD = static_cast<TH1F*>(fileMC_DD->Get("EventSelection"));
   double nEvents_DD = h_EventSelection_DD->GetBinContent(1);

   TFile* fileMC_QCD = TFile::Open((dirMC + "/" + getHistosFileName(genType,runRange,Inelastic,eventSelection)).c_str());
   TH1F* h_EventSelection_QCD = static_cast<TH1F*>(fileMC_QCD->Get("EventSelection"));
   double nEvents_QCD = h_EventSelection_QCD->GetBinContent(1);

   dirs.push_back(std::make_pair("p+p 7 TeV",file_Data));
   dirs.push_back(std::make_pair("PYTHIA - SD",fileMC_SD));
   dirs.push_back(std::make_pair("PYTHIA - DD",fileMC_DD)); 
   dirs.push_back(std::make_pair("PYTHIA - Inel. non-diffractive",fileMC_QCD));
   /*dirs.push_back(std::make_pair("MinBias PHOJET - SD",fileMC_SD));
   dirs.push_back(std::make_pair("MinBias PHOJET - DD",fileMC_DD)); 
   dirs.push_back(std::make_pair("MinBias PHOJET - Inel. non-diffractive",fileMC_QCD));*/

   normFactors.push_back(1.);
   normFactors.push_back((nEventsFullSel_Data/nEventsFullSel_All)*(nEvents_All/nEvents_SD));
   normFactors.push_back((nEventsFullSel_Data/nEventsFullSel_All)*(nEvents_All/nEvents_DD));
   normFactors.push_back((nEventsFullSel_Data/nEventsFullSel_All)*(nEvents_All/nEvents_QCD));
}

void setDirsCompareData(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   //TFile* file_Ref = TFile::Open("analysisMinBiasFWLite_histos_MinimumBias_Dec14thReReco-Run124120_new_highQualityTracks.root");
   TFile* file_Ref = TFile::Open("analysisMinBiasFWLite_histos_MinimumBias_Dec19thReReco-Runs124020-124030_new_highQualityTracks.root");
   TH1F* h_EventSelection_Ref = static_cast<TH1F*>(file_Ref->Get("EventSelection"));
   double nEventsPreSel_Ref = h_EventSelection_Ref->GetBinContent(7);

   TFile* file_Comp = TFile::Open("analysisMinBiasFWLite_histos_MinimumBias_Dec19thReReco-Run124120_new_highQualityTracks.root");
   TH1F* h_EventSelection_Comp = static_cast<TH1F*>(file_Comp->Get("EventSelection"));
   double nEventsPreSel_Comp = h_EventSelection_Comp->GetBinContent(7);

   /*dirs.push_back(std::make_pair("Dec. 14th Re-reco",file_Ref));
   dirs.push_back(std::make_pair("Dec. 19th Re-reco",file_Comp));*/
   dirs.push_back(std::make_pair("Runs 124020-124030 - 900 GeV",file_Ref));
   dirs.push_back(std::make_pair("Run 124120 - 2360 GeV",file_Comp));
   normFactors.push_back(1./nEventsPreSel_Ref);
   normFactors.push_back(1./nEventsPreSel_Comp);
}

/*
std::string getDataFile(int runRange){
   std::string fileName;
   if(runRange == Data900GeV) fileName = "analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_histos.root";
   else if(runRange == Data2360GeV) fileName = "analysisMinBiasTTree_MinimumBias_Run124120_eventSelectionMinBiasBSCOR_histos.root";
   else throw RootException("ERROR: Invalid option");

   return fileName;
}

std::string getMCFile(int genType, int runRange){
   std::string fileName;
   if(genType == PYTHIA){
      if(runRange == Data900GeV) fileName = "analysisMinBiasTTree_PYTHIA_MinBias_900GeV_eventSelectionMinBiasBSCOR_histos_All.root";
      else if(runRange == Data2360GeV) fileName = "analysisMinBiasTTree_PYTHIA_MinBias_2360GeV_eventSelectionMinBiasBSCOR_histos_All.root";
      else throw RootException("ERROR: Invalid option");
   } else if(genType == PHOJET){
      if(runRange == Data900GeV) fileName = "analysisMinBiasTTree_PHOJET_MinBias_900GeV_eventSelectionMinBiasBSCOR_histos_All.root";
      else if(runRange == Data2360GeV) fileName = "analysisMinBiasTTree_PHOJET_MinBias_2360GeV_eventSelectionMinBiasBSCOR_histos_All.root";
      else throw RootException("ERROR: Invalid option");
   } else throw RootException("ERROR: Invalid option");      

   return fileName;
}
*/
