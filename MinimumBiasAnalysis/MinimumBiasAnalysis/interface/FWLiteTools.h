#ifndef MinimumBiasAnalysis_FWLiteTools_h
#define MinimumBiasAnalysis_FWLiteTools_h

#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"

#include <vector>
#include <algorithm>
#endif

namespace minimumBiasAnalysis {

enum calo_region_t {Barrel,Endcap,Transition,Forward};

bool greaterEta( const math::XYZTLorentzVector& a, const math::XYZTLorentzVector& b){ 
   return a.eta() > b.eta();
}

void genRapidityGap(reco::GenParticleCollection const& genParticles, math::XYZTLorentzVector& genGapLow,
                                                                     math::XYZTLorentzVector& genGapHigh){
   // Copy and sort gen particles in eta
   std::vector<math::XYZTLorentzVector> genParticlesSort(0);
   double etaEdgeLow = -999.0;
   double etaEdgeHigh = 999.0;
   reco::GenParticleCollection::const_iterator genpart = genParticles.begin();
   reco::GenParticleCollection::const_iterator genpart_end = genParticles.end();  
   for(; genpart != genpart_end; ++genpart){
      if( genpart->status() != 1 ) continue;

      if((genpart->eta() >= etaEdgeLow) && (genpart->eta() <= etaEdgeHigh))
         genParticlesSort.push_back( genpart->p4() );
   }
   std::stable_sort(genParticlesSort.begin(), genParticlesSort.end(), greaterEta);

   // Cases: 0, 1 or > 1 particles in selected range
   math::XYZTLorentzVector def_vec(0.,0.,0.,0.);
   if( genParticlesSort.size() == 0 ){
      genGapLow = def_vec; genGapHigh = def_vec;
      return;
   } else if( genParticlesSort.size() == 1 ){
      genGapLow = def_vec;
      genGapHigh = genParticlesSort[0];
      return;
   } else{
      //FIXME; There must be a STL algorithm for this
      double deltaEtaMax = 0.;
      std::vector<math::XYZTLorentzVector>::const_iterator genPartDeltaEtaMax = genParticlesSort.end();
      std::vector<math::XYZTLorentzVector>::const_iterator genpart = genParticlesSort.begin();
      std::vector<math::XYZTLorentzVector>::const_iterator genpart_end = genParticlesSort.end();
      for(; genpart != genpart_end; ++genpart){
         std::vector<math::XYZTLorentzVector>::const_iterator next = genpart + 1;
         double deltaEta = ( next != genpart_end ) ? ( next->eta() - genpart->eta() ) : 0.;
         if( deltaEta > deltaEtaMax ){
            deltaEtaMax = deltaEta;
            genPartDeltaEtaMax = genpart;
         } 
      }
      if( genPartDeltaEtaMax != genpart_end ){
         std::vector<math::XYZTLorentzVector>::const_iterator next = genPartDeltaEtaMax + 1;
         if( next != genpart_end ){
            genGapLow = (*genPartDeltaEtaMax);
            genGapHigh = (*next);
         } else{
            genGapLow = def_vec;
            genGapHigh = (*genPartDeltaEtaMax);
         }
      } else{
         genGapLow = def_vec; genGapHigh = def_vec;
         return;
      }
   } 
   
}

void setGenInfo(reco::GenParticleCollection const& genParticles, double Ebeam,
                                                                 math::XYZTLorentzVector& genAllParticles,
                                                                 math::XYZTLorentzVector& genAllParticlesInRange,
                                                                 math::XYZTLorentzVector& genAllParticlesHEPlus,
                                                                 math::XYZTLorentzVector& genAllParticlesHEMinus,
                                                                 math::XYZTLorentzVector& genAllParticlesHFPlus,
                                                                 math::XYZTLorentzVector& genAllParticlesHFMinus,
                                                                 math::XYZTLorentzVector& genEtaMax,
                                                                 math::XYZTLorentzVector& genEtaMin,
                                                                 math::XYZTLorentzVector& genProtonPlus,
                                                                 math::XYZTLorentzVector& genProtonMinus){

   math::XYZTLorentzVector allGenParticles(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesInRange(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHEPlus(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHEMinus(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHFPlus(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHFMinus(0.,0.,0.,0.);
   
   reco::GenParticleCollection::const_iterator proton1 = genParticles.end();
   reco::GenParticleCollection::const_iterator proton2 = genParticles.end();
   for(reco::GenParticleCollection::const_iterator genpart = genParticles.begin(); genpart != genParticles.end();
 ++genpart){
      if( genpart->status() != 1 ) continue;
      if( genpart->pdgId() != 2212 ) continue;
 
      if( ( genpart->pz() > 0.50*Ebeam ) && ( ( proton1 == genParticles.end() ) ||
                                              ( genpart->pz() > proton1->pz() ) ) ) proton1 = genpart;
      if( ( genpart->pz() < -0.50*Ebeam ) && ( ( proton2 == genParticles.end() ) ||
                                               ( genpart->pz() < proton2->pz() ) ) ) proton2 = genpart;
   }

   reco::GenParticleCollection::const_iterator etaMaxParticle = genParticles.end();
   reco::GenParticleCollection::const_iterator etaMinParticle = genParticles.end(); 
   for(reco::GenParticleCollection::const_iterator genpart = genParticles.begin(); genpart != genParticles.end(); ++genpart){
      if(genpart->status() != 1) continue;

      allGenParticles += genpart->p4();
      if(fabs(genpart->eta()) < 5.0) allGenParticlesInRange += genpart->p4();
      if( (genpart->eta() >= 1.3) && (genpart->eta() < 3.0) ) allGenParticlesHEPlus += genpart->p4();
      if( (genpart->eta() > -3.0) && (genpart->eta() <= -1.3) ) allGenParticlesHEMinus += genpart->p4();
      if( (genpart->eta() >= 3.0) && (genpart->eta() < 5.0) ) allGenParticlesHFPlus += genpart->p4();
      if( (genpart->eta() > -5.0) && (genpart->eta() <= -3.0) ) allGenParticlesHFMinus += genpart->p4(); 

      if( (genpart != proton1) && (genpart != proton2) ){
         if( ( etaMaxParticle == genParticles.end() ) ||
             ( genpart->eta() > etaMaxParticle->eta() ) ) etaMaxParticle = genpart;
         if( ( etaMinParticle == genParticles.end() ) ||
             ( genpart->eta() < etaMinParticle->eta() ) ) etaMinParticle = genpart;
      }
   }

   // Commit
   if( proton1 != genParticles.end() ) allGenParticles -= proton1->p4();
   if( proton2 != genParticles.end() ) allGenParticles -= proton2->p4();

   genAllParticles = allGenParticles;
   genAllParticlesInRange = allGenParticlesInRange;
   genAllParticlesHEPlus = allGenParticlesHEPlus;
   genAllParticlesHEMinus = allGenParticlesHEMinus;
   genAllParticlesHFPlus = allGenParticlesHFPlus;
   genAllParticlesHFMinus = allGenParticlesHFMinus;
 
   if( proton1 != genParticles.end() ) genProtonPlus = proton1->p4();
   if( proton2 != genParticles.end() ) genProtonMinus = proton2->p4();

   if( etaMaxParticle != genParticles.end() ) genEtaMax = etaMaxParticle->p4();
   if( etaMinParticle != genParticles.end() ) genEtaMin = etaMinParticle->p4();
}

int pflowId(std::string const& name){
   // FIXME: The labels definition could go somewhere else
   std::vector<std::string> labels_X, labels_h, labels_e, labels_mu, labels_gamma, labels_h0, labels_h_HF, labels_egamma_HF;
   labels_X.push_back("X");
   labels_X.push_back("undefined");
   labels_h.push_back("h");
   labels_h.push_back("chargedHadron");
   labels_h.push_back("hadronCharged");
   labels_e.push_back("e");
   labels_e.push_back("electron");
   labels_mu.push_back("mu");
   labels_mu.push_back("muon");
   labels_gamma.push_back("gamma");
   labels_gamma.push_back("photon");
   labels_h0.push_back("h0");
   labels_h0.push_back("neutralHadron");
   labels_h0.push_back("hadronNeutral");
   labels_h_HF.push_back("h_HF");
   labels_h_HF.push_back("hadronHF");
   labels_egamma_HF.push_back("egamma_HF");
   labels_egamma_HF.push_back("emHF");
   // Find corresponding particle type   
   if( std::find(labels_X.begin(), labels_X.end(), name) != labels_X.end() )
      return reco::PFCandidate::X;
   else if( std::find(labels_h.begin(), labels_h.end(), name) != labels_h.end() )
      return reco::PFCandidate::h;
   else if( std::find(labels_e.begin(), labels_e.end(), name) != labels_e.end() )
      return reco::PFCandidate::e;
   else if( std::find(labels_mu.begin(), labels_mu.end(), name) != labels_mu.end() )
      return reco::PFCandidate::mu;
   else if( std::find(labels_gamma.begin(), labels_gamma.end(), name) != labels_gamma.end() ) 
      return reco::PFCandidate::gamma;
   else if( std::find(labels_h0.begin(), labels_h0.end(), name) != labels_h0.end() ) 
      return reco::PFCandidate::h0;
   else if( std::find(labels_h_HF.begin(), labels_h_HF.end(), name) != labels_h_HF.end() ) 
      return reco::PFCandidate::h_HF;
   else if( std::find(labels_egamma_HF.begin(), labels_egamma_HF.end(), name) != labels_egamma_HF.end() ) 
      return reco::PFCandidate::egamma_HF;
   else return -1;
}

bool pflowThreshold(reco::PFCandidate const& part, std::map<int, std::map<int,std::pair<double,double> > > const& thresholdMap){

   //FIXME
   // HF eta rings 29, 40, 41
   if( ( (fabs(part.eta()) >= 2.866) && (fabs(part.eta()) < 2.976) ) || 
         (fabs(part.eta()) >= 4.730) ) return false;

   bool accept = true;

   double eta = part.eta();
   int region = -1;
   if( (fabs(eta) >= 0.) && (fabs(eta) < 1.4) ) region = Barrel;
   else if( (fabs(eta) >= 1.4) && (fabs(eta) < 2.6) ) region = Endcap;
   else if( (fabs(eta) >= 2.6) && (fabs(eta) < 3.2) ) region = Transition;
   else if( (fabs(eta) >= 3.2) ) region = Forward;
   std::map<int,std::pair<double,double> > const& thresholds = thresholdMap.find(region)->second;
   
   double ptThreshold = -1.0;
   double eThreshold = -1.0;
   int partType = part.particleId();
   std::map<int,std::pair<double,double> >::const_iterator it_threshold = thresholds.find(partType);
   if(it_threshold != thresholds.end()) {
      ptThreshold = it_threshold->second.first;
      eThreshold = it_threshold->second.second;
   }

   if(part.pt() < ptThreshold) accept = false;
   if(part.energy() < eThreshold) accept = false;

   return accept;
}

/*bool caloTowerNoiseAccept(CaloTower const& tower, double emFracThreshold = 1.){
   bool accept = true;

   double emEnergy = tower.emEnergy();
   double hadEnergy = tower.hadEnergy();
   double emFrac = fabs(emEnergy/(emEnergy+hadEnergy));
   if(emFrac > emFracThreshold) accept = false;

   return accept;
}*/

double MassDissGen(reco::GenParticleCollection const& genParticles, double rangeEtaMin = -999.,
                                                                    double rangeEtaMax = 999.){
                                                                    
   math::XYZTLorentzVector allGenParticles(0.,0.,0.,0.);
   reco::GenParticleCollection::const_iterator genpart = genParticles.begin();
   reco::GenParticleCollection::const_iterator genpart_end = genParticles.end();
   for(; genpart != genpart_end; ++genpart){
      if( genpart->status() != 1 ) continue;

      if( (genpart->eta() >= rangeEtaMin) && (genpart->eta() <= rangeEtaMax) ) allGenParticles += genpart->p4();
   }
   return allGenParticles.M();
}

// FIXME: Generalize for any collection with changeable threshold scheme
double MassColl(reco::PFCandidateCollection const& pflowCollection, std::map<int, std::map<int,std::pair<double,double> > > const& thresholdMap){
   math::XYZTLorentzVector allCands(0.,0.,0.,0.);
   reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
   reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
   for(; part != pfCands_end; ++part){
      if(pflowThreshold(*part,thresholdMap)) allCands += part->p4();
   }

   return allCands.M();
}

std::pair<double,double> xi(reco::PFCandidateCollection const& pflowCollection, double Ebeam, std::map<int, std::map<int,std::pair<double,double> > > const& thresholdMap){

   double xi_towers_plus = 0.;
   double xi_towers_minus = 0.;
   reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
   reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
   for(; part != pfCands_end; ++part){
     if(!pflowThreshold(*part,thresholdMap)) continue;

     xi_towers_plus += part->et()*TMath::Exp(part->eta());
     xi_towers_minus += part->et()*TMath::Exp(-part->eta());
   }

   xi_towers_plus /= 2*Ebeam;
   xi_towers_minus /= 2*Ebeam;
   
   return std::make_pair(xi_towers_plus,xi_towers_minus);
}

std::pair<double,double> EPlusPz(reco::PFCandidateCollection const& pflowCollection, std::map<int, std::map<int,std::pair<double,double> > > const& thresholdMap){
   double e_plus_pz = 0.;
   double e_minus_pz = 0.;
   reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
   reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
   for(; part != pfCands_end; ++part){
      if(!pflowThreshold(*part,thresholdMap)) continue;

      e_plus_pz += part->energy() + part->pz(); 
      e_minus_pz += part->energy() - part->pz();
   }

   return std::make_pair(e_plus_pz,e_minus_pz);
}

std::pair<double,double> etaMax(reco::PFCandidateCollection const& pflowCollection, std::map<int, std::map<int,std::pair<double,double> > > const& thresholdMap){
   std::vector<double> etaCands;
   reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
   reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
   for(; part != pfCands_end; ++part){                           
      if(!pflowThreshold(*part,thresholdMap)) continue;            
      etaCands.push_back( part->eta() );
   }                                                             
   double eta_max = etaCands.size() ? *( std::max_element(etaCands.begin(), etaCands.end()) ) : -999.;
   double eta_min = etaCands.size() ? *( std::min_element(etaCands.begin(), etaCands.end()) ) : -999.;

   return std::make_pair(eta_max,eta_min);
}

} // namespace
#endif
