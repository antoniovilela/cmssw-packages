#include "TFractionFitter.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"

#include <iostream>

void fitFraction(const char* fitVariable = "multiplicityHFPlus", int rebin = 1){

  // Data 
  TFile* fileData = TFile::Open("root/analysisMinBiasFWLite_histos_MinimumBias_Dec19thReReco-Runs124020-124030_new_highQualityTracks.root");
  //TFile* fileData = TFile::Open("root/analysisMinBiasFWLite_histos_MinimumBias_Dec19thReReco-Run124120_new_highQualityTracks.root");
  // Use MC for closure test
  //TFile* fileData = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
  //TFile* fileData = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks.root"); 
  //TFile* fileData = TFile::Open("analysisMinBiasFWLite_histos_PHOJET_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
  TH1F* h_EventSelection_Data = static_cast<TH1F*>(fileData->Get("EventSelection"));
  TH1F* h_fitVar_Data_tmp = static_cast<TH1F*>(fileData->Get(fitVariable));
  TH1F* h_fitVar_Data = static_cast<TH1F*>(h_fitVar_Data_tmp->Clone((std::string(fitVariable) + "_Data").c_str()));

  // Components
  /*TFile* fileMC_all = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
  //TFile* fileMC_all = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks.root"); 
  //TFile* fileMC_all = TFile::Open("analysisMinBiasFWLite_histos_PHOJET_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root"); 
  TH1F* h_EventSelection_all = static_cast<TH1F*>(fileMC_all->Get("EventSelection"));*/

  //TFile* fileMC_sig = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_SD.root");
  //TFile* fileMC_sig = TFile::Open("analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_SD_DD.root");
  //TFile* fileMC_sig = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_Diff.root"); 
  //TFile* fileMC_sig = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_SD.root");
  TFile* fileMC_sig = TFile::Open("analysisMinBiasFWLite_histos_PHOJET_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_SD.root");
  TH1F* h_EventSelection_sig = static_cast<TH1F*>(fileMC_sig->Get("EventSelection"));
  TH1F* h_fitVar_sig_original = static_cast<TH1F*>(fileMC_sig->Get(fitVariable));
  TH1F* h_fitVar_sig = static_cast<TH1F*>(h_fitVar_sig_original->Clone((std::string(fitVariable) + "_sig").c_str()));

  //TFile* fileMC_back = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_NonSD.root");
  //TFile* fileMC_back = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_QCD.root");
  //TFile* fileMC_back = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_Inelastic.root");
  //TFile* fileMC_back = TFile::Open("root/analysisMinBiasFWLite_histos_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_NonSD.root");
  TFile* fileMC_back = TFile::Open("analysisMinBiasFWLite_histos_PHOJET_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_NonSD.root");
  TH1F* h_EventSelection_back = static_cast<TH1F*>(fileMC_back->Get("EventSelection"));
  TH1F* h_fitVar_back_original = static_cast<TH1F*>(fileMC_back->Get(fitVariable));
  TH1F* h_fitVar_back = static_cast<TH1F*>(h_fitVar_back_original->Clone((std::string(fitVariable) + "_back").c_str()));

  // Rebin
  h_fitVar_Data->Rebin(rebin);
  h_fitVar_sig->Rebin(rebin);
  h_fitVar_back->Rebin(rebin);  

  // Fraction fitter
  // MC histograms are put in this array
  TObjArray *mc = new TObjArray(2);
  mc->Add(h_fitVar_sig);
  mc->Add(h_fitVar_back);
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
  h_fitVar_sig->SetLineColor(kBlue);
  h_fitVar_sig->SetLineStyle(kDashed);

  h_fitVar_back->SetLineColor(kRed); 
  h_fitVar_back->SetLineStyle(kDotted);

  result->Draw("HIST");
  h_fitVar_sig->DrawNormalized("HISTSAME",frac_sig*h_fitVar_Data->Integral());
  h_fitVar_back->DrawNormalized("HISTSAME",frac_back*h_fitVar_Data->Integral());
  
  std::cout << ">>>>> Fit result <<<<<" << std::endl;
  std::cout << "Frac sig = " << frac_sig << " +/- " << err_sig << std::endl;
  std::cout << "Frac back = " << frac_back << " +/- " << err_back << std::endl;

  // MC efficiencies
  /*double effMCPreSel_all = h_EventSelection_all->GetBinContent(7)/h_EventSelection_all->GetBinContent(2);
  double effMC_sig = h_EventSelection_sig->GetBinContent(11)/h_EventSelection_sig->GetBinContent(2);

  double nEventsPreSel_Data = h_EventSelection_Data->GetBinContent(7);
  double nEventsRun_Data = nEventsPreSel_Data/effMCPreSel_all;
  double nEventsCorr_sig = frac_sig*h_fitVar_Data->Integral()/effMC_sig;
  
  double frac_diff = nEventsCorr_sig/nEventsRun_Data;
  double err_diff = frac_diff*(err_sig/frac_sig);*/

  double effMC_sig = h_fitVar_sig_original->Integral()/h_EventSelection_sig->GetBinContent(2);
  double effMC_back = h_fitVar_back_original->Integral()/h_EventSelection_back->GetBinContent(2);
  double nEventsCorr_sig = frac_sig*h_fitVar_Data->Integral()/effMC_sig;
  double nEventsCorr_back = frac_back*h_fitVar_Data->Integral()/effMC_back; 

  double frac_diff = nEventsCorr_sig/(nEventsCorr_sig + nEventsCorr_back);
  double err_diff = frac_diff*(err_sig/frac_sig);

  double frac_diff_MC = h_EventSelection_sig->GetBinContent(2)/h_EventSelection_sig->GetBinContent(1); 

  std::cout << "Number of events = " << (nEventsCorr_sig + nEventsCorr_back) << std::endl;
  std::cout << "Frac diff (MC)   = " << frac_diff_MC << std::endl;
  std::cout << "Frac diff        = " << frac_diff << " +/- " << err_diff << std::endl;
}
