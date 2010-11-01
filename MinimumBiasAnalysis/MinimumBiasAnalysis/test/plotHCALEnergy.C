#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TF1.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"
#include "Utilities/PlottingTools/interface/Plotter.h"

#include <iostream>
#include <vector>
#include <string>

void plot(const std::string& fileName, const std::string& dirNameColl, const std::string& dirNameNoColl){
    TFile* file = TFile::Open(fileName.c_str());

    std::string labelColl = "p+p (7 TeV) BSC OR and Vertex";
    std::string labelNoColl = "p+p (7 TeV) Unpaired";

    std::vector<std::string> variables;
    variables.push_back("emEnergyEB");
    variables.push_back("emEnergyEE");
    variables.push_back("hadEnergyHB");
    variables.push_back("hadEnergyHE");
    variables.push_back("energyHF");

    /*int indexes[] = {4};
    std::vector<int> vec_indexes(indexes,indexes + sizeof(indexes)/sizeof(int));
    for(std::vector<int>::const_iterator idx = vec_indexes.begin(); idx != vec_indexes.end(); ++idx){
       char aux[20];
       sprintf(aux,"nHFplus_%d",*idx);
       variables.push_back(aux);
       sprintf(aux,"nHFminus_%d",*idx);
       variables.push_back(aux);
       sprintf(aux,"sumEHFplus_%d",*idx);
       variables.push_back(aux);
       sprintf(aux,"sumEHFminus_%d",*idx);
       variables.push_back(aux); 
    }*/

    std::vector<std::pair<std::string,TDirectory*> > dirs;
    dirs.push_back(std::make_pair(labelColl,file->GetDirectory(dirNameColl.c_str())));
    dirs.push_back(std::make_pair(labelNoColl,file->GetDirectory(dirNameNoColl.c_str())));

    TH1F* h_NEventsColl = static_cast<TH1F*>(file->Get((std::string(dirNameColl) + "/NEvents").c_str()));
    TH1F* h_NEventsNoColl = static_cast<TH1F*>(file->Get((std::string(dirNameNoColl) + "/NEvents").c_str()));

    //Plotter<NumberEntriesNorm> plotter;
    Plotter<DefaultNorm> plotter;
    int colors[] = {kBlack,kRed};
    std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
    int linestyles[] = {kSolid,kSolid};
    std::vector<int> histLineStyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));
    int markerstyles[] = {20,24};
    std::vector<int> histMarkerStyles(markerstyles,markerstyles + sizeof(markerstyles)/sizeof(int));
    plotter.SetColors(histColors);
    plotter.SetLineStyles(histLineStyles);
    plotter.SetFillColors(std::vector<int>(1,0));
    plotter.SetFillStyles(std::vector<int>(1,0));
    plotter.SetMarkerColors(histColors);
    plotter.SetMarkerStyles(histMarkerStyles);
    plotter.SetMarkerSizes(std::vector<float>(1,1.5));
    //plotter.SetRebin(rebin);

    std::vector<double> normFactors;
    normFactors.push_back(1./h_NEventsColl->GetBinContent(1));
    normFactors.push_back(1./h_NEventsNoColl->GetBinContent(1));
    plotter.plot(variables,dirs,normFactors);

    // HB
    float fitMinHB = 0.7;
    float fitMaxHB = 4.0;
    TCanvas* canvasHB = new TCanvas("canvasHB","Had Energy HB");
    canvasHB->cd();
    TH1F* h_hadEnergyHB_tmp = static_cast<TH1F*>(file->Get((std::string(dirNameNoColl) + "/hadEnergyHB").c_str()));
    TH1F* h_hadEnergyHB = static_cast<TH1F*>(h_hadEnergyHB_tmp->Clone("hadEnergyHB_fit"));
    h_hadEnergyHB->Scale(1./h_NEventsNoColl->GetBinContent(1));
    TF1* fitHB = new TF1("fitHB","gaus",fitMinHB,fitMaxHB);

    h_hadEnergyHB->Fit(fitHB,"R");
    double par1 = fitHB->GetParameter(1);
    double err1 = fitHB->GetParError(1);
    double par2 = fitHB->GetParameter(2);
    double err2 = fitHB->GetParError(2);

    std::cout << "Fit parameters HB: " << par1 << "+/-" << err1 << "   "
                                       << par2 << "+/-" << err2 << std::endl;
    // HE
    float fitMinHE = 1.0;
    float fitMaxHE = 5.0;
    TCanvas* canvasHE = new TCanvas("canvasHE","Had Energy HE");
    canvasHE->cd();
    TH1F* h_hadEnergyHE_tmp = static_cast<TH1F*>(file->Get((std::string(dirNameNoColl) + "/hadEnergyHE").c_str()));
    TH1F* h_hadEnergyHE = static_cast<TH1F*>(h_hadEnergyHE_tmp->Clone("hadEnergyHE_fit"));
    h_hadEnergyHE->Scale(1./h_NEventsNoColl->GetBinContent(1));
    TF1* fitHE = new TF1("fitHE","gaus",fitMinHE,fitMaxHE);

    h_hadEnergyHE->Fit(fitHE,"R");
    par1 = fitHE->GetParameter(1);
    err1 = fitHE->GetParError(1);
    par2 = fitHE->GetParameter(2);
    err2 = fitHE->GetParError(2);

    std::cout << "Fit parameters HE: " << par1 << "+/-" << err1 << "   "
                                       << par2 << "+/-" << err2 << std::endl;
 
    // HF
    float fitMinHF = 1.0;
    float fitMaxHF = 5.0;
    TCanvas* canvasHF = new TCanvas("canvasHF","Energy HF");
    canvasHF->cd();
    TH1F* h_energyHF_tmp = static_cast<TH1F*>(file->Get((std::string(dirNameNoColl) + "/energyHF").c_str()));
    TH1F* h_energyHF = static_cast<TH1F*>(h_energyHF_tmp->Clone("energyHF_fit"));
    h_energyHF->Scale(1./h_NEventsNoColl->GetBinContent(1));
    TF1* fitHF = new TF1("fitHF","gaus",fitMinHF,fitMaxHF);
    //fitHF->SetLineWidth(2);
    //h_energyHF->SetLineColor();
    //h_energyHF->SetLineWidth();

    h_energyHF->Fit(fitHF,"R"); 
    par1 = fitHF->GetParameter(1);
    err1 = fitHF->GetParError(1);
    par2 = fitHF->GetParameter(2);
    err2 = fitHF->GetParError(2);
 
    std::cout << "Fit parameters HF: " << par1 << "+/-" << err1 << "   "
                                       << par2 << "+/-" << err2 << std::endl;

    // EB
    float fitMinEB = 0.7;
    float fitMaxEB = 4.0;
    TCanvas* canvasEB = new TCanvas("canvasEB","Em Energy EB");
    canvasEB->cd();
    TH1F* h_emEnergyEB_tmp = static_cast<TH1F*>(file->Get((std::string(dirNameNoColl) + "/emEnergyEB").c_str()));
    TH1F* h_emEnergyEB = static_cast<TH1F*>(h_emEnergyEB_tmp->Clone("emEnergyEB_fit"));
    h_emEnergyEB->Scale(1./h_NEventsNoColl->GetBinContent(1));
    TF1* fitEB = new TF1("fitEB","gaus",fitMinEB,fitMaxEB);

    h_emEnergyEB->Fit(fitEB,"R");
    par1 = fitEB->GetParameter(1);
    err1 = fitEB->GetParError(1);
    par2 = fitEB->GetParameter(2);
    err2 = fitEB->GetParError(2);

    std::cout << "Fit parameters EB: " << par1 << "+/-" << err1 << "   "
                                       << par2 << "+/-" << err2 << std::endl;

    // EE
    float fitMinEE = 1.0;
    float fitMaxEE = 5.0;
    TCanvas* canvasEE = new TCanvas("canvasEE","Em Energy EE");
    canvasEE->cd();
    TH1F* h_emEnergyEE_tmp = static_cast<TH1F*>(file->Get((std::string(dirNameNoColl) + "/emEnergyEE").c_str()));
    TH1F* h_emEnergyEE = static_cast<TH1F*>(h_emEnergyEE_tmp->Clone("emEnergyEE_fit"));
    h_emEnergyEE->Scale(1./h_NEventsNoColl->GetBinContent(1));
    TF1* fitEE = new TF1("fitEE","gaus",fitMinEE,fitMaxEE);

    h_emEnergyEE->Fit(fitEE,"R");
    par1 = fitEE->GetParameter(1);
    err1 = fitEE->GetParError(1);
    par2 = fitEE->GetParameter(2);
    err2 = fitEE->GetParError(2);

    std::cout << "Fit parameters EE: " << par1 << "+/-" << err1 << "   "
                                       << par2 << "+/-" << err2 << std::endl;

}
