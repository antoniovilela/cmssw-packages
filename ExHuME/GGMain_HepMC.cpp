//-*-c++-*-
//-*-HiggsExample.cpp-*-
//-*-Example ExHuME main program to generate Higgs-*-
//   Written by James Monk and Andrew Pilkington
/////////////////////////////////////////////////////////////////////////////
#include <cmath>

#include "GG.h"
#include "Event.h"
#include "PythiaRecord.h"

//#include "TFile.h"
//#include "TTree.h"

#include "HepMC/GenEvent.h"
#include "HepMC/PdfInfo.h"
#include "HepMC/HEPEVT_Wrapper.h"
#include "HepMC/IO_HEPEVT.h"
#include "HepMC/IO_GenEvent.h"

#define pylist pylist_
#define pypars pypars_

int main(int argc, char** argv){

  HepMC::IO_HEPEVT conv;
  HepMC::IO_GenEvent ascii_out("exhume.dat",std::ios::out);

  Exhume::GG gluglu (argc, argv);
  Exhume::Event GluGluEvent (gluglu, 1111);
  //                                ^^
  //                       Random number seed

  gluglu.SetThetaMin(0.30);
  GluGluEvent.SetMassRange(100.0, 99999.0);
  GluGluEvent.SetParameterSpace();

  std::cout<<"   Event generation Begins..."<<std::endl;

  unsigned int maxEvents = 300; 
  int maxEventsToPrint = 2;
  int pythiaListVerbosity = 1;
  int pythiaHepMCVerbosity = 1;  
  int sigID = 10001;
  for(unsigned int i_evt = 0; i_evt != maxEvents; ++i_evt){
    GluGluEvent.Generate();
    gluglu.Hadronise();

    HepMC::GenEvent* evt = conv.read_next_event();
    evt->set_signal_process_id(sigID);	
    //evt->set_event_scale(pypars.pari[16]);
    evt->set_event_number(i_evt);

    if(maxEventsToPrint){
       --maxEventsToPrint;
       pylist(pythiaListVerbosity);
       if(pythiaHepMCVerbosity) evt->print();
    }

    ascii_out << evt;
   
    delete evt;
  }

  double XS = GluGluEvent.CrossSectionCalculation();
  double Eff = GluGluEvent.GetEfficiency();
  std::string Name = gluglu.GetName();

  std::cout<<std::endl<<std::endl;
  std::cout<<"   You have just been ExHuMEd."<<std::endl<<std::endl;;
  std::cout<<"   The cross section for process "<<Name
	   <<" is "<<XS<<" fb"<<std::endl<<std::endl;
  std::cout<<"   The efficiency of event generation was "<<Eff<<"%"
	   <<std::endl<<std::endl;

  return(0);
}
/////////////////////////////////////////////////////////////////////////////
