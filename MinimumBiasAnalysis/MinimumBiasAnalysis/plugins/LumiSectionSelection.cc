#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <map>
#include <utility>

class LumiSectionSelection : public edm::EDFilter {
public:
  explicit LumiSectionSelection( const edm::ParameterSet & );
  ~LumiSectionSelection();
  
private:
  typedef std::map<unsigned int,std::pair<unsigned int,unsigned int> > LumiSectionRangeMap;
  virtual bool filter ( edm::Event &, const edm::EventSetup & );
  bool applyfilter;

  LumiSectionRangeMap lumiSections_;
};

#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace edm;
using namespace std;

LumiSectionSelection::LumiSectionSelection(const edm::ParameterSet& pset)
{
  std::vector<unsigned int> runNumbers_ = pset.getParameter<std::vector<unsigned int> >("Runs");
  std::vector<unsigned int> lumiMins_ = pset.getParameter<std::vector<unsigned int> >("LumiSectionMin");
  std::vector<unsigned int> lumiMaxs_ = pset.getParameter<std::vector<unsigned int> >("LumiSectionMax");
  applyfilter = pset.getUntrackedParameter<bool>("applyfilter",true);

  if((runNumbers_.size() != lumiMins_.size())||(runNumbers_.size() != lumiMaxs_.size())) throw edm::Exception(edm::errors::Configuration) << "Runs, LumiSectionMin and LumiSectionMax should have the same size";

  for(size_t irange = 0; irange < runNumbers_.size(); ++irange){
     lumiSections_.insert(std::make_pair(runNumbers_[irange],std::make_pair(lumiMins_[irange],lumiMaxs_[irange])));
  } 
}

LumiSectionSelection::~LumiSectionSelection()
{
}

bool LumiSectionSelection::filter( edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  bool accepted = false;

  //unsigned int ievt = iEvent.id().event();
  unsigned int irun = iEvent.id().run();
  unsigned int ils = iEvent.luminosityBlock();
  /*if (irun==122294)
    if(ils>=37 && ils<=43) accepted=true;
  if (irun==122314)
    if(ils>=24 && ils<=37) accepted=true;*/

  LumiSectionRangeMap::const_iterator it_range = lumiSections_.find(irun);
  if(it_range != lumiSections_.end()){
     unsigned int lumiMin = it_range->second.first;
     unsigned int lumiMax = it_range->second.second;  

     if((ils >= lumiMin)&&(ils <= lumiMax)) accepted = true;
  } 

  if (applyfilter)
    return accepted;
  else
    return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(LumiSectionSelection);
