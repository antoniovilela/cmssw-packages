#include "TFractionFitter.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"

#include <iostream>

void fitFraction(const char* fitVariable = "multiplicityHFPlus", int rebin = 1){

  // Data 
  TFile* fileData = TFile::Open("analysisMinBiasFWLite_histos_MinimumBias_Dec19thReReco-Runs124020-124030_new_highQualityTracks.root");
  TH1F* h_EventSelection_Data = static_cast<TH1F*>(fileData->Get("EventSelection"));
  TH1F* h_fitVar_Data_tmp = static_cast<TH1F*>(fileData->Get(fitVariable));
  TH1F* h_fitVar_Data = static_cast<TH1F*>(h_fitVar_Data_tmp->Clone((std::string(fitVariable) + "_Data").c_str()));

  // Components
  TFile* fileMC_Diff = TFile::Open("analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_SD_DD.root");
  TH1F* h_EventSelection_Diff = static_cast<TH1F*>(fileMC_Diff->Get("EventSelection"));
  TH1F* h_fitVar_Diff_tmp = static_cast<TH1F*>(fileMC_Diff->Get(fitVariable));
  TH1F* h_fitVar_Diff = static_cast<TH1F*>(h_fitVar_Diff_tmp->Clone((std::string(fitVariable) + "_Diff").c_str()));

  TFile* fileMC_QCD = TFile::Open("analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_QCD.root");
  TH1F* h_EventSelection_QCD = static_cast<TH1F*>(fileMC_QCD->Get("EventSelection"));
  TH1F* h_fitVar_QCD_tmp = static_cast<TH1F*>(fileMC_QCD->Get(fitVariable));
  TH1F* h_fitVar_QCD = static_cast<TH1F*>(h_fitVar_QCD_tmp->Clone((std::string(fitVariable) + "_QCD").c_str()));

  // Rebin
  h_fitVar_Data->Rebin(rebin);
  h_fitVar_Diff->Rebin(rebin);
  h_fitVar_QCD->Rebin(rebin);  

  // Fraction fitter
  // MC histograms are put in this array
  TObjArray *mc = new TObjArray(2);
  mc->Add(h_fitVar_Diff);
  mc->Add(h_fitVar_QCD);
  TFractionFitter* fit = new TFractionFitter(h_fitVar_Data, mc);
  // Constrain fraction 1 to be between 0 and 1
  fit->Constrain(1,0.0,1.0);
  //fit->SetRangeX(1,);

  // Perform the fit
  Int_t status = fit->Fit();
  std::cout << "Fit status: " << status << std::endl;
  // Check on fit status
  if (status != 0) {std::cout << ">>>>>>>>>>> Problem in fit...stopping" << endl;return;}

  TH1F* result = static_cast<TH1F*>(fit->GetPlot());
  Double_t frac_sig,err_sig; 
  fit->GetResult(0, frac_sig, err_sig);
  Double_t frac_back,err_back;
  fit->GetResult(1, frac_back, err_back);

  TCanvas* canvas1 = new TCanvas("canvas1","Fit result");
  //result->Draw("Ep");
  result->Draw("HIST");
  h_fitVar_Data->SetMarkerSize(0.9);
  h_fitVar_Data->Draw("same");

  TCanvas* canvas2 = new TCanvas("canvas2","Fit components");
  h_fitVar_Diff->SetLineColor(kBlue);
  h_fitVar_Diff->SetLineStyle(kDashed);

  h_fitVar_QCD->SetLineColor(kRed); 
  h_fitVar_QCD->SetLineStyle(kDotted);

  result->Draw("HIST");
  h_fitVar_Diff->DrawNormalized("HISTSAME",frac_sig*h_fitVar_Data->Integral());
  h_fitVar_QCD->DrawNormalized("HISTSAME",frac_back*h_fitVar_Data->Integral());
  
  std::cout << ">>>>> Fit result <<<<<" << std::endl;
  std::cout << "Frac sig = " << frac_sig << " +/- " << err_sig << std::endl;
  std::cout << "Frac back = " << frac_back << " +/- " << err_back << std::endl;

  // MC efficiencies
  double effMC_Diff = h_EventSelection_Diff->GetBinContent(11)/h_EventSelection_Diff->GetBinContent(2);
  double nEventsRun_Data = h_EventSelection_Data->GetBinContent(2);
  double nEventsCorr_Diff = frac_sig*h_fitVar_Data->Integral()/effMC_Diff;

  double frac_diff = nEventsCorr_Diff/nEventsRun_Data;
  double err_diff = frac_diff*(err_sig/frac_sig);

  double frac_diff_MC = h_EventSelection_Diff->GetBinContent(2)/h_EventSelection_Diff->GetBinContent(1); 

  std::cout << "Number of events= " << nEventsRun_Data << std::endl;
  std::cout << "Frac diff (SD+DD) MC= " << frac_diff_MC << std::endl;
  std::cout << "Frac diff (SD+DD)= " << frac_diff << " +/- " << err_diff << std::endl;
}
