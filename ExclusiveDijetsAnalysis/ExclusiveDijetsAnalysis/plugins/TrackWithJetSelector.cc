#include "FWCore/Framework/interface/MakerMacros.h"

#include "PhysicsTools/RecoAlgos/interface/TrackFullCloneSelectorBase.h"
#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/TrackWithJetSelector.h"

namespace reco { 
  namespace modules {

    typedef TrackFullCloneSelectorBase< ::TrackWithJetSelector > TrackWithJetSelector;

    DEFINE_FWK_MODULE(TrackWithJetSelector);
  }
}
