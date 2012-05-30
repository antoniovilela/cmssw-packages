#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhojetSource.h"
#include "EmptyEventFilter.h"

  using edm::PhojetSource;

  DEFINE_FWK_INPUT_SOURCE(PhojetSource);
  DEFINE_FWK_MODULE(EmptyEventFilter); 
