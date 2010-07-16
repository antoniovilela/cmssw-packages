/*
 *  $Date: 2009/12/11 19:41:50 $
 *  $Revision: 1.2 $
 *  
 */


#include "GeneratorInterface/PhojetInterface/interface/PhojetSource.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>
#include "time.h"
#include "fostream.h"

using namespace edm;
using namespace std;

#include "HepMC/PythiaWrapper6_2.h"
#include "HepMC/IO_HEPEVT.h"

//-------------------------PHOJET functions ------------------------------
extern"C" {
  void pho_event_(int&,double*,double*,double&,int&);
  void pho_pecms_(int&,double&,double&,double&,double&,double&);
  void initphojet_(int&,int&);
}

#define pho_event pho_event_
#define pho_pecms pho_pecms_
#define initphojet initphojet_

HepMC::IO_HEPEVT conv;

//used for defaults
  static const unsigned long kNanoSecPerSec = 1000000000;
  static const unsigned long kAveEventPerSec = 200;

PhojetSource::PhojetSource( const ParameterSet & pset, 
			    InputSourceDescription const& desc ) :
	  GeneratedInputSource(pset, desc), evt(0), 
  pythiaPylistVerbosity_ (pset.getUntrackedParameter<int>("pythiaPylistVerbosity",0)),
  pythiaHepMCVerbosity_ (pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false)),
  maxEventsToPrint_ (pset.getUntrackedParameter<int>("maxEventsToPrint",1)),
  comenergy(pset.getUntrackedParameter<double>("comEnergy",14000.)),
  extCrossSect(pset.getUntrackedParameter<double>("crossSection", -1.)),
  extFilterEff(pset.getUntrackedParameter<double>("filterEfficiency", -1.))
{
  
  /*// PYLIST Verbosity Level
  // Valid PYLIST arguments are: 1, 2, 3, 5, 7, 11, 12, 13
  pythiaPylistVerbosity_ = pset.getUntrackedParameter<int>("pythiaPylistVerbosity",0);
  
  // HepMC event verbosity Level
  pythiaHepMCVerbosity_ = pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false);

  //Max number of events printed on verbosity level 
  maxEventsToPrint_ = pset.getUntrackedParameter<int>("maxEventsToPrint",0);*/
  
  // Set PHOJET parameters in a single ParameterSet
  ParameterSet phojet_params = pset.getParameter<ParameterSet>("PhojetParameters");
  
  // The parameter sets to be read (default, min bias, user ...) in the
  // proper order.
  vector<string> setNames = 
    phojet_params.getParameter<vector<string> >("parameterSets");
  
  // Loop over the sets
  int iunit = 10;	
  fostream phoconfig(iunit,"phojetConfig.txt");	
  phoconfig << "PARTICLE1   2212       0.0";
  phoconfig << "PARTICLE2   2212       0.0";

  std::vector<std::string> invalidParams;
  invalidParams.push_back("WW-HERA");
  invalidParams.push_back("GP-HERA");
  invalidParams.push_back("GG-EPEM");
  invalidParams.push_back("WW-EPEM");
  invalidParams.push_back("GG-HION-F");
  invalidParams.push_back("GG-HION-G");
  invalidParams.push_back("GH-HION-F");
  invalidParams.push_back("HG-HIAS-F");
  invalidParams.push_back("BLASER");
  invalidParams.push_back("BEAMST");
  invalidParams.push_back("EVENT-LAB");
  invalidParams.push_back("EVENT-CMS");
  invalidParams.push_back("EVENT-COLL");
  invalidParams.push_back("READ-RNDM");
  invalidParams.push_back("SAVE-RNDM");
  invalidParams.push_back("INIT-RNDM");
  invalidParams.push_back("PARTICLE1");
  invalidParams.push_back("PARTICLE2");
  for ( unsigned i=0; i<setNames.size(); ++i ) {
    
    string mySet = setNames[i];
    
    // Read the PHOJET parameters for each set of parameters
    vector<string> pars = 
      phojet_params.getParameter<vector<string> >(mySet);
    
    // Loop over all parameters and stop in case of mistake
    for( vector<string>::const_iterator itPar = pars.begin(); itPar != pars.end(); ++itPar ) {
      		/*if( ! call_pygive(*itPar) ) {
			throw edm::Exception(edm::errors::Configuration,"PythiaError") 
	  		<<" pythia did not accept the following \""<<*itPar<<"\"";
      		}*/
		//Don't allow for running or inappropriate parameters here
		for(std::vector<std::string>::const_iterator itInvPar = invalidParams.begin(); itInvPar != invalidParams.end(); ++itInvPar){
			if( 0 == itPar->compare(0,itInvPar->size(),*itInvPar) ) throw edm::Exception(edm::errors::Configuration,"PhojetError") << " Following PHOJET parameter is not accepted in this mode: \"" << *itInvPar << "\"";
		}
		//std::cout << itPar->c_str() << std::endl;
		//phoconfig << itPar->c_str() << "\n"; 
		phoconfig << itPar->c_str();
    }
  }
  	
  /*//In the future, we will get the random number seed on each event and tell 
  // pythia to use that new seed
  edm::Service<RandomNumberGenerator> rng;
  uint32_t seed = rng->mySeed();
  ostringstream sRandomSet;
  sRandomSet <<"MRPY(1)="<<seed;
  call_pygive(sRandomSet.str());*/
  
  /*edm::Service<RandomNumberGenerator> rng;
  uint32_t seed0 = rng->mySeed();
  seed0 = seed0%178;
  std::ostringstream sRandomSet;
  sRandomSet << "INIT-RNDM " << seed0;
  uint32_t seed1 = seed0 + 1;
  seed1 = seed1%178; 	
  sRandomSet << "   " << seed1;
  uint32_t seed2 = seed1 + 1;
  seed2 = seed2%178;
  sRandomSet << "   " << seed2;	
  uint32_t seed3 = seed2 + 1;
  seed3 = seed3%178;
  sRandomSet << "   " << seed3;*/

  edm::Service<RandomNumberGenerator> rng;
  long seed = (long)rng->mySeed();
  cout << " seed= " << seed << endl ;
  fRandomEngine = new CLHEP::HepJamesRandom(seed);
  fRandomGenerator = new CLHEP::RandFlat(fRandomEngine);
 
  long seed0 = fRandomGenerator->fireInt(1L,179L);
  long seed1 = fRandomGenerator->fireInt(1L,179L);	 
  long seed2 = fRandomGenerator->fireInt(1L,179L);
  long seed3 = fRandomGenerator->fireInt(1L,179L);

  std::ostringstream sRandomSet;
  sRandomSet << "INIT-RNDM " << seed0 << "   " << seed1 << "   " << seed2 << "   " << seed3;
  phoconfig << sRandomSet.str().c_str();
  //phoconfig << (const char*)"ENDINPUT  ";
  phoconfig << "ENDINPUT  ";

  phoconfig.rewind();
  int irej;
  initphojet(iunit,irej);
  if(irej != 0) throw edm::Exception(edm::errors::Configuration,"PhojetError") 
                        <<" Error initializing Phojet.";

  /*double pmass1 = 0.938;
  double pmass2 = 0.938;*/
  /*double ecm = comenergy; 
  double pcm;
  double ee;
  int id = 1;
  pho_pecms(id,pmass1,pmass2,ecm,pcm,ee);

  cout << "PCM= " << pcm << endl;
  cout << "EE= " << ee << endl;
 
  id = 2;
  pho_pecms(id,pmass1,pmass2,ecm,pcm,ee);

  cout << "PCM= " << pcm << endl;
  cout << "EE= " << ee << endl;*/

  /*double e1 = comenergy/2.;
  double pp1 = sqrt(e1*e1 - pmass1*pmass1);
  double e2 = comenergy/2.;
  double pp2 = sqrt(e2*e2 - pmass2*pmass2);

  double p1[4] = {0.,0.,pp1,e1};
  double p2[4] = {0.,0.,-pp2,e2};*/

  double p1[4] = {0.,0.,0.,0.};
  double p2[4] = {0.,0.,0.,0.};
  getProtonKinematics(p1,p2);

  double sigmax;
  int mode = -1; 
  pho_event(mode,p1,p2,sigmax,irej);

  sigmaMax = sigmax;
  //cout << "Sigmax = " << sigmax << endl; 
  cout << endl; // Stetically add for the output
  //********                                      
  
  produces<HepMCProduct>();
  produces<GenRunInfoProduct, edm::InRun>();
}


PhojetSource::~PhojetSource(){
  //call_pystat(1);
  //clear(); 
}

void PhojetSource::clear() {
 
}

void PhojetSource::endRun(Run & r) {
 
  /*double cs = pypars.pari[0]; // cross section in mb
  auto_ptr<GenInfoProduct> giprod (new GenInfoProduct());
  giprod->set_cross_section(cs);
  giprod->set_external_cross_section(extCrossSect);
  giprod->set_filter_efficiency(extFilterEff);
  r.put(giprod);*/

  double p1[4] = {0.,0.,0.,0.};
  double p2[4] = {0.,0.,0.,0.};
  getProtonKinematics(p1,p2);

  int mode = -2;
  int irej;
  double sigcur;
  pho_event(mode,p1,p2,sigcur,irej);

  auto_ptr<GenRunInfoProduct> giprod (new GenRunInfoProduct());
  giprod->setInternalXSec(sigmaMax);
  giprod->setExternalXSecLO(extCrossSect);
  giprod->setFilterEfficiency(extFilterEff);
  r.put(giprod);

}

bool PhojetSource::produce(Event & e) {

  auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  

  /*call_pyevnt();      // generate one event with Pythia

  // convert to stdhep format
  //
  call_pyhepc( 1 );*/
    
  //Call Phojet PHO_EVENT here
  /*double pmass1 = 0.938;
  double pmass2 = 0.938;

  double e1 = comenergy/2.;
  double pp1 = sqrt(e1*e1 - pmass1*pmass1);
  double e2 = comenergy/2.;
  double pp2 = sqrt(e2*e2 - pmass2*pmass2);

  double p1[4] = {0.,0.,pp1,e1};
  double p2[4] = {0.,0.,-pp2,e2};*/

  double p1[4] = {0.,0.,0.,0.};
  double p2[4] = {0.,0.,0.,0.};
  getProtonKinematics(p1,p2);

  int mode = 1;
  int irej;
  double sigcur; 	
  pho_event(mode,p1,p2,sigcur,irej);
  if(irej != 0){
     LogWarning("") << "   Problem in event generation...skipping.\n";
     return true;
  }

  //cout << "Sigcur = " << sigcur << endl;		

  call_pyhepc( 1 );

  // convert stdhep (hepevt) to hepmc
  //
  //HepMC::GenEvent* evt = conv.getGenEventfromHEPEVT();
  HepMC::GenEvent* evt = conv.read_next_event();

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

  evt->set_signal_process_id(pypars.msti[0]);
  evt->set_event_scale(pypars.pari[16]);
  evt->set_event_number(numberEventsInRun() - remainingEvents() - 1);
    
  //******** Verbosity ********
    
  if(event() <= maxEventsToPrint_ &&
     (pythiaPylistVerbosity_ || pythiaHepMCVerbosity_)) {

     // Prints PYLIST info
     //
     if(pythiaPylistVerbosity_) {
        call_pylist(pythiaPylistVerbosity_);
     }
      
     // Prints HepMC event
     //
     if(pythiaHepMCVerbosity_) {
        cout << "Event process = " << pypars.msti[0] << endl 
             << "----------------------" << endl;	
        evt->print();
     }
  }
    
  if(evt) bare_product->addHepMCData( evt );

  e.put(bare_product);

  return true;
}

void PhojetSource::getProtonKinematics(double* p1, double* p2){
  double pmass1 = 0.938;
  double pmass2 = 0.938;

  double e1 = comenergy/2.;
  double pp1 = sqrt(e1*e1 - pmass1*pmass1);
  double e2 = comenergy/2.;
  double pp2 = sqrt(e2*e2 - pmass2*pmass2);

  double p1_new[4] = {0.,0.,pp1,e1};
  double p2_new[4] = {0.,0.,-pp2,e2};

  for(int k = 0; k < 4; ++k){
     p1[k] = p1_new[k];
     p2[k] = p2_new[k];
  }
}
