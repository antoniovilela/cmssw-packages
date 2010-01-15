#include "TFractionFitter.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"

#include <iostream>
#include <string>
#include <vector>

void fitFraction(const char* fitVariable = "multiplicityHFPlus", int rebin = 1){

  // Data 
  //TFile* fileData = TFile::Open("root/analysisMinBiasFWLite_histos_MinimumBias_Dec19thReReco-Runs124020-124030_new_highQualityTracks.root");
  TFile* fileData = TFile::Open("root/analysisMinBiasFWLite_histos_MinimumBias_Dec19thReReco-Run124120_new_highQualityTracks.root");
  // Use MC for closure test
  //TFile* fileData = TFile::Open("root/analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
  //TFile* fileData = TFile::Open("root/analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks.root"); 
  //TFile* fileData = TFile::Open("root/analysisMinBiasFWLite_histos_PHOJET_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
  TH1F* h_EventSelection_Data = static_cast<TH1F*>(fileData->Get("EventSelection"));
  TH1F* h_fitVar_Data_tmp = static_cast<TH1F*>(fileData->Get(fitVariable));
  TH1F* h_fitVar_Data = static_cast<TH1F*>(h_fitVar_Data_tmp->Clone((std::string(fitVariable) + "_Data").c_str()));

  double sigmaTot = 52000.;//microbarn
  //double sigmaTot = 60000.;//microbarn

  // Components
  std::vector<std::string> labels;
  labels.push_back("Diffractive");
  labels.push_back("Non-diffractive");
  /*labels.push_back("Single-diffractive");
  labels.push_back("Double-diffractive");
  labels.push_back("Non-diffractive");*/

  std::vector<TFile*> files;
  /*files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_Diff.root"));
  files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_Inelastic.root"));*/
  files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_Diff.root"));
  files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_Inelastic.root"));
  /*files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PHOJET_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_Diff.root"));
  files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PHOJET_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_Inelastic.root"));*/
  /*files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PHOJET_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_Diff.root"));
  files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PHOJET_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_Inelastic.root"));*/
  /*files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PHOJET_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_Diff.root"));
  files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_Inelastic.root"));*/

  /*files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_SD.root"));
  files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_DD.root")); 
  files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks_Inelastic.root"));*/
  /*files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_SD.root"));
  files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_DD.root"));
  files.push_back(TFile::Open("root/analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8L_2360GeV_new_highQualityTracks_Inelastic.root"));*/

  // Fraction fitter
  // MC histograms are put in this array
  TObjArray *mc = new TObjArray(files.size());

  // Rebin
  h_fitVar_Data->Rebin(rebin);

  std::vector<TH1F*> histosEventSelection(files.size());
  std::vector<TH1F*> histosFitVariable(files.size());   
  for(size_t k = 0; k < files.size(); ++k){
     histosEventSelection[k] = static_cast<TH1F*>(files[k]->Get("EventSelection"));
     TH1F* h_fitVar = static_cast<TH1F*>(files[k]->Get(fitVariable));
     std::string hname = fitVariable;
     hname += "_";
     hname += k; 
     histosFitVariable[k] = static_cast<TH1F*>(h_fitVar->Clone(hname.c_str()));
     histosFitVariable[k]->Rebin(rebin);
     mc->Add(histosFitVariable[k]);
  }
  TFractionFitter* fit = new TFractionFitter(h_fitVar_Data, mc);

  // Constrain fraction 1 to be between 0 and 1
  for(size_t k = 0; k < histosFitVariable.size(); ++k){
     fit->Constrain(k,0.,1.);
     //fit->SetRangeX(1,);
  }

  // Perform the fit
  Int_t status = fit->Fit();
  std::cout << "Fit status: " << status << std::endl;
  // Check on fit status
  if (status != 0) {std::cout << ">>>>>>>>>>> Problem in fit...stopping" << endl;return;}

  std::vector<std::pair<double,double> > fitResult(histosFitVariable.size()); 
  std::cout << ">>>>> Fit result <<<<<" << std::endl
            << "Chi2/ndf = " << fit->GetChisquare() << "/" << fit->GetNDF() << std::endl;
  for(size_t k = 0; k < fitResult.size(); ++k){
     Double_t frac,err; 
     fit->GetResult(k,frac,err);
     fitResult[k] = std::make_pair(frac,err);
     std::cout << "Frac " << labels[k] << " = " << frac << " +/- " << err << std::endl;
  }
  
  TH1F* result = static_cast<TH1F*>(fit->GetPlot());
  TCanvas* canvas1 = new TCanvas("canvas1","Fit result");
  canvas1->cd();
  result->Draw("HIST");
  h_fitVar_Data->SetMarkerSize(0.9);
  h_fitVar_Data->Draw("same");

  int colors[] = {kBlue,kMagenta,kRed};
  std::vector<int> vec_colors(colors,colors + sizeof(colors)/sizeof(int));
  int linestyles[] = {kDashed,kDotted,kDashDotted};
  std::vector<int> vec_linestyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));

  TCanvas* canvas2 = new TCanvas("canvas2","Fit components");
  canvas2->cd();
  result->Draw("HIST");
  for(size_t k = 0; k < histosFitVariable.size(); ++k){
     histosFitVariable[k]->SetLineColor(vec_colors[k % vec_colors.size()]);
     histosFitVariable[k]->SetLineStyle(vec_linestyles[k % vec_linestyles.size()]);
     double frac = fitResult[k].first;
     histosFitVariable[k]->DrawNormalized("HISTSAME",frac*h_fitVar_Data->Integral());
  } 

  double nEventsInRangeNorm = 0.;
  for(size_t k = 0; k < histosFitVariable.size(); ++k){
     double nEvents = histosEventSelection[k]->GetBinContent(1);
     nEventsInRangeNorm += histosFitVariable[k]->Integral()*(1./nEvents);
  }

  std::cout << ">>>>> MC fractions <<<<<" << std::endl;
  for(size_t k = 0; k < histosFitVariable.size(); ++k){
     double nEvents = histosEventSelection[k]->GetBinContent(1);
     double fracInRangeMC = histosFitVariable[k]->Integral()*(1./nEvents)/nEventsInRangeNorm;
     std::cout << "Frac " << labels[k] << " (MC) = " << fracInRangeMC << std::endl;
  }

  double nEventsCorrTotal = 0.;
  for(size_t k = 0; k < histosFitVariable.size(); ++k){
     double effMC = histosFitVariable[k]->Integral()/histosEventSelection[k]->GetBinContent(2);
     double frac = fitResult[k].first; 
     double nEventsCorr = frac*h_fitVar_Data->Integral()/effMC;
     nEventsCorrTotal += nEventsCorr; 
  }
  std::cout << "Number of events = " << nEventsCorrTotal << std::endl;
  std::cout << "Estimated lumi in data sample = " << nEventsCorrTotal/sigmaTot << "/mub" << std::endl;
  for(size_t k = 0; k < histosFitVariable.size(); ++k){
     double effMC = histosFitVariable[k]->Integral()/histosEventSelection[k]->GetBinContent(2);
     double frac = fitResult[k].first;
     double err = fitResult[k].second;
     double nEventsCorr = frac*h_fitVar_Data->Integral()/effMC;
  
     double frac_corr = nEventsCorr/nEventsCorrTotal;
     double err_corr = frac_corr*(err/frac);

     double frac_MC = histosEventSelection[k]->GetBinContent(2)/histosEventSelection[k]->GetBinContent(1);
     std::cout << "Frac " << labels[k] << " (MC) = " << frac_MC << std::endl;
     std::cout << "Frac " << labels[k] << "      = " << frac_corr << " +/- " << err_corr << std::endl;
  }
}
