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

std::string getDataFile(int runRange);
std::string getMCFile(int genType,int runRange);

void setDirsMCComponents(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors
);
void setDirsPYTHIAPHOJET(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsDataMC(std::string const& selection,std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsDataMCComponents(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsCompareData(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);

void plot(const char* drawOption = "", std::string const& selection = "NoSel", int rebin = 1){
   std::vector<std::string> variables;
   variables.push_back("nVertex");
   variables.push_back("posXPrimVtx");
   variables.push_back("posYPrimVtx");
   variables.push_back("posZPrimVtx");
   variables.push_back("leadingJetPt");
   variables.push_back("leadingJetEta");
   variables.push_back("leadingJetPhi");
   variables.push_back("trackMultiplicity");
   variables.push_back("trackMultiplicityAssociatedToPV");
   variables.push_back("multiplicityHFPlus");
   variables.push_back("multiplicityHFMinus");
   variables.push_back("sumEnergyHFPlus");
   variables.push_back("sumEnergyHFMinus");
   //variables.push_back("xiTowerPlus");
   //variables.push_back("xiTowerMinus");
   variables.push_back("xiPlusFromTowers");
   variables.push_back("xiMinusFromTowers");
   //variables.push_back("xiPlusFromJets");
   //variables.push_back("xiMinusFromJets");
   variables.push_back("xiPlusFromPFCands");
   variables.push_back("xiMinusFromPFCands");
   //variables.push_back("missingMassFromXiTower");
   //variables.push_back("missingMassFromXiFromJets");
   variables.push_back("missingMassFromXiFromPFCands");
   variables.push_back("EPlusPzFromTowers");
   variables.push_back("EMinusPzFromTowers");
   variables.push_back("EPlusPzFromPFCands");
   variables.push_back("EMinusPzFromPFCands");
   //variables.push_back("MxFromJets");
   variables.push_back("MxFromTowers");
   variables.push_back("MxFromPFCands");
   //variables.push_back("towerEcalTime");
   //variables.push_back("towerHcalTime");
   variables.push_back("sumET");
   //variables.push_back("energySumVsEcalTime");
   //variables.push_back("energySumVsEcalTime");
 
   std::vector<std::pair<std::string,TDirectory*> > dirs;
   std::vector<double> normFactors;

   //setDirsPYTHIAPHOJET(dirs,normFactors);
   //setDirsMCComponents(dirs,normFactors);
   setDirsDataMC(selection,dirs,normFactors);
   //setDirsDataMCComponents(dirs,normFactors);
   //setDirsCompareData(dirs,normFactors);

   //Plotter<NumberEntriesNorm> plotter;
   Plotter<DefaultNorm> plotter;
   int colors[] = {kBlack,kRed,kBlue};
   //int colors[] = {kBlack,kMagenta,kOrange,kBlue,kRed};
   //int colors[] = {kRed,kBlue};
   //int colors[] = {kBlue,kOrange,kRed};
   std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
   int linestyles[] = {kSolid,kDashed,kDashDotted};
   //int linestyles[] = {1,9,10,3,2};
   //int linestyles[] = {1,9,10};
   std::vector<int> histLineStyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));
   plotter.SetColors(histColors);
   plotter.SetLineStyles(histLineStyles);
   plotter.SetRebin(rebin);
   //plotter.plot(variables,dirs,drawOption);
   plotter.plot(variables,dirs,normFactors,drawOption);
   //plotter.plotComponents(variables,dirs,normFactors,drawOption);

}

void setDirsMCComponents(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   TFile* fileMC_All = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
   //TFile* fileMC_All = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks.root");
   TH1F* h_EventSelection_All = static_cast<TH1F*>(fileMC_All->Get("EventSelection"));
   double nEvents_All = h_EventSelection_All->GetBinContent(1);

   TFile* fileMC_Diff = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_Diff.root");
   TH1F* h_EventSelection_Diff = static_cast<TH1F*>(fileMC_Diff->Get("EventSelection"));
   double nEvents_Diff = h_EventSelection_Diff->GetBinContent(1);

   TFile* fileMC_SD = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_SD.root");
   //TFile* fileMC_SD = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_SD.root");
   TH1F* h_EventSelection_SD = static_cast<TH1F*>(fileMC_SD->Get("EventSelection"));
   double nEvents_SD = h_EventSelection_SD->GetBinContent(1);

   TFile* fileMC_DD = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_DD.root");
   //TFile* fileMC_DD = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_DD.root");
   TH1F* h_EventSelection_DD = static_cast<TH1F*>(fileMC_DD->Get("EventSelection"));
   double nEvents_DD = h_EventSelection_DD->GetBinContent(1);

   TFile* fileMC_QCD = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_Inelastic.root");
   //TFile* fileMC_QCD = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_QCD.root");
   TH1F* h_EventSelection_QCD = static_cast<TH1F*>(fileMC_QCD->Get("EventSelection"));
   double nEvents_QCD = h_EventSelection_QCD->GetBinContent(1);

   dirs.push_back(std::make_pair("MinBias PYTHIA",fileMC_All));
   dirs.push_back(std::make_pair("Diffractive",fileMC_Diff));
   dirs.push_back(std::make_pair("Single-diffractive",fileMC_SD));
   dirs.push_back(std::make_pair("Double-diffractive",fileMC_DD));
   dirs.push_back(std::make_pair("QCD",fileMC_QCD));
   normFactors.push_back(1./nEvents_All);
   normFactors.push_back(1./nEvents_Diff);
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
   std::string dir = "root/900GeV/" + selection;
   
   TFile* fileData = TFile::Open((dir + "/" + getDataFile(runRange)).c_str());
   TH1F* h_EventSelectionData = static_cast<TH1F*>(fileData->Get("EventSelection"));
   double nEventsDataFullSel = h_EventSelectionData->GetBinContent(11);

   TFile* fileMC_PYTHIA = TFile::Open((dir + "/" + getMCFile(PYTHIA,runRange)).c_str());
   TH1F* h_EventSelectionMC_PYTHIA = static_cast<TH1F*>(fileMC_PYTHIA->Get("EventSelection"));
   double nEventsMCFullSel_PYTHIA = h_EventSelectionMC_PYTHIA->GetBinContent(11);

   TFile* fileMC_PHOJET = TFile::Open((dir + "/" + getMCFile(PHOJET,runRange)).c_str());
   TH1F* h_EventSelectionMC_PHOJET = static_cast<TH1F*>(fileMC_PHOJET->Get("EventSelection"));
   double nEventsMCFullSel_PHOJET = h_EventSelectionMC_PHOJET->GetBinContent(11);

   dirs.push_back(std::make_pair("Data 900 GeV",fileData));
   dirs.push_back(std::make_pair("MinBias PYTHIA 900 GeV",fileMC_PYTHIA));
   dirs.push_back(std::make_pair("MinBias PHOJET 900 GeV",fileMC_PHOJET)); 
   normFactors.push_back(1.);
   normFactors.push_back(nEventsDataFullSel/nEventsMCFullSel_PYTHIA);
   normFactors.push_back(nEventsDataFullSel/nEventsMCFullSel_PHOJET);
}

void setDirsDataMCComponents(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   //TFile* file_Data = TFile::Open("analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_histos.root"); 
   TFile* file_Data = TFile::Open("root/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasPixel_histos.root");
   TH1F* h_EventSelection_Data = static_cast<TH1F*>(file_Data->Get("EventSelection"));
   double nEventsRun_Data = h_EventSelection_Data->GetBinContent(2);
   double nEventsTrig_Data = h_EventSelection_Data->GetBinContent(3);
   double nEventsPreSel_Data = h_EventSelection_Data->GetBinContent(7);
   double nEventsFullSel_Data = h_EventSelection_Data->GetBinContent(11);

   //TFile* fileMC_All = TFile::Open("analysisMinBiasTTree_PYTHIA_MinBias_900GeV_eventSelectionMinBiasBSCOR_histos_All.root");
   TFile* fileMC_All = TFile::Open("root/analysisMinBiasTTree_PYTHIA_MinBias_900GeV_eventSelectionMinBiasPixel_histos_All.root");
   TH1F* h_EventSelection_All = static_cast<TH1F*>(fileMC_All->Get("EventSelection"));
   double nEvents_All = h_EventSelection_All->GetBinContent(2);
   double nEventsRun_All = h_EventSelection_All->GetBinContent(2);
   double nEventsTrig_All = h_EventSelection_All->GetBinContent(3);
   double nEventsPreSel_All = h_EventSelection_All->GetBinContent(7);
   double nEventsFullSel_All = h_EventSelection_All->GetBinContent(11);

   /*TFile* fileMC_Diff = TFile::Open("");
   TH1F* h_EventSelection_Diff = static_cast<TH1F*>(fileMC_Diff->Get("EventSelection"));
   double nEvents_Diff = h_EventSelection_Diff->GetBinContent(1);
   double nEventsRun_Diff = h_EventSelection_Diff->GetBinContent(2);
   double nEventsPreSel_Diff = h_EventSelection_Diff->GetBinContent(7);
   double nEventsFullSel_Diff = h_EventSelection_Diff->GetBinContent(11);

   TFile* fileMC_Inel = TFile::Open("");
   TH1F* h_EventSelection_Inel = static_cast<TH1F*>(fileMC_Inel->Get("EventSelection"));
   double nEvents_Inel = h_EventSelection_Inel->GetBinContent(1);
   double nEventsRun_Inel = h_EventSelection_Inel->GetBinContent(2);
   double nEventsPreSel_Inel = h_EventSelection_Inel->GetBinContent(7);
   double nEventsFullSel_Inel = h_EventSelection_Inel->GetBinContent(11);*/

   //TFile* fileMC_SD = TFile::Open("analysisMinBiasTTree_PYTHIA_MinBias_900GeV_eventSelectionMinBiasBSCOR_histos_SD.root");
   TFile* fileMC_SD = TFile::Open("root/analysisMinBiasTTree_PYTHIA_MinBias_900GeV_eventSelectionMinBiasPixel_histos_SD.root");
   TH1F* h_EventSelection_SD = static_cast<TH1F*>(fileMC_SD->Get("EventSelection"));
   double nEvents_SD = h_EventSelection_SD->GetBinContent(1);
   double nEventsRun_SD = h_EventSelection_SD->GetBinContent(2);
   double nEventsPreSel_SD = h_EventSelection_SD->GetBinContent(7);

   //TFile* fileMC_DD = TFile::Open("analysisMinBiasTTree_PYTHIA_MinBias_900GeV_eventSelectionMinBiasBSCOR_histos_DD.root");
   TFile* fileMC_DD = TFile::Open("root/analysisMinBiasTTree_PYTHIA_MinBias_900GeV_eventSelectionMinBiasPixel_histos_DD.root");
   TH1F* h_EventSelection_DD = static_cast<TH1F*>(fileMC_DD->Get("EventSelection"));
   double nEvents_DD = h_EventSelection_DD->GetBinContent(1);
   double nEventsRun_DD = h_EventSelection_DD->GetBinContent(2);
   double nEventsPreSel_DD = h_EventSelection_DD->GetBinContent(7);

   //TFile* fileMC_QCD = TFile::Open("analysisMinBiasTTree_PYTHIA_MinBias_900GeV_eventSelectionMinBiasBSCOR_histos_Inelastic.root");
   TFile* fileMC_QCD = TFile::Open("root/analysisMinBiasTTree_PYTHIA_MinBias_900GeV_eventSelectionMinBiasPixel_histos_Inelastic.root");
   TH1F* h_EventSelection_QCD = static_cast<TH1F*>(fileMC_QCD->Get("EventSelection"));
   double nEvents_QCD = h_EventSelection_QCD->GetBinContent(1);
   double nEventsRun_QCD = h_EventSelection_QCD->GetBinContent(2);
   double nEventsPreSel_QCD = h_EventSelection_QCD->GetBinContent(7);

   dirs.push_back(std::make_pair("First collisions",file_Data));
   dirs.push_back(std::make_pair("MinBias PYTHIA - SD",fileMC_SD));
   dirs.push_back(std::make_pair("MinBias PYTHIA - DD",fileMC_DD)); 
   dirs.push_back(std::make_pair("MinBias PYTHIA - QCD",fileMC_QCD));

   /*double fracDiff = 0.30;
   normFactors.push_back(1.);
   normFactors.push_back(nEventsPreSel_Data*(nEvents_All/nEventsPreSel_All)*fracDiff/nEventsRun_Diff);
   normFactors.push_back(nEventsPreSel_Data*(nEvents_All/nEventsPreSel_All)*(1. - fracDiff)/nEventsRun_Inel);*/

   /*normFactors.push_back(1.);
   normFactors.push_back((nEventsFullSel_Data/nEventsFullSel_All)*(nEvents_All/nEvents_Diff));
   normFactors.push_back((nEventsFullSel_Data/nEventsFullSel_All)*(nEvents_All/nEvents_Inel));*/

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
