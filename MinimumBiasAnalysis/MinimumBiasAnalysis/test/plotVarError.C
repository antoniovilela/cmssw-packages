#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"
#include "Utilities/PlottingTools/interface/Plotter.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

TH1F* histoWithVarError(std::string const& variable, std::string const& fileNameRef, std::vector<std::string> const& fileNamesVarError, int rebin);
void plotVarError(std::vector<std::string> const& variables, std::string const& fileNameRef, std::vector<std::string> const& fileNamesVarError, int rebin, int errorBandColor, bool saveHistos, std::string const& outFileName);
void drawVarError(TH1F* histWithError);
void setHistoStyle(TH1F* histWithError, int errorBandColor);

void plotErrorBands(char const* drawOption = "", int rebin = 1){

  std::vector<std::string> variables;
  /*variables.push_back("nVertex");
  variables.push_back("posXPrimVtx");
  variables.push_back("posYPrimVtx");
  variables.push_back("posZPrimVtx");
  variables.push_back("trackMultiplicity");
  variables.push_back("trackMultiplicityAssociatedToPV");
  variables.push_back("multiplicityHFPlus");
  variables.push_back("multiplicityHFMinus");
  variables.push_back("sumEnergyHFPlus");
  variables.push_back("sumEnergyHFMinus");
  variables.push_back("xiPlusFromTowers");
  variables.push_back("xiMinusFromTowers");
  variables.push_back("xiPlusFromPFCands");
  variables.push_back("xiMinusFromPFCands");
  variables.push_back("missingMassFromXiFromPFCands");
  variables.push_back("EPlusPzFromTowers");
  variables.push_back("EMinusPzFromTowers");
  variables.push_back("EPlusPzFromPFCands");
  variables.push_back("EMinusPzFromPFCands");
  variables.push_back("MxFromTowers");
  variables.push_back("MxFromPFCands");*/
  variables.push_back("EPlusPzFromTowersVarBin_dist");
  variables.push_back("EMinusPzFromTowersVarBin_dist");
  variables.push_back("sumEnergyHFPlusVarBin_dist");
  variables.push_back("sumEnergyHFMinusVarBin_dist");
  variables.push_back("multiplicityHFPlusVarBin_dist");
  variables.push_back("multiplicityHFMinusVarBin_dist");  

  std::vector<std::pair<std::string,TDirectory*> > dirs;
  /*dirs.push_back(std::make_pair("HF + HB/HE threshold",TFile::Open("root/900GeV/NoSel/histosErrorBand_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_All.root")));
  dirs.push_back(std::make_pair("HF threshold",TFile::Open("root/900GeV/NoSel/histosErrorBand_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_EnergyThresholdHFVar.root")));
  dirs.push_back(std::make_pair("Stat.",TFile::Open("root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_histos.root")));*/

  /*dirs.push_back(std::make_pair("HCAL tower energy #pm10%",TFile::Open("histosErrorBand_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_EnergyScale.root")));
  dirs.push_back(std::make_pair("Stat.",TFile::Open("analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_ApplyEnergyScaleHCAL_False_histos.root")));*/

  dirs.push_back(std::make_pair("Energy scale #pm10%",TFile::Open("root/2360GeV/NoSel/histosErrorBand_MinimumBias_Run124120_eventSelectionMinBiasBSCOR_EnergyScale.root")));
  dirs.push_back(std::make_pair("Data 2360 GeV",TFile::Open("root/2360GeV/NoSel/analysisMinBiasTTree_MinimumBias_Run124120_eventSelectionMinBiasBSCOR_ApplyEnergyScaleHCAL_False_histos.root")));
  //dirs.push_back(std::make_pair("Energy scale #pm10%",TFile::Open("root/2360GeV/NoSel/histosErrorBand_MinimumBias_Run124120_eventSelectionMinBiasBSCOR_EnergyScale.root"))); 

  Plotter<DefaultNorm> plotter;
  //int colors[] = {kOrange,kYellow,kRed};
  int colors[] = {kYellow,kBlack};
  std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
  //int linestyles[] = {kSolid,kSolid,kSolid};
  int linestyles[] = {kSolid,kSolid};
  std::vector<int> histLineStyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));
  int fillstyles[] = {0,1001};
  std::vector<int> histFillStyles(fillstyles,fillstyles + sizeof(fillstyles)/sizeof(int));
  plotter.SetColors(histColors);
  plotter.SetLineStyles(histLineStyles);
  plotter.SetFillStyles(histFillStyles);
  //plotter.SetRebin(rebin);
  std::vector<double> normFactors(2,1.); 
  plotter.plot(variables,dirs,drawOption);
  //plotter.plotStack(variables,dirs,normFactors,"NOSTACK");

}

void plotErrorBandsAll(){

  std::vector<std::string> variables;
  variables.push_back("EPlusPzFromTowersVarBin_dist");
  variables.push_back("EMinusPzFromTowersVarBin_dist");
  variables.push_back("sumEnergyHFPlusVarBin_dist");
  variables.push_back("sumEnergyHFMinusVarBin_dist");
  variables.push_back("multiplicityHFPlusVarBin_dist");
  variables.push_back("multiplicityHFMinusVarBin_dist");

  TFile* fileData = TFile::Open("root/2360GeV/NoSel/analysisMinBiasTTree_MinimumBias_Run124120_eventSelectionMinBiasBSCOR_ApplyEnergyScaleHCAL_False_histos.root");
  TFile* fileErrorBands = TFile::Open("root/2360GeV/NoSel/histosErrorBand_MinimumBias_Run124120_eventSelectionMinBiasBSCOR_EnergyScale.root");
 
  std::string labelData = "Data 2360 GeV";
  std::string labelErrorBands = "Energy scale #pm10%";
  std::vector<TCanvas*> canvasVec;
  std::vector<TLegend*> legendVec;
     
  std::vector<std::string>::const_iterator var = variables.begin();
  std::vector<std::string>::const_iterator vars_end = variables.end();
  for(; var != vars_end; ++var){
     TH1F* h_var_data_tmp = static_cast<TH1F*>(fileData->Get(var->c_str()));
     TH1F* h_var_err_tmp = static_cast<TH1F*>(fileErrorBands->Get(var->c_str())); 
     std::string hname;
     hname = *var;hname += "_data";
     TH1F* h_var_data = static_cast<TH1F*>(h_var_data_tmp->Clone(hname.c_str()));
     hname = *var;hname += "_error";
     TH1F* h_var_err = static_cast<TH1F*>(h_var_err_tmp->Clone(hname.c_str()));

     h_var_data->SetLineWidth(2);
     h_var_data->SetMarkerStyle(22);
     h_var_data->SetMarkerSize(1.2);

     h_var_err->SetFillColor(kYellow);
     h_var_err->SetFillStyle(1001);
     h_var_err->SetMarkerSize(0);

     TLegend* leg = new TLegend(0.4,0.7,0.95,0.8);
     leg->AddEntry(h_var_data,labelData.c_str(),"LP");
     leg->AddEntry(h_var_err,labelErrorBands.c_str(),"F");
     leg->SetFillColor(0);    
     legendVec.push_back(leg);
     
     std::string canvasName(*var);
     canvasName += "_canvas";
     TCanvas* canvas = new TCanvas(canvasName.c_str(),canvasName.c_str());  
     canvasVec.push_back(canvas);
     canvas->cd();
    
     h_var_err->Draw("E2");
     h_var_data->Draw("EPSAME");
     leg->Draw("SAME"); 
  }

  /*TH1F* h_var1_ref = static_cast<TH1F*>(fileRef->Get("EPlusPzFromTowersVarBin_dist")); 
  TH1F* h_var1_err = static_cast<TH1F*>(fileErrorBands->Get("EPlusPzFromTowersVarBin_dist"));
  TLegend* leg1 = new TLegend(0.4,0.7,0.95,0.8);
  TCanvas* canvas1 = new TCanvas("canvas1","EPlusPzFromTowers");
  canvas1->cd();
  h_var1_err->SetFillColor(kYellow);
  h_var1_err->SetFillStyle(1001);
  //h_var1_err->SetFillStyle(4090);
  h_var1_err->SetMarkerSize(0);

  h_var1_ref->SetLineWidth(2);
  h_var1_ref->SetMarkerStyle(22);
  h_var1_ref->SetMarkerSize(1.2);

  leg1->AddEntry(h_var1_ref,"Data 2360 GeV","LP");
  leg1->AddEntry(h_var1_err,"Energy scale #pm10%","F");
  leg1->SetFillColor(0);
  h_var1_err->Draw("E2");
  h_var1_ref->Draw("EPSAME");
  leg1->Draw("SAME");*/
}

void getFileNames(std::string& fileNameRef, std::vector<std::string>& fileNamesError){
  /*fileNameRef = "root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_histos.root";

  fileNamesError.push_back("root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_HFThresholdIndex_16_EnergyThresholdHF_3_2_EnergyThresholdHBHE_1_5_histos.root");
  fileNamesError.push_back("root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_HFThresholdIndex_17_EnergyThresholdHF_3_4_EnergyThresholdHBHE_1_5_histos.root");
  fileNamesError.push_back("root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_HFThresholdIndex_19_EnergyThresholdHF_3_8_EnergyThresholdHBHE_1_5_histos.root");
  fileNamesError.push_back("root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_HFThresholdIndex_20_EnergyThresholdHF_4_0_EnergyThresholdHBHE_1_5_histos.root");
  fileNamesError.push_back("root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_1_0_histos.root");
  fileNamesError.push_back("root/900GeV/NoSel/analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_HFThresholdIndex_18_EnergyThresholdHF_3_6_EnergyThresholdHBHE_2_0_histos.root");*/

  /*fileNameRef = "analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_ApplyEnergyScaleHCAL_False_histos.root";
  fileNamesError.push_back("analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale09_EnergyScaleFactorHCAL_0_9_histos.root");
  fileNamesError.push_back("analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale11_EnergyScaleFactorHCAL_1_1_histos.root");*/

  fileNameRef = "root/2360GeV/NoSel/analysisMinBiasTTree_MinimumBias_Run124120_eventSelectionMinBiasBSCOR_ApplyEnergyScaleHCAL_False_histos.root";
  fileNamesError.push_back("root/2360GeV/NoSel/analysisMinBiasTTree_MinimumBias_Run124120_eventSelectionMinBiasBSCOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale09_EnergyScaleFactorHCAL_0_9_histos.root");
  fileNamesError.push_back("root/2360GeV/NoSel/analysisMinBiasTTree_MinimumBias_Run124120_eventSelectionMinBiasBSCOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale11_EnergyScaleFactorHCAL_1_1_histos.root");
}


//=====================================================================================

void plotVarError(std::string const& variable, std::string const& fileNameRef, std::string const& fileNameVar, int rebin = 1, int errorBandColor = kYellow){

  plotVarError(std::vector<std::string>(1,variable),fileNameRef,std::vector<std::string>(1,fileNameVar),rebin,errorBandColor,false,"");
}

void plotVarError(std::vector<std::string> const& variables, int rebin = 1, int errorBandColor = kYellow, bool saveHistos = false, std::string const& outFileName = ""){

  std::string fileNameRef;
  std::vector<std::string> fileNamesError;
  getFileNames(fileNameRef,fileNamesError);

  plotVarError(variables,fileNameRef,fileNamesError,rebin,errorBandColor,saveHistos,outFileName);
}

void plotVarError(int rebin = 1, int errorBandColor = kYellow, bool saveHistos = false, std::string const& outFileName = ""){
  std::vector<std::string> variables;
  /*variables.push_back("nVertex");
  variables.push_back("posXPrimVtx");
  variables.push_back("posYPrimVtx");
  variables.push_back("posZPrimVtx");
  variables.push_back("trackMultiplicity");
  variables.push_back("trackMultiplicityAssociatedToPV");
  variables.push_back("multiplicityHFPlus");
  variables.push_back("multiplicityHFMinus");
  variables.push_back("sumEnergyHFPlus");
  variables.push_back("sumEnergyHFMinus");
  variables.push_back("xiPlusFromTowers");
  variables.push_back("xiMinusFromTowers");
  variables.push_back("xiPlusFromPFCands");
  variables.push_back("xiMinusFromPFCands");
  variables.push_back("missingMassFromXiFromPFCands");
  variables.push_back("EPlusPzFromTowers");
  variables.push_back("EMinusPzFromTowers");
  variables.push_back("EPlusPzFromPFCands");
  variables.push_back("EMinusPzFromPFCands");
  variables.push_back("MxFromTowers");
  variables.push_back("MxFromPFCands");
  variables.push_back("sumET");*/
  variables.push_back("EPlusPzFromTowersVarBin_dist");
  variables.push_back("EMinusPzFromTowersVarBin_dist");
  variables.push_back("sumEnergyHFPlusVarBin_dist");
  variables.push_back("sumEnergyHFMinusVarBin_dist");
  variables.push_back("multiplicityHFPlusVarBin_dist");
  variables.push_back("multiplicityHFMinusVarBin_dist");

  plotVarError(variables,rebin,errorBandColor,saveHistos,outFileName);
}

void plotVarError(std::string const& variable,int rebin = 1, int errorBandColor = kYellow){

  plotVarError(std::vector<std::string>(1,variable),rebin,errorBandColor);
}

//=====================================================================================

void plotVarError(std::vector<std::string> const& variables, std::string const& fileNameRef, std::vector<std::string> const& fileNamesVarError, int rebin, int errorBandColor, bool saveHistos, std::string const& outFileName){

  //TH1::SetDefaultSumw2(true);

  std::vector<TH1F*> histosWithError;
  std::vector<std::string>::const_iterator it_var = variables.begin();
  std::vector<std::string>::const_iterator it_vars_end = variables.end();
  for(; it_var != it_vars_end; ++it_var){
     TH1F* histWithError = histoWithVarError(*it_var,fileNameRef,fileNamesVarError,rebin);
     setHistoStyle(histWithError,errorBandColor);
     histosWithError.push_back(histWithError);

     drawVarError(histosWithError.back());
  }

  if(saveHistos){
     TFile* outFile = new TFile(outFileName.c_str(),"recreate");
     outFile->cd();
     for(size_t k = 0; k < histosWithError.size(); ++k) histosWithError[k]->Write();
  }
}

/*void plotVarError(std::string const& variable, std::string const& fileNameRef, std::vector<std::string> const& fileNamesVarError, int rebin, int errorBandColor){
  TH1F* histWithError = histoWithVarError(variable,fileNameRef,fileNamesVarError,rebin);

  drawVarError(histWithError);
}*/

TH1F* histoWithVarError(std::string const& variable, std::string const& fileNameRef, std::vector<std::string> const& fileNamesVarError, int rebin){
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
     char suffix[20]; sprintf(suffix,"_Var_%u",index); 
     std::string histoName = variable + suffix;
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
        double diffBin = fabs(histRef->GetBinContent(binNumber) - (*it_histo)->GetBinContent(binNumber)); 
        //std::cout << histRef->GetBinContent(binNumber) << ", " << (*it_histo)->GetBinContent(binNumber) << ", " << diffBin << std::endl;
        binVars.push_back(diffBin);
     }
     double binErrorVar = *std::max_element(binVars.begin(),binVars.end());
     double originalBinError = histWithError->GetBinError(binNumber);
     //double binError = sqrt(originalBinError*originalBinError + binErrorVar*binErrorVar);
     double binError = binErrorVar; 
     //std::cout << "Bin " << binNumber << " " << originalBinError << ", " << binErrorVar << ", " << binError << std::endl;
     histWithError->SetBinError(binNumber,binError);
  }

  return histWithError;
}

void drawVarError(TH1F* histWithError){
  TCanvas* canvas = new TCanvas((std::string(histWithError->GetName()) + "_canvas").c_str(),histWithError->GetName());
  //setHistoStyle(histWithError,errorBandColor); 
  histWithError->Draw("HISTE2P");
}

void setHistoStyle(TH1F* histWithError, int errorBandColor){
  histWithError->SetFillColor(errorBandColor);
  histWithError->SetMarkerSize(0.9); 
}
