
#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/MinimumBiasAnalysis.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "FWCore/Common/interface/TriggerNames.h" 
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "Utilities/AnalysisTools/interface/FWLiteTools.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/EventData.h"
#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/FWLiteTools.h"

#include <sstream>

namespace minimumBiasAnalysis {

MinimumBiasAnalysis::MinimumBiasAnalysis(const edm::ParameterSet& pset):
  vertexTag_(pset.getParameter<edm::InputTag>("VertexTag")),
  trackTag_(pset.getParameter<edm::InputTag>("TrackTag")),
  metTag_(pset.getParameter<edm::InputTag>("METTag")),
  jetTag_(pset.getParameter<edm::InputTag>("JetTag")),
  caloTowerTag_(pset.getParameter<edm::InputTag>("CaloTowerTag")),
  particleFlowTag_(pset.getParameter<edm::InputTag>("ParticleFlowTag")),
  genChargedTag_(pset.getParameter<edm::InputTag>("GenChargedParticlesTag")),
  triggerResultsTag_(pset.getParameter<edm::InputTag>("TriggerResultsTag")),
  hcalTowerSummaryTag_(pset.getParameter<edm::InputTag>("HCALTowerSummaryTag")),
  energyThresholdHB_(pset.getParameter<double>("EnergyThresholdHB")),
  energyThresholdHE_(pset.getParameter<double>("EnergyThresholdHE")),
  energyThresholdHF_(pset.getParameter<double>("EnergyThresholdHF")),
  Ebeam_(pset.getParameter<double>("EBeam")),
  applyEnergyScaleHCAL_(pset.getParameter<bool>("ApplyEnergyScaleHCAL")),
  energyScaleHCAL_(-1.),
  accessMCInfo_(pset.getUntrackedParameter<bool>("AccessMCInfo",false)),
  //hltPathNames_(pset.getParameter<std::vector<std::string> >("HLTPathNames")),
  hltPathName_(pset.getParameter<std::string>("HLTPath"))
  //selectionPathName_(pset.getParameter<std::string>("SelectionPath"))
{
  //FIXME
  /*thresholdsPFlow_[reco::PFCandidate::X] = std::make_pair(-1.,1.0);
  thresholdsPFlow_[reco::PFCandidate::h] = std::make_pair(0.5,-1.);
  thresholdsPFlow_[reco::PFCandidate::e] = std::make_pair(0.5,-1.);
  thresholdsPFlow_[reco::PFCandidate::mu] = std::make_pair(0.5,-1.);
  thresholdsPFlow_[reco::PFCandidate::gamma] = std::make_pair(0.5,-1.);
  thresholdsPFlow_[reco::PFCandidate::h0] = std::make_pair(-1.,std::max(energyThresholdHB_,energyThresholdHE_));
  thresholdsPFlow_[reco::PFCandidate::h_HF] = std::make_pair(-1.,energyThresholdHF_);
  thresholdsPFlow_[reco::PFCandidate::egamma_HF] = std::make_pair(-1.,energyThresholdHF_);*/
  resetPFThresholds(thresholdsPFlowBarrel_);
  resetPFThresholds(thresholdsPFlowEndcap_);
  resetPFThresholds(thresholdsPFlowTransition_);
  resetPFThresholds(thresholdsPFlowForward_);
  if(pset.exists("PFlowThresholds")){ 
     edm::ParameterSet const& thresholdsPFPset = pset.getParameter<edm::ParameterSet>("PFlowThresholds");
 
     edm::ParameterSet const& thresholdsBarrel = thresholdsPFPset.getParameter<edm::ParameterSet>("Barrel");
     edm::ParameterSet const& thresholdsEndcap = thresholdsPFPset.getParameter<edm::ParameterSet>("Endcap");
     edm::ParameterSet const& thresholdsTransition = thresholdsPFPset.getParameter<edm::ParameterSet>("Transition");
     edm::ParameterSet const& thresholdsForward = thresholdsPFPset.getParameter<edm::ParameterSet>("Forward");

     setPFThresholds(thresholdsPFlowBarrel_,thresholdsBarrel);
     setPFThresholds(thresholdsPFlowEndcap_,thresholdsEndcap);
     setPFThresholds(thresholdsPFlowTransition_,thresholdsTransition);
     setPFThresholds(thresholdsPFlowForward_,thresholdsForward);
  }
  thresholdsPFlow_[Barrel] = thresholdsPFlowBarrel_;
  thresholdsPFlow_[Endcap] = thresholdsPFlowEndcap_; 
  thresholdsPFlow_[Transition] = thresholdsPFlowTransition_;
  thresholdsPFlow_[Forward] = thresholdsPFlowForward_;

  std::map<int,std::pair<double,double> >::const_iterator pfThreshold = thresholdsPFlowBarrel_.begin();
  std::map<int,std::pair<double,double> >::const_iterator pfThresholds_end = thresholdsPFlowBarrel_.end(); 
  std::ostringstream oss;
  oss << "Using the following PF thresholds:\n";
  for(; pfThreshold != pfThresholds_end; ++pfThreshold){
     int key = pfThreshold->first;    
     oss << "  " << key << ": "
                 << "(" << thresholdsPFlow_[Barrel][key].first
                 << "," << thresholdsPFlow_[Barrel][key].second << ")  "
                 << "(" << thresholdsPFlow_[Endcap][key].first
                 << "," << thresholdsPFlow_[Endcap][key].second << ")  "
                 << "(" << thresholdsPFlow_[Transition][key].first
                 << "," << thresholdsPFlow_[Transition][key].second << ")  "
                 << "(" << thresholdsPFlow_[Forward][key].first
                 << "," << thresholdsPFlow_[Forward][key].second << ")\n";   
  }
  LogDebug("Analysis") << oss.str();

  if(applyEnergyScaleHCAL_) energyScaleHCAL_ = pset.getParameter<double>("EnergyScaleFactorHCAL");
}

MinimumBiasAnalysis::~MinimumBiasAnalysis(){}

void MinimumBiasAnalysis::servicesBeginRun(const edm::Run& run, const edm::EventSetup& setup) {}

void MinimumBiasAnalysis::fillEventData(EventData& eventData, const edm::Event& event, const edm::EventSetup& setup){
  fillEventInfo(eventData,event,setup);
  fillNoiseInfo(eventData,event,setup); 
  fillTriggerInfo(eventData,event,setup);
  //fillSelectionInfo(eventData,event,setup);
  fillVertexInfo(eventData,event,setup);
  fillTrackInfo(eventData,event,setup);
  fillJetInfo(eventData,event,setup);
  fillMETInfo(eventData,event,setup);
  fillEventVariables(eventData,event,setup);
}

void MinimumBiasAnalysis::fillEventInfo(EventData& eventData, const edm::Event& event, const edm::EventSetup& setup){
  if(accessMCInfo_){
     edm::Handle<GenEventInfoProduct> genEventInfo;
     event.getByLabel("generator",genEventInfo);

     int processId = -1;
     if(genEventInfo.isValid()){
        processId = genEventInfo->signalProcessID();
     } else {
        edm::Handle<edm::HepMCProduct> hepMCProduct;
        event.getByLabel("source",hepMCProduct);
        processId = hepMCProduct->getHepMCData().signal_process_id();
     }
     eventData.processId_ = processId;
  } else{
     eventData.processId_ = -1;
  } 

  unsigned int eventNumber = event.id().event();
  unsigned int runNumber = event.id().run();
  unsigned int lumiSection = event.luminosityBlock();
  int bunchCrossing = event.bunchCrossing();

  edm::Handle<double> lumiWeight;
  event.getByLabel("lumiWeight",lumiWeight);
  if(lumiWeight.isValid()){
     double lumi = *lumiWeight;
     eventData.lumiWeight_ = lumi;
  } else{
     eventData.lumiWeight_ = -1.; 
  }

  eventData.eventNumber_ = eventNumber;
  eventData.runNumber_ = runNumber;
  eventData.lumiSection_ = lumiSection;
  eventData.bunchCrossing_ = bunchCrossing;
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

void MinimumBiasAnalysis::fillTriggerInfo(EventData& eventData, const edm::Event& event, const edm::EventSetup& setup){
  //FIXME
  edm::Handle<edm::TriggerResults> triggerResults;
  event.getByLabel(triggerResultsTag_, triggerResults);

  //edm::TriggerNames triggerNames;
  //triggerNames.init(*triggerResults);
  const edm::TriggerNames& triggerNames = event.triggerNames(*triggerResults);

  /*std::vector<std::string>::const_iterator pathName = triggerResultsPaths_.begin();
  std::vector<std::string>::const_iterator pathNames_end = triggerResultsPaths_.end();
  for(; pathName != pathNames_end; ++pathName){
     int trigIdx = triggerNames.triggerIndex(*pathName);
     bool wasrun = triggerResults->wasrun(trigIdx);
     bool accept = triggerResults->accept(trigIdx);
  }*/
  /*int idx_HLT_MinBiasBSCOR = triggerNames.triggerIndex("HLT_MinBiasBSC_OR");
  int idx_HLT_MinBiasPixel = triggerNames.triggerIndex("HLT_MinBiasPixel_SingleTrack"); 
  eventData.HLT_MinBiasBSCOR_ = (triggerResults->wasrun(idx_HLT_MinBiasBSCOR) && triggerResults->accept(idx_HLT_MinBiasBSCOR)) ? 1 : 0;
  eventData.HLT_MinBiasPixel_ = (triggerResults->wasrun(idx_HLT_MinBiasPixel) && triggerResults->accept(idx_HLT_MinBiasPixel)) ? 1 : 0;*/
  unsigned int idxHLT = triggerNames.triggerIndex(hltPathName_);
  eventData.HLTPath_ = (triggerResults->wasrun(idxHLT) && triggerResults->accept(idxHLT)) ? 1 : 0; 
}

/*
void MinimumBiasAnalysis::fillSelectionInfo(EventData& eventData, const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::TriggerResults> triggerResults;
  event.getByLabel("TriggerResults", triggerResults);

  const edm::TriggerNames& triggerNames = event.triggerNames(*triggerResults);
  unsigned int idxPath = triggerNames.triggerIndex(selectionPathName_);
  if(idxPath >= triggerNames.size()) throw cms::Exception("Configuration") << "Could not find path with name "
                                                    << selectionPathName_ << " from "
                                                    << triggerResults.provenance()->moduleLabel() << "::"
                                                    << triggerResults.provenance()->processName() << "\n"; 
  //eventData.SelectionPath_ = (triggerResults->wasrun(idxPath) && triggerResults->accept(idxPath)) ? 1 : 0;
  eventData.SelectionPath_ = -1.; 
}
*/

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

void MinimumBiasAnalysis::fillTrackInfo(EventData& eventData, const edm::Event& event, const edm::EventSetup& setup){
  // Access collection
  edm::Handle<edm::View<reco::Track> > trackCollectionH;
  event.getByLabel(trackTag_,trackCollectionH);
  const edm::View<reco::Track>& trackColl = *(trackCollectionH.product());

  double ptSum = 0.;
  int nTracks = 0; 
  edm::View<reco::Track>::const_iterator track = trackColl.begin();
  edm::View<reco::Track>::const_iterator tracks_end = trackColl.end();
  for(; track != tracks_end; ++track){
     ptSum += track->pt();
     ++nTracks;
     // Other variables..
  }

  eventData.multiplicityTracks_ = nTracks;
  eventData.sumPtTracks_ = ptSum;

  if(accessMCInfo_){
     edm::Handle<reco::GenParticleCollection> genChargedParticlesH;
     event.getByLabel(genChargedTag_,genChargedParticlesH);
     const reco::GenParticleCollection& genChargedParticles = *genChargedParticlesH;
     reco::GenParticleCollection::const_iterator genpart = genChargedParticles.begin();
     reco::GenParticleCollection::const_iterator genparts_end = genChargedParticles.end();

     double ptSumGen = 0.;
     int nTracksGen = 0;
     for(; genpart != genparts_end; ++genpart){
        ptSumGen += genpart->pt();
        ++nTracksGen; 
        //...
     }
     eventData.multiplicityTracksGen_ = nTracksGen;
     eventData.sumPtTracksGen_ = ptSumGen;
  } else{
     eventData.multiplicityTracksGen_ = -1;
     eventData.sumPtTracksGen_ = -1.;
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
  /*edm::Handle<unsigned int> trackMultiplicity; 
  event.getByLabel("trackMultiplicity","trackMultiplicity",trackMultiplicity);
  
  edm::Handle<unsigned int> trackMultiplicityAssociatedToPV;
  event.getByLabel("trackMultiplicityAssociatedToPV","trackMultiplicity",trackMultiplicityAssociatedToPV);*/

  edm::Handle<std::vector<unsigned int> > nHEPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHEplus"),nHEPlus);
  
  edm::Handle<std::vector<unsigned int> > nHEMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHEminus"),nHEMinus);

  edm::Handle<std::vector<unsigned int> > nHFPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHFplus"),nHFPlus);
  
  edm::Handle<std::vector<unsigned int> > nHFMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHFminus"),nHFMinus);

  /*edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"mapThreshToiEtaplus"),mapThreshToiEtaPlus);

  edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"mapThreshToiEtaminus"),mapThreshToiEtaMinus);*/

  edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFMultiplicityPlus"),iEtaHFMultiplicityPlus);

  edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFMultiplicityMinus"),iEtaHFMultiplicityMinus);
  
  edm::Handle<std::vector<double> > sumEHEplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHEplus"),sumEHEplus);

  edm::Handle<std::vector<double> > sumEHEminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHEminus"),sumEHEminus);

  edm::Handle<std::vector<double> > sumETHEplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHEplus"),sumETHEplus);
 
  edm::Handle<std::vector<double> > sumETHEminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHEminus"),sumETHEminus);

  edm::Handle<std::vector<double> > sumEHFplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHFplus"),sumEHFplus);

  edm::Handle<std::vector<double> > sumEHFminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHFminus"),sumEHFminus);

  edm::Handle<std::map<unsigned int, std::vector<double> > > iEtaHFEnergySumPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFEnergySumPlus"),iEtaHFEnergySumPlus);

  edm::Handle<std::map<unsigned int, std::vector<double> > > iEtaHFEnergySumMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFEnergySumMinus"),iEtaHFEnergySumMinus);

  edm::Handle<std::vector<double> > sumETHFplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHFplus"),sumETHFplus);
  
  edm::Handle<std::vector<double> > sumETHFminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHFminus"),sumETHFminus);
 
  edm::Handle<std::map<unsigned int, std::vector<double> > > iEtaHFETSumPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFETSumPlus"),iEtaHFETSumPlus);

  edm::Handle<std::map<unsigned int, std::vector<double> > > iEtaHFETSumMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFETSumMinus"),iEtaHFETSumMinus);

  // FIXME
  edm::Handle<std::vector<double> > thresholdsHE;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"thresholdsHE"),thresholdsHE);
  size_t indexThresholdHE = std::lower_bound((*thresholdsHE).begin(),(*thresholdsHE).end(),energyThresholdHE_) - (*thresholdsHE).begin();

  edm::Handle<std::vector<double> > thresholdsHF;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"thresholdsHF"),thresholdsHF);
  size_t indexThresholdHF = std::lower_bound((*thresholdsHF).begin(),(*thresholdsHF).end(),energyThresholdHF_) - (*thresholdsHF).begin();
  
  /*unsigned int nTracks = *trackMultiplicity;
  unsigned int nTracksAssociatedToPV = *trackMultiplicityAssociatedToPV;*/

  unsigned int nHE_plus = (*nHEPlus)[indexThresholdHE];
  unsigned int nHE_minus = (*nHEMinus)[indexThresholdHE];

  unsigned int nHF_plus = (*nHFPlus)[indexThresholdHF];
  unsigned int nHF_minus = (*nHFMinus)[indexThresholdHF];

  double sumEHE_plus = (*sumEHEplus)[indexThresholdHE];
  double sumEHE_minus = (*sumEHEminus)[indexThresholdHE];

  double sumETHE_plus = (*sumETHEplus)[indexThresholdHE];
  double sumETHE_minus = (*sumETHEminus)[indexThresholdHE];

  double sumEHF_plus = (*sumEHFplus)[indexThresholdHF];
  double sumEHF_minus = (*sumEHFminus)[indexThresholdHF];

  double sumETHF_plus = (*sumETHFplus)[indexThresholdHF];
  double sumETHF_minus = (*sumETHFminus)[indexThresholdHF];
 
  /*eventData.trackMultiplicity_ = nTracks;
  eventData.trackMultiplicityAssociatedToPV_ = nTracksAssociatedToPV;*/
  eventData.multiplicityHEPlus_ = nHE_plus;
  eventData.multiplicityHEMinus_ = nHE_minus;
  eventData.multiplicityHFPlus_ = nHF_plus;
  eventData.multiplicityHFMinus_ = nHF_minus;
  eventData.sumEnergyHEPlus_ = sumEHE_plus;
  eventData.sumEnergyHEMinus_ = sumEHE_minus;
  eventData.sumETHEPlus_ = sumETHE_plus;
  eventData.sumETHEMinus_ = sumETHE_minus;
  eventData.sumEnergyHFPlus_ = sumEHF_plus;
  eventData.sumEnergyHFMinus_ = sumEHF_minus;
  eventData.sumETHFPlus_ = sumETHF_plus;
  eventData.sumETHFMinus_ = sumETHF_minus;

  for(unsigned int ieta = 29, index = 0; ieta <= 41; ++ieta,++index){
     unsigned int nHFPlus_ieta = analysisTools::nHCALiEta(*iEtaHFMultiplicityPlus,indexThresholdHF,ieta);
     eventData.multiplicityHFPlusVsiEta_[index] = nHFPlus_ieta;
     double sumEHFPlus_ieta = analysisTools::sumEHCALiEta(*iEtaHFEnergySumPlus,indexThresholdHF,ieta);
     eventData.sumEHFPlusVsiEta_[index] = sumEHFPlus_ieta;
     double sumETHFPlus_ieta = analysisTools::sumEHCALiEta(*iEtaHFETSumPlus,indexThresholdHF,ieta);
     eventData.sumETHFPlusVsiEta_[index] = sumETHFPlus_ieta; 

     unsigned int nHFMinus_ieta = analysisTools::nHCALiEta(*iEtaHFMultiplicityMinus,indexThresholdHF,ieta);
     eventData.multiplicityHFMinusVsiEta_[index] = nHFMinus_ieta;
     double sumEHFMinus_ieta = analysisTools::sumEHCALiEta(*iEtaHFEnergySumMinus,indexThresholdHF,ieta);
     eventData.sumEHFMinusVsiEta_[index] = sumEHFMinus_ieta;     
     double sumETHFMinus_ieta = analysisTools::sumEHCALiEta(*iEtaHFETSumMinus,indexThresholdHF,ieta);
     eventData.sumETHFMinusVsiEta_[index] = sumETHFMinus_ieta;
  }
}

void MinimumBiasAnalysis::fillEventVariables(EventData& eventData, const edm::Event& event, const edm::EventSetup& setup){
  if(accessMCInfo_){
     // Gen particles
     edm::Handle<reco::GenParticleCollection> genParticlesCollectionH;
     event.getByLabel("genParticles",genParticlesCollectionH);
     const reco::GenParticleCollection& genParticles = *genParticlesCollectionH;   

     math::XYZTLorentzVector genAllParticles(0.,0.,0.,0.),
                             genAllParticlesHEPlus(0.,0.,0.,0.),genAllParticlesHEMinus(0.,0.,0.,0.),
                             genAllParticlesHFPlus(0.,0.,0.,0.),genAllParticlesHFMinus(0.,0.,0.,0.),
                             genEtaMax(0.,0.,0.,0.),genEtaMin(0.,0.,0.,0.),
                             genProtonPlus(0.,0.,0.,0.),genProtonMinus(0.,0.,0.,0.);

     setGenInfo(genParticles,Ebeam_,genAllParticles,
                                    genAllParticlesHEPlus,genAllParticlesHEMinus,
                                    genAllParticlesHFPlus,genAllParticlesHFMinus,
                                    genEtaMax,genEtaMin, 
                                    genProtonPlus,genProtonMinus);

     double xigen_plus = -1.;
     double xigen_minus = -1.;
     if(genProtonPlus.pz() > 0.75*Ebeam_) xigen_plus = 1 - genProtonPlus.pz()/Ebeam_;
     if(genProtonMinus.pz() < -0.75*Ebeam_) xigen_minus = 1 + genProtonMinus.pz()/Ebeam_;

     eventData.xiGenPlus_ = xigen_plus;
     eventData.xiGenMinus_ = xigen_minus;
     eventData.MxGen_ = genAllParticles.mass();
     eventData.sumEnergyHEPlusGen_ = genAllParticlesHEPlus.energy();
     eventData.sumEnergyHEMinusGen_ = genAllParticlesHEMinus.energy();
     eventData.sumEnergyHFPlusGen_ = genAllParticlesHFPlus.energy();
     eventData.sumEnergyHFMinusGen_ = genAllParticlesHFMinus.energy();
     eventData.etaMaxGen_ = genEtaMax.eta();
     eventData.etaMinGen_ = genEtaMin.eta();
  } else{
     eventData.xiGenPlus_ = -1.;
     eventData.xiGenMinus_ = -1.;
     eventData.MxGen_ = -1.;
     eventData.sumEnergyHEPlusGen_ = -1.;
     eventData.sumEnergyHEMinusGen_ = -1.;
     eventData.sumEnergyHFPlusGen_ = -1.;
     eventData.sumEnergyHFMinusGen_ = -1.;
     eventData.etaMaxGen_ = -999.;
     eventData.etaMinGen_ = -999.;
  }

  /*edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);*/

  edm::Handle<CaloTowerCollection> caloTowerCollectionH;
  event.getByLabel(caloTowerTag_,caloTowerCollectionH);

  edm::Handle<reco::PFCandidateCollection> particleFlowCollectionH;
  event.getByLabel(particleFlowTag_,particleFlowCollectionH);

  double energyScale = (applyEnergyScaleHCAL_) ? energyScaleHCAL_ : -1.; 
  //double MxFromJets = MassColl(*jetCollectionH,10.);
  double MxFromTowers = analysisTools::MassColl(*caloTowerCollectionH,-1.,energyThresholdHB_,energyThresholdHE_,energyThresholdHF_,energyScale);
  double MxFromPFCands = MassColl(*particleFlowCollectionH,thresholdsPFlow_);
  eventData.MxFromTowers_ = MxFromTowers;
  eventData.MxFromPFCands_ = MxFromPFCands;

  //std::pair<double,double> xiFromJets = xi(*jetCollectionH,Ebeam_,10.);
  std::pair<double,double> xiFromTowers = analysisTools::xi(*caloTowerCollectionH,Ebeam_,-1.,energyThresholdHB_,energyThresholdHE_,energyThresholdHF_,energyScale);
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

  std::pair<double,double> EPlusPzFromTowers = analysisTools::EPlusPz(*caloTowerCollectionH,-1.,energyThresholdHB_,energyThresholdHE_,energyThresholdHF_,energyScale);
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

  std::pair<double,double> etaMaxFromPFCands = etaMax(*particleFlowCollectionH,thresholdsPFlow_);
  eventData.etaMaxFromPFCands_ = etaMaxFromPFCands.first;
  eventData.etaMinFromPFCands_ = etaMaxFromPFCands.second;

  fillMultiplicities(eventData,event,setup);
}

void MinimumBiasAnalysis::resetPFThresholds(std::map<int,std::pair<double,double> >& thresholdsPFlow){
  thresholdsPFlow[reco::PFCandidate::X] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::h] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::e] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::mu] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::gamma] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::h0] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::h_HF] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::egamma_HF] = std::make_pair(-1.,-1.);
}

void MinimumBiasAnalysis::setPFThresholds(std::map<int,std::pair<double,double> >& thresholdsPFlow, edm::ParameterSet const& thresholdsPFPset){
  std::vector<std::string> pfThresholdNames = thresholdsPFPset.getParameterNames();
  std::vector<std::string>::const_iterator param = pfThresholdNames.begin();
  std::vector<std::string>::const_iterator params_end = pfThresholdNames.end();
  for(; param != params_end; ++param){
     //reco::PFCandidate::ParticleType particleType = pflowId(*param);
     int particleType = pflowId(*param);
     if(particleType == -1)
        throw cms::Exception("Configuration") << "Parameter " << *param
                                              << " does not correspond to any particle type (PF)";

     edm::ParameterSet const& typePSet = thresholdsPFPset.getParameter<edm::ParameterSet>(*param);
     double ptThreshold = typePSet.getParameter<double>("ptMin");
     double energyThreshold = typePSet.getParameter<double>("energyMin");
     thresholdsPFlow[particleType].first = ptThreshold;
     thresholdsPFlow[particleType].second = energyThreshold;
  }
}

} // namespace
