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

void setGenInfo(reco::GenParticleCollection const& genParticles, double Ebeam,
                                                                 math::XYZTLorentzVector& genAllParticles,
                                                                 math::XYZTLorentzVector& genAllParticlesHEPlus,
                                                                 math::XYZTLorentzVector& genAllParticlesHEMinus,
                                                                 math::XYZTLorentzVector& genAllParticlesHFPlus,
                                                                 math::XYZTLorentzVector& genAllParticlesHFMinus,
                                                                 math::XYZTLorentzVector& genProtonPlus,
                                                                 math::XYZTLorentzVector& genProtonMinus){
   /*fwlite::Handle<std::vector<reco::GenParticle> > genParticlesCollection;
   genParticlesCollection.getByLabel(ev,"genParticles");
   const reco::GenParticleCollection& genParticles = *genParticlesCollection;*/

   math::XYZTLorentzVector allGenParticles(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHEPlus(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHEMinus(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHFPlus(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHFMinus(0.,0.,0.,0.);
   
   reco::GenParticleCollection::const_iterator proton1 = genParticles.end();
   reco::GenParticleCollection::const_iterator proton2 = genParticles.end();
   for(reco::GenParticleCollection::const_iterator genpart = genParticles.begin(); genpart != genParticles.end(); ++genpart){
      if(genpart->status() != 1) continue;
      //histosTH1F["EnergyVsEta"]->Fill(genpart->eta(),genpart->energy());      
      if(fabs(genpart->eta()) < 5.0) allGenParticles += genpart->p4();
      if( (genpart->eta() >= 1.3) && (genpart->eta() < 3.0) ) allGenParticlesHEPlus += genpart->p4();
      if( (genpart->eta() > -3.0) && (genpart->eta() <= -1.3) ) allGenParticlesHEMinus += genpart->p4();
      if( (genpart->eta() >= 3.0) && (genpart->eta() < 5.0) ) allGenParticlesHFPlus += genpart->p4();
      if( (genpart->eta() > -5.0) && (genpart->eta() <= -3.0) ) allGenParticlesHFMinus += genpart->p4(); 

      double pz = genpart->pz();
      if((proton1 == genParticles.end())&&(genpart->pdgId() == 2212)&&(pz > 0.75*Ebeam)) proton1 = genpart;
      else if((proton2 == genParticles.end())&&(genpart->pdgId() == 2212)&&(pz < -0.75*Ebeam)) proton2 = genpart;
   }
   /*// Subtract the two protons' momenta
   if(proton1 != genParticles.end()) allGenParticles -= proton1->p4();
   if(proton2 != genParticles.end()) allGenParticles -= proton2->p4();*/

   // Commit
   genAllParticles = allGenParticles;
   genAllParticlesHEPlus = allGenParticlesHEPlus;
   genAllParticlesHEMinus = allGenParticlesHEMinus;
   genAllParticlesHFPlus = allGenParticlesHFPlus;
   genAllParticlesHFMinus = allGenParticlesHFMinus;
 
   if(proton1 != genParticles.end()) genProtonPlus = proton1->p4();
   if(proton2 != genParticles.end()) genProtonMinus = proton2->p4();
}

bool pflowThreshold(reco::PFCandidate const& part, std::map<int,std::pair<double,double> > const& thresholds){

   bool accept = true;

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

// FIXME: Generalize for any collection with changeable threshold scheme
double MassColl(reco::PFCandidateCollection const& pflowCollection, std::map<int,std::pair<double,double> > const& thresholds){
   math::XYZTLorentzVector allCands(0.,0.,0.,0.);
   reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
   reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
   for(; part != pfCands_end; ++part){
      if(pflowThreshold(*part,thresholds)) allCands += part->p4();
   }

   return allCands.M();
}

std::pair<double,double> xi(reco::PFCandidateCollection const& pflowCollection, double Ebeam,  std::map<int,std::pair<double,double> > const& thresholds){

   double xi_towers_plus = 0.;
   double xi_towers_minus = 0.;
   reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
   reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
   for(; part != pfCands_end; ++part){
     if(!pflowThreshold(*part,thresholds)) continue;

     xi_towers_plus += part->et()*TMath::Exp(part->eta());
     xi_towers_minus += part->et()*TMath::Exp(-part->eta());
   }

   xi_towers_plus /= 2*Ebeam;
   xi_towers_minus /= 2*Ebeam;
   
   return std::make_pair(xi_towers_plus,xi_towers_minus);
}

std::pair<double,double> EPlusPz(reco::PFCandidateCollection const& pflowCollection, std::map<int,std::pair<double,double> > const& thresholds){
   double e_plus_pz = 0.;
   double e_minus_pz = 0.;
   reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
   reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
   for(; part != pfCands_end; ++part){
      if(!pflowThreshold(*part,thresholds)) continue;

      e_plus_pz += part->energy() + part->pz(); 
      e_minus_pz += part->energy() - part->pz();
   }

   return std::make_pair(e_plus_pz,e_minus_pz);
}

std::pair<double,double> etaMax(reco::PFCandidateCollection const& pflowCollection, std::map<int,std::pair<double,double> > const& thresholds){
   std::vector<double> etaCands;
   reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
   reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
   for(; part != pfCands_end; ++part){                           
      if(!pflowThreshold(*part,thresholds)) continue;            
      etaCands.push_back( part->eta() );
   }                                                             
   double eta_max = *( std::max_element(etaCands.begin(), etaCands.end()) );
   double eta_min = *( std::min_element(etaCands.begin(), etaCands.end()) );

   return std::make_pair(eta_max,eta_min);
}

} // namespace
#endif
