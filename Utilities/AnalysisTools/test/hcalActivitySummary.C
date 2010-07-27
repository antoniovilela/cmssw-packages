#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"

#include <iostream>
#include <algorithm>

#if !defined(__CINT__) && !defined(__MAKECINT__)
//Headers for the data items
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#endif

void hcalActivitySummary(std::vector<std::string>& fileNames,
                         std::string const& outFileName = "analysis_histos.root",
                         int maxEvents = -1){

  std::cout << ">>> Reading files: " << std::endl;
  for(std::vector<std::string>::const_iterator it = fileNames.begin(); it != fileNames.end(); ++it)
     std::cout << "  " << *it << std::endl; 

  // Chain the input files
  fwlite::ChainEvent ev(fileNames);
   
  // Create output file
  TFile* hfile = new TFile(outFileName.c_str(),"recreate","data histograms");

  // Book Histograms
  TH1::SetDefaultSumw2(true);

  double threshHB = 1.5;
  double threshHE = 2.0;
  double threshHF = 4.0; 

  int nEvts = 0;
  for( ev.toBegin(); ! ev.atEnd(); ++ev) {

     if((maxEvents > 0)&&(nEvts == maxEvents)) break;
	
     ++nEvts;
     if(nEvts%1000 == 0) std::cout << ">>> Number of processed events: " << nEvts << endl;

     if(nEvts == 1) {
        fwlite::Handle<std::vector<double> > thresholdsHB;
        thresholdsHB.getByLabel(ev,"hcalActivitySummary","thresholdsHB");

        fwlite::Handle<std::vector<double> > thresholdsHE;
        thresholdsHE.getByLabel(ev,"hcalActivitySummary","thresholdsHE");

        fwlite::Handle<std::vector<double> > thresholdsHF;
        thresholdsHF.getByLabel(ev,"hcalActivitySummary","thresholdsHF");

        std::cout << "Number of Thresholds HB= " << thresholdsHB->size() << std::endl; 
        for(size_t k = 0; k < thresholdsHB->size(); ++k) std::cout << "Theshold index " << k << ": " << (*thresholdsHB)[k] << std::endl;

        std::cout << "Number of Thresholds HE= " << thresholdsHE->size() << std::endl;
        for(size_t k = 0; k < thresholdsHE->size(); ++k) std::cout << "Theshold index " << k << ": " << (*thresholdsHE)[k] << std::endl;

        std::cout << "Number of Thresholds HF= " << thresholdsHF->size() << std::endl;
        for(size_t k = 0; k < thresholdsHF->size(); ++k) std::cout << "Theshold index " << k << ": " << (*thresholdsHF)[k] << std::endl;  

        size_t lowHB = std::lower_bound(thresholdsHB->begin(),thresholdsHB->end(),threshHB) - thresholdsHB->begin();
        size_t upHB = std::upper_bound(thresholdsHB->begin(),thresholdsHB->end(),threshHB) - thresholdsHB->begin();
        std::cout << "Index for HB threshold " << threshHB << ": (" << lowHB << "," << upHB << ")" << std::endl;

        size_t lowHE = std::lower_bound(thresholdsHE->begin(),thresholdsHE->end(),threshHE) - thresholdsHE->begin();
        size_t upHE = std::upper_bound(thresholdsHE->begin(),thresholdsHE->end(),threshHE) - thresholdsHE->begin();
        std::cout << "Index for HE threshold " << threshHE << ": (" << lowHE << "," << upHE << ")" << std::endl;

        size_t lowHF = std::lower_bound(thresholdsHF->begin(),thresholdsHF->end(),threshHF) - thresholdsHF->begin();
        size_t upHF = std::upper_bound(thresholdsHF->begin(),thresholdsHF->end(),threshHF) - thresholdsHF->begin();
        std::cout << "Index for HF threshold " << threshHF << ": (" << lowHF << "," << upHF << ")" << std::endl;
     }

     fwlite::Handle<std::vector<unsigned int> > nHFPlus;
     nHFPlus.getByLabel(ev,"hcalActivitySummary","nHFplus");
  
     if(!nHFPlus.isValid()) {std::cout << ">>> ERROR: Product could not be accessed" << std::endl;continue;}

     fwlite::Handle<std::vector<unsigned int> > nHFMinus;
     nHFMinus.getByLabel(ev,"hcalActivitySummary","nHFminus");

     if(!nHFMinus.isValid()) {std::cout << ">>> ERROR: Product could not be accessed" << std::endl;continue;}

  }

  hfile->Write();
  hfile->Close();

}
