#ifndef ExclusiveDijetsAnalysis_FWLiteTools_h
#define ExclusiveDijetsAnalysis_FWLiteTools_h

#include <map>
#include <algorithm>

namespace exclusiveDijetsAnalysis {

template <class PartColl>
double MassColl(PartColl& partCollection){
   math::XYZTLorentzVector allCands(0.,0.,0.,0.);
   for(typename PartColl::const_iterator part = partCollection.begin();
                                         part != partCollection.end(); ++part) allCands += part->p4();

   return allCands.M();
}

template <class Coll>
std::pair<double,double> xi(Coll& partCollection, double Ebeam){

   double xi_towers_plus = 0.;
   double xi_towers_minus = 0.;
   for(typename Coll::const_iterator part = partCollection.begin(); part != partCollection.end(); ++part){
     //double correction = (jetCorrector)?(jetCorrector->getCorrection(part->pt(),part->eta())):1.;
     xi_towers_plus += part->et()*TMath::Exp(part->eta());
     xi_towers_minus += part->et()*TMath::Exp(-part->eta());
   }

   xi_towers_plus /= 2*Ebeam;
   xi_towers_minus /= 2*Ebeam;
   
   return std::make_pair(xi_towers_plus,xi_towers_minus);
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
