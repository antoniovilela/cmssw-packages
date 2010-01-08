#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/test/PlottingTools.h"
#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/test/Plotter.h"

#include <iostream>
#include <vector>

void setDirsDataMCComponents(double,std::vector<std::pair<std::string,TDirectory*> >&, std::vector<double>&);

void plot(double fracDiff, const char* drawOption = "", int rebin = 1){
   std::vector<std::string> variables;
   variables.push_back("nVertex");
   variables.push_back("posXPrimVtx");
   variables.push_back("posYPrimVtx");
   variables.push_back("posZPrimVtx");
   /*variables.push_back("leadingJetPt");
   variables.push_back("leadingJetEta");
   variables.push_back("leadingJetPhi");*/
   variables.push_back("trackMultiplicity");
   variables.push_back("trackMultiplicityAssociatedToPV");
   variables.push_back("multiplicityHFPlus");
   variables.push_back("multiplicityHFMinus");
   variables.push_back("sumEnergyHFPlus");
   variables.push_back("sumEnergyHFMinus");
   variables.push_back("xiTowerPlus");
   variables.push_back("xiTowerMinus");
   variables.push_back("xiPlusFromTowers");
   variables.push_back("xiMinusFromTowers");
   /*variables.push_back("xiPlusFromJets");
   variables.push_back("xiMinusFromJets");*/
   variables.push_back("xiPlusFromPFCands");
   variables.push_back("xiMinusFromPFCands");
   variables.push_back("missingMassFromXiTower");
   //variables.push_back("missingMassFromXiFromJets");
   variables.push_back("missingMassFromXiFromPFCands");
   variables.push_back("EPlusPzFromTowers");
   variables.push_back("EMinusPzFromTowers");
   variables.push_back("EPlusPzFromPFCands");
   variables.push_back("EMinusPzFromPFCands");
   //variables.push_back("MxFromJets");
   variables.push_back("MxFromPFCands");
   variables.push_back("towerEcalTime");
   variables.push_back("towerHcalTime");
   variables.push_back("sumET");
   variables.push_back("energySumVsEcalTime");
   variables.push_back("energySumVsEcalTime");
 
   std::vector<std::pair<std::string,TDirectory*> > dirs;
   std::vector<double> normFactors;

   setDirsDataMCComponents(fracDiff,dirs,normFactors);

   Plotter<DefaultNorm> plotter;
   plotter.SetRebin(rebin);
   plotter.plotComponents(variables,dirs,normFactors,drawOption);
}

void setDirsDataMCComponents(double fracDiff, std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   TFile* file_Data = TFile::Open("analysisMinBiasFWLite_histos_MinimumBias_Dec19thReReco-Runs124020-124030_new_highQualityTracks.root");
   //TFile* file_Data = TFile::Open("root/analysisMinBiasFWLite_histos_MinimumBias_Dec19thReReco-Run124120.root");
   //TFile* file_Data = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
   TH1F* h_EventSelection_Data = static_cast<TH1F*>(file_Data->Get("EventSelection"));
   double nEventsRun_Data = h_EventSelection_Data->GetBinContent(2);
   double nEventsTrig_Data = h_EventSelection_Data->GetBinContent(3);
   double nEventsPreSel_Data = h_EventSelection_Data->GetBinContent(7);
   double nEventsFullSel_Data = h_EventSelection_Data->GetBinContent(11);

   /*TFile* fileMC_All = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
   //TFile* fileMC_All = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks.root");
   TH1F* h_EventSelection_All = static_cast<TH1F*>(fileMC_All->Get("EventSelection"));
   double nEvents_All = h_EventSelection_All->GetBinContent(2);
   double nEventsRun_All = h_EventSelection_All->GetBinContent(2);
   double nEventsTrig_All = h_EventSelection_All->GetBinContent(3);
   double nEventsPreSel_All = h_EventSelection_All->GetBinContent(7);
   double nEventsFullSel_All = h_EventSelection_All->GetBinContent(11);*/

   TFile* fileMC_Diff = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_Diff.root");
   //TFile* fileMC_Diff = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_Diff.root");
   TH1F* h_EventSelection_Diff = static_cast<TH1F*>(fileMC_Diff->Get("EventSelection"));
   double nEvents_Diff = h_EventSelection_Diff->GetBinContent(1);
   double nEventsRun_Diff = h_EventSelection_Diff->GetBinContent(2);
   double nEventsPreSel_Diff = h_EventSelection_Diff->GetBinContent(7);
   double nEventsFullSel_Diff = h_EventSelection_Diff->GetBinContent(11);

   TFile* fileMC_Inel = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_Inelastic.root");
   //TFile* fileMC_Inel = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_Inel.root");
   TH1F* h_EventSelection_Inel = static_cast<TH1F*>(fileMC_Inel->Get("EventSelection"));
   double nEvents_Inel = h_EventSelection_Inel->GetBinContent(1);
   double nEventsRun_Inel = h_EventSelection_Inel->GetBinContent(2);
   double nEventsPreSel_Inel = h_EventSelection_Inel->GetBinContent(7);
   double nEventsFullSel_Inel = h_EventSelection_Inel->GetBinContent(11);

   dirs.push_back(std::make_pair("First collisions",file_Data));
   dirs.push_back(std::make_pair("MinBias PYTHIA - Diffractive",fileMC_Diff));
   dirs.push_back(std::make_pair("MinBias PYTHIA - Non-diffractive",fileMC_Inel));

   /*double effDiff = fracDiff*(nEventsPreSel_Diff/nEventsRun_Diff);
   double effInel = (1. - fracDiff)*(nEventsPreSel_Inel/nEventsRun_Inel);
 
   normFactors.push_back(1.);
   normFactors.push_back(nEventsPreSel_Data*(effDiff/(effDiff + effInel))/nEventsPreSel_Diff);
   normFactors.push_back(nEventsPreSel_Data*(effInel/(effDiff + effInel))/nEventsPreSel_Inel);*/

   double effDiff = fracDiff*(nEventsFullSel_Diff/nEventsRun_Diff);
   double effInel = (1. - fracDiff)*(nEventsFullSel_Inel/nEventsRun_Inel);
 
   normFactors.push_back(1.);
   normFactors.push_back(nEventsFullSel_Data*(effDiff/(effDiff + effInel))/nEventsFullSel_Diff);
   normFactors.push_back(nEventsFullSel_Data*(effInel/(effDiff + effInel))/nEventsFullSel_Inel);
}
