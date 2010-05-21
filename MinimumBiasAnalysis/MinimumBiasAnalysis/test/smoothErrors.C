#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TKey.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

void smoothErrorsHistos(std::string const& fileName, std::string const& dirName, std::vector<std::string> variables, bool saveHistos);
void smoothErrors(TH1F*);

void smoothErrorsHistos(std::string const& fileName, std::string const& dirName, bool saveHistos = false){

  std::vector<std::string> variables;
  variables.push_back("multiplicityHFPlusVarBin_dist_smoothErr");
  variables.push_back("multiplicityHFMinusVarBin_dist_smoothErr");
 
  smoothErrorsHistos(fileName,dirName,variables,saveHistos); 
}

void smoothErrorsHistos(std::string const& fileName, std::string const& dirName, std::vector<std::string> variables, bool saveHistos){
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
        // I have just tested if it is TH1F 
        name += "_smoothErr";
        std::cout << "Creating histo " << name << std::endl;
        histosTH1F.push_back(static_cast<TH1F*>(hist->Clone(name.c_str())));
        smoothErrors(histosTH1F.back());
     }
  }

  if(saveHistos){
     file->cd(dirName.c_str());
     for(size_t k = 0; k < histosTH1F.size(); ++k) histosTH1F[k]->Write();
  }

  file->Close();
}

void smoothErrors(TH1F* histo){
  Int_t nBins = histo->GetNbinsX();
  // i)  For each bin from 2..N-1, use average of adjacent bins and current one
  // ii) For each bin from 2..N, use average of relative error of current and previous one
  for(Int_t ibin = 2; ibin <= nBins; ++ibin){
     float binError = histo->GetBinError(ibin);
     float previousError = histo->GetBinError(ibin - 1);
     float nextError = histo->GetBinError(ibin + 1); 
     float binContent = histo->GetBinContent(ibin);
     float previousContent = histo->GetBinContent(ibin - 1); 

     //float averageError = (previousError + binError + nextError)/3.;
     float averageError = ((previousError/previousContent + binError/binContent)/2.)*binContent;
     histo->SetBinError(ibin,std::max(binError,averageError));
  }
}
