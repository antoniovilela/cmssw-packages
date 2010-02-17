#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

//#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/test/PlottingTools.h"
//#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/test/Plotter.h"
#include "Utilities/PlottingTools/interface/PlottingTools.h"
#include "Utilities/PlottingTools/interface/Plotter.h"

#include <iostream>
#include <vector>
#include <string>

//double ConstNorm::normalization = 1.0;
 
void plot(const std::string& fileFullSim, const std::string& dirNameFullSim, const std::string& fileFastSim, const std::string& dirNameFastSim, bool saveReweight = false, const std::string& outFileName = "reweightHisto.root"){
    TFile* file_full = TFile::Open(fileFullSim.c_str());
    TFile* file_fast = TFile::Open(fileFastSim.c_str());

    std::vector<std::string> variables;
    variables.push_back("energyHF");
    variables.push_back("energyHFplus");
    variables.push_back("energyHFminus");
    variables.push_back("hfhitenergy");
    variables.push_back("hflongenergy");
    variables.push_back("hfshortenergy");
    variables.push_back("hfhitenergyplus");
    variables.push_back("hflongenergyplus");
    variables.push_back("hfshortenergyplus");
    variables.push_back("hfhitenergyminus");
    variables.push_back("hflongenergyminus");
    variables.push_back("hfshortenergyminus");

    int indexes[] = {0,15,30,45,60,75,90};
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
    dirs.push_back(std::make_pair("FullSim",file_full->GetDirectory(dirNameFullSim.c_str())));
    dirs.push_back(std::make_pair("FastSim",file_fast->GetDirectory(dirNameFastSim.c_str())));

    Plotter<NumberEntriesNorm> plotter;
    //Plotter<ConstNorm> plotter;
    plotter.plot(variables,dirs);

    TH1F* h_NEvents_full = static_cast<TH1F*>(file_full->Get((std::string(dirNameFullSim) + "/NEvents").c_str()));
    TH1F* h_NEvents_fast = static_cast<TH1F*>(file_fast->Get((std::string(dirNameFastSim) + "/NEvents").c_str()));

    TH1F* h_energyHFplus_full = static_cast<TH1F*>(file_full->Get((std::string(dirNameFullSim) + "/energyHFplus").c_str()));
    TH1F* h_energyHFplus_fast = static_cast<TH1F*>(file_fast->Get((std::string(dirNameFastSim) + "/energyHFplus").c_str()));
    h_energyHFplus_full->Rebin(2);
    h_energyHFplus_fast->Rebin(2);
    h_energyHFplus_full->Scale(1/h_NEvents_full->GetBinContent(1));
    h_energyHFplus_fast->Scale(1/h_NEvents_fast->GetBinContent(1));
 
    TH1F* h_energyHFplus_ratio = static_cast<TH1F*>(h_energyHFplus_full->Clone("energyHFplusRatio"));
    h_energyHFplus_ratio->Divide(h_energyHFplus_fast);

    TH1F* h_energyHFminus_full = static_cast<TH1F*>(file_full->Get((std::string(dirNameFullSim) + "/energyHFminus").c_str()));
    TH1F* h_energyHFminus_fast = static_cast<TH1F*>(file_fast->Get((std::string(dirNameFastSim) + "/energyHFminus").c_str()));
    h_energyHFminus_full->Rebin(2);
    h_energyHFminus_fast->Rebin(2);
    h_energyHFminus_full->Scale(1/h_NEvents_full->GetBinContent(1));
    h_energyHFminus_fast->Scale(1/h_NEvents_fast->GetBinContent(1));

    TH1F* h_energyHFminus_ratio = static_cast<TH1F*>(h_energyHFminus_full->Clone("energyHFminusRatio"));
    h_energyHFminus_ratio->Divide(h_energyHFminus_fast);

    TH1F* h_energyHFAll_full = static_cast<TH1F*>(h_energyHFplus_full->Clone("energyHFAllFull"));
    h_energyHFAll_full->Add(h_energyHFminus_full);

    TH1F* h_energyHFAll_fast = static_cast<TH1F*>(h_energyHFplus_fast->Clone("energyHFAllFast"));
    h_energyHFAll_fast->Add(h_energyHFminus_fast);

    TH1F* h_energyHFAverage_ratio = static_cast<TH1F*>(h_energyHFAll_full->Clone("energyHFRatioAverage"));
    h_energyHFAverage_ratio->Divide(h_energyHFAll_fast);
    
    TCanvas* canvas1 = new TCanvas("canvas1","HF-plus energy");
    canvas1->cd();
    h_energyHFplus_full->Draw();
    //h_energyHFplus_fast->SetLineColor(kRed);
    h_energyHFplus_fast->Draw("same");
    TLegend* leg1 = new TLegend(0.4,0.7,0.95,0.8);
    leg1->AddEntry(h_energyHFplus_full,"Full simulation","LPF");
    leg1->AddEntry(h_energyHFplus_fast,"Fast simulation","LPF");
    leg1->SetFillColor(0);
    leg1->Draw("same");

    TCanvas* canvas2 = new TCanvas("canvas2","HF-minus energy");
    canvas2->cd();
    h_energyHFminus_full->Draw();
    //h_energyHFminus_fast->SetLineColor(kRed);
    h_energyHFminus_fast->Draw("same");
    TLegend* leg2 = new TLegend(0.4,0.7,0.95,0.8);
    leg2->AddEntry(h_energyHFminus_full,"Full simulation","LPF");
    leg2->AddEntry(h_energyHFminus_fast,"Fast simulation","LPF");
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
