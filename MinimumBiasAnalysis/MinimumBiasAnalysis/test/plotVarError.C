#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

void plotVarError(std::string const& variable, std::string const& fileNameRef, std::vector<std::string> const& fileNamesVarError, int rebin);

void plotVarError(std::string const& variable, std::string const& fileNameRef, std::string const& fileNameVar, int rebin = 1){
  plotVarError(variable,fileNameRef,std::vector<std::string>(1,fileNameVar),rebin);
}

void plotVarError(std::string const& variable,int rebin = 1){

  std::string fileNameRef = "root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasPixel_HFThresholdIndex_17_EnergyThresholdHF_3_4_EnergyThresholdHBHE_1_5_histos.root";
  std::vector<std::string> fileNamesError;
  //fileNamesError.push_back("root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasPixel_HFThresholdIndex_15_EnergyThresholdHF_3_0_EnergyThresholdHBHE_1_5_histos.root");
  fileNamesError.push_back("root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasPixel_HFThresholdIndex_16_EnergyThresholdHF_3_2_EnergyThresholdHBHE_1_5_histos.root");
  fileNamesError.push_back("root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasPixel_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_1_5_histos.root");
  //fileNamesError.push_back("root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasPixel_HFThresholdIndex_19_EnergyThresholdHF_3_8_EnergyThresholdHBHE_1_5_histos.root");
  
  plotVarError(variable,fileNameRef,fileNamesError,rebin);
}

void plotVarError(std::string const& variable, std::string const& fileNameRef, std::vector<std::string> const& fileNamesVarError, int rebin){

  TFile* fileRef = TFile::Open(fileNameRef.c_str());
  TH1F* histRef_tmp = getHisto(fileRef,variable);
  TH1F* histRef = static_cast<TH1F*>(histRef_tmp->Clone((variable + "_Ref").c_str()));
  histRef->Rebin(rebin);

  std::vector<TFile*> filesVarError;
  std::vector<TH1F*> histosVarError;
  size_t index = 0;
  std::vector<std::string>::const_iterator it_fname = fileNamesVarError.begin();
  std::vector<std::string>::const_iterator it_fnames_end = fileNamesVarError.end();
  for(; it_fname != it_fnames_end; ++it_fname,++index){
     filesVarError.push_back(TFile::Open(it_fname->c_str())); 
     TH1F* histVar_tmp = getHisto(filesVarError.back(),variable);
     std::string histoName = variable + "_Var" + "_";
     histoName += index;
     histosVarError.push_back(static_cast<TH1F*>(histVar_tmp->Clone(histoName.c_str())));
     histosVarError.back()->Rebin(rebin);
  }

  TH1F* histWithError = static_cast<TH1F*>(histRef->Clone((variable + "_errorBand").c_str()));
  
  // Loop over bins of histogram
  for(int k = 0; k < histRef->GetNbinsX(); ++k){
     int binNumber = k + 1;
     // Requires some criteria
     std::vector<TH1F*>::const_iterator it_histo = histosVarError.begin();
     std::vector<TH1F*>::const_iterator it_histos_end = histosVarError.end();
     std::vector<double> binVars;
     for(; it_histo != it_histos_end; ++it_histo){
        binVars.push_back(fabs(histRef->GetBinContent(binNumber) - (*it_histo)->GetBinContent(binNumber)));
     }
     double binError = *std::max_element(binVars.begin(),binVars.end());
     histWithError->SetBinError(binNumber,binError);
  }

  TCanvas* canvas = new TCanvas((variable + "_canvas").c_str(),variable.c_str());
  histWithError->SetFillColor(kYellow);
  histWithError->SetMarkerSize(0.9); 
  histWithError->Draw("HISTE2P");
}
