
#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/MinimumBiasAnalysis.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/EventData.h"
#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/FWLiteTools.h"
#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/FWLiteTools.h"

using namespace exclusiveDijetsAnalysis;

namespace minimumBiasAnalysis {

MinimumBiasAnalysis::MinimumBiasAnalysis(const edm::ParameterSet& pset):
  vertexTag_(pset.getParameter<edm::InputTag>("VertexTag")), 
  metTag_(pset.getParameter<edm::InputTag>("METTag")),
  jetTag_(pset.getParameter<edm::InputTag>("JetTag")),
  caloTowerTag_(pset.getParameter<edm::InputTag>("CaloTowerTag")),
  particleFlowTag_(pset.getParameter<edm::InputTag>("ParticleFlowTag")),
  thresholdHF_(pset.getParameter<unsigned int>("HFThresholdIndex")),
  energyThresholdHBHE_(pset.getParameter<double>("EnergyThresholdHBHE")),
  energyThresholdHF_(pset.getParameter<double>("EnergyThresholdHF")),
  Ebeam_(pset.getParameter<double>("EBeam")),
  accessMCInfo_(pset.getUntrackedParameter<bool>("AccessMCInfo",false)),
  genAllParticles_(0.,0.,0.,0.),genProtonPlus_(0.,0.,0.,0.),genProtonMinus_(0.,0.,0.,0.)
{
  thresholdsPFlow_[reco::PFCandidate::X] = std::make_pair(-1.,-1.);
  thresholdsPFlow_[reco::PFCandidate::h] = std::make_pair(-1.,-1.);
  thresholdsPFlow_[reco::PFCandidate::e] = std::make_pair(-1.,-1.); 
  thresholdsPFlow_[reco::PFCandidate::mu] = std::make_pair(-1.,-1.); 
  thresholdsPFlow_[reco::PFCandidate::gamma] = std::make_pair(-1.,-1.);
  thresholdsPFlow_[reco::PFCandidate::h0] = std::make_pair(-1.,-1.);
  thresholdsPFlow_[reco::PFCandidate::h_HF] = std::make_pair(-1.,-1.);
  thresholdsPFlow_[reco::PFCandidate::egamma_HF] = std::make_pair(-1.,-1.);
}

MinimumBiasAnalysis::~MinimumBiasAnalysis(){}

void MinimumBiasAnalysis::servicesBeginRun(const edm::Run& run, const edm::EventSetup& setup) {}

void MinimumBiasAnalysis::fillEventData(EventData& eventData, const edm::Event& event, const edm::EventSetup& setup){
  fillNoiseInfo(eventData,event,setup); 
  fillVertexInfo(eventData,event,setup);
  fillJetInfo(eventData,event,setup);
  fillEventVariables(eventData,event,setup);
}

void MinimumBiasAnalysis::fillNoiseInfo(EventData& eventData, const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<HcalNoiseSummary> noiseSummaryH;
  event.getByLabel("hcalnoise",noiseSummaryH);   

  bool passNoiseLoose = noiseSummaryH->passLooseNoiseFilter();
  bool passNoiseTight = noiseSummaryH->passTightNoiseFilter();

  edm::Handle<reco::BeamHaloSummary> beamHaloSummaryH;
  event.getByLabel("BeamHaloSummary",beamHaloSummaryH);

  bool beamHaloLooseId = beamHaloSummaryH->LooseId(); 
  bool beamHaloTightId = beamHaloSummaryH->TightId();

  eventData.LooseNoiseFilter_ = passNoiseLoose ? 1 : 0;
  eventData.TightNoiseFilter_ = passNoiseTight ? 1 : 0;

  eventData.BeamHaloLooseId_ = beamHaloLooseId ? 1 : 0;
  eventData.BeamHaloTightId_ = beamHaloTightId ? 1 : 0;
}

void MinimumBiasAnalysis::fillVertexInfo(EventData& eventData, const edm::Event& event, const edm::EventSetup& setup){
  // Access vertex collection
  edm::Handle<edm::View<reco::Vertex> > vertexCollectionH;
  event.getByLabel(vertexTag_,vertexCollectionH);
  const edm::View<reco::Vertex>& vtxColl = *(vertexCollectionH.product());

  // Find number of good vertices
  int nGoodVertices = 0;
  for(edm::View<reco::Vertex>::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
     if(!vtx->isValid()) continue; // skip non-valid vertices
     if(vtx->isFake()) continue; // skip vertex from beam spot
     ++nGoodVertices;
  } 
  eventData.nVertex_ = nGoodVertices;

  const reco::Vertex& primVertex = (*vertexCollectionH)[0];
  if(primVertex.isValid() && !primVertex.isFake()){
     eventData.posXPrimVtx_ = primVertex.x();
     eventData.posYPrimVtx_ = primVertex.y();
     eventData.posZPrimVtx_ = primVertex.z();
  } else{
     eventData.posXPrimVtx_ = -999.;
     eventData.posYPrimVtx_ = -999.;
     eventData.posZPrimVtx_ = -999.;
  }

}

void MinimumBiasAnalysis::fillMETInfo(EventData& eventData, const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<reco::MET> > metCollectionH;
  event.getByLabel(metTag_,metCollectionH);

  const reco::MET& met = (*metCollectionH)[0];
  double sumET = met.sumEt();
  double met_px = met.px();
  double met_py = met.py();
  double met_et = met.pt();
  eventData.sumET_ = sumET;
  eventData.MEx_ = met_px;
  eventData.MEy_ = met_py;
  eventData.MET_ = met_et;
}

void MinimumBiasAnalysis::fillJetInfo(EventData& eventData, const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);

  if(jetCollectionH->size() > 0){
     const reco::Jet& leadingJet = (*jetCollectionH)[0];

     eventData.leadingJetPt_ = leadingJet.pt();
     eventData.leadingJetEta_ = leadingJet.eta();
     eventData.leadingJetPhi_ = leadingJet.phi();
  } else{
     eventData.leadingJetPt_ = -999.;
     eventData.leadingJetEta_ = -999.;
     eventData.leadingJetPhi_ = -999.;
  }  
}

void MinimumBiasAnalysis::fillMultiplicities(EventData& eventData, const edm::Event& event, const edm::EventSetup& setup){
  // Access multiplicities
  edm::Handle<unsigned int> trackMultiplicity; 
  event.getByLabel("trackMultiplicity","trackMultiplicity",trackMultiplicity);
  
  edm::Handle<unsigned int> trackMultiplicityAssociatedToPV;
  event.getByLabel("trackMultiplicityAssociatedToPV","trackMultiplicity",trackMultiplicityAssociatedToPV);

  edm::Handle<std::vector<unsigned int> > nHFPlus;
  event.getByLabel("hfTower","nHFplus",nHFPlus);
  
  edm::Handle<std::vector<unsigned int> > nHFMinus;
  event.getByLabel("hfTower","nHFminus",nHFMinus);

  edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaPlus;
  event.getByLabel("hfTower","mapTreshToiEtaplus",mapThreshToiEtaPlus);

  edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaMinus;
  event.getByLabel("hfTower","mapTreshToiEtaminus",mapThreshToiEtaMinus);

  edm::Handle<std::vector<double> > sumEHFplus;
  event.getByLabel("hfTower","sumEHFplus",sumEHFplus);

  edm::Handle<std::vector<double> > sumEHFminus;
  event.getByLabel("hfTower","sumEHFminus",sumEHFminus);

  unsigned int nTracks = *trackMultiplicity;
  unsigned int nTracksAssociatedToPV = *trackMultiplicityAssociatedToPV;

  unsigned int nHF_plus = (*nHFPlus)[thresholdHF_];
  unsigned int nHF_minus = (*nHFMinus)[thresholdHF_];

  double sumE_plus = (*sumEHFplus)[thresholdHF_];
  double sumE_minus = (*sumEHFminus)[thresholdHF_];

  eventData.trackMultiplicity_ = nTracks;
  eventData.trackMultiplicityAssociatedToPV_ = nTracksAssociatedToPV;
  eventData.multiplicityHFPlus_ = nHF_plus;
  eventData.multiplicityHFMinus_ = nHF_minus;
  eventData.sumEnergyHFPlus_ = sumE_plus;
  eventData.sumEnergyHFMinus_ = sumE_minus;

  for(unsigned int ieta = 30, index = 0; ieta <= 41; ++ieta,++index){
     unsigned int nHFPlus_ieta = nHFSlice(*mapThreshToiEtaPlus,thresholdHF_,ieta);
     eventData.multiplicityHFPlusVsiEta_[index] = nHFPlus_ieta;

     unsigned int nHFMinus_ieta = nHFSlice(*mapThreshToiEtaMinus,thresholdHF_,ieta);
     eventData.multiplicityHFMinusVsiEta_[index] = nHFMinus_ieta;
  }
}

void MinimumBiasAnalysis::fillEventVariables(EventData& eventData, const edm::Event& event, const edm::EventSetup& setup){
  if(accessMCInfo_){
     // Gen particles
     edm::Handle<reco::GenParticleCollection> genParticlesCollectionH;
     event.getByLabel("genParticles",genParticlesCollectionH);
     const reco::GenParticleCollection& genParticles = *genParticlesCollectionH;   

     setGenInfo(genParticles,Ebeam_,genAllParticles_,genProtonPlus_,genProtonMinus_);

     double xigen_plus = -1.;
     double xigen_minus = -1.;
     if(genProtonPlus_.pz() > 0.75*Ebeam_) xigen_plus = 1 - genProtonPlus_.pz()/Ebeam_;
     if(genProtonMinus_.pz() < -0.75*Ebeam_) xigen_minus = 1 + genProtonMinus_.pz()/Ebeam_;

     eventData.xiGenPlus_ = xigen_plus;
     eventData.xiGenMinus_ = xigen_minus;
     eventData.MxGen_ = genAllParticles_.M();
  } else{
     eventData.xiGenPlus_ = -1.;
     eventData.xiGenMinus_ = -1.;
     eventData.MxGen_ = -1.;
  }

  /*edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);*/

  edm::Handle<CaloTowerCollection> caloTowerCollectionH;
  event.getByLabel(caloTowerTag_,caloTowerCollectionH);

  edm::Handle<reco::PFCandidateCollection> particleFlowCollectionH;
  event.getByLabel(particleFlowTag_,particleFlowCollectionH);

  //double MxFromJets = MassColl(*jetCollectionH,10.);
  double MxFromTowers = MassColl(*caloTowerCollectionH,-1.,energyThresholdHBHE_,energyThresholdHF_);
  double MxFromPFCands = MassColl(*particleFlowCollectionH,thresholdsPFlow_);
  eventData.MxFromTowers_ = MxFromTowers;
  eventData.MxFromPFCands_ = MxFromPFCands;

  //std::pair<double,double> xiFromJets = xi(*jetCollectionH,Ebeam_,10.);
  std::pair<double,double> xiFromTowers = xi(*caloTowerCollectionH,Ebeam_,-1.,energyThresholdHBHE_,energyThresholdHF_);
  double xiFromTowers_plus = xiFromTowers.first;
  double xiFromTowers_minus = xiFromTowers.second;
  std::pair<double,double> xiFromPFCands = xi(*particleFlowCollectionH,Ebeam_,thresholdsPFlow_);
  double xiFromPFCands_plus = xiFromPFCands.first;
  double xiFromPFCands_minus = xiFromPFCands.second;

  eventData.xiPlusFromTowers_ = xiFromTowers_plus;
  eventData.xiMinusFromTowers_ = xiFromTowers_minus;
  //eventData.xiPlusFromJets_ = xiFromJets_plus;
  //eventData.xiMinusFromJets_ = xiFromJets_minus;
  eventData.xiPlusFromPFCands_ = xiFromPFCands_plus;
  eventData.xiMinusFromPFCands_ = xiFromPFCands_minus;

  std::pair<double,double> EPlusPzFromTowers = EPlusPz(*caloTowerCollectionH,-1.,energyThresholdHBHE_,energyThresholdHF_);
  eventData.EPlusPzFromTowers_ = EPlusPzFromTowers.first;
  eventData.EMinusPzFromTowers_ = EPlusPzFromTowers.second;
  std::pair<double,double> EPlusPzFromPFCands = EPlusPz(*particleFlowCollectionH,thresholdsPFlow_);
  eventData.EPlusPzFromPFCands_ = EPlusPzFromPFCands.first;
  eventData.EMinusPzFromPFCands_ = EPlusPzFromPFCands.second;

  double missingMassFromXiFromTowers = 2*Ebeam_*sqrt(xiFromTowers_plus*xiFromTowers_minus);
  eventData.missingMassFromXiFromTowers_ = missingMassFromXiFromTowers;
  //double missingMassFromXiFromJets = 2*Ebeam_*sqrt(xiFromJets_plus*xiFromJets_minus);
  //eventData.missingMassFromXiFromJets_ = missingMassFromXiFromJets;
  double missingMassFromXiFromPFCands = 2*Ebeam_*sqrt(xiFromPFCands_plus*xiFromPFCands_minus);
  eventData.missingMassFromXiFromPFCands_ = missingMassFromXiFromPFCands;

  fillMultiplicities(eventData,event,setup);
}
} // namespace
