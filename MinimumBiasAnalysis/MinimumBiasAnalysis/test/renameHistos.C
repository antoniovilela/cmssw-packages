#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TKey.h"

#include <vector>
#include <string>
#include <iostream>

void renameHistos(std::string const& fileName, std::string const& dirName, std::string const& str1, std::string const& str2, bool saveHistos = false){
  TFile* file = TFile::Open(fileName.c_str(),(saveHistos)?"update":"read");
  file->cd(dirName.c_str());
  gDirectory->pwd();

  std::vector<TH1F*> histosTH1F;
  TIter next(gDirectory->GetListOfKeys());
  TKey *key;
  while((key=(TKey*)next())){
     std::string name(key->GetName());
     TH1F* hist = dynamic_cast<TH1F*>(gDirectory->Get(name.c_str()));
     if(hist){
        name.replace(name.find(str1),str1.size(),str2);
        // I have just tested if it is TH1F 
        histosTH1F.push_back(static_cast<TH1F*>(hist->Clone(name.c_str())));
     }
  }

  if(saveHistos){
     file->cd(dirName.c_str());
     for(size_t k = 0; k < histosTH1F.size(); ++k) histosTH1F[k]->Write();
  }

  file->Close();
}
