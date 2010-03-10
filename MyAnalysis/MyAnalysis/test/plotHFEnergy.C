#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"
#include "Utilities/PlottingTools/interface/Plotter.h"

#include <iostream>
#include <vector>
#include <string>

//double ConstNorm::normalization = 1.0;
 
void plot(const std::string& fileRef, const std::string& dirNameRef, const std::string& fileRew, const std::string& dirNameRew, const std::string& labelRef = "Reference", const std::string& labelRew = "Reweight", bool saveReweight = false, const std::string& outFileName = "reweightHisto.root"){
    TFile* file_ref = TFile::Open(fileRef.c_str());
    TFile* file_rew = TFile::Open(fileRew.c_str());

    //std::string labelRef = "Reference";
    //std::string labelRew = "Reweight";

    std::vector<std::string> variables;
    variables.push_back("energyHF");
    variables.push_back("energyHFplus");
    variables.push_back("energyHFminus");
    variables.push_back("emEnergyHF");
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
    variables.push_back("hfshortenergyminus");

    //int indexes[] = {0,5,10,15,20,30};
    int indexes[] = {5,15,20};
    std::vector<int> vec_indexes(indexes,indexes + sizeof(indexes)/sizeof(int));
    for(std::vector<int>::const_iterator idx = vec_indexes.begin(); idx != vec_indexes.end(); ++idx){
       char aux[20];
       sprintf(aux,"nhfplus_%d",*idx);
       variables.push_back(aux);
       sprintf(aux,"nhfminus_%d",*idx);
       variables.push_back(aux);
       sprintf(aux,"sumEHFplus_%d",*idx);
       variables.push_back(aux);
       sprintf(aux,"sumEHFminus_%d",*idx);
       variables.push_back(aux); 
    }

    std::vector<std::pair<std::string,TDirectory*> > dirs;
    dirs.push_back(std::make_pair(labelRef,file_ref->GetDirectory(dirNameRef.c_str())));
    dirs.push_back(std::make_pair(labelRew,file_rew->GetDirectory(dirNameRew.c_str())));

    TH1F* h_NEvents_ref = static_cast<TH1F*>(file_ref->Get((std::string(dirNameRef) + "/NEvents").c_str()));
    TH1F* h_NEvents_rew = static_cast<TH1F*>(file_rew->Get((std::string(dirNameRew) + "/NEvents").c_str()));

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
    normFactors.push_back(1./h_NEvents_ref->GetBinContent(1));
    normFactors.push_back(1./h_NEvents_rew->GetBinContent(1));
    plotter.plot(variables,dirs,normFactors);

    TH1F* h_energyHFplus_ref = static_cast<TH1F*>(file_ref->Get((std::string(dirNameRef) + "/energyHFplus").c_str()));
    TH1F* h_energyHFplus_rew = static_cast<TH1F*>(file_rew->Get((std::string(dirNameRew) + "/energyHFplus").c_str()));
    h_energyHFplus_ref->Rebin(2);
    h_energyHFplus_rew->Rebin(2);
    h_energyHFplus_ref->Scale(1/h_NEvents_ref->GetBinContent(1));
    h_energyHFplus_rew->Scale(1/h_NEvents_rew->GetBinContent(1));
 
    TH1F* h_energyHFplus_ratio = static_cast<TH1F*>(h_energyHFplus_ref->Clone("energyHFplusRatio"));
    h_energyHFplus_ratio->Divide(h_energyHFplus_rew);

    TH1F* h_energyHFminus_ref = static_cast<TH1F*>(file_ref->Get((std::string(dirNameRef) + "/energyHFminus").c_str()));
    TH1F* h_energyHFminus_rew = static_cast<TH1F*>(file_rew->Get((std::string(dirNameRew) + "/energyHFminus").c_str()));
    h_energyHFminus_ref->Rebin(2);
    h_energyHFminus_rew->Rebin(2);
    h_energyHFminus_ref->Scale(1/h_NEvents_ref->GetBinContent(1));
    h_energyHFminus_rew->Scale(1/h_NEvents_rew->GetBinContent(1));

    TH1F* h_energyHFminus_ratio = static_cast<TH1F*>(h_energyHFminus_ref->Clone("energyHFminusRatio"));
    h_energyHFminus_ratio->Divide(h_energyHFminus_rew);

    TH1F* h_energyHFAll_ref = static_cast<TH1F*>(h_energyHFplus_ref->Clone("energyHFAllRef"));
    h_energyHFAll_ref->Add(h_energyHFminus_ref);

    TH1F* h_energyHFAll_rew = static_cast<TH1F*>(h_energyHFplus_rew->Clone("energyHFAllRew"));
    h_energyHFAll_rew->Add(h_energyHFminus_rew);

    TH1F* h_energyHFAverage_ratio = static_cast<TH1F*>(h_energyHFAll_ref->Clone("energyHFRatioAverage"));
    h_energyHFAverage_ratio->Divide(h_energyHFAll_rew);
    
    TCanvas* canvas1 = new TCanvas("canvas1","HF-plus energy");
    canvas1->cd();
    h_energyHFplus_ref->Draw();
    //h_energyHFplus_rew->SetLineColor(kRed);
    h_energyHFplus_rew->Draw("same");
    TLegend* leg1 = new TLegend(0.4,0.7,0.95,0.8);
    leg1->AddEntry(h_energyHFplus_ref,labelRef.c_str(),"LPF");
    leg1->AddEntry(h_energyHFplus_rew,labelRew.c_str(),"LPF");
    leg1->SetFillColor(0);
    leg1->Draw("same");

    TCanvas* canvas2 = new TCanvas("canvas2","HF-minus energy");
    canvas2->cd();
    h_energyHFminus_ref->Draw();
    //h_energyHFminus_rew->SetLineColor(kRed);
    h_energyHFminus_rew->Draw("same");
    TLegend* leg2 = new TLegend(0.4,0.7,0.95,0.8);
    leg2->AddEntry(h_energyHFminus_ref,labelRef.c_str(),"LPF");
    leg2->AddEntry(h_energyHFminus_rew,labelRew.c_str(),"LPF");
    leg2->SetFillColor(0);
    leg2->Draw("same");

    TCanvas* canvas3 = new TCanvas("canvas3","Energy reweight HF-plus");
    canvas3->cd();
    h_energyHFplus_ratio->Draw();
 
    TCanvas* canvas4 = new TCanvas("canvas4","Energy reweight HF-minus");
    canvas4->cd();
    h_energyHFminus_ratio->Draw();

    TCanvas* canvas5 = new TCanvas("canvas5","Energy reweight HF average plus/minus");
    canvas5->cd();
    h_energyHFAverage_ratio->Draw();

    if(saveReweight){
       TFile* outFile = new TFile(outFileName.c_str(),"recreate");
       outFile->cd();
       h_energyHFplus_ratio->Write();
       h_energyHFminus_ratio->Write();
       h_energyHFAverage_ratio->Write();
    }
   
}
