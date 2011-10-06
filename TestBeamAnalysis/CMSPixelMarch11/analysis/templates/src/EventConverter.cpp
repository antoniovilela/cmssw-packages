/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/

#include "EventConverter.h"
#include "Data.h"
#include <TFile.h>
#include <TTree.h>
#include <string>
#include <sstream>

using namespace std;

//====================================================================
// This class receives a pointer to the eventReader class. This pointer 
// is used to retrieve individual pointers to the objects stored on
// file (theHeader_, theEvent_ and theGeometry_)
//
EventConverter::EventConverter(EventReader * reader) : 
   theEventReader_ ( reader)
  ,theHeader_      ( reader->getEventHeaderPointer())
  ,theEvent_       ( reader->getEventPointer   ())
  ,theGeometry_    ( reader->getGeometryPointer())
{
}

//====================================================================
// This is the method of the EventConverter class where users are supposed 
// to implement their own private code for analysis. Here you will find 
// an exampl of how to retrieve and manipulate components of the event
// and the geometry.
//
// NOTE: for a detailed description of the Event class, its public access
//       methods and the meaning of invidual containers, please consult
//       ../include/Event.h and comments therein.
//
void EventConverter::convert()
{
  stringstream plaqName;
  
  string outFileName = theEventReader_->getFileName();
  outFileName.replace(outFileName.find(".root"),5,"_Converted.root");
  if(theEventReader_->getFileName() == outFileName)
  {
    STDLINE("Can't find root in file name", ACRed);
    exit(EXIT_FAILURE);
  }
  TFile* outFile = new TFile(outFileName.c_str(),"RECREATE");
  TTree* outTree = new TTree("CaptanTrack","The Track Tree");
  Data   aData;
  aData.branch(outTree);
  
  unsigned int numberOfEvents = theEventReader_->getNumberOfEvents() ;
  // Retrieve individual events and analyze them individually
  for(unsigned int e=0; e<numberOfEvents; e++)
  {
    theEventReader_->readEvent(e) ;
    if(e % 10000 == 0)
    {
      cout << __PRETTY_FUNCTION__ << "\tChecking event: " << e << "/" << numberOfEvents << endl;
    }
    
    Event::plaqMapDef  	            & theRawData	     = theEvent_->getRawData();
    Event::clustersMapDef	    & clusters  	     = theEvent_->getClusters();
    Event::clustersHitsMapDef       & clustersHits	     = theEvent_->getClustersHits();
    Event::fittedTracksDef          & fittedTracks           = theEvent_->getFittedTracks();
    Event::chi2VectorDef	    & fittedTracksChi2       = theEvent_->getFittedTracksChi2();
    Event::residualsMapDef	    & fittedTrackResiduals   = theEvent_->getFittedTrackResiduals();
    Event::trackCandidatesDef       & trackCandidates	     = theEvent_->getTrackCandidates();
    Event::residualsMapDef	    & fittedTrackDeviations  = theEvent_->getFittedTrackDeviations();
    Event::residualsMapDef	    & fittedTrackPulls       = theEvent_->getFittedTrackPulls();
    Event::fittedTrackCovarianceDef & fittedTracksCovariance = theEvent_->getFittedTrackCovariance();

    if( trackCandidates.size() == 0 ) continue ;

    aData.setEvtId   (e);   
    aData.setNtrack  (trackCandidates.size());  
    if(trackCandidates.size() != fittedTracks.size())
    {
      STDLINE("There is something very wrong in the track sizes", ACRed);
    }
    for(int t=0; t<trackCandidates.size(); t++)
    {
      aData.setItrack  (t);
//      STDLINE(trackCandidates[t].size(), ACRed);
      aData.setNhits   (trackCandidates[t].size());
//      aData.setNgoodx  ();
//      aData.setNgoody  ();
      aData.setNdof    (trackCandidates[t].size()-4);
      aData.setChi2    (fittedTracksChi2[t]);
      aData.setProb    (fittedTracksChi2[t]);//TO BE FIXED
      aData.setXproj0  (fittedTracks[t][1]/100);
      aData.setYproj0  (fittedTracks[t][3]/100);
//	aData.setXprojErr();
//	aData.setYprojErr();
      aData.setXslope  (fittedTracks[t][0]/100);
      aData.setYslope  (fittedTracks[t][2]/100);
      for(int p=0; p<10; p++)
      {
	plaqName.str("");
	if((p/4)*2 == 4)
	  plaqName << "Station: " << (p/4)*2 << " - " << "Plaq: " << 1-(p%4);
	else
 	  plaqName << "Station: " << (p/4)*2 << " - " << "Plaq: " << (p%4);
	if(trackCandidates[t].find(plaqName.str()) != trackCandidates[t].end())
	{
	  int clusterID = (int)trackCandidates[t][plaqName.str()]["num"];
	  aData.setHasHit  (true, p);
//	  for(map<string, double>::iterator it=trackCandidates[t][plaqName.str()].begin(); it!=trackCandidates[t][plaqName.str()].end(); it++)
//            STDLINE(trackCandidates[t][plaqName.str()]["size"], ACRed)
//	  trackCandidates[t][plaqName.str()]["size"]
//	    aData.setMeasureX(, p);
	    aData.setNpix    ((int)trackCandidates[t][plaqName.str()]["size"], p);
//	    aData.setNcol    (, p);
//	    aData.setNrow    (, p); 
	    aData.setCharge  (clusters[plaqName.str()][clusterID]["charge"], p);
	    float row = 0;
	    float col = 0;
	    unsigned int size = (unsigned int)clustersHits[plaqName.str()][clusterID].size();
	    for(int h=0; h<size; h++)
	    {
	    	row = (float)clustersHits[plaqName.str()][clusterID][h]["row"];
	    	col = (float)clustersHits[plaqName.str()][clusterID][h]["col"];
	    }
	    
	    aData.setCol     (col/size, p);
	    aData.setRow     (row/size, p);
//	    aData.setXp      (clusters[plaqName.str()][(int)trackCandidates[t][plaqName.str()]["num"]]["x"]/100, p);
//	    aData.setYp      (clusters[plaqName.str()][(int)trackCandidates[t][plaqName.str()]["num"]]["y"]/100, p);
	    aData.setXl      (trackCandidates[t][plaqName.str()]["x"]/100, p);
	    aData.setYl      (trackCandidates[t][plaqName.str()]["y"]/100, p);
	    aData.setXpErr   (trackCandidates[t][plaqName.str()]["xErr"]/100, p);
	    aData.setYpErr   (trackCandidates[t][plaqName.str()]["yErr"]/100, p);
	    aData.setNdofExcl(trackCandidates[t].size()-6, p);//TO BE FIXED
	    aData.setChi2Excl(fittedTracksChi2[t], p);//TO BE FIXED
//	    aData.setProbExcl(, p);
	    aData.setXresid  (fittedTrackResiduals[t][plaqName.str()].first/100, p);
	    aData.setYresid  (fittedTrackResiduals[t][plaqName.str()].second/100, p);
//	    aData.setXtErr   (, p);
//	    aData.setYtErr   (, p);
	  }
	else{
	  aData.setHasHit(false,p);
	}
      }	     
      outTree->Fill();
    }
  }
  outFile->Write();
  outFile->Close();
  delete outFile;
}
