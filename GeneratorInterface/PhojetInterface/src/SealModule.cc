#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "GeneratorInterface/PhojetInterface/interface/PhojetSource.h"
#include "GeneratorInterface/PhojetInterface/interface/EmptyEventFilter.h"

  using edm::PhojetSource;

  DEFINE_SEAL_MODULE();
  DEFINE_ANOTHER_FWK_INPUT_SOURCE(PhojetSource);
  DEFINE_ANOTHER_FWK_MODULE(EmptyEventFilter); 
