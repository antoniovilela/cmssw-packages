#ifndef HFTowerEdmNtupleDumperAOD_h
#define HFTowerEdmNtupleDumperAOD_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/EventSetup.h"

class HFTowerEdmNtupleDumperAOD : public edm::EDProducer {
public:
  HFTowerEdmNtupleDumperAOD( const edm::ParameterSet & );
private:
  void produce( edm::Event &, const edm::EventSetup & );
  edm::InputTag calotowersLabel_;

  //double e_tresholdHF;	
  double etresh_min;
  double etresh_max;
  unsigned int n_iter;			
};

#endif
