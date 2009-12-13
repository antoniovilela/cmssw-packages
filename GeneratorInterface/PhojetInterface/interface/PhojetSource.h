#ifndef PhojetInterface_PhojetSource_h
#define PhojetInterface_PhojetSource_h

/** \class PhojetSource
 *
 * Generates Phojet HepMC events
 *
 ***************************************/

#define PYCOMP pycomp_

#include "FWCore/Framework/interface/GeneratedInputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <map>
#include <string>
#include "HepMC/GenEvent.h"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/RandFlat.h"

class Run;

namespace edm
{
  class PhojetSource : public GeneratedInputSource {
  public:

    /// Constructor
    PhojetSource(const ParameterSet &, const InputSourceDescription &);
    /// Destructor
    virtual ~PhojetSource();

    void endRun( Run& r);


  private:

    /*/// Interface to the PYGIVE pythia routine, with add'l protections
    bool call_pygive(const std::string& iParm );
    bool call_txgive(const std::string& iParm );
    bool call_txgive_init();
    bool call_slhagive(const std::string& iParm );
    bool call_slha_init();*/
  
  private:
    
    virtual bool produce(Event & e);
    void clear();
    
    HepMC::GenEvent  *evt;
 
    /// Pythia PYLIST Verbosity flag
    unsigned int pythiaPylistVerbosity_;
    /// HepMC verbosity flag
    bool pythiaHepMCVerbosity_;
    /// Events to print if verbosity
    unsigned int maxEventsToPrint_;    
   
    double comenergy;

    // external cross section and filter efficiency
    double extCrossSect;
    double extFilterEff;    
 
    double sigmaMax; 

    CLHEP::HepRandomEngine* fRandomEngine;
    CLHEP::RandFlat*        fRandomGenerator; 

  };
} 

#endif
