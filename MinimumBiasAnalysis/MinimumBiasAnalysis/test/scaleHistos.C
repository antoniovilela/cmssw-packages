#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TKey.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

void scaleByWidthHistos(std::string const& fileName, std::string const& dirName, std::vector<std::string> variables, bool saveHistos);
void scaleByWidth(TH1F*);

void scaleByWidthHistos(std::string const& fileName, std::string const& dirName, bool saveHistos = false){

  std::vector<std::string> variables;
  variables.push_back("EPlusPzFromTowersVarBin");
  variables.push_back("EMinusPzFromTowersVarBin"); 
  variables.push_back("sumEnergyHFPlusVarBin");
  variables.push_back("sumEnergyHFMinusVarBin");
  variables.push_back("multiplicityHFPlusVarBin");
  variables.push_back("multiplicityHFMinusVarBin"); 
 
  scaleByWidthHistos(fileName,dirName,variables,saveHistos); 
}

void scaleByWidthHistos(std::string const& fileName, std::string const& dirName, std::vector<std::string> variables, bool saveHistos){
  TFile* file = TFile::Open(fileName.c_str(),(saveHistos)?"update":"read");
  file->cd(dirName.c_str());
  gDirectory->pwd();

  std::vector<TH1F*> histosTH1F;
  TIter next(gDirectory->GetListOfKeys());
  TKey *key;
  while((key=(TKey*)next())){
     std::string name(key->GetName());
     if(std::find(variables.begin(),variables.end(),name) == variables.end()) continue;

     TH1F* hist = dynamic_cast<TH1F*>(gDirectory->Get(name.c_str()));
     if(hist){
        //name.replace(name.find(str1),str1.size(),str2);
        // I have just tested if it is TH1F 
        name += "_dist";
        std::cout << "Creating histo " << name << std::endl;
        histosTH1F.push_back(static_cast<TH1F*>(hist->Clone(name.c_str())));
        //histosTH1F.back()->Scale(1,"width");
        scaleByWidth(histosTH1F.back());
     }
  }

  if(saveHistos){
     file->cd(dirName.c_str());
     for(size_t k = 0; k < histosTH1F.size(); ++k) histosTH1F[k]->Write();
  }

  file->Close();
}

void scaleByWidth(TH1F* histo){
  Int_t nBins = histo->GetNbinsX();
  for(Int_t ibin = 1; ibin <= nBins; ++ibin){
     float binContent = histo->GetBinContent(ibin);
     float binError = histo->GetBinError(ibin);
     float binWidth = histo->GetBinWidth(ibin);
     histo->SetBinContent(ibin,binContent/binWidth);
     histo->SetBinError(ibin,binError/binWidth);
  }
}
