#ifndef MinimumBiasAnalysis_FWLiteTools_h
#define MinimumBiasAnalysis_FWLiteTools_h

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

void setGenInfo(reco::GenParticleCollection const& genParticles, double Ebeam,
                                                                 math::XYZTLorentzVector& genAllParticles,
                                                                 math::XYZTLorentzVector& genProtonPlus,
                                                                 math::XYZTLorentzVector& genProtonMinus){
   /*fwlite::Handle<std::vector<reco::GenParticle> > genParticlesCollection;
   genParticlesCollection.getByLabel(ev,"genParticles");
   const reco::GenParticleCollection& genParticles = *genParticlesCollection;*/

   math::XYZTLorentzVector allGenParticles(0.,0.,0.,0.);
   
   reco::GenParticleCollection::const_iterator proton1 = genParticles.end();
   reco::GenParticleCollection::const_iterator proton2 = genParticles.end();
   for(reco::GenParticleCollection::const_iterator genpart = genParticles.begin(); genpart != genParticles.end(); ++genpart){
      if(genpart->status() != 1) continue;
      //histosTH1F["EnergyVsEta"]->Fill(genpart->eta(),genpart->energy());      
      if(fabs(genpart->eta()) < 5.0) allGenParticles += genpart->p4();

      double pz = genpart->pz();
      if((proton1 == genParticles.end())&&(genpart->pdgId() == 2212)&&(pz > 0.75*Ebeam)) proton1 = genpart;
      else if((proton2 == genParticles.end())&&(genpart->pdgId() == 2212)&&(pz < -0.75*Ebeam)) proton2 = genpart;
   }
   /*// Subtract the two protons' momenta
   if(proton1 != genParticles.end()) allGenParticles -= proton1->p4();
   if(proton2 != genParticles.end()) allGenParticles -= proton2->p4();*/

   // Commit
   genAllParticles.SetPxPyPzE(allGenParticles.px(),allGenParticles.py(),
                              allGenParticles.pz(),allGenParticles.energy());
   if(proton1 != genParticles.end()) genProtonPlus.SetPxPyPzE(proton1->px(),proton1->py(),
                                                              proton1->pz(),proton1->energy());
   if(proton2 != genParticles.end()) genProtonMinus.SetPxPyPzE(proton2->px(),proton2->py(),
                                                               proton2->pz(),proton2->energy());
}
#endif
