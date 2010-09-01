#include "FWCore/Framework/interface/MakerMacros.h"

#include "CommonTools/RecoAlgos/interface/TrackFullCloneSelectorBase.h"
#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/TrackAnalysisSelector.h"

namespace reco { 
  namespace modules {

    typedef TrackFullCloneSelectorBase< minimumBiasAnalysis::TrackAnalysisSelector > TrackAnalysisSelector;

    DEFINE_FWK_MODULE(TrackAnalysisSelector);
  }
}
