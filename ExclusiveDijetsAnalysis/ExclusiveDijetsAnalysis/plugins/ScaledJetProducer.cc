#include "FWCore/Framework/interface/MakerMacros.h"

#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/ScaledObjectProducer.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

typedef ScaledObjectProducer<reco::Jet> ScaledJetProducer;
typedef ScaledObjectProducer<pat::Jet> ScaledPATJetProducer;

DEFINE_FWK_MODULE(ScaledJetProducer);
DEFINE_FWK_MODULE(ScaledPATJetProducer);
