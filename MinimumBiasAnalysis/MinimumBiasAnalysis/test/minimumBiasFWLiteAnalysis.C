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
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"
#include "DataFormats/FWLite/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#endif

#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/FWLiteTools.h"

using namespace exclusiveDijetsAnalysis;

typedef std::map<std::string,TH1F*> HistoMapTH1F;
typedef std::map<std::string,TH2F*> HistoMapTH2F;

void bookHistosTH1F(HistoMapTH1F&);
void getProcessIds(std::vector<int>&, std::vector<std::string>&);

void minimumBiasFWLiteAnalysis(std::vector<std::string>& fileNames,
                               std::string const& outFileName = "analysisMinBiasFWLite_histos.root",
                               bool accessMCInfo = false,
                               bool selectEventsInRuns = false, 
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
   bookHistosTH1F(histosTH1F);

   HistoMapTH2F histosTH2F;
   histosTH2F["iEtaVsHFCountPlus"] = new TH2F("iEtaVsHFCountPlus","iEtaVsHFCountPlus",13,29,42,20,0,20);
   histosTH2F["iEtaVsHFCountMinus"] = new TH2F("iEtaVsHFCountMinus","iEtaVsHFCountMinus",13,29,42,20,0,20);
 
   int absiEtaMax = 42;
   int absiEtaMaxLim = absiEtaMax + 1; 
   histosTH2F["ecalTimeVsiEta"] = new TH2F("ecalTimeVsiEta","ecalTimeVsiEta",2*absiEtaMaxLim,-absiEtaMaxLim,absiEtaMaxLim,200,-100.,100.);
   histosTH2F["hcalTimeVsiEta"] = new TH2F("hcalTimeVsiEta","hcalTimeVsiEta",2*absiEtaMaxLim,-absiEtaMaxLim,absiEtaMaxLim,200,-100.,100.);
   histosTH2F["avgEcalTimeVsiEta"] = new TH2F("avgEcalTimeVsiEta","avgEcalTimeVsiEta",2*absiEtaMaxLim,-absiEtaMaxLim,absiEtaMaxLim,200,-100.,100.);
   histosTH2F["avgHcalTimeVsiEta"] = new TH2F("avgHcalTimeVsiEta","avgHcalTimeVsiEta",2*absiEtaMaxLim,-absiEtaMaxLim,absiEtaMaxLim,200,-100.,100.);
 
   std::vector<int> processIDs;
   std::vector<std::string> processNames; 
   getProcessIds(processIDs,processNames);
   histosTH1F["ProcessId"] = new TH1F("ProcessId","ProcessId",processIDs.size(),0,processIDs.size());
   for(size_t iprocess = 0; iprocess < processIDs.size(); ++iprocess) histosTH1F["ProcessId"] ->GetXaxis()->SetBinLabel(iprocess + 1,processNames[iprocess].c_str());

   double Ebeam = 450.;
   int thresholdHF = 10;// 0.2 GeV

   bool selectProcessIDs = true;
   std::vector<int> selectedProcIDs;
   selectedProcIDs.push_back(11); //f_i f_j -> f_i f_j (QCD)
   selectedProcIDs.push_back(12); //f_i f_ibar -> f_k f_kbar
   selectedProcIDs.push_back(13); //f_i f_ibar -> g g
   selectedProcIDs.push_back(28); //f_i g -> f_i g
   selectedProcIDs.push_back(53); //g g -> f_k f_kbar
   selectedProcIDs.push_back(68); //g g -> g g
   /*selectedProcIDs.push_back(92); //SD AB->XB
   selectedProcIDs.push_back(93); //SD AB->AX
   selectedProcIDs.push_back(94); //DD*/ 

   //bool selectEventsInRuns = true;
   std::vector<int> selectedRuns;
   selectedRuns.push_back(122314);
 
   bool doTriggerSelection = false;
   std::vector<std::string> hltPaths;
   //hltPaths.push_back("");

   // Event selection
   // Prim. vertices
   bool doVertexSelection = true;
   double primVtxZMax = 10.0;
   /*// Jets
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
 
     if(accessMCInfo){
        fwlite::Handle<GenEventInfoProduct> genEventInfo;
        genEventInfo.getByLabel(ev,"generator");
 
        if(!genEventInfo.isValid()) {std::cout << ">>> ERROR: Gen event info could not be accessed" << std::endl;continue;}

        unsigned int processId = genEventInfo->signalProcessID();
        std::vector<int>::const_iterator it_processId = find(processIDs.begin(),processIDs.end(),processId);
        if(it_processId != processIDs.end()){
           int idx_processId = it_processId - processIDs.begin();
           histosTH1F["ProcessId"]->Fill(idx_processId);
        }  
 
        if(selectProcessIDs && find(selectedProcIDs.begin(),selectedProcIDs.end(),processId) == selectedProcIDs.end()) continue;
     }

     int eventNumber = ev.id().event();
     int runNumber = ev.id().run();
     int lumiSection = ev.luminosityBlock();
     if(verbose){
        std::cout << "  Event number: " << eventNumber << std::endl
                  << "  Run number: " << runNumber << std::endl
                  << "  Lumi section: " << lumiSection << std::endl;
     }
     if(!accessMCInfo && selectEventsInRuns && find(selectedRuns.begin(),selectedRuns.end(),runNumber) == selectedRuns.end()) continue;

     if(doTriggerSelection){
        fwlite::Handle<edm::TriggerResults> triggerResults;
        triggerResults.getByLabel(ev,"TriggerResults","","HLT");
 
        if(!triggerResults.isValid()) {std::cout << ">>> ERROR: Trigger results product could not be accessed" << std::endl;continue;}

        fwlite::TriggerNames const & triggerNames = ev.triggerNames(*triggerResults);
  
        bool accept = false;
        for(std::vector<std::string>::const_iterator path = hltPaths.begin(); path != hltPaths.end(); ++path){
           std::string const& pathName = *path;
           unsigned int idx = triggerNames.triggerIndex(pathName);
           if(triggerResults->accept(idx)){
              if(verbose) std::cout << "  Passed trigger " << pathName << std::endl; 
              accept = true;
           } 
        } 
        if(!accept) continue;
     }

     // Hcal noise
     fwlite::Handle<HcalNoiseSummary> noiseSummary;
     noiseSummary.getByLabel(ev,"hcalnoise");   

     bool passLoose = noiseSummary->passLooseNoiseFilter();
     bool passTight = noiseSummary->passTightNoiseFilter();

     if(passLoose) histosTH1F["HcalNoiseId"]->Fill(0);
     if(passTight) histosTH1F["HcalNoiseId"]->Fill(1); 

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
     
     if(cscLooseHaloId) histosTH1F["BeamHaloId"]->Fill(0);
     if(cscTightHaloId) histosTH1F["BeamHaloId"]->Fill(1);
     if(ecalLooseHaloId) histosTH1F["BeamHaloId"]->Fill(2);
     if(ecalTightHaloId) histosTH1F["BeamHaloId"]->Fill(3);
     if(hcalLooseHaloId) histosTH1F["BeamHaloId"]->Fill(4);
     if(hcalTightHaloId) histosTH1F["BeamHaloId"]->Fill(5);
     if(globalLooseHaloId) histosTH1F["BeamHaloId"]->Fill(6);
     if(globalTightHaloId) histosTH1F["BeamHaloId"]->Fill(7);
     if(beamHaloLooseId) histosTH1F["BeamHaloId"]->Fill(8);
     if(beamHaloTightId) histosTH1F["BeamHaloId"]->Fill(9);

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

     if(!vertexCollection.isValid()) {std::cout << ">>> ERROR: Vertex collection cout not be accessed" << std::endl;continue;}

     int nGoodVertices = 0;
     std::vector<reco::Vertex>::const_iterator vtx = vertexCollection->begin();
     std::vector<reco::Vertex>::const_iterator vtx_end = vertexCollection->end();
     for(; vtx != vtx_end; ++vtx){
        if(!vtx->isValid()) continue; // skip non-valid vertices
        if(vtx->isFake()) continue; // skip vertex from beam spot
        ++nGoodVertices;
     }

     histosTH1F["nVertex"]->Fill(nGoodVertices);
     if(doVertexSelection && (nGoodVertices < 1)) continue;

     const reco::Vertex& primVertex = (*vertexCollection)[0];
     if(primVertex.isValid() && !primVertex.isFake()){
        histosTH1F["posXPrimVtx"]->Fill(primVertex.x());
        histosTH1F["posYPrimVtx"]->Fill(primVertex.y());
        histosTH1F["posZPrimVtx"]->Fill(primVertex.z());

        if(doVertexSelection && (fabs(primVertex.z()) > primVtxZMax)) continue;
     }

     // Timing from Calo Towers
     fwlite::Handle<CaloTowerCollection> caloTowerCollection;
     caloTowerCollection.getByLabel(ev,"towerMaker");

     if(!caloTowerCollection.isValid()) {std::cout << ">>> ERROR: Calo tower collection cout not be accessed" << std::endl;continue;}

     std::map<int,float> avgEcalTimePeriEta;
     std::map<int,float> avgHcalTimePeriEta;
     std::map<int,float> sumETowersPeriEta; 
     for(int k = -absiEtaMax; k <= absiEtaMax; ++k){
        avgEcalTimePeriEta[k] = 0.;
        avgHcalTimePeriEta[k] = 0.;
        sumETowersPeriEta[k] = 0.;
     }

     std::vector<CaloTower>::const_iterator caloTower = caloTowerCollection->begin();
     std::vector<CaloTower>::const_iterator caloTower_end = caloTowerCollection->end();
     float timeMin = -200.;
     float timeMax = 200.;
     for(; caloTower != caloTower_end; ++caloTower){
        double energy = caloTower->energy();
        if(energy < 1.0) continue;
 
        int ieta = caloTower->id().ieta();
        float ecalTime = caloTower->ecalTime();
        float hcalTime = caloTower->hcalTime();
 
        histosTH1F["towerEcalTime"]->Fill(ecalTime);
        histosTH1F["towerHcalTime"]->Fill(hcalTime);
        histosTH2F["ecalTimeVsiEta"]->Fill(ieta,ecalTime);
        histosTH2F["hcalTimeVsiEta"]->Fill(ieta,hcalTime);
   
        if((ecalTime < timeMin)||(ecalTime > timeMax)) continue;
        if((hcalTime < timeMin)||(hcalTime > timeMax)) continue;

        histosTH1F["energySumVsEcalTime"]->Fill(ecalTime,energy);
        histosTH1F["energySumVsHcalTime"]->Fill(hcalTime,energy);
 
        avgEcalTimePeriEta[ieta] += caloTower->energy()*ecalTime;
        avgHcalTimePeriEta[ieta] += caloTower->energy()*hcalTime;
        sumETowersPeriEta[ieta] += caloTower->energy(); 
     }
     for(std::map<int,float>::const_iterator it_ieta = sumETowersPeriEta.begin(); it_ieta != sumETowersPeriEta.end(); ++it_ieta){
        int ieta = it_ieta->first;
        float sumE = it_ieta->second;
        if(sumE > 0.){
           avgEcalTimePeriEta[ieta] /= sumE;
           avgHcalTimePeriEta[ieta] /= sumE;
        }
        histosTH2F["avgEcalTimeVsiEta"]->Fill(ieta,avgEcalTimePeriEta[ieta]);
        histosTH2F["avgHcalTimeVsiEta"]->Fill(ieta,avgHcalTimePeriEta[ieta]);
     }

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

void bookHistosTH1F(HistoMapTH1F& histosTH1F){
   histosTH1F["nVertex"] = new TH1F("nVertex","Nr. of offline primary vertexes",10,0,10);
   histosTH1F["posXPrimVtx"] = new TH1F("posXPrimVtx","x position of primary vertexes",100,-15.,15.);
   histosTH1F["posYPrimVtx"] = new TH1F("posYPrimVtx","y position of primary vertexes",100,-15.,15.);
   histosTH1F["posZPrimVtx"] = new TH1F("posZPrimVtx","z position of primary vertexes",100,-30.,30.);
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
   histosTH1F["towerEcalTime"] = new TH1F("towerEcalTime","towerEcalTime",200,-100.,100.);
   histosTH1F["towerHcalTime"] = new TH1F("towerHcalTime","towerHcalTime",200,-100.,100.);
   histosTH1F["energySumVsEcalTime"] = new TH1F("energySumVsEcalTime","energySumVsEcalTime",200,-100.,100.);
   histosTH1F["energySumVsHcalTime"] = new TH1F("energySumVsHcalTime","energySumVsHcalTime",200,-100.,100.);

   histosTH1F["BeamHaloId"] = new TH1F("BeamHaloId","BeamHaloId",10,0,10);
   histosTH1F["BeamHaloId"]->GetXaxis()->SetBinLabel(1,"CSCLooseHaloId");
   histosTH1F["BeamHaloId"]->GetXaxis()->SetBinLabel(2,"CSCTightHaloId");
   histosTH1F["BeamHaloId"]->GetXaxis()->SetBinLabel(3,"EcalLooseHaloId");
   histosTH1F["BeamHaloId"]->GetXaxis()->SetBinLabel(4,"EcalTightHaloId");
   histosTH1F["BeamHaloId"]->GetXaxis()->SetBinLabel(5,"HcalLooseHaloId");
   histosTH1F["BeamHaloId"]->GetXaxis()->SetBinLabel(6,"HcalTightHaloId");
   histosTH1F["BeamHaloId"]->GetXaxis()->SetBinLabel(7,"GlobalLooseHaloId");
   histosTH1F["BeamHaloId"]->GetXaxis()->SetBinLabel(8,"GlobalTightHaloId");
   histosTH1F["BeamHaloId"]->GetXaxis()->SetBinLabel(9,"BeamHaloLooseId");
   histosTH1F["BeamHaloId"]->GetXaxis()->SetBinLabel(10,"BeamHaloTightId");

   histosTH1F["HcalNoiseId"] = new TH1F("HcalNoiseId","HcalNoiseId",2,0,2);
   histosTH1F["HcalNoiseId"]->GetXaxis()->SetBinLabel(1,"LooseNoiseFilter");
   histosTH1F["HcalNoiseId"]->GetXaxis()->SetBinLabel(2,"TightNoiseFilter");
}

void getProcessIds(std::vector<int>& processIDs, std::vector<std::string>& processNames){
   processIDs.push_back(11); //f_i f_j -> f_i f_j (QCD)
   processNames.push_back("f_{i} f_{j} #rightarrow f_{i} f_{j}");
   processIDs.push_back(12); //f_i f_ibar -> f_k f_kbar
   processNames.push_back("f_{i} #bar{f_{i}} #rightarrow f_{k} #bar{f_{k}}");
   processIDs.push_back(13); //f_i f_ibar -> g g
   processNames.push_back("f_{i} #bar{f_{i}} #rightarrow g g");
   processIDs.push_back(28); //f_i g -> f_i g
   processNames.push_back("f_{i} g #rightarrow f_{i} g");
   processIDs.push_back(53); //g g -> f_k f_kbar
   processNames.push_back("g g #rightarrow f_{k} #bar{f_{k}}");
   processIDs.push_back(68); //g g -> g g
   processNames.push_back("g g #rightarrow g g");
   processIDs.push_back(92); //SD AB->XB
   processNames.push_back("SD AB #rightarrow XB");
   processIDs.push_back(93); //SD AB->AX
   processNames.push_back("SD AB #rightarrow AX");
   processIDs.push_back(94); //DD 
   processNames.push_back("DD");
   processIDs.push_back(95); //low-p_T production 
   processNames.push_back("low-p_{T}");

}
