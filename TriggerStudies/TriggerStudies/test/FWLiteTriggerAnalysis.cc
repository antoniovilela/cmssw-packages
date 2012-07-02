#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include <TH1F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

//#include "DataFormats/MuonReco/interface/Muon.h"
//#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
//#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"


int main(int argc, char* argv[]) 
{
  // define what muon you are using; this is necessary as FWLite is not 
  // capable of reading edm::Views
  //using reco::Muon;
  using reco::CaloJet;

  // ----------------------------------------------------------------------
  // First Part: 
  //
  //  * enable the AutoLibraryLoader 
  //  * book the histograms of interest 
  //  * open the input file
  // ----------------------------------------------------------------------

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // initialize command line parser
  optutl::CommandLineParser parser ("Analyze FWLite Histograms");

  // set defaults
  parser.integerValue ("maxEvents"  ) = 1000;
  parser.integerValue ("outputEvery") =   10;
  parser.stringValue  ("outputFile" ) = "analyzeFWLiteHistograms.root";

  // parse arguments
  parser.parseArguments (argc, argv);
  int maxEvents_ = parser.integerValue("maxEvents");
  unsigned int outputEvery_ = parser.integerValue("outputEvery");
  std::string outputFile_ = parser.stringValue("outputFile");
  std::vector<std::string> inputFiles_ = parser.stringVector("inputFiles");

  // book a set of histograms
  fwlite::TFileService fs = fwlite::TFileService(outputFile_.c_str());
  TFileDirectory dir = fs.mkdir("analyzeBasicPat");
  TH1F* h_hltJetPt_  = dir.make<TH1F>("hltJetPt"  , "pt"  ,   100,   0., 300.);
  TH1F* h_hltJetEta_ = dir.make<TH1F>("hltJetEta" , "eta" ,   100,  -3.,   3.);
  TH1F* h_hltJetPhi_ = dir.make<TH1F>("hltJetPhi" , "phi" ,   100,  -5.,   5.);  

  // loop the events
  int ievt=0;  
  for(unsigned int iFile=0; iFile<inputFiles_.size(); ++iFile){
    // open input file (can be located on castor)
    TFile* inFile = TFile::Open(inputFiles_[iFile].c_str());
    if( inFile ){
      // ----------------------------------------------------------------------
      // Second Part: 
      //
      //  * loop the events in the input file 
      //  * receive the collections of interest via fwlite::Handle
      //  * fill the histograms
      //  * after the loop close the input file
      // ----------------------------------------------------------------------      
      fwlite::Event ev(inFile);
      for(ev.toBegin(); !ev.atEnd(); ++ev, ++ievt){
	edm::EventBase const & event = ev;
	// break loop if maximal number of events is reached 
	if(maxEvents_>0 ? ievt+1>maxEvents_ : false) break;
	// simple event counter
	if(outputEvery_!=0 ? (ievt>0 && ievt%outputEvery_==0) : false) 
	  std::cout << "  processing event: " << ievt << std::endl;

	// Handle to the muon collection
	/*edm::Handle<std::vector<Muon> > muons;
	event.getByLabel(std::string("muons"), muons);*/
	edm::Handle<std::vector<CaloJet> > hltJets;
	event.getByLabel(std::string("hltCaloJetL1FastJetCorrected"), hltJets);
	
	// loop muon collection and fill histograms
	for(std::vector<CaloJet>::const_iterator jet =  hltJets->begin();
                                                 jet != hltJets->end(); ++jet){
	  h_hltJetPt_ ->Fill( jet->pt() );
	  h_hltJetEta_->Fill( jet->eta() );
	  h_hltJetPhi_->Fill( jet->phi() );	  
	}
      }  
      // close input file
      inFile->Close();
    }
    // break loop if maximal number of events is reached:
    // this has to be done twice to stop the file loop as well
    if(maxEvents_>0 ? ievt+1>maxEvents_ : false) break;
  }
  return 0;
}
