/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/

#include "ReaderTemplate.h"
#include "EventConverter.h"

//====================================================================
int main(int argv, char **args)
{
  // Instantiate a reader: this class is delegated to handling the root input 
  // file and provide pointers to the classes contained in the file to the
  // eventAnalyzer, wehre the actual data analysis is implemented and carried 
  // out by the user. 
  // As a user you are not supposed nor required to deal with details of this
  // class which is provided to you as a service to manage the I/O of events
  // from file.
  EventReader * reader = new EventReader() ; 

  // To open a root file with aligned and reconstructed tracks, just specify 
  // the full path and then the name of the input files: these must be files
  // produced by Monicelli, containing telescope reconstructed tracks and all
  // associated quantities (tracks have a provenance with pointers to the used
  // hits in the telescope planes). The associated geometry file contains the
  // telescope description and the correctly aligned telescope planes.

  std::string runNumber;
  
  std::cout << "runNumber = ";
  std::cin >> runNumber;

   reader->setPath ("/home/uplegger/tmp/"            ) ;
   reader->openFile("test20110923_1.root"   ) ;
   reader->openFile("20110923_1.geo"    ) ;
//  reader->setPath ("../../../tmp/"            ) ;
//  reader->openFile(("test"+runNumber+".root").c_str()) ;
//  reader->openFile(("test"+runNumber+".geo").c_str()) ;
 
  // Finally invoke the eventAnalyzer class to process your own analysis on
  // the reconstructed track (use the predicted impact points on the DUT(s)
  // to compare with actual hits). This is the class you are supposed to 
  // modify to place your customized analysis algorithm.
  // Note that we hand out to this class the pointer to the reader in calling
  // sequence: the reader, in turn, owns the pointers to the even components 
  // stored in the file and these will be made available to the analyzer to
  // allow user manipulation of the event components (hits, clusters and 
  // reconstructed tracks)
  EventConverter * converter = new EventConverter(reader) ;
  converter->convert();

  return 0 ;
}
