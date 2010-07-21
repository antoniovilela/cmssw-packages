#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPaveLabel.h"
#include "TLatex.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"
#include "Utilities/PlottingTools/interface/Plotter.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

TH1F* histoWithVarError(std::string const& variable, std::string const& fileNameRef, std::vector<std::string> const& fileNamesVarError, int rebin);
void plotVarError(std::vector<std::string> const& variables, std::string const& fileNameRef, std::vector<std::string> const& fileNamesVarError, int rebin, int errorBandColor, bool saveHistos, std::string const& outFileName);
void drawVarError(TH1F* histWithError);
void setHistoStyle(TH1F* histWithError, int errorBandColor);

void getFileNames(std::string& fileNameRef, std::vector<std::string>& fileNamesError){
  fileNameRef = "root/7TeV/NoSel/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_False_histos.root";
  fileNamesError.push_back("root/7TeV/NoSel/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale090_EnergyScaleFactorHCAL_0_9_histos.root");
  fileNamesError.push_back("root/7TeV/NoSel/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale092_EnergyScaleFactorHCAL_0_92_histos.root");
  fileNamesError.push_back("root/7TeV/NoSel/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale095_EnergyScaleFactorHCAL_0_95_histos.root");
  fileNamesError.push_back("root/7TeV/NoSel/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale098_EnergyScaleFactorHCAL_0_98_histos.root");
  fileNamesError.push_back("root/7TeV/NoSel/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale102_EnergyScaleFactorHCAL_1_02_histos.root");
  fileNamesError.push_back("root/7TeV/NoSel/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale105_EnergyScaleFactorHCAL_1_05_histos.root");
  fileNamesError.push_back("root/7TeV/NoSel/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale108_EnergyScaleFactorHCAL_1_08_histos.root");
  fileNamesError.push_back("root/7TeV/NoSel/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_True_HFTowerSummaryTag_hfTowerScale110_EnergyScaleFactorHCAL_1_1_histos.root"); 
}

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
  variables.push_back("EPlusPzFromTowersVarBin_dist_newBinning");
  variables.push_back("EMinusPzFromTowersVarBin_dist_newBinning");
  variables.push_back("sumEnergyHFPlusVarBin_dist_newBinning");
  variables.push_back("sumEnergyHFMinusVarBin_dist_newBinning");
  variables.push_back("multiplicityHFPlusVarBin_dist");
  variables.push_back("multiplicityHFMinusVarBin_dist");
  std::vector<std::string> titlesY(variables.size());
  titlesY[0] = "dN/d(#sumE+pz) (GeV^{-1})";
  titlesY[1] = "dN/d(#sumE-pz) (GeV^{-1})";
  titlesY[2] = "dN/dE_{HF} (GeV^{-1})";
  titlesY[3] = "dN/dE_{HF} (GeV^{-1})";
  titlesY[4] = "dN/dN_{HF}";
  titlesY[5] = "dN/dN_{HF}";

  std::vector<std::string> titlesX(variables.size());
  titlesX[0] = "#sum(E+pz) (GeV)"; 
  titlesX[1] = "#sum(E-pz) (GeV)";
  titlesX[2] = "#sum E (HF-plus) (GeV)";
  titlesX[3] = "#sum E (HF-minus) (GeV)";
  titlesX[4] = "tower multiplicity HF-plus";
  titlesX[5] = "tower multiplicity HF-minus";

  //std::string rootDir = "root/2360GeV/NoSel";
  //std::string rootDir = "root/2360GeV/SumEnergyMaxHFPlus_8_0";
  //std::string rootDir = "root/900GeV/NoSel";
  //std::string rootDir = "root/900GeV/SumEnergyMaxHFPlus_8_0";
  //std::string rootDir = "root/900GeV/SumEnergyMaxHFMinus_8_0";
  std::string rootDir = "root/7TeV/NoSel"; 
  
  std::string fileNameData = rootDir + "/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOR_ApplyEnergyScaleHCAL_False_histos.root";
  TFile* fileData = TFile::Open(fileNameData.c_str());
  std::string fileNameErrorBands = rootDir + "/histosErrorBand_MinimumBias_7TeV_eventSelectionBscMinBiasOR.root";
  TFile* fileErrorBands = TFile::Open(fileNameErrorBands.c_str());
  std::vector<TFile*> filesMC;
  std::vector<TFile*> filesMCComponent;
  std::string fileNameMC = rootDir + "/";
  std::string fileNameMCComponent = rootDir + "/";
  //filesMC.push_back(TFile::Open(fileNameMC.c_str()));
  //filesMCComponent.push_back(TFile::Open(fileNameMCComponent.c_str()));
  
  std::cout << "Using: " << std::endl
            << fileData->GetName() << std::endl
            << fileErrorBands->GetName() << std::endl;
  for(size_t ifile = 0; ifile < filesMC.size(); ++ifile) std::cout << filesMC[ifile]->GetName() << std::endl;
  for(size_t ifile = 0; ifile < filesMCComponent.size(); ++ifile) std::cout << filesMCComponent[ifile]->GetName() << std::endl; 

  TH1F* h_EventSelectionData = static_cast<TH1F*>(fileData->Get("EventSelection"));
  double nEventsDataFullSel = h_EventSelectionData->GetBinContent(11);
  std::cout << "N events in data= " << nEventsDataFullSel << std::endl;

  //std::string labelData = "p+p (2.36 TeV) BSC OR and Vertex";
  //std::string labelData = "p+p (0.9 TeV) BSC OR and Vertex";
  std::string labelData = "p+p (7 TeV) BSC OR and Vertex";
  std::string labelCMSPrel = "CMS Preliminary 2010";
  std::string labelLum = "Run 132440 L=3.5 #mub^{-1}";  

  std::string labelErrorBands = "Energy scale #pm10%";
  std::vector<std::string> labelsMC(filesMC.size());
  //labelsMC[0] = "PYTHIA 8";
  std::vector<std::string> labelsMCComponent(filesMCComponent.size());
  //labelsMCComponent[0] = "PYTHIA Non-diffractive";

  std::vector<int> histColors(filesMC.size(),1);
  //histColors[0] = 2;
  std::vector<int> histColorsComponent(filesMCComponent.size(),1);
  //histColorsComponent[0] = 43;

  std::vector<int> histLineStyles(filesMC.size(),1);
  //histLineStyles[0] = 1;
  std::vector<int> histLineStylesComponent(filesMCComponent.size(),1);
  //histLineStylesComponent[0] = 9;

  std::vector<TCanvas*> canvasVec;
  std::vector<TLegend*> legendVec;
     
  std::vector<std::string>::const_iterator var = variables.begin();
  std::vector<std::string>::const_iterator vars_end = variables.end();
  for(size_t index = 0; var != vars_end; ++var,++index){
     TH1F* h_var_data_tmp = static_cast<TH1F*>(fileData->Get(var->c_str()));
     if(!h_var_data_tmp){
      std::cout << "ERROR: Could not find " << *var << " in " << fileData->GetName() << std::endl;
      return;
     }
     TH1F* h_var_err_tmp = static_cast<TH1F*>(fileErrorBands->Get(var->c_str())); 
     if(!h_var_err_tmp){
      std::cout << "ERROR: Could not find " << *var << " in " << fileErrorBands->GetName() << std::endl;
      return;
     }
     std::string hname;
     hname = *var;hname += "_data";
     TH1F* h_var_data = static_cast<TH1F*>(h_var_data_tmp->Clone(hname.c_str()));
     hname = *var;hname += "_error";
     TH1F* h_var_err = static_cast<TH1F*>(h_var_err_tmp->Clone(hname.c_str()));

     h_var_data->SetLineWidth(3);
     h_var_data->SetMarkerStyle(20);
     h_var_data->SetMarkerSize(1.5);
     h_var_data->SetStats(0);

     h_var_err->SetFillColor(5);
     h_var_err->SetFillStyle(1001);
     h_var_err->SetMarkerSize(0);
     h_var_err->SetXTitle(titlesX[index].c_str());
     h_var_err->SetYTitle(titlesY[index].c_str());
     h_var_err->SetTitleSize(0.05,"X");
     h_var_err->SetTitleSize(0.05,"Y");
     h_var_err->SetTitleOffset(1.18,"X");
     h_var_err->SetTitleOffset(1.35,"Y");
     h_var_err->SetStats(0);
 
     TLegend* leg = new TLegend(0.45,0.55,0.95,0.80);
     leg->AddEntry(h_var_data,labelData.c_str(),"LP");
     leg->AddEntry(h_var_err,labelErrorBands.c_str(),"F");
 
     std::vector<TH1F*> histosMC;
     std::vector<TFile*>::const_iterator fileMC = filesMC.begin();
     std::vector<TFile*>::const_iterator filesMC_end = filesMC.end();
     std::vector<double> scaleFactors;
     for(size_t idxMC = 0; fileMC != filesMC_end; ++fileMC,++idxMC){
        TH1F* h_var_mc_tmp = static_cast<TH1F*>((*fileMC)->Get(var->c_str()));
        if(!h_var_mc_tmp){
           std::cout << "ERROR: Could not find " << *var << " in " << (*fileMC)->GetName() << std::endl;
           return;
        }
        std::stringstream hnameMC; hnameMC << *var << "_mc_" << idxMC; 
        std::cout << hnameMC.str() << std::endl;
        histosMC.push_back(static_cast<TH1F*>(h_var_mc_tmp->Clone(hnameMC.str().c_str())));
        TH1F* h_EventSelectionMC = static_cast<TH1F*>((*fileMC)->Get("EventSelection"));
        double nEventsMCFullSel = h_EventSelectionMC->GetBinContent(11);
        scaleFactors.push_back(nEventsDataFullSel/nEventsMCFullSel); 
        std::cout << "Scaling MC by " << scaleFactors.back() << std::endl;
        histosMC.back()->Scale(scaleFactors.back());
        histosMC.back()->SetLineWidth(3);
        histosMC.back()->SetLineColor(histColors[idxMC]);
        histosMC.back()->SetLineStyle(histLineStyles[idxMC]);
        histosMC.back()->SetStats(0); 

        leg->AddEntry(histosMC.back(),labelsMC[idxMC].c_str(),"LF");
     }
     // Now components
     std::vector<TH1F*> histosMCComponent;
     fileMC = filesMCComponent.begin();
     filesMC_end = filesMCComponent.end();
     for(size_t idxMC = 0; fileMC != filesMC_end; ++fileMC,++idxMC){
        TH1F* h_var_mc_tmp = static_cast<TH1F*>((*fileMC)->Get(var->c_str()));
        if(!h_var_mc_tmp){ 
           std::cout << "ERROR: Could not find " << *var << " in " << (*fileMC)->GetName() << std::endl;
           return;
        }
        std::stringstream hnameMC; hnameMC << *var << "_mc_comp_" << idxMC;
        std::cout << hnameMC.str() << std::endl;
        histosMCComponent.push_back(static_cast<TH1F*>(h_var_mc_tmp->Clone(hnameMC.str().c_str())));
        std::cout << "Scaling MC by " << scaleFactors[idxMC] << std::endl;
        histosMCComponent.back()->Scale(scaleFactors[idxMC]);
        histosMCComponent.back()->SetLineWidth(3);
        histosMCComponent.back()->SetLineColor(histColorsComponent[idxMC]);
        histosMCComponent.back()->SetLineStyle(histLineStylesComponent[idxMC]);
        histosMCComponent.back()->SetStats(0);

        leg->AddEntry(histosMCComponent.back(),labelsMCComponent[idxMC].c_str(),"LF");
     }

     leg->SetFillColor(0);
     legendVec.push_back(leg);

     std::string canvasName(*var);
     canvasName += "_canvas";
     TCanvas* canvas = new TCanvas(canvasName.c_str(),canvasName.c_str());  
     canvasVec.push_back(canvas);
     canvas->cd();
     //canvas->SetLogx();
    
     h_var_err->Draw("E2");
     //h_var_err->GetXaxis()->SetLimits(1.,h_var_err->GetXaxis()->GetXmax());
     h_var_data->Draw("EPSAME");
     std::vector<TH1F*>::const_iterator histo = histosMC.begin();
     std::vector<TH1F*>::const_iterator histosMC_end = histosMC.end();
     for(; histo != histosMC_end; ++histo) (*histo)->Draw("HIST][SAME");
     histo = histosMCComponent.begin();
     histosMC_end = histosMCComponent.end();
     for(; histo != histosMC_end; ++histo) (*histo)->Draw("HIST][SAME");
     leg->Draw("SAME"); 

     TPaveLabel* paveLabel = new TPaveLabel(0.45,0.80,0.80,0.90,labelCMSPrel.c_str(),"NDC");
     paveLabel->SetFillColor(0);
     paveLabel->Draw("SAME");

     TLatex* latexLum = new TLatex(0.80,0.85,labelLum.c_str());
     //TLatex* latexLum = new TLatex(0.45,0.75,labelLum.c_str());
     latexLum->SetNDC();
     latexLum->SetTextAlign(12);
     latexLum->SetTextSize(0.04);
     latexLum->Draw("SAME");
     
  }
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

  // Get number of events passing cuts
  int iBinSelection = 11;
  TH1F* hEventSelectionRef = getHisto(fileRef,"EventSelection"); 
  double nEventsRef = hEventSelectionRef->GetBinContent(iBinSelection);
  std::cout << "=====================================" << std::endl;
  std::cout << "Number of events in reference sample= " << nEventsRef << std::endl; 
 
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
    
     // Scale histogram to number of events in reference set
     TH1F* hEventSelectionVarError = getHisto(filesVarError.back(),"EventSelection");
     double nEventsVarError = hEventSelectionVarError->GetBinContent(iBinSelection); 
     double scaleVarError = nEventsRef/nEventsVarError;
     std::cout << "Scaling histogram " << histosVarError.back()->GetName() << " by " << scaleVarError << std::endl; 
     histosVarError.back()->Scale(scaleVarError); 
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
