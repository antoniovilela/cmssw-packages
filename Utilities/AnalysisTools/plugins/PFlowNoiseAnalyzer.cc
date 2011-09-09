// user include files
#include "PFlowNoiseAnalyzer.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"


#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h" 

using namespace reco;
 
PFlowNoiseAnalyzer::PFlowNoiseAnalyzer(const edm::ParameterSet& pset):
  particleFlowTag_(pset.getParameter<edm::InputTag>("particleFlowTag")){

}  
  
PFlowNoiseAnalyzer::~PFlowNoiseAnalyzer()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

void PFlowNoiseAnalyzer::beginJob(){

  edm::Service<TFileService> fs;
  TH1::SetDefaultSumw2(true);

  histosTH1F_["NEvents"] = fs->make<TH1F>("NEvents","NEvents",1,0,1);

  double etaBinsHCALBoundaries[] = {-5.205, -4.903, -4.730,
                                    -4.552, -4.377, -4.204, -4.027, -3.853, -3.677, -3.503, -3.327, -3.152,
                                    -3.000, -2.868, -2.650, -2.500,
                                    -2.322, -2.172, -2.043, -1.930, -1.830, -1.740, -1.653, -1.566, -1.479,
                                    -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.870, -0.783,
                                    -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087,
                                    0.000, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696,
                                    0.783, 0.870, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392,
                                    1.479, 1.566, 1.653, 1.740, 1.830, 1.930, 2.043, 2.172, 2.322,
                                    2.500, 2.650, 2.868, 3.000,
                                    3.152, 3.327, 3.503, 3.677, 3.853, 4.027, 4.204, 4.377, 4.552,
                                    4.730, 4.903, 5.205}; // 41 + 41 bins

  double energyMin = -1.0;
  double energyMax = 10.0;
  int nBinsEnergy = 200;
  histosTH2F_["energyVsEtaAllTypes"] = fs->make<TH2F>("energyVsEtaAllTypes","energyVsEtaAllTypes",82,etaBinsHCALBoundaries,nBinsEnergy,energyMin,energyMax);
  histosTH2F_["energyVsEtaUndefined"] = fs->make<TH2F>("energyVsEtaUndefined","energyVsEtaUndefined",82,etaBinsHCALBoundaries,nBinsEnergy,energyMin,energyMax);
  histosTH2F_["energyVsEtaChargedHadron"] = fs->make<TH2F>("energyVsEtaChargedHadron","energyVsEtaChargedHadron",82,etaBinsHCALBoundaries,nBinsEnergy,energyMin,energyMax);
  histosTH2F_["energyVsEtaElectron"] = fs->make<TH2F>("energyVsEtaElectron","energyVsEtaElectron",82,etaBinsHCALBoundaries,nBinsEnergy,energyMin,energyMax);
  histosTH2F_["energyVsEtaMuon"] = fs->make<TH2F>("energyVsEtaMuon","energyVsEtaMuon",82,etaBinsHCALBoundaries,nBinsEnergy,energyMin,energyMax);
  histosTH2F_["energyVsEtaGamma"] = fs->make<TH2F>("energyVsEtaGamma","energyVsEtaGamma",82,etaBinsHCALBoundaries,nBinsEnergy,energyMin,energyMax);
  histosTH2F_["energyVsEtaNeutralHadron"] = fs->make<TH2F>("energyVsEtaNeutralHadron","energyVsEtaNeutralHadron",82,etaBinsHCALBoundaries,nBinsEnergy,energyMin,energyMax);
  histosTH2F_["energyVsEtaHadronHF"] = fs->make<TH2F>("energyVsEtaHadronHF","energyVsEtaHadronHF",82,etaBinsHCALBoundaries,nBinsEnergy,energyMin,energyMax);
  histosTH2F_["energyVsEtaEGammaHF"] = fs->make<TH2F>("energyVsEtaEGammaHF","energyVsEtaEGammaHF",82,etaBinsHCALBoundaries,nBinsEnergy,energyMin,energyMax);
   
}     

void PFlowNoiseAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup) {

  histosTH1F_["NEvents"]->Fill(0.);

  edm::Handle<reco::PFCandidateCollection> particleFlowCollectionH;
  event.getByLabel(particleFlowTag_,particleFlowCollectionH);
  reco::PFCandidateCollection const& pflowCollection = *particleFlowCollectionH;

  reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
  reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
  for(; part != pfCands_end; ++part){
     int partType = part->particleId();
     double eta = part->eta();
     double energy = part->energy();
     histosTH2F_["energyVsEtaAllTypes"]->Fill(eta,energy);

     if(partType == reco::PFCandidate::X)
        histosTH2F_["energyVsEtaUndefined"]->Fill(eta,energy);
     else if(partType == reco::PFCandidate::h)
        histosTH2F_["energyVsEtaChargedHadron"]->Fill(eta,energy); 
     else if(partType == reco::PFCandidate::e) 
        histosTH2F_["energyVsEtaElectron"]->Fill(eta,energy);
     else if(partType == reco::PFCandidate::mu) 
        histosTH2F_["energyVsEtaMuon"]->Fill(eta,energy);
     else if(partType == reco::PFCandidate::gamma) 
        histosTH2F_["energyVsEtaGamma"]->Fill(eta,energy);
     else if(partType == reco::PFCandidate::h0) 
        histosTH2F_["energyVsEtaNeutralHadron"]->Fill(eta,energy);
     else if(partType == reco::PFCandidate::h_HF) 
        histosTH2F_["energyVsEtaHadronHF"]->Fill(eta,energy);
     else if(partType == reco::PFCandidate::egamma_HF) 
        histosTH2F_["energyVsEtaEGammaHF"]->Fill(eta,energy); 

  }

}

DEFINE_FWK_MODULE(PFlowNoiseAnalyzer);
