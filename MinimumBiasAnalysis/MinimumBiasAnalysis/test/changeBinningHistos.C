#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TKey.h"

#include <vector>
#include <map>
#include <string>
#include <iostream>

void changeBinning(TH1F const* histo_orig, TH1F* histo_new);

void changeBinningHistos(std::string const& fileName, std::string const& dirName, bool saveHistos = false){

  Float_t minEVarBin = 1.0;
  Float_t binningEPlusPz[]={minEVarBin,5.,10.,15.,20.,25.,30.,40.,50.,60.,70.,80.,90.,
                                       100.,120.,140.,160.,180.,200.,225.,250.,
                                       275.,300.,350.,400.};
  TH1F* histEPlusPz = new TH1F("EPlusPz_template","EPlusPz Histogram",24,binningEPlusPz);

  Float_t binningESumHF[]={minEVarBin,5.,10.,15.,20.,25.,30.,40.,50.,60.,70.,80.,90.,
                                      100.,120.,140.,160.,180.,200.};
  TH1F* histESumHF = new TH1F("ESumHF_template","ESumHF Histogram",18,binningESumHF);

  /*std::vector<std::string> variablesEPlusPz;
  variablesEPlusPz.push_back("EPlusPzFromTowersVarBin_dist");
  variablesEPlusPz.push_back("EMinusPzFromTowersVarBin_dist");
  std::vector<std::string> variablesESumHF;
  variablesESumHF.push_back("sumEnergyHFPlusVarBin_dist");
  variablesESumHF.push_back("sumEnergyHFMinusVarBin_dist");*/
  std::map<std::string,TH1F*> binnings;
  binnings["EPlusPzFromTowersVarBin_dist"] = histEPlusPz;
  binnings["EMinusPzFromTowersVarBin_dist"] = histEPlusPz;
  binnings["sumEnergyHFPlusVarBin_dist"] = histESumHF;
  binnings["sumEnergyHFMinusVarBin_dist"] = histESumHF; 

  TFile* file = TFile::Open(fileName.c_str(),(saveHistos)?"update":"read");
  file->cd(dirName.c_str());
  gDirectory->pwd();

  std::vector<TH1F*> histosTH1F;
  TIter next(gDirectory->GetListOfKeys());
  TKey *key;
  while((key=(TKey*)next())){
     std::string name(key->GetName());
     if(binnings.find(name) == binnings.end()) continue;

     TH1F* hist = dynamic_cast<TH1F*>(gDirectory->Get(name.c_str()));
     if(hist){
        // I have just tested if it is TH1F 
        TH1F* hist_template = binnings[name];
        name += "_newBinning";
        std::cout << "Creating histo " << name << std::endl;
        TH1F* hist_new = static_cast<TH1F*>(hist_template->Clone(name.c_str())); 
        changeBinning(hist,hist_new);
        histosTH1F.push_back(hist_new);
     }
  }

  if(saveHistos){
     file->cd(dirName.c_str());
     for(size_t k = 0; k < histosTH1F.size(); ++k) histosTH1F[k]->Write();
  }

  file->Close();
}

void changeBinning(TH1F const* histo_orig, TH1F* histo_new){
  Int_t nBins = histo_orig->GetNbinsX(); 
  if(histo_new->GetNbinsX() != nBins){
     std::cout << "ERROR: Different number of bins from " << histo_orig->GetName() << std::endl;
     return;
  }
  for(Int_t ibin = 1; ibin <= nBins; ++ibin){
     float binContent = histo_orig->GetBinContent(ibin);
     float binError = histo_orig->GetBinError(ibin);
     histo_new->SetBinContent(ibin,binContent);
     histo_new->SetBinError(ibin,binError);
  }
}
