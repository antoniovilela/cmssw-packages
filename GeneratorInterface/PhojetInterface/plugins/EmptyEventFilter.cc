#include "EmptyEventFilter.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

EmptyEventFilter::EmptyEventFilter(const edm::ParameterSet& ppp) 
{}

EmptyEventFilter::~EmptyEventFilter() 
{}

bool
EmptyEventFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   
   std::vector< Handle<HepMCProduct> > AllProds;
   iEvent.getManyByType(AllProds);
   
   if(AllProds.size()==0) {
     LogInfo("")<<"   Event is skipped and removed.\n";
     return false;
   }
   else return true;
}


void 
EmptyEventFilter::beginJob(const edm::EventSetup&)
{
}

void 
EmptyEventFilter::endJob() {
}
