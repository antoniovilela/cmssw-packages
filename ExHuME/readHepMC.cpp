#include "HepMC/GenEvent.h"
#include "HepMC/PdfInfo.h"
#include "HepMC/HEPEVT_Wrapper.h"
#include "HepMC/IO_HEPEVT.h"
#include "HepMC/IO_GenEvent.h"

int main(int argc, char** argv){
   // specify an input file
   HepMC::IO_GenEvent ascii_in("exhume.dat",std::ios::in);
   // get the first event
   HepMC::GenEvent* evt = ascii_in.read_next_event();
   // loop until we run out of events
   while ( evt ) {
      // analyze the event
      evt->print();
      // delete the created event from memory
      delete evt;
      // read the next event
      ascii_in >> evt;
   }

   return 0;
}
