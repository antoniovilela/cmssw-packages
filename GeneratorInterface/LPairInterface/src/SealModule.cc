#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "GeneratorInterface/LPairInterface/interface/LPairSource.h"
//#include "GeneratorInterface/Pythia6Interface/interface/PythiaProducer.h"

  using edm::LPairSource;
  //using edm::PythiaProducer;

  DEFINE_SEAL_MODULE();
  DEFINE_ANOTHER_FWK_INPUT_SOURCE(LPairSource);
  //DEFINE_ANOTHER_FWK_MODULE(PythiaProducer);

