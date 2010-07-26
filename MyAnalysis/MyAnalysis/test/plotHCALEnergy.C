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

    std::string labelColl = "Coll.";
    std::string labelNoColl = "No Coll.";

    std::vector<std::string> variables;
    variables.push_back("energyHB");
    variables.push_back("energyHBplus");
    variables.push_back("energyHBminus");
    variables.push_back("energyHE");
    variables.push_back("energyHEplus");
    variables.push_back("energyHEminus");
    variables.push_back("energyHF");
    variables.push_back("energyHFplus");
    variables.push_back("energyHFminus");
    /*variables.push_back("emEnergyHF");
    variables.push_back("hadEnergyHF");
    variables.push_back("emFractionHF");
    variables.push_back("hfhitenergy");
    variables.push_back("hflongenergy");
    variables.push_back("hfshortenergy");
    variables.push_back("hfhitenergyplus");
    variables.push_back("hflongenergyplus");
    variables.push_back("hfshortenergyplus");
    variables.push_back("hfhitenergyminus");
    variables.push_back("hflongenergyminus");
    variables.push_back("hfshortenergyminus");*/

    //int indexes[] = {0,5,10,15,20,30};
    int indexes[] = {4};
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
    }

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
    plotter.SetColors(histColors);
    plotter.SetLineStyles(histLineStyles);
    //plotter.SetRebin(rebin);

    std::vector<double> normFactors;
    normFactors.push_back(1./h_NEventsColl->GetBinContent(1));
    normFactors.push_back(1./h_NEventsNoColl->GetBinContent(1));
    plotter.plot(variables,dirs,normFactors);

    float fitMinHB = 0.7;
    float fitMaxHB = 4.0;
    TCanvas* canvas1 = new TCanvas("canvas1","Energy HB");
    canvas1->cd();
    TH1F* h_energyHB_tmp = static_cast<TH1F*>(file->Get((std::string(dirNameNoColl) + "/energyHB").c_str()));
    TH1F* h_energyHB = static_cast<TH1F*>(h_energyHB_tmp->Clone("energyHB_fit"));
    h_energyHB->Scale(1./h_NEventsNoColl->GetBinContent(1));
    TF1* fitHB = new TF1("fitHB","gaus",fitMinHB,fitMaxHB);

    h_energyHB->Fit(fitHB,"R");
    double par1 = fitHB->GetParameter(1);
    double err1 = fitHB->GetParError(1);
    double par2 = fitHB->GetParameter(2);
    double err2 = fitHB->GetParError(2);

    std::cout << "Fit parameters HB: " << par1 << "+/-" << err1 << "   "
                                       << par2 << "+/-" << err2 << std::endl;

    float fitMinHE = 1.0;
    float fitMaxHE = 5.0;
    TCanvas* canvas2 = new TCanvas("canvas2","Energy HE");
    canvas2->cd();
    TH1F* h_energyHE_tmp = static_cast<TH1F*>(file->Get((std::string(dirNameNoColl) + "/energyHE").c_str()));
    TH1F* h_energyHE = static_cast<TH1F*>(h_energyHE_tmp->Clone("energyHE_fit"));
    h_energyHE->Scale(1./h_NEventsNoColl->GetBinContent(1));
    TF1* fitHE = new TF1("fitHE","gaus",fitMinHE,fitMaxHE);

    h_energyHE->Fit(fitHE,"R");
    par1 = fitHE->GetParameter(1);
    err1 = fitHE->GetParError(1);
    par2 = fitHE->GetParameter(2);
    err2 = fitHE->GetParError(2);

    std::cout << "Fit parameters HE: " << par1 << "+/-" << err1 << "   "
                                       << par2 << "+/-" << err2 << std::endl;
 
    float fitMinHF = 1.;
    float fitMaxHF = 5.0;
    TCanvas* canvas3 = new TCanvas("canvas3","Energy HF");
    canvas3->cd();
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

}
