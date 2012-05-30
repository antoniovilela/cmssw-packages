#ifndef PhojetInterface_EmptyEventFilter_h
#define PhojetInterface_EmptyEventFilter_h

//
// Copy of Herwig6Filter
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class declaration
//

class EmptyEventFilter : public edm::EDFilter {
   public:
      explicit EmptyEventFilter(const edm::ParameterSet&);
      ~EmptyEventFilter();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
};

#endif

