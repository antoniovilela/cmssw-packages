#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TProfile.h"
#include "TString.h"
#include "TFile.h"
#include "TStyle.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TRandom.h"
#include "TTree.h"
#include "TMath.h"
#include "Math/GenVector/LorentzVector.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <map>

// Trick to make CINT happy
//#include "DataFormats/FWLite/interface/Handle.h"
//#include "DataFormats/FWLite/interface/Event.h"
#if !defined(__CINT__) && !defined(__MAKECINT__)
//Headers for the data items
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/ValueMap.h"
//#include "DataFormats/Common/interface/View.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"
#endif

#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/FWLiteTools.h"

typedef std::map<std::string,TH1F*> HistoMapTH1F;
typedef std::map<std::string,TH2F*> HistoMapTH2F;

void minimumBiasFWLiteAnalysis(std::vector<std::string>& fileNames,
                               std::string const& outFileName = "analysisMinBiasFWLite_histos.root",
                               int maxEvents = -1, bool verbose = false) {
   if(verbose){
     std::cout << ">>> Reading files: " << std::endl;
     for(std::vector<std::string>::const_iterator it = fileNames.begin(); it != fileNames.end(); ++it)
               std::cout << "  " << *it << std::endl; 
   } 

   // Chain the input files
   fwlite::ChainEvent ev(fileNames);
   
   // Create output file
   TFile* hfile = new TFile(outFileName.c_str(),"recreate","data histograms");

   // Book Histograms
   TH1::SetDefaultSumw2(true);

   HistoMapTH1F histosTH1F;
   HistoMapTH2F histosTH2F;
   histosTH1F["nVertex"] = new TH1F("nVertex","Nr. of offline primary vertexes",10,0,10); 
   histosTH1F["leadingJetPt"] = new TH1F("leadingJetPt","leadingJetPt",100,0.,100.);
   histosTH1F["leadingJetEta"] = new TH1F("leadingJetEta","leadingJetEta",100,-5.,5.);
   histosTH1F["leadingJetPhi"] = new TH1F("leadingJetPhi","leadingJetPhi",100,-1.1*M_PI,1.1*M_PI);
   histosTH1F["trackMultiplicity"] = new TH1F("trackMultiplicity","trackMultiplicity",20,0,20);
   histosTH1F["trackMultiplicityAssociatedToPV"] = new TH1F("trackMultiplicityAssociatedToPV","trackMultiplicityAssociatedToPV",20,0,20);
   histosTH1F["multiplicityHFPlus"] = new TH1F("multiplicityHFPlus","multiplicityHFPlus",20,0,20);
   histosTH1F["multiplicityHFMinus"] = new TH1F("multiplicityHFMinus","multiplicityHFMinus",20,0,20);
   histosTH1F["sumEnergyHFPlus"] = new TH1F("sumEnergyHFPlus","sumEnergyHFPlus",100,0.,100.);
   histosTH1F["sumEnergyHFMinus"] = new TH1F("sumEnergyHFMinus","sumEnergyHFMinus",100,0.,100.);
   histosTH1F["xiTowerPlus"] = new TH1F("xiTowerPlus","xiTowerPlus",200,0.,1.);
   histosTH1F["xiTowerMinus"] = new TH1F("xiTowerMinus","xiTowerMinus",200,0.,1.);
   histosTH1F["xiPlusFromJets"] = new TH1F("xiPlusFromJets","xiPlusFromJets",200,0.,1.);
   histosTH1F["xiMinusFromJets"] = new TH1F("xiMinusFromJets","xiMinusFromJets",200,0.,1.);
   histosTH1F["xiPlusFromPFCands"] = new TH1F("xiPlusFromPFCands","xiPlusFromPFCands",200,0.,1.);
   histosTH1F["xiMinusFromPFCands"] = new TH1F("xiMinusFromPFCands","xiMinusFromPFCands",200,0.,1.);
   histosTH1F["missingMassFromXiTower"] = new TH1F("missingMassFromXiTower","missingMassFromXiTower",200,-10.,800.);
   histosTH1F["missingMassFromXiFromJets"] = new TH1F("missingMassFromXiFromJets","missingMassFromXiFromJets",200,-10.,800.);
   histosTH1F["missingMassFromXiFromPFCands"] = new TH1F("missingMassFromXiFromPFCands","missingMassFromXiFromPFCands",200,-10.,800.);
   histosTH1F["MxFromJets"] = new TH1F("MxFromJets","MxFromJets",200,-10.,400.);
   histosTH1F["MxFromPFCands"] = new TH1F("MxFromPFCands","MxFromPFCands",200,-10.,400.);
   histosTH2F["iEtaVsHFCountPlus"] = new TH2F("iEtaVsHFCountPlus","iEtaVsHFCountPlus",13,29,42,20,0,20);
   histosTH2F["iEtaVsHFCountMinus"] = new TH2F("iEtaVsHFCountMinus","iEtaVsHFCountMinus",13,29,42,20,0,20);

   double Ebeam = 450.;
   int thresholdHF = 10;// 0.2 GeV

   /*// Event selection
   // Prim. vertices
   bool doVertexSelection = true;
   // Jets
   double ptmin = 10.;
   double etamax = 5.0;
   // HF-multiplicity
   bool doHFMultiplicitySelection = false; 
   int nHFPlusMax = 0;
   int nHFMinusMax = 0;*/

   // Loop over the events
   int nEvts = 0;
   for( ev.toBegin(); ! ev.atEnd(); ++ev) {

     if((maxEvents > 0)&&(nEvts == maxEvents)) break;
	
     ++nEvts;
     if(verbose) std::cout << ">>> Event number: " << nEvts << endl;
 
     // Hcal noise
     fwlite::Handle<HcalNoiseSummary> noiseSummary;
     noiseSummary.getByLabel(ev,"hcalnoise");   

     bool passLoose = noiseSummary->passLooseNoiseFilter();
     bool passTight = noiseSummary->passTightNoiseFilter();

     if(verbose){
        std::cout << " =============== Hcal Noise =============== " << std::endl
                  << "   Loose noise filter accept: " << passLoose << std::endl
                  << "   Tight noise filter accept: " << passTight << std::endl;
     }

     // Beam Halo summary
     fwlite::Handle<reco::BeamHaloSummary> beamHaloSummary;
     beamHaloSummary.getByLabel(ev,"BeamHaloSummary");

     bool cscLooseHaloId = beamHaloSummary->CSCLooseHaloId(); 
     bool cscTightHaloId = beamHaloSummary->CSCTightHaloId(); 
     bool ecalLooseHaloId = beamHaloSummary->EcalLooseHaloId();
     bool ecalTightHaloId = beamHaloSummary->EcalTightHaloId(); 
     bool hcalLooseHaloId = beamHaloSummary->HcalLooseHaloId(); 
     bool hcalTightHaloId = beamHaloSummary->HcalTightHaloId(); 
     bool globalLooseHaloId = beamHaloSummary->GlobalLooseHaloId(); 
     bool globalTightHaloId = beamHaloSummary->GlobalTightHaloId();
     bool beamHaloLooseId = beamHaloSummary->LooseId(); 
     bool beamHaloTightId = beamHaloSummary->TightId();
    
     if(verbose){
        std::cout << " =============== Beam Halo Id =============== " << std::endl
                  << "   CSC loose Halo id: " << cscLooseHaloId << std::endl
                  << "   CSC tight Halo id: " << cscTightHaloId << std::endl
                  << "   ECAL loose Halo id: " << ecalLooseHaloId << std::endl
                  << "   ECAL tight Halo id: " << ecalTightHaloId << std::endl
                  << "   HCAL loose Halo id: " << hcalLooseHaloId << std::endl
                  << "   HCAL tight Halo id: " << hcalTightHaloId << std::endl
                  << "   Global loose Halo id: " << globalLooseHaloId << std::endl
                  << "   Global tight Halo id: " << globalTightHaloId << std::endl
                  << "   Loose Halo id: " << beamHaloLooseId << std::endl
                  << "   Tight Halo id: " << beamHaloTightId << std::endl;
     }

     // Vertex Info
     fwlite::Handle<std::vector<reco::Vertex> > vertexCollection;
     vertexCollection.getByLabel(ev,"offlinePrimaryVertices");

     int nGoodVertices = 0;
     std::vector<reco::Vertex>::const_iterator vtx = vertexCollection->begin();
     std::vector<reco::Vertex>::const_iterator vtx_end = vertexCollection->end();
     for(; vtx != vtx_end; ++vtx){
        if(!vtx->isValid()) continue; // skip non-valid vertices
        if(vtx->isFake()) continue; // skip vertex from beam spot
        ++nGoodVertices;
     }

     // Single-vertex
     histosTH1F["nVertex"]->Fill(nGoodVertices);
     //if(doVertexSelection&&(nGoodVertices != 1)) continue;

     // Jets
     fwlite::Handle<std::vector<reco::CaloJet> > jetCollection;
     //fwlite::Handle<edm::View<reco::Jet> > jetCollection;
     jetCollection.getByLabel(ev,"sisCone5CaloJets");

     if(!jetCollection.isValid()) {std::cout << ">>> ERROR: Jet collection cout not be accessed" << std::endl;continue;}
  
     if(jetCollection->size() > 0){     
        const reco::Jet& jet1 = (*jetCollection)[0];

        histosTH1F["leadingJetPt"]->Fill(jet1.pt());
        histosTH1F["leadingJetEta"]->Fill(jet1.eta());
        histosTH1F["leadingJetPhi"]->Fill(jet1.phi());
     }
 
     // PF candidates
     fwlite::Handle<std::vector<reco::PFCandidate> > pfCandCollection;
     pfCandCollection.getByLabel(ev,"particleFlow");

     // Compute Mx
     double MxFromJets = MassColl(*jetCollection);
     double MxFromPFCands = MassColl(*pfCandCollection);

     histosTH1F["MxFromJets"]->Fill(MxFromJets);
     histosTH1F["MxFromPFCands"]->Fill(MxFromPFCands);

     std::pair<double,double> xiFromJets = xi(*jetCollection,Ebeam);
     histosTH1F["xiPlusFromJets"]->Fill(xiFromJets.first);
     histosTH1F["xiMinusFromJets"]->Fill(xiFromJets.second);

     std::pair<double,double> xiFromPFCands = xi(*pfCandCollection,Ebeam);
     histosTH1F["xiPlusFromPFCands"]->Fill(xiFromPFCands.first);
     histosTH1F["xiMinusFromPFCands"]->Fill(xiFromPFCands.second);

     // Access multiplicities
     fwlite::Handle<unsigned int> trackMultiplicity; 
     trackMultiplicity.getByLabel(ev,"trackMultiplicity","trackMultiplicity");

     fwlite::Handle<unsigned int> trackMultiplicityAssociatedToPV;
     trackMultiplicityAssociatedToPV.getByLabel(ev,"trackMultiplicityAssociatedToPV","trackMultiplicity");

     fwlite::Handle<std::vector<unsigned int> > nHFPlus;
     nHFPlus.getByLabel(ev,"hfTower","nHFplus");
  
     fwlite::Handle<std::vector<unsigned int> > nHFMinus;
     nHFMinus.getByLabel(ev,"hfTower","nHFminus");

     fwlite::Handle<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaPlus;
     mapThreshToiEtaPlus.getByLabel(ev,"hfTower","mapTreshToiEtaplus");

     fwlite::Handle<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaMinus;
     mapThreshToiEtaMinus.getByLabel(ev,"hfTower","mapTreshToiEtaminus");

     fwlite::Handle<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityPlus;
     iEtaHFMultiplicityPlus.getByLabel(ev,"hfTower","iEtaHFMultiplicityPlus");

     fwlite::Handle<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityMinus;
     iEtaHFMultiplicityMinus.getByLabel(ev,"hfTower","iEtaHFMultiplicityMinus");

     fwlite::Handle<double> xiTowerPlus;
     xiTowerPlus.getByLabel(ev,"xiTower","xiTowerplus");

     fwlite::Handle<double> xiTowerMinus;
     xiTowerMinus.getByLabel(ev,"xiTower","xiTowerminus");

     unsigned int nTracks = *trackMultiplicity;
     unsigned int nTracksAssociatedToPV = *trackMultiplicityAssociatedToPV;

     unsigned int nHF_plus = (*nHFPlus)[thresholdHF];
     unsigned int nHF_minus = (*nHFMinus)[thresholdHF];
 
     double xiTower_plus = *xiTowerPlus;
     double xiTower_minus = *xiTowerMinus;
 
     histosTH1F["trackMultiplicity"]->Fill(nTracks);
     histosTH1F["trackMultiplicityAssociatedToPV"]->Fill(nTracksAssociatedToPV);

     histosTH1F["multiplicityHFPlus"]->Fill(nHF_plus);
     histosTH1F["multiplicityHFMinus"]->Fill(nHF_minus);     

     for(unsigned int ieta = 28; ieta <= 41; ++ieta){
        //const std::map<unsigned int, std::vector<unsigned int> >& mapThreshToiEta_plus = *mapThreshToiEtaPlus;
        unsigned int nHFPlus_ieta = nHFSlice(*mapThreshToiEtaPlus,thresholdHF,ieta);
        histosTH2F["iEtaVsHFCountPlus"]->Fill(ieta,nHFPlus_ieta);

        std::map<unsigned int, std::vector<unsigned int> >::const_iterator ieta_plus = iEtaHFMultiplicityPlus->find(ieta);
        unsigned int nHFPlus_ieta_v2 = (ieta_plus == iEtaHFMultiplicityPlus->end())?0:ieta_plus->second[thresholdHF]; 
        //std::cout << "ieta= " << ieta << " HF mult. plus= " << nHFPlus_ieta << ", " << nHFPlus_ieta_v2 << std::endl;
        if(nHFPlus_ieta != nHFPlus_ieta_v2) std::cout << "ieta= " << ieta << " different multiplicity plus " << nHFPlus_ieta << ", " << nHFPlus_ieta_v2 << std::endl;

        //const std::map<unsigned int, std::vector<unsigned int> >& mapThreshToiEta_minus = *mapThreshToiEtaMinus;
        unsigned int nHFMinus_ieta = nHFSlice(*mapThreshToiEtaMinus,thresholdHF,ieta);
        histosTH2F["iEtaVsHFCountMinus"]->Fill(ieta,nHFMinus_ieta); 

        std::map<unsigned int, std::vector<unsigned int> >::const_iterator ieta_minus = iEtaHFMultiplicityMinus->find(ieta);
        unsigned int nHFMinus_ieta_v2 = (ieta_minus == iEtaHFMultiplicityMinus->end())?0:ieta_minus->second[thresholdHF]; 
        //std::cout << "ieta= " << ieta << " HF mult. minus= " << nHFMinus_ieta << ", " << nHFMinus_ieta_v2 << std::endl;
        if(nHFMinus_ieta != nHFMinus_ieta_v2) std::cout << "ieta= " << ieta << " different multiplicity minus " << nHFMinus_ieta << ", " << nHFMinus_ieta_v2 << std::endl;
     }

     histosTH1F["xiTowerPlus"]->Fill(xiTower_plus);
     histosTH1F["xiTowerMinus"]->Fill(xiTower_minus);

     double missingMassFromXiTower = 2*Ebeam*sqrt(xiTower_plus*xiTower_minus);
     histosTH1F["missingMassFromXiTower"]->Fill(missingMassFromXiTower);
     double missingMassFromXiFromJets = 2*Ebeam*sqrt(xiFromJets.first*xiFromJets.second);
     histosTH1F["missingMassFromXiFromJets"]->Fill(missingMassFromXiFromJets);
     double missingMassFromXiFromPFCands = 2*Ebeam*sqrt(xiFromPFCands.first*xiFromPFCands.second);
     histosTH1F["missingMassFromXiFromPFCands"]->Fill(missingMassFromXiFromPFCands);

     fwlite::Handle<std::vector<double> > sumEHFplus;
     sumEHFplus.getByLabel(ev,"hfTower","sumEHFplus");
   
     fwlite::Handle<std::vector<double> > sumEHFminus;
     sumEHFminus.getByLabel(ev,"hfTower","sumEHFminus");

     double sumE_plus = (*sumEHFplus)[thresholdHF];
     double sumE_minus = (*sumEHFminus)[thresholdHF];
     histosTH1F["sumEnergyHFPlus"]->Fill(sumE_plus);
     histosTH1F["sumEnergyHFMinus"]->Fill(sumE_minus);

   }  // End loop over events
  
   hfile->Write();
   hfile->Close();
}
