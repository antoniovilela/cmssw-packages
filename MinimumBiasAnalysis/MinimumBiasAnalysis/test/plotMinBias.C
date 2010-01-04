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

void setDirsMCComponents(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors
);
void setDirsPYTHIAPHOJET(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsDataMC(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsDataMCComponents(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);

void plot(const char* drawOption = ""){
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
   variables.push_back("xiTowerPlus");
   variables.push_back("xiTowerMinus");
   variables.push_back("xiPlusFromJets");
   variables.push_back("xiMinusFromJets");
   variables.push_back("xiPlusFromPFCands");
   variables.push_back("xiMinusFromPFCands");
   variables.push_back("missingMassFromXiTower");
   variables.push_back("missingMassFromXiFromJets");
   variables.push_back("missingMassFromXiFromPFCands");
   variables.push_back("MxFromJets");
   variables.push_back("MxFromPFCands");
   variables.push_back("towerEcalTime");
   variables.push_back("towerHcalTime");
   variables.push_back("sumET");
   variables.push_back("energySumVsEcalTime");
   variables.push_back("energySumVsEcalTime");
 
   std::vector<std::pair<std::string,TDirectory*> > dirs;
   std::vector<double> normFactors;

   //setDirsPYTHIAPHOJET(dirs,normFactors);
   //setDirsMCComponents(dirs,normFactors);
   //setDirsDataMC(dirs,normFactors);
   setDirsDataMCComponents(dirs,normFactors);

   //Plotter<NumberEntriesNorm> plotter;
   Plotter<DefaultNorm> plotter;
   //plotter.SetRebin(2);
   //plotter.plot(variables,dirs,drawOption);
   plotter.plot(variables,dirs,normFactors,drawOption);

}

void setDirsMCComponents(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   TFile* fileMC_All = TFile::Open("analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
   TH1F* h_EventSelection_All = static_cast<TH1F*>(fileMC_All->Get("EventSelection"));
   double nEvents_All = h_EventSelection_All->GetBinContent(1);

   TFile* fileMC_QCD = TFile::Open("analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_QCD.root");
   TH1F* h_EventSelection_QCD = static_cast<TH1F*>(fileMC_QCD->Get("EventSelection"));
   double nEvents_QCD = h_EventSelection_QCD->GetBinContent(1);

   TFile* fileMC_Diff = TFile::Open("analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_SD_DD.root");
   TH1F* h_EventSelection_Diff = static_cast<TH1F*>(fileMC_Diff->Get("EventSelection"));
   double nEvents_Diff = h_EventSelection_Diff->GetBinContent(1);

   dirs.push_back(std::make_pair("MinBias PYTHIA 900 GeV",fileMC_All));
   dirs.push_back(std::make_pair("QCD",fileMC_QCD));
   dirs.push_back(std::make_pair("SD + DD",fileMC_Diff));
   normFactors.push_back(1./nEvents_All);
   normFactors.push_back(1./nEvents_QCD);
   normFactors.push_back(1./nEvents_Diff);
}

void setDirsPYTHIAPHOJET(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   TFile* filePYTHIA = TFile::Open("analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8K_900GeV.root");
   TH1F* h_EventSelectionPYTHIA = static_cast<TH1F*>(filePYTHIA->Get("EventSelection"));
   double nEventsPreSelPYTHIA = h_EventSelectionPYTHIA->GetBinContent(7);

   TFile* filePHOJET = TFile::Open("analysisMinBiasFWLite_histos_PHOJET_STARTUP3X_V8I_900GeV.root");
   TH1F* h_EventSelectionPHOJET = static_cast<TH1F*>(filePHOJET->Get("EventSelection"));
   double nEventsPreSelPHOJET = h_EventSelectionPHOJET->GetBinContent(7);

   dirs.push_back(std::make_pair("MinBias PYTHIA 900 GeV",filePYTHIA));
   dirs.push_back(std::make_pair("MinBias PHOJET 900 GeV",filePHOJET));
   normFactors.push_back(1./nEventsPreSelPYTHIA);
   normFactors.push_back(1./nEventsPreSelPHOJET);
}

void setDirsDataMC(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   TFile* fileData = TFile::Open("analysisMinBiasFWLite_histos_MinimumBias_Dec19thReReco-Runs124020-124030_new_highQualityTracks.root");
   //TFile* fileData = TFile::Open("analysisMinBiasFWLite_histos_MinimumBias_Dec19thReReco-Run124120.root");
   TH1F* h_EventSelectionData = static_cast<TH1F*>(fileData->Get("EventSelection"));
   double nEventsDataTrig = h_EventSelectionData->GetBinContent(3);

   TFile* fileMC = TFile::Open("analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
   //TFile* fileMC = TFile::Open("analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8L_2360GeV.root");
   TH1F* h_EventSelectionMC = static_cast<TH1F*>(fileMC->Get("EventSelection"));
   double nEventsMCTrig = h_EventSelectionMC->GetBinContent(3);

   dirs.push_back(std::make_pair("First collisions 900 GeV",fileData));
   dirs.push_back(std::make_pair("MinBias PYTHIA 900 GeV",fileMC));
   normFactors.push_back(1.);
   normFactors.push_back(nEventsDataTrig/nEventsMCTrig);
}

void setDirsDataMCComponents(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   TFile* fileData = TFile::Open("analysisMinBiasFWLite_histos_MinimumBias_Dec19thReReco-Runs124020-124030_new_highQualityTracks.root");
   //TFile* fileData = TFile::Open("analysisMinBiasFWLite_histos_MinimumBias_Dec19thReReco-Run124120.root");
   TH1F* h_EventSelectionData = static_cast<TH1F*>(fileData->Get("EventSelection"));
   double nEventsDataRun = h_EventSelectionData->GetBinContent(2);
   double nEventsDataTrig = h_EventSelectionData->GetBinContent(3);

   TFile* fileMC_QCD = TFile::Open("analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_QCD.root");
   TH1F* h_EventSelection_QCD = static_cast<TH1F*>(fileMC_QCD->Get("EventSelection"));
   double nEventsRun_QCD = h_EventSelection_QCD->GetBinContent(2);

   TFile* fileMC_Diff = TFile::Open("analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_SD_DD.root");
   TH1F* h_EventSelection_Diff = static_cast<TH1F*>(fileMC_Diff->Get("EventSelection"));
   double nEventsRun_Diff = h_EventSelection_Diff->GetBinContent(2);

   dirs.push_back(std::make_pair("First collisions 900 GeV",fileData));
   dirs.push_back(std::make_pair("MinBias PYTHIA 900 GeV - QCD",fileMC_QCD));
   dirs.push_back(std::make_pair("MinBias PYTHIA 900 GeV - SD+DD",fileMC_Diff));

   double fracDiff = 0.20;
   //double effTrig = nEventsDataTrig/nEventsDataRun; 
   normFactors.push_back(1.);
   normFactors.push_back(nEventsDataRun*(1. - fracDiff)/nEventsRun_QCD);
   normFactors.push_back(nEventsDataRun*fracDiff/nEventsRun_Diff);
}
