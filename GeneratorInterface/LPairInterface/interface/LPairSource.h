#ifndef LPair_Source_h
#define LPair_Source_h

/** \class LPairSource
 *
 * Generates LPair HepMC events
 *
 ***************************************/

#include "FWCore/Framework/interface/GeneratedInputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <map>
#include <string>
#include "HepMC/GenEvent.h"

class Run;
namespace CLHEP {
class HepRandomEngine;
class RandFlat;
}

namespace edm
{
  class LPairSource : public GeneratedInputSource {
  public:

    /// Constructor
    LPairSource(const ParameterSet &, const InputSourceDescription &);
    /// Destructor
    virtual ~LPairSource();

    void endRun( Run& r);

  private:

    virtual bool produce(Event & e);
    
    HepMC::GenEvent  *evt;
  
    // Verbosity
    bool lpairHepMCVerbosity_;
  
    // external cross section and filter efficiency
    double extCrossSect;
    double extFilterEff;    
 
    double comenergy;

    CLHEP::HepRandomEngine& fRandomEngine;
    CLHEP::RandFlat*        fRandomGenerator; 
  };
} 

#endif
