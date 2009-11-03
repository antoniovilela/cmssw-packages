#ifndef ExclusiveDijetsAnalysis_ScaledObjectProducer_h
#define ExclusiveDijetsAnalysis_ScaledObjectProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"

template <class T>
class ScaledObjectProducer: public edm::EDProducer{
  public:
     explicit ScaledObjectProducer(const edm::ParameterSet&);
     ~ScaledObjectProducer() {}

     virtual void produce(edm::Event&,const edm::EventSetup&);
  private:
     edm::InputTag srcTag_;
     double scale_;  
};

#include "ScaledObjectProducer.icc"

#endif
