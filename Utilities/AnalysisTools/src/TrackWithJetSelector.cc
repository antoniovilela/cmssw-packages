#include "Utilities/AnalysisTools/interface/TrackWithJetSelector.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Math/interface/deltaPhi.h"

using namespace reco;

TrackWithJetSelector::TrackWithJetSelector(const edm::ParameterSet& pset):
   jetTag_(pset.getParameter<edm::InputTag>("JetTag")),
   deltaPhiMin_(pset.getParameter<double>("DeltaPhiMin")),
   deltaPhiMax_(pset.getParameter<double>("DeltaPhiMax")){}

TrackWithJetSelector::~TrackWithJetSelector() {}

bool TrackWithJetSelector::operator()(const Track& track, const edm::Event& event) const {
   edm::Handle<edm::View<Candidate> > jetCollectionH;
   event.getByLabel(jetTag_,jetCollectionH);
   const Candidate& leadingJet = *(jetCollectionH->begin());
 
   double dphi = fabs(leadingJet.phi() - track.phi());

   bool accept = (dphi >= deltaPhiMin_)&&(dphi < deltaPhiMax_);
  
   return accept;
}
