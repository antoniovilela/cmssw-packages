#ifndef ExclusiveDijetsAnalysis_FWLiteTools_h
#define ExclusiveDijetsAnalysis_FWLiteTools_h

#include <map>
#include <algorithm>

#if !defined(__CINT__) && !defined(__MAKECINT__)
//Headers for the data items
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
#endif

namespace exclusiveDijetsAnalysis {

template <class VertexColl>
bool goodVertexFilter(const VertexColl& vertexCollection, unsigned int minNumTracks = 2, double maxAbsZ = -1., double maxd0 = -1.){
   bool accept = false; 
   for(typename VertexColl::const_iterator it = vertexCollection.begin();
                                           it != vertexCollection.end(); ++it){
      if(it->tracksSize() > minNumTracks && 
        ( (maxAbsZ <= 0. ) || fabs(it->z()) <= maxAbsZ ) &&
        ( (maxd0 <= 0. ) || fabs(it->position().rho()) <= maxd0 ) ) {accept = true;break;}
   }

   return accept;

}

template <class TrackColl>
bool highPurityTracksFilter(const TrackColl& trackCollection, double thresh, unsigned int numtrack){

   int numhighpurity = 0;
   float fraction = 0.;

   reco::TrackBase::TrackQuality trkQuality = reco::TrackBase::qualityByName("highPurity");

   bool accept = false;
   if(trackCollection.size() > numtrack){ 
      typename TrackColl::const_iterator itk = trackCollection.begin();
      typename TrackColl::const_iterator itk_end = trackCollection.end();
      for(; itk != itk_end; ++itk){
         // std::cout << "HighPurity?  " << itk->quality(_trackQuality) << std::endl;
         if(itk->quality(trkQuality)) ++numhighpurity;
      }
      fraction = (float)numhighpurity/(float)trackCollection.size();
      if(fraction > thresh) accept = true;
  } else{
    //if less than 10 Tracks accept the event anyway    
    accept = true;
  }
  
  return accept;
}

template <class PartColl>
double MassColl(PartColl& partCollection, double ptThreshold = -1., double energyHBHEThreshold = -1., double energyHFThreshold = -1.){
   math::XYZTLorentzVector allCands(0.,0.,0.,0.);
   for(typename PartColl::const_iterator part = partCollection.begin();
                                         part != partCollection.end(); ++part){
      if(part->pt() < ptThreshold) continue;
      if((fabs(part->eta()) < 3.0) && (part->energy() < energyHBHEThreshold)) continue;
      if((fabs(part->eta()) >= 3.0) && ((fabs(part->eta()) <= 5.0)) && (part->energy() < energyHFThreshold)) continue;
      allCands += part->p4();
   }

   return allCands.M();
}

template <class Coll>
std::pair<double,double> xi(Coll& partCollection, double Ebeam, double ptThreshold = -1., double energyHBHEThreshold = -1., double energyHFThreshold = -1.){

   double xi_towers_plus = 0.;
   double xi_towers_minus = 0.;
   for(typename Coll::const_iterator part = partCollection.begin(); part != partCollection.end(); ++part){
     if(part->pt() < ptThreshold) continue;
     if((fabs(part->eta()) < 3.0) && (part->energy() < energyHBHEThreshold)) continue;
     if((fabs(part->eta()) >= 3.0) && ((fabs(part->eta()) <= 5.0)) && (part->energy() < energyHFThreshold)) continue;

     //double correction = (jetCorrector)?(jetCorrector->getCorrection(part->pt(),part->eta())):1.;
     xi_towers_plus += part->et()*TMath::Exp(part->eta());
     xi_towers_minus += part->et()*TMath::Exp(-part->eta());
   }

   xi_towers_plus /= 2*Ebeam;
   xi_towers_minus /= 2*Ebeam;
   
   return std::make_pair(xi_towers_plus,xi_towers_minus);
}

template <class Coll>
std::pair<double,double> EPlusPz(Coll& partCollection, double ptThreshold = -1., double energyHBHEThreshold = -1., double energyHFThreshold = -1.){
   double e_plus_pz = 0.;
   double e_minus_pz = 0.;
   typename Coll::const_iterator part = partCollection.begin();
   typename Coll::const_iterator part_end = partCollection.end();
   for(; part != part_end; ++part){
      if(part->pt() < ptThreshold) continue;
      if((fabs(part->eta()) < 3.0) && (part->energy() < energyHBHEThreshold)) continue;
      if((fabs(part->eta()) >= 3.0) && ((fabs(part->eta()) <= 5.0)) && (part->energy() < energyHFThreshold)) continue;

      e_plus_pz += part->energy() + part->pz(); 
      e_minus_pz += part->energy() - part->pz();
   }

   return std::make_pair(e_plus_pz,e_minus_pz);
}

template <class JetColl,class PartColl>
double Rjj(JetColl& jetCollection,PartColl& partCollection){
   math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
   dijetSystem += (jetCollection[0]).p4();
   dijetSystem += (jetCollection[1]).p4(); 

   double Mx = MassColl(partCollection);

   return (dijetSystem.M()/Mx);
}

unsigned int nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapTreshToiEta, unsigned int thresholdHF, unsigned int ieta){
   const std::vector<unsigned int>& vec_iEta = mapTreshToiEta.find(thresholdHF)->second;

   // Count number of ieta entries in vector 
   int count_ieta = (int)std::count(vec_iEta.begin(),vec_iEta.end(),ieta);

   return count_ieta;
}

}
#endif