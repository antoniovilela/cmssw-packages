#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TTree.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/EventData.h"
#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/Histos.h" 
#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

#include "Utilities/PlottingTools/interface/StdAllocatorAdaptor.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace minimumBiasAnalysis;

void minimumBiasTTreeAnalysis(std::string const& fileName,
                              std::string const& treeName,
                              std::string const& outFileName = "analysisMinBiasTTree_histos.root",
                              bool selectEventsInRuns = false,
                              bool accessMCInfo = false,
                              int genType = PYTHIA,  
                              int processCategory = All, 
                              int maxEvents = -1, bool verbose = false);

void minimumBiasTTreeAnalysis(TTree* data,
                              std::string const& outFileName = "analysisMinBiasTTree_histos.root",
                              bool selectEventsInRuns = false,
                              bool accessMCInfo = false,
                              int genType = PYTHIA,
                              int processCategory = All,
                              int maxEvents = -1, bool verbose = false);

void minimumBiasTTreeAnalysis(std::string const& fileName,
                              std::string const& treeName,
                              std::string const& outFileName,
                              bool selectEventsInRuns,
                              bool accessMCInfo,
                              int genType,
                              int processCategory,
                              int maxEvents, bool verbose) {
   
   std::cout << ">>> Reading file: " << fileName << std::endl;

   TFile* file = TFile::Open(fileName.c_str(),"read");
   if(!file){
      std::cout << "ERROR: Could not find " << fileName << std::endl;
      return;
   }

   // Get TTree
   TTree* data = dynamic_cast<TTree*>(file->Get(treeName.c_str()));
   if(!data){
      std::cout << "ERROR: Could not find " << treeName << " in " << fileName << std::endl;
      file->Close();
      return;
   }

   minimumBiasTTreeAnalysis(data,outFileName,selectEventsInRuns,accessMCInfo,genType,processCategory,maxEvents,verbose);

}

void minimumBiasTTreeAnalysis(TTree* data,
                              std::string const& outFileName,
                              bool selectEventsInRuns,
                              bool accessMCInfo,
                              int genType,
                              int processCategory,
                              int maxEvents, bool verbose) {

   std::cout << ">>> Reading TTree: " << data->GetName() << std::endl;

   std::cout << "Use only selected runs: " << selectEventsInRuns << std::endl
             << "Access MC Info: " << accessMCInfo << std::endl
             << "Generator type (PYTHIA=0, PHOJET=1): " << genType << std::endl
             << "Processes to analyze (All=0, SD=1, NonSD=2, Inelastic=3, DD=4, Diff=5): " << processCategory << std::endl
             << ">>> Writing histograms to " << outFileName << std::endl;

   std::vector<int> selectedRuns;
   selectedRuns.push_back(124020);
   selectedRuns.push_back(124025);
   selectedRuns.push_back(124027);
   selectedRuns.push_back(124030);

   bool selectProcessIds = (accessMCInfo && (processCategory != All));
   std::vector<int> selectedProcIds;
   if(accessMCInfo){
      if(selectProcessIds){
         if(genType == PYTHIA) getSelectedProcIdsPYTHIA(processCategory,selectedProcIds);
         else if(genType == PHOJET) getSelectedProcIdsPHOJET(processCategory,selectedProcIds);
      }
      if(verbose){
         std::stringstream out_str;
         out_str << "Selecting events with process id=";
         std::vector<int>::const_iterator proc = selectedProcIds.begin();
         std::vector<int>::const_iterator proc_end = selectedProcIds.end();
         for(; proc != proc_end; ++proc) out_str << " " << *proc;
         out_str << "\n";
         std::cout << out_str.str();
      }
   }

   EventData eventData;
   setTTreeBranches(*data,eventData);
   int nEntries = data->GetEntries(); 

   // Create output file
   TFile* hfile = new TFile(outFileName.c_str(),"recreate","data histograms");

   // Book Histograms
   TH1::SetDefaultSumw2(true);

   HistoMapTH1F histosTH1F;
   bookHistos(histosTH1F,StdAllocatorAdaptor());

   HistoMapTH2F histosTH2F;
   bookHistos(histosTH2F,StdAllocatorAdaptor());

   std::vector<int> processIDs;
   std::vector<std::string> processNames;
   if(accessMCInfo){ 
      if(genType == PYTHIA) getProcessIdsPYTHIA(processIDs,processNames);
      else if(genType == PHOJET) getProcessIdsPHOJET(processIDs,processNames);
      else {std::cout << ">>> ERROR: Need to set a generator type" << std::endl;return;}

      histosTH1F["ProcessId"] = new TH1F("ProcessId","ProcessId",processIDs.size(),0,processIDs.size());
      for(size_t iprocess = 0; iprocess < processIDs.size(); ++iprocess) histosTH1F["ProcessId"] ->GetXaxis()->SetBinLabel(iprocess + 1,processNames[iprocess].c_str());
   }

   bool doTriggerSelection = false;
   bool doHcalNoiseSelection = true;
   /*// Pre-selection
   bool doGoodVertexSelection = false;
   bool doHighQualityTracksSelection = true;*/
   // Event selection
   // Prim. vertices
   bool doVertexSelection = false;
   double primVtxZMax = 10.0;
   // HF
   bool doHFSelection = false;
   double sumEnergyHFPlusMax = 8.;
   double sumEnergyHFMinusMax = 99999.;
   // MET-SumET
   bool doSumETSelection = false;
   double sumETMin = 40.;
   // Tracks
   //bool doTrackSelection = false;
   // Mx
   bool doMxSelection = false;
   double MxMin = 100.;
   double MxMax = 999.;

   double eventWeight = 1.0;
   //std::vector<std::pair<int,int> > selectedEvents;
   // Loop over the events
   for(int ientry = 0; ientry < nEntries; ++ientry){
      if((maxEvents > 0)&&(ientry == maxEvents)) break;
      if(verbose && ientry%2000 == 0) std::cout << ">>> Analysing " << ientry << "th entry" << std::endl;

      data->GetEntry(ientry);

      int eventNumber = eventData.eventNumber_;
      int runNumber = eventData.runNumber_;
      int lumiSection = eventData.lumiSection_;
     if(verbose){
        std::cout << "  Event number: " << eventNumber << std::endl
                  << "  Run number: " << runNumber << std::endl
                  << "  Lumi section: " << lumiSection << std::endl;
     }
     if((runNumber == 0)&&(eventNumber == 0)) {std::cout << ">>> ERROR: Problem with event...skipping" << std::endl;continue;}

     histosTH1F["EventSelection"]->Fill("All",eventWeight);

     if(accessMCInfo){
        int processId = eventData.processId_;
        if(selectProcessIds && find(selectedProcIds.begin(),selectedProcIds.end(),processId) == selectedProcIds.end()) continue;

        std::vector<int>::const_iterator it_processId = find(processIDs.begin(),processIDs.end(),processId);
        if(it_processId != processIDs.end()){
           int idx_processId = it_processId - processIDs.begin();
           histosTH1F["ProcessId"]->Fill(idx_processId);
        }  
     }

     if(!accessMCInfo && selectEventsInRuns && find(selectedRuns.begin(),selectedRuns.end(),runNumber) == selectedRuns.end()) continue;

     histosTH1F["EventSelection"]->Fill("ProcessIdOrRunSelection",eventWeight);

     if(doTriggerSelection){
        bool accept = true;
        /*
        */
        if(!accept) continue;
     }

     histosTH1F["EventSelection"]->Fill("TriggerSelection",eventWeight);

     // Hcal noise
     bool passNoiseLoose = (eventData.LooseNoiseFilter_ == 1) ? true : false;
     bool passNoiseTight = (eventData.TightNoiseFilter_ == 1) ? true : false;

     if(passNoiseLoose) histosTH1F["HcalNoiseId"]->Fill(0);
     if(passNoiseTight) histosTH1F["HcalNoiseId"]->Fill(1); 

     if(verbose){
        std::cout << " =============== Hcal Noise =============== " << std::endl
                  << "   Loose noise filter accept: " << passNoiseLoose << std::endl
                  << "   Tight noise filter accept: " << passNoiseTight << std::endl;
     }

     if(doHcalNoiseSelection && !passNoiseTight) continue;

     histosTH1F["EventSelection"]->Fill("HcalNoise",eventWeight);

     // Beam Halo summary
     bool beamHaloLooseId = (eventData.BeamHaloLooseId_ == 1) ? true : false;
     bool beamHaloTightId = (eventData.BeamHaloTightId_ == 1) ? true : false;
     
     if(beamHaloLooseId) histosTH1F["BeamHaloId"]->Fill(0);
     if(beamHaloTightId) histosTH1F["BeamHaloId"]->Fill(1);

     if(verbose){
        std::cout << " =============== Beam Halo Id =============== " << std::endl
                  << "   Loose Halo id: " << beamHaloLooseId << std::endl
                  << "   Tight Halo id: " << beamHaloTightId << std::endl;
     }

     histosTH1F["EventSelection"]->Fill("BeamHaloId",eventWeight);
 
     // Pre-selection
     histosTH1F["EventSelection"]->Fill("GoodVertexFilter",eventWeight);
     histosTH1F["EventSelection"]->Fill("HighQualityTracks",eventWeight);
 
     // Vertex Info
     int nGoodVertices = eventData.nVertex_;
     histosTH1F["nVertex"]->Fill(nGoodVertices);
     if(doVertexSelection && (nGoodVertices < 1)) continue;

     double posXPrimVtx = eventData.posXPrimVtx_;
     double posYPrimVtx = eventData.posYPrimVtx_;
     double posZPrimVtx = eventData.posZPrimVtx_;
     histosTH1F["posXPrimVtx"]->Fill(posXPrimVtx);
     histosTH1F["posYPrimVtx"]->Fill(posYPrimVtx);
     histosTH1F["posZPrimVtx"]->Fill(posZPrimVtx);
     //histosTH1F["posRPrimVtx"]->Fill();
     //histosTH1F["numberDOF"]->Fill();

     if(doVertexSelection && (fabs(posZPrimVtx) > primVtxZMax)) continue;
     histosTH1F["EventSelection"]->Fill("VertexSelection",eventWeight);

     // HF Selection
     if(doHFSelection && ( eventData.sumEnergyHFPlus_ > sumEnergyHFPlusMax || eventData.sumEnergyHFMinus_ > sumEnergyHFMinusMax)) continue;
     histosTH1F["EventSelection"]->Fill("HFSelection",eventWeight);

     // MET - SumET
     if(doSumETSelection && (eventData.sumET_ < sumETMin)) continue;
     histosTH1F["EventSelection"]->Fill("SumETSelection",eventWeight);

     // Mx
     if(doMxSelection && ((eventData.MxFromPFCands_ < MxMin)||eventData.MxFromPFCands_ > MxMax)) continue;
     histosTH1F["EventSelection"]->Fill("MxSelection",eventWeight);

     // Fill Histos
     // SumET
     double sumET = eventData.sumET_;
     histosTH1F["sumET"]->Fill(sumET);

     // Mx
     double MxFromTowers = eventData.MxFromTowers_;
     double MxFromPFCands = eventData.MxFromPFCands_;

     //histosTH1F["MxFromJets"]->Fill(MxFromJets);
     histosTH1F["MxFromTowers"]->Fill(MxFromTowers);
     histosTH1F["MxFromPFCands"]->Fill(MxFromPFCands);
     if(accessMCInfo){
        double MxGen = eventData.MxGen_;
        histosTH1F["MxGen"]->Fill(MxGen);
        histosTH1F["ResMxFromTowers"]->Fill(MxFromTowers - MxGen);
        histosTH1F["ResMxFromPFCands"]->Fill(MxFromPFCands - MxGen);
     }

     // Jets
     double leadingJetPt = eventData.leadingJetPt_;
     double leadingJetEta = eventData.leadingJetEta_;
     double leadingJetPhi = eventData.leadingJetPhi_;
     if(leadingJetPt > 0.){
        histosTH1F["leadingJetPt"]->Fill(leadingJetPt);
        histosTH1F["leadingJetEta"]->Fill(leadingJetEta);
        histosTH1F["leadingJetPhi"]->Fill(leadingJetPhi);
     }

     // Xi
     /*double xiPlusFromJets =
     double xiMinusFromJets 
     histosTH1F["xiPlusFromJets"]->Fill();
     histosTH1F["xiMinusFromJets"]->Fill();*/

     double xiPlusFromTowers = eventData.xiPlusFromTowers_;
     double xiMinusFromTowers = eventData.xiMinusFromTowers_;
     histosTH1F["xiPlusFromTowers"]->Fill(xiPlusFromTowers);
     histosTH1F["xiMinusFromTowers"]->Fill(xiMinusFromTowers);

     double xiPlusFromPFCands = eventData.xiPlusFromPFCands_;
     double xiMinusFromPFCands = eventData.xiMinusFromPFCands_;
     histosTH1F["xiPlusFromPFCands"]->Fill(xiPlusFromPFCands);
     histosTH1F["xiMinusFromPFCands"]->Fill(xiMinusFromPFCands);

     double EPlusPzFromTowers = eventData.EPlusPzFromTowers_;
     double EMinusPzFromTowers = eventData.EMinusPzFromTowers_;
     histosTH1F["EPlusPzFromTowers"]->Fill(EPlusPzFromTowers);
     histosTH1F["EMinusPzFromTowers"]->Fill(EMinusPzFromTowers);

     double EPlusPzFromPFCands = eventData.EPlusPzFromPFCands_;
     double EMinusPzFromPFCands = eventData.EMinusPzFromPFCands_;
     histosTH1F["EPlusPzFromPFCands"]->Fill(EPlusPzFromPFCands);
     histosTH1F["EMinusPzFromPFCands"]->Fill(EMinusPzFromPFCands);

     if(accessMCInfo){
        // xi <= 0.1
        double xigen_plus = eventData.xiGenPlus_;
        double xigen_minus = eventData.xiGenMinus_;
        if(xigen_plus > 0. && xigen_plus <= 0.1){
           histosTH1F["xiGenPlus"]->Fill(xigen_plus); 
           histosTH1F["ResXiPlusFromTowers"]->Fill(xiPlusFromTowers - xigen_plus);
           histosTH1F["ResXiPlusFromPFCands"]->Fill(xiPlusFromPFCands - xigen_plus);
           histosTH2F["xiFromTowersVsxiGenPlus"]->Fill(xigen_plus,xiPlusFromTowers);
           histosTH2F["xiFromPFCandsVsxiGenPlus"]->Fill(xigen_plus,xiPlusFromPFCands);
        }
        if(xigen_minus > 0. && xigen_minus <= 0.1){
           histosTH1F["xiGenMinus"]->Fill(xigen_minus);
           histosTH1F["ResXiMinusFromTowers"]->Fill(xiMinusFromTowers - xigen_minus);
           histosTH1F["ResXiMinusFromPFCands"]->Fill(xiMinusFromPFCands - xigen_minus);
           histosTH2F["xiFromTowersVsxiGenMinus"]->Fill(xigen_minus,xiMinusFromTowers);
           histosTH2F["xiFromPFCandsVsxiGenMinus"]->Fill(xigen_minus,xiMinusFromPFCands);
        }
     }

     // Access multiplicities
     unsigned int nTracks = eventData.trackMultiplicity_;
     unsigned int nTracksAssociatedToPV = eventData.trackMultiplicityAssociatedToPV_;

     unsigned int nHF_plus = eventData.multiplicityHFPlus_;
     unsigned int nHF_minus = eventData.multiplicityHFMinus_;
 
     histosTH1F["trackMultiplicity"]->Fill(nTracks);
     histosTH1F["trackMultiplicityAssociatedToPV"]->Fill(nTracksAssociatedToPV);

     histosTH1F["multiplicityHFPlus"]->Fill(nHF_plus);
     histosTH1F["multiplicityHFMinus"]->Fill(nHF_minus);     

     for(unsigned int ieta = 29, index = 0; ieta <= 41; ++ieta,++index){
        int nHFPlus_ieta = eventData.multiplicityHFPlusVsiEta_[index];
        histosTH2F["iEtaVsHFCountPlus"]->Fill(ieta,nHFPlus_ieta);

        int nHFMinus_ieta = eventData.multiplicityHFMinusVsiEta_[index]; 
        histosTH2F["iEtaVsHFCountMinus"]->Fill(ieta,nHFMinus_ieta); 
     }

     double missingMassFromXiFromTowers = eventData.missingMassFromXiFromTowers_;
     histosTH1F["missingMassFromXiFromTowers"]->Fill(missingMassFromXiFromTowers);
     //double missingMassFromXiFromJets =
     //histosTH1F["missingMassFromXiFromJets"]->Fill(missingMassFromXiFromJets);
     double missingMassFromXiFromPFCands = eventData.missingMassFromXiFromPFCands_;
     histosTH1F["missingMassFromXiFromPFCands"]->Fill(missingMassFromXiFromPFCands);

     double sumE_plus = eventData.sumEnergyHFPlus_;
     double sumE_minus = eventData.sumEnergyHFMinus_;
     histosTH1F["sumEnergyHFPlus"]->Fill(sumE_plus);
     histosTH1F["sumEnergyHFMinus"]->Fill(sumE_minus);

     /*selectedEvents.push_back(std::make_pair(runNumber,eventNumber));
     if(verbose){
        std::cout << "======== Selected event ========" << std::endl
                  << "  Event number: " << eventNumber << std::endl
                  << "  Run number: " << runNumber << std::endl
                  << "  Lumi section: " << lumiSection << std::endl
                  << "================================" << std::endl;
     }*/
   }  // End loop over events

   /*std::cout << "======== List of selected events ========" << std::endl 
             << " Total number of events: " << selectedEvents.size() << std::endl;
   for(std::vector<std::pair<int,int> >::const_iterator it = selectedEvents.begin(); it != selectedEvents.end(); ++it){
      std::cout << " Run " << it->first << " Event " << it->second << std::endl;
   }*/

   hfile->Write();
   hfile->Close();
}
