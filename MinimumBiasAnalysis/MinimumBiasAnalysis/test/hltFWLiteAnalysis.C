#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>

#if !defined(__CINT__) && !defined(__MAKECINT__)
//Headers for the data items
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/TriggerNames.h"
//#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#endif

//void bookHistos(std::map<std::string,TH1F*>&, fwlite::TriggerNames const &);
void bookHistos(std::map<std::string,TH1F*>&, std::vector<std::string> const &);
void bookHistos(std::map<int,std::vector<TH1F*> >&, int);

void hltFWLiteAnalysis(std::vector<std::string>& fileNames,
                                   std::string const& outFileName = "analysisHLTFWLite_histos.root",
                                   int maxEvents = -1, bool verbose = false) {
   if(verbose){
     std::cout << ">>> Reading files: " << std::endl;
     for(std::vector<std::string>::const_iterator it = fileNames.begin(); it != fileNames.end(); ++it)
               std::cout << "  " << *it << std::endl; 
   } 

   // Chain the input files
   fwlite::ChainEvent ev(fileNames);
   
   // Create output file
   TFile* hfile = new TFile(outFileName.c_str(),"recreate","data histograms");

   std::map<std::string, TH1F*> histos;
   std::map<int,std::vector<TH1F*> > histosPerRun;
   //histos["nEventVsLumiSection"] = new TH1F("nEventVsLumiSection","nEventVsLumiSection",200,0,200);
  
   //edm::TriggerNames triggerNames;
   // Loop over the events
   int nEvts = 0;
   for( ev.toBegin(); ! ev.atEnd(); ++ev) {

     if((maxEvents > 0)&&(nEvts == maxEvents)) break;
	
     ++nEvts;
     if(verbose) std::cout << ">>> Number of processed events: " << nEvts << endl;

     int eventNumber = ev.id().event();
     int runNumber = ev.id().run();
     int lumiSection = ev.luminosityBlock();
     if(verbose){
        std::cout << "  Event number: " << eventNumber << std::endl
                  << "  Run number: " << runNumber << std::endl
                  << "  Lumi section: " << lumiSection << std::endl;
     }
     if(histosPerRun.find(runNumber) == histosPerRun.end()) bookHistos(histosPerRun,runNumber);
     histosPerRun[runNumber][0]->Fill(lumiSection); 

     fwlite::Handle<edm::TriggerResults> triggerResults;
     triggerResults.getByLabel(ev,"TriggerResults","","HLT");
 
     if(!triggerResults.isValid()) continue;

     //triggerNames.init(*triggerResults);
     fwlite::TriggerNames const & triggerNames = ev.triggerNames(*triggerResults);
     if(histos.find("countAll") == histos.end()) bookHistos(histos,triggerNames.triggerNames());
 
     // Loop over triggers
     for(size_t idx = 0; idx < triggerResults->size(); ++idx){
        std::string trigName = triggerNames.triggerName(idx);
        bool wasrun = triggerResults->wasrun(idx);
        bool accept = triggerResults->accept(idx);
        unsigned int module_index = triggerResults->index(idx);
        if(verbose) std::cout << "  Path " << trigName << "  "  << "  was run: " << wasrun << "  "
                              << "  accept:  " << accept << "  "  << "  index:   " << module_index << std::endl;
        if(accept) histos["countAll"]->Fill(idx); 
     } 

   }
   hfile->Write();
   hfile->Close();
}

//void bookHistos(std::map<std::string,TH1F*>& histos, fwlite::TriggerNames const & triggerNames){
void bookHistos(std::map<std::string,TH1F*>& histos, std::vector<std::string> const & triggerNames){
   histos.insert(std::make_pair("countAll",new TH1F("countAll","countAll",triggerNames.size(),0,triggerNames.size())));
   for(size_t idx = 0; idx < triggerNames.size(); ++idx){
      std::string const& trigName = triggerNames[idx];
      histos["countAll"]->GetXaxis()->SetBinLabel((idx + 1),trigName.c_str());
   }
}

void bookHistos(std::map<int,std::vector<TH1F*> >& histosPerRun, int runNumber){
   histosPerRun.insert(std::make_pair(runNumber,std::vector<TH1F*>(1)));
   char hname[50];
   sprintf(hname,"Run%d_nEventVsLumiSection",runNumber);
   histosPerRun[runNumber][0] = new TH1F(hname,hname,200,0,200);
}
