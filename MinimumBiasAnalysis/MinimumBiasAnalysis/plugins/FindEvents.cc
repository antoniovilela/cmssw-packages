#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <map>

class FindEvents : public edm::EDFilter {
public:
  explicit FindEvents( const edm::ParameterSet & );
  ~FindEvents();
  
private:
  virtual bool filter ( edm::Event &, const edm::EventSetup & );
  bool applyfilter;

  unsigned int runNumber_;
  std::vector<unsigned int> eventNumbers_;
};

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace edm;
using namespace std;

FindEvents::FindEvents(const edm::ParameterSet& pset)
{
  runNumber_ = pset.getParameter<unsigned int>("RunNumber");
  eventNumbers_ = pset.getParameter<std::vector<unsigned int> >("EventNumbers");
  applyfilter = pset.getUntrackedParameter<bool>("applyfilter",true);
}

FindEvents::~FindEvents()
{
}

bool FindEvents::filter( edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  bool accepted = false;

  unsigned int ievt = iEvent.id().event();
  unsigned int irun = iEvent.id().run();
  //unsigned int ils = iEvent.luminosityBlock();

  if(irun == runNumber_){
     std::vector<unsigned int>::const_iterator it_event = std::find(eventNumbers_.begin(),eventNumbers_.end(),ievt);
     if(it_event != eventNumbers_.end()) accepted = true;
  }

  if (applyfilter)
    return accepted;
  else
    return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(FindEvents);
