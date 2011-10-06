/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/
#ifndef __EventAnalyzer_H__
#define __EventAnalyzer_H__

#include "EventReader.h"

#include <string>
#include <map>

class TFile;
class TH1F;
class TH2F;
//====================================================================
class EventAnalyzer
{
 public:
   explicit EventAnalyzer(EventReader  * reader);
   ~EventAnalyzer();
  
   void analyze (int maxEvents = -1);
   void analyzeEvent (unsigned int   event ) ;

 private:
   EventReader * reader_      ;
   Event       * theEvent_    ;
   EventHeader * theHeader_   ;
   Geometry    * theGeometry_ ;
   
   int hit_,hitcl_,total_,totalcl_;
   TFile* rootFile_;
   std::map<std::string,TH1F*> histosTH1F_;
   std::map<std::string,TH2F*> histosTH2F_;

   std::stringstream ss_ ;
};

#endif
