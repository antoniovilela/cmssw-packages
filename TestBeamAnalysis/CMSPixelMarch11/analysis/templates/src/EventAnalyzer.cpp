/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/

#include "EventAnalyzer.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

//====================================================================
// This class receives a pointer to the eventReader class. This pointer 
// is used to retrieve individual pointers to the objects stored on
// file (theHeader_, theEvent_ and theGeometry_)
//
EventAnalyzer::EventAnalyzer(EventReader * reader):
   reader_(reader),
   theHeader_( reader->getEventHeaderPointer() ),
   theEvent_( reader->getEventPointer() ),
   theGeometry_( reader->getGeometryPointer() ),
   hit_(0),hitcl_(0),
   total_(0),totalcl_(0){

  std::string outFileName = reader_->getFileName();
  outFileName.replace(outFileName.find(".root"),5,"_analysis.root");
  if(reader_->getFileName() == outFileName)
  {
    STDLINE("Can't find root in file name", ACRed);
    exit(EXIT_FAILURE);
  }
  rootFile_ = new TFile(outFileName.c_str(),"RECREATE");
  ss_.str(""); ss_ << "Created ROOT file " << rootFile_->GetName();
  STDLINE(ss_.str(),ACYellow) ;
}

EventAnalyzer::~EventAnalyzer(){
  delete rootFile_;
}

void EventAnalyzer::analyze(int maxEvents)
{ 
  // Book Histos
  rootFile_->cd();
  TH1::SetDefaultSumw2(true);
  histosTH1F_["clusterChargeOnDUT"] = new TH1F("clusterChargeOnDUT","clusterChargeOnDUT",100,-50.,200.);
  histosTH1F_["clusterSizeOnDUT"] = new TH1F("clusterSizeOnDUT","clusterSizeOnDUT",10,0,10);
  histosTH1F_["clusterMatchChargeOnDUT"] = new TH1F("clusterMatchChargeOnDUT","clusterMatchChargeOnDUT",100,-50.,200.);
  histosTH1F_["clusterMatchSizeOnDUT"] = new TH1F("clusterMatchSizeOnDUT","clusterMatchSizeOnDUT",10,0,10);
  histosTH1F_["clusterMatchResidualX"] = new TH1F("clusterMatchResidualX","clusterMatchResidualX",100,-2.,2.);  
  histosTH1F_["clusterMatchResidualY"] = new TH1F("clusterMatchResidualY","clusterMatchResidualY",100,-2.,2.);  
  histosTH1F_["clusterMatchAcceptResidualX"] = new TH1F("clusterMatchAcceptResidualX","clusterMatchAcceptResidualX",100,-2.,2.);  
  histosTH1F_["clusterMatchAcceptResidualY"] = new TH1F("clusterMatchAcceptResidualY","clusterMatchAcceptResidualY",100,-2.,2.);  
  // From tracks
  histosTH1F_["clusterCharge"] = new TH1F("clusterCharge","clusterCharge",100,-50.,200.);
  histosTH1F_["trackResidualX"] = new TH1F("trackResidualX","trackResidualX",100,-2.,2.);
  histosTH1F_["trackResidualY"] = new TH1F("trackResidualY","trackResidualY",100,-2.,2.);

  int nBinsX = 40;
  int nBinsY = 60;
  double widthDUT = 8.10;
  histosTH2F_["posTrackOnDUT"] = new TH2F("posTrackOnDUT","posTrackOnDUT",nBinsX,0.,widthDUT,nBinsY,0.,widthDUT);
  histosTH2F_["posTrackMatchOnDUT"] = new TH2F("posTrackMatchOnDUT","posTrackMatchOnDUT",nBinsX,0.,widthDUT,nBinsY,0.,widthDUT);
  histosTH2F_["posTrackMatchAcceptOnDUT"] = new TH2F("posTrackMatchAcceptOnDUT","posTrackMatchAcceptOnDUT",nBinsX,0.,widthDUT,nBinsY,0.,widthDUT);
  histosTH2F_["posClusterOnDUT"] = new TH2F("posClusterOnDUT","posClusterOnDUT",nBinsX,0.,widthDUT,nBinsY,0.,widthDUT);
  histosTH2F_["posClusterMatchOnDUT"] = new TH2F("posClusterMatchOnDUT","posClusterMatchOnDUT",nBinsX,0.,widthDUT,nBinsY,0.,widthDUT);
  histosTH2F_["posClusterMatchAcceptOnDUT"] = new TH2F("posClusterMatchAcceptOnDUT","posClusterMatchAcceptOnDUT",nBinsX,0.,widthDUT,nBinsY,0.,widthDUT);
  histosTH2F_["hitsClusterMatchOnDUT"] = new TH2F("hitsClusterMatchOnDUT","hitsClusterMatchOnDUT",52,0,52,80,0,80);
  histosTH2F_["adcClusterMatchOnDUT"] = new TH2F("adcClusterMatchOnDUT","adcClusterMatchOnDUT",52,0,52,80,0,80);
  histosTH2F_["hitMaxADCClusterMatchOnDUT"] = new TH2F("hitMaxADCClusterMatchOnDUT","hitMaxADCClusterMatchOnDUT",52,0,52,80,0,80);
  histosTH2F_["adcMaxADCClusterMatchOnDUT"] = new TH2F("adcMaxADCClusterMatchOnDUT","adcMaxADCClusterMatchOnDUT",52,0,52,80,0,80);
  histosTH2F_["hitMaxADCClusterMatchAcceptOnDUT"] = new TH2F("hitMaxADCClusterMatchAcceptOnDUT","hitMaxADCClusterMatchAcceptOnDUT",52,0,52,80,0,80);
  histosTH2F_["adcMaxADCClusterMatchAcceptOnDUT"] = new TH2F("adcMaxADCClusterMatchAcceptOnDUT","adcMaxADCClusterMatchAcceptOnDUT",52,0,52,80,0,80);

  // From tracks
  histosTH2F_["clustersXY"] = new TH2F("clustersXY","clustersXY",nBinsX,0.,widthDUT,nBinsY,0.,widthDUT);
  histosTH2F_["hitsDUT"] = new TH2F("hitsDUT","hitsDUT",52,0,52,80,0,80);
  histosTH2F_["adcDUT"] = new TH2F("adcDUT","adcDUT",52,0,52,80,0,80);

  // Retrieve from file the number of stored events  
  unsigned int numberOfEvents = reader_->getNumberOfEvents() ;
  ss_.str(""); ss_ << "Found " << numberOfEvents << " events on file" ;
  STDLINE(ss_.str(),ACYellow) ;
  STDLINE("       ",ACYellow) ;

  hit_=0  ;
  total_=0;
  for(unsigned int event = 0; event < numberOfEvents; ++event)
  {
    if(maxEvents >= 0 && event > maxEvents) break;

    reader_->readEvent(event) ;
    this->analyzeEvent(event) ;   
  }
  rootFile_->cd();
  histosTH2F_["efficiencyVsPosClusterOnDUT"] = static_cast<TH2F*>(histosTH2F_["posClusterMatchAcceptOnDUT"]->Clone("efficiencyVsPosClusterOnDUT"));
  histosTH2F_["efficiencyVsPosClusterOnDUT"]->Divide(histosTH2F_["posClusterMatchOnDUT"]);
  histosTH2F_["efficiencyVsPixelOnDUT"] = static_cast<TH2F*>(histosTH2F_["hitMaxADCClusterMatchAcceptOnDUT"]->Clone("efficiencyVsPixelOnDUT"));
  histosTH2F_["efficiencyVsPixelOnDUT"]->Divide(histosTH2F_["hitMaxADCClusterMatchOnDUT"]);
  histosTH2F_["efficiencyVsPosTrackOnDUT"] = static_cast<TH2F*>(histosTH2F_["posTrackMatchAcceptOnDUT"]->Clone("efficiencyVsPosTrackOnDUT"));
  histosTH2F_["efficiencyVsPosTrackOnDUT"]->Divide(histosTH2F_["posTrackOnDUT"]);
  rootFile_->Write();
  rootFile_->Close();
  
  double efficencycl = -1.;
  if(totalcl_) efficencycl = (1.*hitcl_)/(1.*totalcl_) * 100.;
  std::cout << "efficency (cluster) = " << efficencycl << std::endl;
  std::cout << "       total tracks = " << totalcl_    << std::endl;
  std::cout << "       hits         = " << hitcl_      << std::endl;
  double efficency = -1.;
  if(total_) efficency = (1.*hit_)/(1.*total_) * 100.;
  std::cout << "efficency (tracks) = " << efficency << std::endl;
  std::cout << "      total tracks = " << total_    << std::endl;
  std::cout << "      hits         = " << hit_      << std::endl;
}

//====================================================================
// This is the method of the eventAnalyzer class where users are supposed 
// to implement their own private code for analysis. Here you will find 
// an exampl of how to retrieve and manipulate components of the event
// and the geometry.
//
// NOTE: for a detailed description of the Event class, its public access
//       methods and the meaning of invidual containers, please consult
//       ../include/Event.h and comments therein.
//
void EventAnalyzer::analyzeEvent(unsigned int event)
{
  Event::clustersMapDef           & clusters                 = theEvent_->getClusters        ()     ;
  Event::clustersHitsMapDef       & clustersHits             = theEvent_->getClustersHits    ()     ;  
  Event::fittedTracksDef          & fittedTracks             = theEvent_->getFittedTracks    ()     ;
  Event::chi2VectorDef            & chi2                     = theEvent_->getFittedTracksChi2()     ;
  Event::fittedTrackCovarianceDef & fittedTrackCovariance    = theEvent_->getFittedTrackCovariance();
  Event::trackCandidatesDef       & trackPoints              = theEvent_->getTrackCandidates ()     ;

  if( fittedTracks.size() == 0 ) return ;

  bool verbose = (event % 1000 == 0);

  ss_.str(""); ss_ << "Event "           << event
                   << "  Clusters found: " << clusters.size() 
                   << "  Tracks: "       << fittedTracks.size() ;
  if(verbose) STDLINE(ss_.str(),ACWhite) ;
  
  // DUT
  int plaqId = 0;
  Detector    *dut         = theGeometry_->getDetector(4,plaqId)  ;
  std::string detectorName = theGeometry_->getDetectorID(4,plaqId);
  ss_.str(""); ss_ << "DUT " << detectorName;
  if(verbose) STDLINE(ss_.str(),ACWhite) ;

  for(unsigned int tr = 0; tr < fittedTracks.size(); ++tr)
  {
    ROOT::Math::SVector<double,4> tParameters = fittedTracks[tr] ;

    ss_.str("") ;
    ss_.setf(std::ios_base::right,std::ios_base::adjustfield) ;
    ss_ << std::setprecision(8) << " ";
    ss_ << std::setw( 4) << tr  
        << std::setw(15) << tParameters[0] 
        << std::setw(15) << tParameters[2] 
        << std::setw(15) << tParameters[1] 
        << std::setw(15) << tParameters[3] 
        << std::setprecision(3)
        << std::setw( 6) << chi2[tr] ; 
    //if(verbose) STDLINE(ss_.str(),ACWhite) ;
    
    // Track selection
    double ndof = trackPoints[tr].size() - 4;
    //if( chi2[tr] > 5 ) continue;
    if(ndof < 2) continue;

    // Get the track impact point on DUT in the local detector frame
    double xp, yp, tmp;
    dut->getPredicted( tParameters, xp, yp );
    Detector::xyPair predPair = dut->getTrackErrorsOnPlane( fittedTracks[tr], fittedTrackCovariance[tr] );

    // Check if the impact point is on the dut surface
    if( xp < 0 || xp > dut->getDetectorLengthX(true) || 
        yp < 0 || yp > dut->getDetectorLengthY(true)   ) continue;

    ++total_;
    ++totalcl_;
    histosTH2F_["posTrackOnDUT"]->Fill(xp/100,yp/100);
    // Loop on clusters directly and find best match
    Event::aClusterMapDef::const_iterator cluster      = clusters[detectorName].begin();
    Event::aClusterMapDef::const_iterator clusters_end = clusters[detectorName].end();
    int clusterBestMatchId = -1;
    double minDistanceCluster = -1.;
    for(; cluster != clusters_end; ++cluster){
       int clusterId = (*cluster).first;
       double x_cl = clusters[detectorName][clusterId]["x"];
       double y_cl = clusters[detectorName][clusterId]["y"];
       double xErr_cl = clusters[detectorName][clusterId]["xErr"];
       double yErr_cl = clusters[detectorName][clusterId]["yErr"];
       double charge_cl = clusters[detectorName][clusterId]["charge"];
       histosTH2F_["posClusterOnDUT"]->Fill(x_cl/100,y_cl/100);
       histosTH1F_["clusterChargeOnDUT"]->Fill(charge_cl);

       unsigned int nHitsClusters = (unsigned int)clustersHits[detectorName][ clusterId ].size();
       histosTH1F_["clusterSizeOnDUT"]->Fill(nHitsClusters);
        
       double distX_cl = ( x_cl - xp );
       double distY_cl = ( y_cl - yp );

       /*// Get the track error and sum to the cluster error
       double nSigma = 5;
       double xWindow = predPair.first  + xErr_cl*xErr_cl; xWindow = sqrt(xWindow);
       double yWindow = predPair.second + yErr_cl*yErr_cl; yWindow = sqrt(yWindow);
       if( fabs(distX_cl) > nSigma*xWindow ) continue;
       if( fabs(distY_cl) > nSigma*yWindow ) continue;*/

       double distXY_cl = distX_cl*distX_cl + distY_cl*distY_cl;
       distXY_cl = sqrt(distXY_cl);
       if( (minDistanceCluster < 0) ||
           (minDistanceCluster >= 0. && distXY_cl < minDistanceCluster) ){
          minDistanceCluster = distXY_cl;
          clusterBestMatchId = clusterId;
       }

    }
    // Best match
    if(clusterBestMatchId >= 0){
       double x_cl = clusters[detectorName][ clusterBestMatchId ]["x"];
       double y_cl = clusters[detectorName][ clusterBestMatchId ]["y"];
       double xErr_cl = clusters[detectorName][ clusterBestMatchId ]["xErr"];
       double yErr_cl = clusters[detectorName][ clusterBestMatchId ]["yErr"];
       double charge_cl = clusters[detectorName][ clusterBestMatchId ]["charge"];
       ss_.str(""); ss_ << "  Cluster best match id = " << clusterBestMatchId
                        << std::setprecision(4) << " " 
                        << std::setw(10) << x_cl
                        << std::setw(10) << xErr_cl
                        << std::setw(10) << y_cl
                        << std::setw(10) << yErr_cl;
       if(verbose) STDLINE(ss_.str(),ACWhite) ;
       histosTH1F_["clusterMatchChargeOnDUT"]->Fill(charge_cl);
       histosTH2F_["posTrackMatchOnDUT"]->Fill(xp/100,yp/100);
       histosTH2F_["posClusterMatchOnDUT"]->Fill(x_cl/100,y_cl/100);

       unsigned int nHitsClusters = (unsigned int)clustersHits[detectorName][ clusterBestMatchId ].size();
       histosTH1F_["clusterMatchSizeOnDUT"]->Fill(nHitsClusters);
       int hitMaxADC = -1;
       int adcMax = -1;
       for(int h = 0; h < nHitsClusters; ++h){
          int row,col,adc;
          row = clustersHits[detectorName][ clusterBestMatchId ][h]["row"];
          col = clustersHits[detectorName][ clusterBestMatchId ][h]["col"];
          adc = clustersHits[detectorName][ clusterBestMatchId ][h]["adc"];
          histosTH2F_["hitsClusterMatchOnDUT"]->Fill(col,row);
          histosTH2F_["adcClusterMatchOnDUT"]->Fill(col,row,adc);
          if(adc > adcMax) {hitMaxADC = h; adcMax = adc;}
       }
       int rowMaxADC = clustersHits[detectorName][ clusterBestMatchId ][hitMaxADC]["row"];
       int colMaxADC = clustersHits[detectorName][ clusterBestMatchId ][hitMaxADC]["col"];
       histosTH2F_["hitMaxADCClusterMatchOnDUT"]->Fill(colMaxADC,rowMaxADC);
       histosTH2F_["adcMaxADCClusterMatchOnDUT"]->Fill(colMaxADC,rowMaxADC,adcMax);
       
       double distX_cl = ( x_cl - xp );
       double distY_cl = ( y_cl - yp );

       histosTH1F_["clusterMatchResidualX"]->Fill(distX_cl/100);
       histosTH1F_["clusterMatchResidualY"]->Fill(distY_cl/100);

       // Get the track error and sum to the cluster error
       double nSigma = 3;
       double xWindow = predPair.first  + xErr_cl*xErr_cl; xWindow = sqrt(xWindow);
       double yWindow = predPair.second + yErr_cl*yErr_cl; yWindow = sqrt(yWindow);
       ss_.str(""); ss_ << "  dist(X),dist(Y),X,Y window = "
                        << std::setprecision(4) << " " 
                        << std::setw(10) << distX_cl
                        << std::setw(10) << distY_cl
                        << std::setw(10) << xWindow
                        << std::setw(10) << yWindow;
       if(verbose) STDLINE(ss_.str(),ACWhite) ;

       if( fabs(distX_cl) > nSigma*xWindow ) continue;
       if( fabs(distY_cl) > nSigma*yWindow ) continue;

       ss_.str(""); ss_ << "  Cluster accepted.";
       if(verbose) STDLINE(ss_.str(),ACWhite) ;
       ++hitcl_;
       histosTH2F_["posClusterMatchAcceptOnDUT"]->Fill(x_cl/100,y_cl/100);
       histosTH2F_["posTrackMatchAcceptOnDUT"]->Fill(xp/100,yp/100);
       histosTH1F_["clusterMatchAcceptResidualX"]->Fill(distX_cl/100);
       histosTH1F_["clusterMatchAcceptResidualY"]->Fill(distY_cl/100);
       histosTH2F_["hitMaxADCClusterMatchAcceptOnDUT"]->Fill(colMaxADC,rowMaxADC);
       histosTH2F_["adcMaxADCClusterMatchAcceptOnDUT"]->Fill(colMaxADC,rowMaxADC,adcMax);
    }

    // Check if there is a hit for this track on the DUT
    //if( trackPoints[tr].find(detectorName) == trackPoints[tr].end() ) continue;
    if( trackPoints[tr].find(detectorName) != trackPoints[tr].end() ){
       // Get the relative cluster coordinate (that's in the local coordinate frame)
       int clusterNumber = (int)trackPoints[tr][detectorName]["num"];
       double xcl_trk,ycl_trk,charge;
       xcl_trk = clusters[detectorName][ clusterNumber ]["x"];
       ycl_trk = clusters[detectorName][ clusterNumber ]["y"];
       charge = clusters[detectorName][ clusterNumber ]["charge"];
       histosTH1F_["clusterCharge"]->Fill(charge);
       histosTH2F_["clustersXY"]->Fill(xcl_trk/100,ycl_trk/100);

       unsigned int nHitsClusters = (unsigned int)clustersHits[detectorName][ clusterNumber ].size();
       for(int h = 0; h < nHitsClusters; ++h){
          int row,col,adc;
          row = clustersHits[detectorName][ clusterNumber ][h]["row"];
          col = clustersHits[detectorName][ clusterNumber ][h]["col"];
          adc = clustersHits[detectorName][ clusterNumber ][h]["adc"];
          histosTH2F_["hitsDUT"]->Fill(col,row);
          histosTH2F_["adcDUT"]->Fill(col,row,adc);
       }

       // Compute the local coordinate from the global ones
       //xm = trackPoints[tr][detectorName]["x"] 
       //ym = trackPoints[tr][detectorName]["y"]
       //dut->fromGlobalToDetector(&xm   ,
       //                          &ym   ,
       //                          &trackPoints[tr][detectorName]["xErr"],
       //                          &trackPoints[tr][detectorName]["yErr"],
       //                          &trackPoints[tr][detectorName]["zErr"]  );

       // Compute the x and y distances between mesured and predicted point
       double distX = ( xcl_trk - xp );
       double distY = ( ycl_trk - yp );
       histosTH1F_["trackResidualX"]->Fill(distX/100);
       histosTH1F_["trackResidualY"]->Fill(distY/100);

       // Get the track error and sum to the cluster error
       //Detector::xyPair predPair = dut->getTrackErrorsOnPlane( fittedTracks[tr], fittedTrackCovariance[tr] );
       double xErr = clusters[detectorName][ clusterNumber ]["xErr"];
       double yErr = clusters[detectorName][ clusterNumber ]["yErr"];
       double xWindow = predPair.first  + xErr*xErr;
       double yWindow = predPair.second + yErr*yErr;
       // If the point is within n sigma got a hit
       double nSigma = 2;
       if( fabs(distX) > nSigma*sqrt(xWindow) ) continue;
       if( fabs(distY) > nSigma*sqrt(yWindow) ) continue;
       ++hit_;
    }
  } // track candidates
}
