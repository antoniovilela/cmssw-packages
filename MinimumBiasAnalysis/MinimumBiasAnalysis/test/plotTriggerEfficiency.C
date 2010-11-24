#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"
#include "Utilities/PlottingTools/interface/Plotter.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/EventData.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace minimumBiasAnalysis;

void plotTriggerEfficiency(TTree*,std::string const&);

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

void plotTriggerEfficiency(std::string const& fileName, std::string const& treeName, std::string const& outFileName = ""){

  TFile* file = TFile::Open(fileName.c_str());
  TTree* data = static_cast<TTree*>(file->Get(treeName.c_str()));

  if(!data){                 
      std::cout << "ERROR: Could not find " << treeName << " in " << fileName << std::endl;
      file->Close();          
      return;                 
  }

  plotTriggerEfficiency(data,outFileName);
}

void plotTriggerEfficiency(std::vector<std::string> const& fileNames, std::string const& treeName, std::string const& outFileName = ""){

  TChain* chain = new TChain(treeName.c_str());
  for(size_t ifile = 0; ifile < fileNames.size(); ++ifile) {std::cout << "Adding " << fileNames[ifile] << std::endl; chain->Add(fileNames[ifile].c_str());}

  plotTriggerEfficiency(chain,outFileName);
}

void plotTriggerEfficiency(TTree* data, std::string const& outFileName){
  
  EventData eventData;
  setTTreeBranches(*data,eventData);
  int nEntries = data->GetEntries();
 
  TFile outFile(outFileName.c_str(),"recreate");

  std::vector<TH1F*> histosAll;
  std::vector<TH2F*> histos2DAll;
  TH1F* hTrackMult = new TH1F("HLTBSCORVsTrackMult","HLTBSCORVsTrackMult",5,0,30);
  TH1F* hHFPlusMult = new TH1F("HLTBSCORVsHFPlusMult","HLTBSCORVsHFPlusMult",5,0,20);
  TH1F* hHFMinusMult = new TH1F("HLTBSCORVsHFMinusMult","HLTBSCORVsHFMinusMult",5,0,20);
  TH1F* hSumEHFPlus = new TH1F("HLTBSCORVsSumEHFPlus","HLTBSCORVsSumEHFPlus",5,0.,100.);
  TH1F* hSumEHFMinus = new TH1F("HLTBSCORVsSumEHFMinus","HLTBSCORVsSumEHFMinus",5,0.,100.);
  TH2F* hSumEHFPlusVsSumEHFMinus = new TH2F("HLTBSCORVsSumEHFPlusVsSumEHFMinus","HLTBSCORVsSumEHFPlusVsSumEHFMinus",10,0.,100.,10,0.,100.);
  histosAll.push_back(hTrackMult);
  histosAll.push_back(hHFPlusMult);
  histosAll.push_back(hHFMinusMult);
  histosAll.push_back(hSumEHFPlus);
  histosAll.push_back(hSumEHFMinus); 
  histos2DAll.push_back(hSumEHFPlusVsSumEHFMinus);

  std::vector<TH1F*> histosEff;
  std::vector<TH1F*>::const_iterator it_histoAll = histosAll.begin();
  std::vector<TH1F*>::const_iterator it_histosAll_end = histosAll.end();
  for(; it_histoAll != it_histosAll_end; ++it_histoAll){
     std::string hname = (*it_histoAll)->GetName();
     hname += "_eff";
     histosEff.push_back(static_cast<TH1F*>((*it_histoAll)->Clone(hname.c_str())));
  }
  std::vector<TH2F*> histos2DEff;
  std::vector<TH2F*>::const_iterator it_histo2DAll = histos2DAll.begin();
  std::vector<TH2F*>::const_iterator it_histos2DAll_end = histos2DAll.end();
  for(; it_histo2DAll != it_histos2DAll_end; ++it_histo2DAll){
     std::string hname = (*it_histo2DAll)->GetName();
     hname += "_eff";
     histos2DEff.push_back(static_cast<TH2F*>((*it_histo2DAll)->Clone(hname.c_str())));
  }

  double nAcc = 0.;
  double nAll = 0.;
  // Loop over the events
  for(int ientry = 0; ientry < nEntries; ++ientry){

     data->GetEntry(ientry);
     int trigBSCOR = eventData.HLTPath_;
     int trackMultiplicity = eventData.multiplicityTracks_;
     int multiplicityHFPlus = eventData.multiplicityHFPlus_;
     int multiplicityHFMinus = eventData.multiplicityHFMinus_;
     double sumEnergyHFPlus = eventData.sumEnergyHFPlus_;
     double sumEnergyHFMinus = eventData.sumEnergyHFMinus_;

     ++nAll;
     histosAll[0]->Fill(trackMultiplicity);
     histosAll[1]->Fill(multiplicityHFPlus);
     histosAll[2]->Fill(multiplicityHFMinus);
     histosAll[3]->Fill(sumEnergyHFPlus);
     histosAll[4]->Fill(sumEnergyHFMinus);
     histos2DAll[0]->Fill(sumEnergyHFPlus,sumEnergyHFMinus);
     if(trigBSCOR == 1){
        ++nAcc;
        histosEff[0]->Fill(trackMultiplicity);
        histosEff[1]->Fill(multiplicityHFPlus);
        histosEff[2]->Fill(multiplicityHFMinus);
        histosEff[3]->Fill(sumEnergyHFPlus);
        histosEff[4]->Fill(sumEnergyHFMinus);
        histos2DEff[0]->Fill(sumEnergyHFPlus,sumEnergyHFMinus); 
     }
  }
  double effAve = nAcc/nAll;
  double effError = sqrt(nAll*effAve*(1. - effAve))/nAll;
  std::cout << "Efficiency = " << effAve << " +/- " << effError << std::endl;
 
  std::vector<TGraphAsymmErrors*> graphsEff(histosEff.size());  
  for(size_t k = 0; k < histosAll.size(); ++k){
     TH1F* hAll = histosAll[k];
     TH1F* hEff = histosEff[k];
     std::cout << "================================" << std::endl;
     std::cout << " Histogram " << hAll->GetName() << std::endl;
     graphsEff[k] = new TGraphAsymmErrors(hEff,hAll);
     graphsEff[k]->SetName( (std::string(hEff->GetName()) + "_graph" ).c_str() );
     std::cout << "  Created eff. graph " << graphsEff[k]->GetName() << std::endl;

     for(int ibin = 0; ibin < hAll->GetNbinsX(); ++ibin){
        int binNumber = ibin + 1;
        double nAllBin = hAll->GetBinContent(binNumber);
        double effBin = hEff->GetBinContent(binNumber)/nAllBin;
        double errBin = sqrt(nAllBin*effBin*(1. - effBin))/nAllBin;

        hEff->SetBinContent(binNumber,effBin);
        hEff->SetBinError(binNumber,errBin);

        std::cout << " Bin " << binNumber << ": " << effBin << " +/- " << errBin << std::endl;
     }

     /*TCanvas* canvas = new TCanvas(("canvas_" + std::string(hAll->GetName())).c_str(),hAll->GetName());
     canvas->cd();
     hEff->Draw();*/
  }
  
  outFile.cd();
  for(size_t k = 0; k < graphsEff.size(); ++k) graphsEff[k]->Write();
  outFile.Write(); 
  outFile.Close();
}
