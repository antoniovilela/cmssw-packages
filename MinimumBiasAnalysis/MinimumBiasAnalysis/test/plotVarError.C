#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"

#include <iostream>
#include <string>
#include <vector>

void plotVarError(std::string const& variable, std::string const& fileNameRef, std::string const& fileNameVar, int rebin = 1){

  TFile* fileRef = TFile::Open(fileNameRef.c_str());
  TFile* fileVar = TFile::Open(fileNameVar.c_str());

  TH1F* histRef_tmp = getHisto(fileRef,variable);
  TH1F* histRef = static_cast<TH1F*>(histRef_tmp->Clone((variable + "_Ref").c_str()));

  TH1F* histVar_tmp = getHisto(fileVar,variable);
  TH1F* histVar = static_cast<TH1F*>(histVar_tmp->Clone((variable + "_Var").c_str()));

  histRef->Rebin(rebin);
  histVar->Rebin(rebin);

  TH1F* histWithError = static_cast<TH1F*>(histRef->Clone((variable + "_errorBand").c_str()));
  
  // Loop over bins of histogram
  for(size_t k = 0; k < histRef->GetNbinsX(); ++k){
     int binNumber = k + 1;
     // Requires some criteria
     double binError = fabs(histRef->GetBinContent(binNumber) - histVar->GetBinContent(binNumber));
     histWithError->SetBinError(binNumber,binError);
  }

  TCanvas* canvas = new TCanvas((variable + "_canvas").c_str(),variable.c_str());
  histWithError->SetFillColor(kYellow);
  histWithError->SetMarkerSize(0.9); 
  histWithError->Draw("HISTE2P");
}
