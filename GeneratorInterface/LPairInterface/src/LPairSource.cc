#include "GeneratorInterface/LPairInterface/interface/LPairSource.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/HepMCProduct/interface/GenInfoProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandFlat.h"


#include <iostream>
#include "time.h"
#include "fostream.h"

using namespace edm;
using namespace std;

extern "C" {
  void zduini_(void);
}
#define zduini zduini_

extern "C" {
  void zduevt_(int&);
}
#define zduevt zduevt_

extern "C" {
  void zduend_(void);
}
#define zduend zduend_

const int maxlujets = 4000;
extern "C" {
  extern struct {
    int N,K[5][maxlujets];
    float P[5][maxlujets],V[5][maxlujets];	
  } lujets_;
}
#define lujets lujets_

extern "C" {
  void get_seeds_(int&);
}
#define get_seeds get_seeds_

namespace {
  HepRandomEngine& getEngineReference()
  {

   Service<RandomNumberGenerator> rng;
   if(!rng.isAvailable()) {
    throw cms::Exception("Configuration")
       << "The RandomNumberProducer module requires the RandomNumberGeneratorService\n"
          "which appears to be absent.  Please add that service to your configuration\n"
          "or remove the modules that require it.";
   }

// The Service has already instantiated an engine.  Make contact with it.
   return (rng->getEngine());
  }
}

LPairSource::LPairSource( const ParameterSet & pset, 
			    InputSourceDescription const& desc ) :
  GeneratedInputSource(pset, desc), evt(0), 
  lpairHepMCVerbosity_ (pset.getUntrackedParameter<bool>("lpairHepMCVerbosity",false)),
  extCrossSect(pset.getUntrackedParameter<double>("crossSection", -1.)),
  extFilterEff(pset.getUntrackedParameter<double>("filterEfficiency", -1.)),
  comenergy(pset.getUntrackedParameter<double>("comEnergy",14000.)),
  fRandomEngine(getEngineReference())
{
  
  //Initialize the random engine unconditionally!
  //randomEngine = &fRandomEngine;
  fRandomGenerator = new CLHEP::RandFlat(fRandomEngine) ;

  // Set LPAIR parameters in a single ParameterSet
  ParameterSet lpair_params = 
    pset.getParameter<ParameterSet>("LPairParameters") ;
  
  // The parameter sets to be read (default, min bias, user ...) in the
  // proper order.
  vector<string> setNames = 
    lpair_params.getParameter<vector<string> >("parameterSets");
  
  // Loop over the sets
  int iunit = 10;       
  fostream lpairconfig(iunit,"lpairConfig.txt"); 
  for ( unsigned i=0; i<setNames.size(); ++i ) {
    
    string mySet = setNames[i];
    
    // Read the PYTHIA parameters for each set of parameters
    vector<string> pars = 
      lpair_params.getParameter<vector<string> >(mySet);
    
    // Loop over all parameters and stop in case of mistake
    for( vector<string>::const_iterator  
	   itPar = pars.begin(); itPar != pars.end(); ++itPar ) {

	lpairconfig << itPar->c_str();
    }
  }

  lpairconfig.rewind();

  //In the future, we will get the random number seed on each event and tell 
  // pythia to use that new seed
  // The random engine has already been initialized.  DO NOT do it again!
/*#ifdef NOTYET
  edm::Service<RandomNumberGenerator> rng;
  uint32_t seed = rng->mySeed();
  ostringstream sRandomSet;
  sRandomSet <<"MRPY(1)="<<seed;
  call_pygive(sRandomSet.str());
#endif*/

  int aux = 1;
  get_seeds(aux);//modify properly later
  zduini();
  
  produces<HepMCProduct>();
  produces<GenInfoProduct, edm::InRun>();
}


LPairSource::~LPairSource(){}

void LPairSource::endRun(Run & r) {
 
 zduend();

 /*double cs = pypars.pari[0]; // cross section in mb
 auto_ptr<GenInfoProduct> giprod (new GenInfoProduct());
 giprod->set_cross_section(cs);
 giprod->set_external_cross_section(extCrossSect);
 giprod->set_filter_efficiency(extFilterEff);
 r.put(giprod);*/
 auto_ptr<GenInfoProduct> giprod (new GenInfoProduct());
 giprod->set_cross_section(-1.);
 giprod->set_external_cross_section(extCrossSect);
 giprod->set_filter_efficiency(extFilterEff);
 r.put(giprod);

}

bool LPairSource::produce(Event & e) {

    int ireturn;
    zduevt(ireturn);

    evt = new HepMC::GenEvent();
 
    HepMC::GenVertex* Vtx = new HepMC::GenVertex(HepMC::FourVector(0.,0.,0.));

    //Loop over particles
    int barcode = 1 ;
    for(int ipart = 0; ipart < lujets.N; ++ipart){
	int istatus = lujets.K[0][ipart];
	if(istatus != 1) continue;

	int icode = lujets.K[1][ipart];
	/*double px = lujets.P[0][ipart];
	double py = lujets.P[1][ipart];
	double pz = lujets.P[2][ipart];
	double energy = lujets.P[3][ipart];
	double mass = lujets.P[4][ipart];*/
	float px = lujets.P[0][ipart];
        float py = lujets.P[1][ipart];
        float pz = lujets.P[2][ipart];
        float energy = lujets.P[3][ipart];
        float mass = lujets.P[4][ipart];

	HepMC::FourVector p(px,py,pz,energy);
	
	HepMC::GenParticle* Part = new HepMC::GenParticle(p,icode,1);
        Part->suggest_barcode(barcode);
        barcode++;
        Vtx->add_particle_out(Part);
    }	

    evt->add_vertex(Vtx) ;
    evt->set_event_number(event()) ;
    evt->set_signal_process_id(-1) ; 
        
    if (lpairHepMCVerbosity_)
    {
       evt->print() ;  
    }

    auto_ptr<HepMCProduct> bare_product(new HepMCProduct());
	 	
    // fix for 1-part events
    //if ( particleID ) evt->set_beam_particles(0,0);
    
    /*evt->set_signal_process_id(pypars.msti[0]);
    evt->set_event_scale(pypars.pari[16]);
    evt->set_event_number(numberEventsInRun() - remainingEvents() - 1);*/

    /*// int id1 = pypars.msti[14];
    // int id2 = pypars.msti[15];
    int id1 = pyint1.mint[14];
    int id2 = pyint1.mint[15];
    if ( id1 == 21 ) id1 = 0;
    if ( id2 == 21 ) id2 = 0; 
    double x1 = pyint1.vint[40];
    double x2 = pyint1.vint[41];  
    double Q  = pyint1.vint[50];
    double pdf1 = pyint1.vint[38];
    pdf1 /= x1 ;
    double pdf2 = pyint1.vint[39];
    pdf2 /= x2 ;
    evt->set_pdf_info( HepMC::PdfInfo(id1,id2,x1,x2,Q,pdf1,pdf2) ) ;
    
    evt->weights().push_back( pyint1.vint[96] );*/

    if(evt) bare_product->addHepMCData(evt);

    e.put(bare_product);

    return true;
}
