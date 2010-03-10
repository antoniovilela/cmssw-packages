#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"
#include "Utilities/PlottingTools/interface/Plotter.h"

#include <iostream>
#include <string>
#include <vector>

TH1F* createTH1F(TH1D const& hist, std::string const& name);

void plotDataVsMC(std::string const& fileNameData,
                  std::string const& fileNameMC,
                  std::string const& labelData = "Data",
                  std::string const& labelMC = "MC",
                  int rebin = 1){

  std::vector<std::string> variables;
  variables.push_back("HLTBSCORVsTrackMult_eff");
  variables.push_back("HLTBSCORVsHFPlusMult_eff");
  variables.push_back("HLTBSCORVsHFMinusMult_eff");
  variables.push_back("HLTBSCORVsSumEHFPlus_eff");
  variables.push_back("HLTBSCORVsSumEHFMinus_eff");

  TFile* fileData = TFile::Open(fileNameData.c_str());
  TFile* fileMC = TFile::Open(fileNameMC.c_str());
  std::vector<std::pair<std::string,TDirectory*> > dirs;
  dirs.push_back(std::make_pair(labelData,fileData)); 
  dirs.push_back(std::make_pair(labelMC,fileMC));

   Plotter<DefaultNorm> plotter;
   int colors[] = {kBlack,kRed};
   std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
   int linestyles[] = {kSolid,kSolid};
   std::vector<int> histLineStyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));
   plotter.SetColors(histColors);
   plotter.SetLineStyles(histLineStyles);
   plotter.SetRebin(rebin);
   plotter.plot(variables,dirs);
}

void plotTriggerEfficiency(std::string const& fileName, std::string const& treeName, bool saveHistos = false, std::string const& outFileName = ""){

  TFile* file = TFile::Open(fileName.c_str());

  TTree* data = static_cast<TTree*>(file->Get(treeName.c_str()));

  std::vector<TH2F*> histosTH2F;
  // Track mult.
  TH2F* hHLTBSCORVsTrackMult = new TH2F("HLTBSCORVsTrackMult","HLTBSCORVsTrackMult",5,0,30,300,0.,1.1);
  data->Project("HLTBSCORVsTrackMult","HLT_MinBiasBSCOR:trackMultiplicityAssociatedToPV");
  histosTH2F.push_back(hHLTBSCORVsTrackMult);

  // HF mult.
  TH2F* hHLTBSCORVsHFPlusMult = new TH2F("HLTBSCORVsHFPlusMult","HLTBSCORVsHFPlusMult",5,0,20,300,0.,1.1);
  data->Project("HLTBSCORVsHFPlusMult","HLT_MinBiasBSCOR:multiplicityHFPlus");
  histosTH2F.push_back(hHLTBSCORVsHFPlusMult);

  TH2F* hHLTBSCORVsHFMinusMult = new TH2F("HLTBSCORVsHFMinusMult","HLTBSCORVsHFMinusMult",5,0,20,300,0.,1.1);
  data->Project("HLTBSCORVsHFMinusMult","HLT_MinBiasBSCOR:multiplicityHFMinus");
  histosTH2F.push_back(hHLTBSCORVsHFMinusMult);

  // HF energy sum
  TH2F* hHLTBSCORVsSumEHFPlus = new TH2F("HLTBSCORVsSumEHFPlus","HLTBSCORVsSumEHFPlus",5,0.,100.,300,0.,1.1);
  data->Project("HLTBSCORVsSumEHFPlus","HLT_MinBiasBSCOR:sumEnergyHFPlus");
  histosTH2F.push_back(hHLTBSCORVsSumEHFPlus);

  TH2F* hHLTBSCORVsSumEHFMinus = new TH2F("HLTBSCORVsSumEHFMinus","HLTBSCORVsSumEHFMinus",5,0.,100.,300,0.,1.1);
  data->Project("HLTBSCORVsSumEHFMinus","HLT_MinBiasBSCOR:sumEnergyHFMinus");
  histosTH2F.push_back(hHLTBSCORVsSumEHFMinus);

  std::vector<TH1F*> histosTH1F;
  std::vector<TH2F*>::const_iterator it_histoTH2F = histosTH2F.begin();
  std::vector<TH2F*>::const_iterator it_histosTH2F_end = histosTH2F.end();
  for(; it_histoTH2F != it_histosTH2F_end; ++it_histoTH2F){
     TH2F* histHLTBSCORVsVar = *it_histoTH2F;
     TProfile* profileHLTBSCORVsVar = histHLTBSCORVsVar->ProfileX();
     TH1F* hHLTBSCOREffVsVar = createTH1F(*profileHLTBSCORVsVar
                                          ,(std::string(histHLTBSCORVsVar->GetName()) + "_eff"));
     histosTH1F.push_back(hHLTBSCOREffVsVar);
     TCanvas* canvas = new TCanvas(("canvas_" + std::string(histHLTBSCORVsVar->GetName())).c_str(),histHLTBSCORVsVar->GetName()); 
     canvas->cd();
     histosTH1F.back()->Draw(); 
  }
  if(saveHistos){
     TFile* outFile = new TFile(outFileName.c_str(),"recreate");
     outFile->cd();
     for(size_t k = 0; k < histosTH2F.size(); ++k) histosTH2F[k]->Write();
     for(size_t k = 0; k < histosTH1F.size(); ++k) histosTH1F[k]->Write();
  }

}

TH1F* createTH1F(TH1D const& hist, std::string const& name){
   int nBins = hist.GetNbinsX();
   TH1F hist_tmp(name.c_str(),name.c_str(),nBins
                                          ,hist.GetXaxis()->GetBinLowEdge(1)
                                          ,hist.GetXaxis()->GetBinUpEdge(nBins));

   for(int ibin = 0; ibin <= nBins + 1; ++ibin){
      hist_tmp.SetBinContent(ibin,hist.GetBinContent(ibin));
      hist_tmp.SetBinError(ibin,hist.GetBinError(ibin));
   }

   TH1F* hist_new = new TH1F(hist_tmp);

   return hist_new; 
}
