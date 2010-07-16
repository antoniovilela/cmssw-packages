#include "FWCore/Framework/interface/MakerMacros.h"

#include "CommonTools/RecoAlgos/interface/TrackFullCloneSelectorBase.h"
#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/TrackOutsideJetsSelector.h"

namespace reco { 
  namespace modules {

    typedef TrackFullCloneSelectorBase< ::TrackOutsideJetsSelector > TrackOutsideJetsSelector;

    DEFINE_FWK_MODULE(TrackOutsideJetsSelector);
  }
}
