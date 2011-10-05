#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TTree.h"

//#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/EventData.h"
#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/MinimumBiasEventData.h"
#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/Histos.h" 
#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

#include "Utilities/PlottingTools/interface/StdAllocatorAdaptor.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

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
   // Extra options
   // FIXME
   //=================================================================
   // Xi correction factor
   double EBeam = 3500.;
   double xiCorrectionFactor = 2.0;
   // Event selection
   //=================================================================
   bool doEtaMaxGenSelection = false;
   bool doEtaMinGenSelection = false;
   double etaMaxGenMax = 0.;
   double etaMinGenMin = 0.; 
   
   bool doDeltaEtaGenSelection = false;
   double deltaEtaGenMin = 3.0;

   bool doLogXiGenPlusSelection  = true;
   bool doLogXiGenMinusSelection = false;
   //double logXiGenPlusMax  = -5.5;
   double logXiGenPlusMax  = -6.5;
   double logXiGenMinusMax = -5.0;
   //=================================================================
   bool doTriggerSelection = false;
   bool doHcalNoiseSelection = false;
   // Prim. vertices
   bool doVertexSelection = false;
   double primVtxZMax = 10.0;
   // HF
   bool doHFSelection = false;
   double sumEnergyHFPlusMax = 99999.;
   double sumEnergyHFMinusMax = 99999.;
   // MET-SumET
   bool doSumETSelection = false;
   double sumETMin = 40.;
   // Mx
   bool doMxSelection = false;
   double MxMin = 100.;
   double MxMax = 999.;
   // Xi
   bool doXiPlusSelection = false;
   bool doXiMinusSelection = false;
   double xiMax = 0.01;
   // EtaMax
   bool doEtaMaxSelection = false;
   bool doEtaMinSelection = false;
   double etaMaxMax = 1.0;
   double etaMinMin = -1.0;
   //=================================================================

   std::cout << ">>> Reading TTree: " << data->GetName() << std::endl;

   std::cout << "Use only selected runs: " << selectEventsInRuns << std::endl
             << "Access MC Info: " << accessMCInfo << std::endl
             << "Generator type (PYTHIA6=0, PHOJET=1, PYTHIA8=2): " << genType << std::endl
             << "Processes to analyze (All=0, SD=1, NonSD=2, Inelastic=3, DD=4, Diff=5): " << processCategory << std::endl
             << ">>> Writing histograms to " << outFileName << std::endl;

   std::vector<int> selectedRuns;

   bool selectProcessIds = (accessMCInfo && (processCategory != All));
   std::vector<int> selectedProcIds;
   if(accessMCInfo){
      if(selectProcessIds){
         if(genType == PYTHIA) getSelectedProcIdsPYTHIA(processCategory,selectedProcIds);
         else if(genType == PHOJET) getSelectedProcIdsPHOJET(processCategory,selectedProcIds);
         else if(genType == PYTHIA8) getSelectedProcIdsPYTHIA8(processCategory,selectedProcIds);
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

   // Reweighing 
   //==========================
   TH1F h_reweightPosZVertex;
   TH1F h_reweightEffBscOr;
   if(accessMCInfo){
      TFile reweightVtxFile("reweight_posZPrimVtx_Pythia8Tune4C_eventSelectionAnalysis-v1.root","read"); 
      TH1F* h_reweightPosZVertex_tmp = static_cast<TH1F*>( reweightVtxFile.Get("posZPrimVtx_reweight_range") );
      h_reweightPosZVertex = *h_reweightPosZVertex_tmp;
      reweightVtxFile.Close();

      TFile reweightEffBscOrFile("reweight_eventSelectionBscMinBiasORVsBeamHaloVeto_Pythia8Tune4C_minimumBiasTTreeAnalysis-v5.root");
      TH1F* h_reweightEffBscOr_tmp = static_cast<TH1F*>( reweightEffBscOrFile.Get("multiplicityTracks_reweight") );
      h_reweightEffBscOr = *h_reweightEffBscOr_tmp;
      reweightEffBscOrFile.Close();
   }

   // Access event data
   //==========================
   //EventData eventData;
   //setTTreeBranches(*data,eventData);
   MinimumBiasEventData* eventData_ptr = new MinimumBiasEventData;
   TBranch* eventsBranch = data->GetBranch("Events");
   eventsBranch->SetAddress(&eventData_ptr);
   MinimumBiasEventData const& eventData = *eventData_ptr;
 
   // Create output file and book histograms
   //==========================
   TFile* hfile = new TFile(outFileName.c_str(),"recreate","data histograms");

   // Book Histograms
   //TH1::SetDefaultSumw2(true);
   HistoMapTH1F histosTH1F;
   bookHistos(histosTH1F,StdAllocatorAdaptor());
   HistoMapTH2F histosTH2F;
   bookHistos(histosTH2F,StdAllocatorAdaptor());

   histosTH1F["EventsPerBunchCrossing"] = new TH1F("EventsPerBunchCrossing","EventsPerBunchCrossing",3565,0,3565); 
   histosTH1F["sumEnergyHFPlusGen_Ntrk0_5"] = new TH1F("sumEnergyHFPlusGen_Ntrk0_5","sumEnergyHFPlusGen_Ntrk0_5",100,0.,100.);
   histosTH1F["sumEnergyHFPlusGen_Ntrk6_10"] = new TH1F("sumEnergyHFPlusGen_Ntrk6_10","sumEnergyHFPlusGen_Ntrk6_10",100,0.,100.);
   histosTH1F["sumEnergyHFPlusGen_Ntrk11_15"] = new TH1F("sumEnergyHFPlusGen_Ntrk11_15","sumEnergyHFPlusGen_Ntrk11_15",100,0.,100.);
   histosTH1F["sumEnergyHFPlusGen_Ntrk16_25"] = new TH1F("sumEnergyHFPlusGen_Ntrk16_25","sumEnergyHFPlusGen_Ntrk16_25",100,0.,100.);
   histosTH1F["sumEnergyHFPlusGen_Ntrk26_100"] = new TH1F("sumEnergyHFPlusGen_Ntrk26_100","sumEnergyHFPlusGen_Ntrk26_100",100,0.,100.);
   //float binningLogXi[] = {-5.,-4.5,-4.,-3.5,-3.,-2.5,-2.0,0.};
   float binningLogXi[] = {-4.,-3.5,-3.};
   std::vector<float> vec_binningLogXi(binningLogXi,binningLogXi + sizeof(binningLogXi)/sizeof(float));
   for(size_t ibinLogXi = 0; ibinLogXi < (vec_binningLogXi.size()-1); ++ibinLogXi){
      std::stringstream histoNameXiPlus,histoNameGenXiPlus,
                        histoNameXiGenPlus,histoNameGenXiGenPlus,
                        histoNameXiMinus,histoNameGenXiMinus,
                        histoNameXiGenMinus,histoNameGenXiGenMinus,
                        histoNameRecVsGenXiPlus,
                        histoNameRecVsGenXiMinus,
                        histoNameRecVsGenXiGenPlus,
                        histoNameRecVsGenXiGenMinus;
      histoNameXiPlus << "multiplicityTracks_LogXiPlus_" << ibinLogXi;
      histoNameGenXiPlus << "multiplicityTracksGen_LogXiPlus_" << ibinLogXi; 
      histoNameXiGenPlus << "multiplicityTracks_LogXiGenPlus_" << ibinLogXi;
      histoNameGenXiGenPlus << "multiplicityTracksGen_LogXiGenPlus_" << ibinLogXi;
      histoNameXiMinus << "multiplicityTracks_LogXiMinus_" << ibinLogXi;
      histoNameGenXiMinus << "multiplicityTracksGen_LogXiMinus_" << ibinLogXi;
      histoNameXiGenMinus << "multiplicityTracks_LogXiGenMinus_" << ibinLogXi;
      histoNameGenXiGenMinus << "multiplicityTracksGen_LogXiGenMinus_" << ibinLogXi; 
      histoNameRecVsGenXiPlus << "multiplicityTracksVsGen_LogXiPlus_" << ibinLogXi;
      histoNameRecVsGenXiMinus << "multiplicityTracksVsGen_LogXiMinus_" << ibinLogXi;
      histoNameRecVsGenXiGenPlus << "multiplicityTracksVsGen_LogXiGenPlus_" << ibinLogXi;
      histoNameRecVsGenXiGenMinus << "multiplicityTracksVsGen_LogXiGenMinus_" << ibinLogXi;

      histosTH1F[histoNameXiPlus.str()] = new TH1F(histoNameXiPlus.str().c_str(),histoNameXiPlus.str().c_str(),40,0,40);
      histosTH1F[histoNameGenXiPlus.str()] = new TH1F(histoNameGenXiPlus.str().c_str(),histoNameGenXiPlus.str().c_str(),40,0,40);
      histosTH1F[histoNameXiGenPlus.str()] = new TH1F(histoNameXiGenPlus.str().c_str(),histoNameXiGenPlus.str().c_str(),40,0,40);
      histosTH1F[histoNameGenXiGenPlus.str()] = new TH1F(histoNameGenXiGenPlus.str().c_str(),histoNameGenXiGenPlus.str().c_str(),40,0,40);

      histosTH1F[histoNameXiMinus.str()] = new TH1F(histoNameXiMinus.str().c_str(),histoNameXiMinus.str().c_str(),40,0,40);
      histosTH1F[histoNameGenXiMinus.str()] = new TH1F(histoNameGenXiMinus.str().c_str(),histoNameGenXiMinus.str().c_str(),40,0,40);
      histosTH1F[histoNameXiGenMinus.str()] = new TH1F(histoNameXiGenMinus.str().c_str(),histoNameXiGenMinus.str().c_str(),40,0,40);
      histosTH1F[histoNameGenXiGenMinus.str()] = new TH1F(histoNameGenXiGenMinus.str().c_str(),histoNameGenXiGenMinus.str().c_str(),40,0,40);

      histosTH2F[histoNameRecVsGenXiPlus.str()] = new TH2F(histoNameRecVsGenXiPlus.str().c_str(),histoNameRecVsGenXiPlus.str().c_str(),40,0,40,40,0,40);
      histosTH2F[histoNameRecVsGenXiMinus.str()] = new TH2F(histoNameRecVsGenXiMinus.str().c_str(),histoNameRecVsGenXiMinus.str().c_str(),40,0,40,40,0,40);
      histosTH2F[histoNameRecVsGenXiGenPlus.str()] = new TH2F(histoNameRecVsGenXiGenPlus.str().c_str(),histoNameRecVsGenXiGenPlus.str().c_str(),40,0,40,40,0,40); 
      histosTH2F[histoNameRecVsGenXiGenMinus.str()] = new TH2F(histoNameRecVsGenXiGenMinus.str().c_str(),histoNameRecVsGenXiGenMinus.str().c_str(),40,0,40,40,0,40);
   }

   for(HistoMapTH1F::const_iterator it = histosTH1F.begin(); it != histosTH1F.end(); ++it)
      it->second->Sumw2();
   for(HistoMapTH2F::const_iterator it = histosTH2F.begin(); it != histosTH2F.end(); ++it)
      it->second->Sumw2();
 
   std::vector<int> processIDs;
   std::vector<std::string> processNames;
   if(accessMCInfo){ 
      if(genType == PYTHIA) getProcessIdsPYTHIA(processIDs,processNames);
      else if(genType == PHOJET) getProcessIdsPHOJET(processIDs,processNames);
      else if(genType == PYTHIA8) getProcessIdsPYTHIA8(processIDs,processNames);
      else {std::cout << ">>> ERROR: Need to set a generator type" << std::endl;return;}

      histosTH1F["ProcessId"] = new TH1F("ProcessId","ProcessId",processIDs.size(),0,processIDs.size());
      for(size_t iprocess = 0; iprocess < processIDs.size(); ++iprocess) histosTH1F["ProcessId"] ->GetXaxis()->SetBinLabel(iprocess + 1,processNames[iprocess].c_str());
   }

   // Event loop
   //==========================
   double eventWeight = 1.0;
   //std::vector<std::pair<int,int> > selectedEvents;
   int nEntries = data->GetEntries();
   for(int ientry = 0; ientry < nEntries; ++ientry){
      if((maxEvents > 0)&&(ientry == maxEvents)) break;
      //if(verbose && ientry%2000 == 0) std::cout << ">>> Analysing " << ientry << "th entry" << std::endl;
      if(ientry%2000 == 0) std::cout << ">>> Analysing " << ientry << "th entry" << std::endl;

      data->GetEntry(ientry);

      int eventNumber = eventData.eventNumber_;
      int runNumber = eventData.runNumber_;
      int lumiSection = eventData.lumiSection_;
      int bunchCrossing = eventData.bunchCrossing_;
      if(verbose){
         std::cout << "  Event number: " << eventNumber << std::endl
                   << "  Run number: " << runNumber << std::endl
                   << "  Lumi section: " << lumiSection << std::endl;
      }
      if((runNumber == 0)&&(eventNumber == 0)) {std::cout << ">>> ERROR: Problem with event...skipping" << std::endl;continue;}

      // Reweighing
      if(accessMCInfo){
         double weight_vtx = 1.;
         int ibin_vtx = h_reweightPosZVertex.FindBin( eventData.posZPrimVtx_ );
         if(ibin_vtx == 0) ibin_vtx = 1;
         if(ibin_vtx > h_reweightPosZVertex.GetNbinsX()) ibin_vtx = h_reweightPosZVertex.GetNbinsX();
         if( ibin_vtx >= 1 && ibin_vtx <= h_reweightPosZVertex.GetNbinsX() ){
            weight_vtx = h_reweightPosZVertex.GetBinContent(ibin_vtx);
         }else{
            weight_vtx = 0.;
         }
    
         double weight_eff = 1.;
         double eff_variable = eventData.multiplicityTracks_;
         int ibin_eff = h_reweightEffBscOr.FindBin( eff_variable );
         if( ibin_eff >= 1 && ibin_eff <= h_reweightEffBscOr.GetNbinsX() ){
            weight_eff = h_reweightEffBscOr.GetBinContent(ibin_eff);
         }else{
            weight_eff = 0.;
         }
         // Apply weight
         double weight_all = weight_vtx*weight_eff;
         if(ientry%2000 == 0) std::cout << ">>> Event weight " << weight_all << std::endl
                                        << "     BSC OR eff. " << weight_eff << std::endl
                                        << "   Vertex Z pos. " << weight_vtx << std::endl;
         //eventWeight *= weight_vtx;   
      }      

      histosTH1F["EventsPerBunchCrossing"]->Fill(bunchCrossing,eventWeight);
      histosTH1F["EventSelection"]->Fill("All",eventWeight);

      if(accessMCInfo){
         int processId = eventData.processId_;
         if(selectProcessIds && std::find(selectedProcIds.begin(),selectedProcIds.end(),processId) == selectedProcIds.end()) continue;
         
         // Gen level selection
         if(doEtaMaxGenSelection && eventData.etaMaxGen_ > etaMaxGenMax) continue;
         if(doEtaMinGenSelection && eventData.etaMinGen_ < etaMinGenMin) continue;

         if(doDeltaEtaGenSelection && eventData.deltaEtaGen_ < deltaEtaGenMin) continue;

         double logXiGenPlus = (eventData.MxGenMinus_*eventData.MxGenMinus_)/(4*EBeam*EBeam);
         double logXiGenMinus = (eventData.MxGenPlus_*eventData.MxGenPlus_)/(4*EBeam*EBeam);
         logXiGenPlus = (logXiGenPlus > 0.) ? log10(logXiGenPlus) : -999.;
         logXiGenMinus = (logXiGenMinus > 0.) ? log10(logXiGenMinus) : -999.; 
         if(doLogXiGenPlusSelection && logXiGenPlus > logXiGenPlusMax) continue;
         if(doLogXiGenMinusSelection && logXiGenMinus > logXiGenMinusMax) continue;
          
         // Fill histo with process Id's
         std::vector<int>::const_iterator it_processId = std::find(processIDs.begin(),processIDs.end(),processId);
         if(it_processId != processIDs.end()){
           int idx_processId = it_processId - processIDs.begin();
           histosTH1F["ProcessId"]->Fill(idx_processId,eventWeight);
         }  
      }

      if(!accessMCInfo && selectEventsInRuns && std::find(selectedRuns.begin(),selectedRuns.end(),runNumber) == selectedRuns.end()) continue;

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

      if(passNoiseLoose) histosTH1F["HcalNoiseId"]->Fill(0.,eventWeight);
      if(passNoiseTight) histosTH1F["HcalNoiseId"]->Fill(1.,eventWeight); 

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
     
      if(beamHaloLooseId) histosTH1F["BeamHaloId"]->Fill(0.,eventWeight);
      if(beamHaloTightId) histosTH1F["BeamHaloId"]->Fill(1.,eventWeight);

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
      histosTH1F["nVertex"]->Fill(nGoodVertices,eventWeight);
      if(doVertexSelection && (nGoodVertices < 1)) continue;

      double posXPrimVtx = eventData.posXPrimVtx_;
      double posYPrimVtx = eventData.posYPrimVtx_;
      double posZPrimVtx = eventData.posZPrimVtx_;
      histosTH1F["posXPrimVtx"]->Fill(posXPrimVtx,eventWeight);
      histosTH1F["posYPrimVtx"]->Fill(posYPrimVtx,eventWeight);
      histosTH1F["posZPrimVtx"]->Fill(posZPrimVtx,eventWeight);
      histosTH1F["posRPrimVtx"]->Fill(sqrt(posXPrimVtx*posXPrimVtx + posYPrimVtx*posYPrimVtx),eventWeight);
      //histosTH1F["numberDOF"]->Fill();

      if(doVertexSelection && (fabs(posZPrimVtx) > primVtxZMax)) continue;
      histosTH1F["EventSelection"]->Fill("VertexSelection",eventWeight);

      // HF Selection
      if(doHFSelection && ( eventData.sumEnergyHFPlus_ > sumEnergyHFPlusMax || eventData.sumEnergyHFMinus_ > sumEnergyHFMinusMax)) continue;
      histosTH1F["EventSelection"]->Fill("HCALSelection",eventWeight);

      // MET - SumET
      if(doSumETSelection && (eventData.sumET_ < sumETMin)) continue;
      histosTH1F["EventSelection"]->Fill("SumETSelection",eventWeight);

      // Mx
      if(doMxSelection && ((eventData.MxFromPFCands_ < MxMin)||eventData.MxFromPFCands_ > MxMax)) continue;
      histosTH1F["EventSelection"]->Fill("MxSelection",eventWeight);

      // Xi
      if( doXiPlusSelection && (eventData.xiPlusFromPFCands_ > xiMax) ) continue;
      histosTH1F["EventSelection"]->Fill("XiPlusSelection",eventWeight);
      if( doXiMinusSelection && (eventData.xiMinusFromPFCands_ > xiMax) ) continue;
      histosTH1F["EventSelection"]->Fill("XiMinusSelection",eventWeight); 

      // EtaMax
      if( doEtaMaxSelection && (eventData.etaMaxFromPFCands_ > etaMaxMax) ) continue;
      histosTH1F["EventSelection"]->Fill("EtaMaxSelection",eventWeight);
      if( doEtaMinSelection && (eventData.etaMinFromPFCands_ < etaMinMin) ) continue;
      histosTH1F["EventSelection"]->Fill("EtaMinSelection",eventWeight);

      // Fill Histos
      // SumET
      double sumET = eventData.sumET_;
      histosTH1F["sumET"]->Fill(sumET,eventWeight);

      // Mx
      double MxFromTowers = eventData.MxFromTowers_;
      double MxFromPFCands = eventData.MxFromPFCands_;

      histosTH1F["MxFromTowers"]->Fill(MxFromTowers,eventWeight);
      histosTH1F["MxFromPFCands"]->Fill(MxFromPFCands,eventWeight);
      if(accessMCInfo){
         double MxGen = eventData.MxGen_;
         double MxGenDiss = eventData.MxGenDiss_;
         double MyGenDiss = eventData.MyGenDiss_;
         double MxGenRange = eventData.MxGenRange_; 
         //histosTH1F["MxGen"]->Fill(MxGen,eventWeight);
         histosTH1F["MxGen"]->Fill(MxGenRange,eventWeight); 
         histosTH1F["MxGenDiss"]->Fill(MxGenDiss,eventWeight); 
         histosTH1F["MyGenDiss"]->Fill(MyGenDiss,eventWeight); 
         histosTH1F["ResMxFromTowers"]->Fill((MxFromTowers - MxGenRange)/MxGenRange,eventWeight);
         histosTH1F["ResMxFromPFCands"]->Fill((MxFromPFCands - MxGenRange)/MxGenRange,eventWeight);
      }

      // Jets
      double leadingJetPt = eventData.leadingJetPt_;
      double leadingJetEta = eventData.leadingJetEta_;
      double leadingJetPhi = eventData.leadingJetPhi_;
      if(leadingJetPt > 0.){
         histosTH1F["leadingJetPt"]->Fill(leadingJetPt,eventWeight);
         histosTH1F["leadingJetEta"]->Fill(leadingJetEta,eventWeight);
         histosTH1F["leadingJetPhi"]->Fill(leadingJetPhi,eventWeight);
      }

      // Xi
      //double xiPlusFromTowers = eventData.xiPlusFromTowers_;
      //double xiMinusFromTowers = eventData.xiMinusFromTowers_;
      double xiPlusFromTowers = eventData.EPlusPzFromTowers_; xiPlusFromTowers /= 2*EBeam;
      double xiMinusFromTowers = eventData.EMinusPzFromTowers_; xiMinusFromTowers /= 2*EBeam;
      histosTH1F["xiPlusFromTowers"]->Fill(xiPlusFromTowers,eventWeight);
      histosTH1F["xiMinusFromTowers"]->Fill(xiMinusFromTowers,eventWeight);

      //double xiPlusFromPFCands = eventData.xiPlusFromPFCands_;
      //double xiMinusFromPFCands = eventData.xiMinusFromPFCands_;
      double xiPlusFromPFCands = eventData.EPlusPzFromPFCands_; xiPlusFromPFCands /= 2*EBeam;
      double xiMinusFromPFCands = eventData.EMinusPzFromPFCands_; xiMinusFromPFCands /= 2*EBeam;
      // Apply correction factor
      xiPlusFromPFCands *= xiCorrectionFactor;
      xiMinusFromPFCands *= xiCorrectionFactor;

      histosTH1F["xiPlusFromPFCands"]->Fill(xiPlusFromPFCands,eventWeight);
      histosTH1F["xiMinusFromPFCands"]->Fill(xiMinusFromPFCands,eventWeight);
      if(xiPlusFromPFCands){
         histosTH1F["logXiPlusFromPFCands"]->Fill( log10(xiPlusFromPFCands),eventWeight );
         histosTH1F["logXiPlusFromPFCandsVarBin"]->Fill( log10(xiPlusFromPFCands),eventWeight );
      }
      if(xiMinusFromPFCands){
         histosTH1F["logXiMinusFromPFCands"]->Fill( log10(xiMinusFromPFCands),eventWeight );
         histosTH1F["logXiMinusFromPFCandsVarBin"]->Fill( log10(xiMinusFromPFCands),eventWeight );
      }

      double EPlusPzFromTowers = eventData.EPlusPzFromTowers_;
      double EMinusPzFromTowers = eventData.EMinusPzFromTowers_;
      histosTH1F["EPlusPzFromTowers"]->Fill(EPlusPzFromTowers,eventWeight);
      histosTH1F["EMinusPzFromTowers"]->Fill(EMinusPzFromTowers,eventWeight);
      /*histosTH1F["EPlusPzFromTowersVarBin"]->Fill((EPlusPzFromTowers < minEVarBin) ? minEVarBin : EPlusPzFromTowers);
      histosTH1F["EMinusPzFromTowersVarBin"]->Fill((EMinusPzFromTowers < minEVarBin) ? minEVarBin : EMinusPzFromTowers);*/
      histosTH1F["EPlusPzFromTowersVarBin"]->Fill(EPlusPzFromTowers,eventWeight);
      histosTH1F["EMinusPzFromTowersVarBin"]->Fill(EMinusPzFromTowers,eventWeight);
 
      double EPlusPzFromPFCands = eventData.EPlusPzFromPFCands_;
      double EMinusPzFromPFCands = eventData.EMinusPzFromPFCands_;
      histosTH1F["EPlusPzFromPFCands"]->Fill(EPlusPzFromPFCands,eventWeight);
      histosTH1F["EMinusPzFromPFCands"]->Fill(EMinusPzFromPFCands,eventWeight);

      /*unsigned int nTracks = eventData.trackMultiplicity_;
      unsigned int nTracksAssociatedToPV = eventData.trackMultiplicityAssociatedToPV_;*/
      int multiplicityTracks = eventData.multiplicityTracks_;
      double sumPtTracks = eventData.sumPtTracks_;

      //histosTH1F["trackMultiplicity"]->Fill(nTracks);
      //histosTH1F["trackMultiplicityAssociatedToPV"]->Fill(nTracksAssociatedToPV);
      histosTH1F["multiplicityTracks"]->Fill(multiplicityTracks,eventWeight);
      histosTH1F["sumPtTracks"]->Fill(sumPtTracks,eventWeight);

      // Access multiplicities
      unsigned int nHE_plus = eventData.multiplicityHEPlus_;
      unsigned int nHE_minus = eventData.multiplicityHEMinus_;
      unsigned int nHF_plus = eventData.multiplicityHFPlus_;
      unsigned int nHF_minus = eventData.multiplicityHFMinus_;
 
      histosTH1F["multiplicityHEPlus"]->Fill(nHE_plus,eventWeight);
      histosTH1F["multiplicityHEMinus"]->Fill(nHE_minus,eventWeight);
      histosTH1F["multiplicityHFPlus"]->Fill(nHF_plus,eventWeight);
      histosTH1F["multiplicityHFMinus"]->Fill(nHF_minus,eventWeight);     
      histosTH1F["multiplicityHFPlusVarBin"]->Fill(nHF_plus,eventWeight);
      histosTH1F["multiplicityHFMinusVarBin"]->Fill(nHF_minus,eventWeight);

      histosTH2F["multiplicityHFVsHEPlus"]->Fill(nHF_plus,nHE_plus,eventWeight);
      histosTH2F["multiplicityHFVsHEMinus"]->Fill(nHF_minus,nHE_minus,eventWeight);
 
      for(unsigned int ieta = 29, index = 0; ieta <= 41; ++ieta,++index){
         int nHFPlus_ieta = eventData.multiplicityHFPlusVsiEta_[index];
         double sumEHFPlus_ieta = eventData.sumEHFPlusVsiEta_[index];
         double sumETHFPlus_ieta = eventData.sumETHFPlusVsiEta_[index];
         histosTH2F["multiplicityHFVsiEtaPlus"]->Fill(ieta,nHFPlus_ieta,eventWeight);
         histosTH2F["sumEnergyHFVsiEtaPlus"]->Fill(ieta,sumEHFPlus_ieta,eventWeight);
         histosTH2F["sumETHFVsiEtaPlus"]->Fill(ieta,sumETHFPlus_ieta,eventWeight);

         int nHFMinus_ieta = eventData.multiplicityHFMinusVsiEta_[index]; 
         double sumEHFMinus_ieta = eventData.sumEHFMinusVsiEta_[index];
         double sumETHFMinus_ieta = eventData.sumETHFMinusVsiEta_[index];
         histosTH2F["multiplicityHFVsiEtaMinus"]->Fill(ieta,nHFMinus_ieta,eventWeight); 
         histosTH2F["sumEnergyHFVsiEtaMinus"]->Fill(ieta,sumEHFMinus_ieta,eventWeight);
         histosTH2F["sumETHFVsiEtaMinus"]->Fill(ieta,sumETHFMinus_ieta,eventWeight);
      }

      double missingMassFromXiFromTowers = eventData.missingMassFromXiFromTowers_;
      histosTH1F["missingMassFromXiFromTowers"]->Fill(missingMassFromXiFromTowers,eventWeight);
      //double missingMassFromXiFromJets =
      //histosTH1F["missingMassFromXiFromJets"]->Fill(missingMassFromXiFromJets);
      double missingMassFromXiFromPFCands = eventData.missingMassFromXiFromPFCands_;
      histosTH1F["missingMassFromXiFromPFCands"]->Fill(missingMassFromXiFromPFCands,eventWeight);

      double sumEHE_plus = eventData.sumEnergyHEPlus_;
      double sumEHE_minus = eventData.sumEnergyHEMinus_;
      double sumEHF_plus = eventData.sumEnergyHFPlus_;
      double sumEHF_minus = eventData.sumEnergyHFMinus_;
      histosTH1F["sumEnergyHEPlus"]->Fill(sumEHE_plus,eventWeight);
      histosTH1F["sumEnergyHEMinus"]->Fill(sumEHE_minus,eventWeight);
      histosTH1F["sumEnergyHFPlus"]->Fill(sumEHF_plus,eventWeight);
      histosTH1F["sumEnergyHFMinus"]->Fill(sumEHF_minus,eventWeight);
      /*histosTH1F["sumEnergyHFPlusVarBin"]->Fill((sumE_plus < minEVarBin) ? minEVarBin : sumE_plus);
      histosTH1F["sumEnergyHFMinusVarBin"]->Fill((sumE_minus < minEVarBin) ? minEVarBin: sumE_minus);*/
      histosTH1F["sumEnergyHFPlusVarBin"]->Fill(sumEHF_plus,eventWeight);
      histosTH1F["sumEnergyHFMinusVarBin"]->Fill(sumEHF_minus,eventWeight);

      double sumECASTOR = eventData.sumETotCastor_;
      histosTH1F["sumEnergyCASTOR"]->Fill(sumECASTOR,eventWeight);

      double etaMaxFromPFCands = eventData.etaMaxFromPFCands_;
      double etaMinFromPFCands = eventData.etaMinFromPFCands_;
      histosTH1F["etaMaxFromPFCands"]->Fill(etaMaxFromPFCands,eventWeight);
      histosTH1F["etaMinFromPFCands"]->Fill(etaMinFromPFCands,eventWeight);
      histosTH1F["etaMaxFromPFCandsVarBin"]->Fill(etaMaxFromPFCands,eventWeight);
      histosTH1F["etaMinFromPFCandsVarBin"]->Fill(etaMinFromPFCands,eventWeight);

      // Histos as a function of xi bin
      for(size_t ibinLogXi = 0; ibinLogXi < (vec_binningLogXi.size() - 1); ++ibinLogXi){
         std::stringstream histoNameXiPlus,histoNameXiMinus;
         histoNameXiPlus << "multiplicityTracks_LogXiPlus_" << ibinLogXi;
         histoNameXiMinus << "multiplicityTracks_LogXiMinus_" << ibinLogXi;
         if(xiPlusFromPFCands > 0.){
            if( (log10(xiPlusFromPFCands) >= vec_binningLogXi[ibinLogXi]) &&
                (log10(xiPlusFromPFCands) < vec_binningLogXi[ibinLogXi+1]) ){
               histosTH1F[histoNameXiPlus.str()]->Fill(multiplicityTracks,eventWeight);
            }
         }
         if(xiMinusFromPFCands > 0.){
            if( (log10(xiMinusFromPFCands) >= vec_binningLogXi[ibinLogXi]) &&
                (log10(xiMinusFromPFCands) < vec_binningLogXi[ibinLogXi+1]) ){
               histosTH1F[histoNameXiMinus.str()]->Fill(multiplicityTracks,eventWeight);
            }
         }
      }

      if(accessMCInfo){
         double etaMaxGen = eventData.etaMaxGen_;
         double etaMinGen = eventData.etaMinGen_;

         double deltaEtaGen = eventData.deltaEtaGen_; 
         double etaGapLow   = eventData.etaGapLow_;
         double etaGapHigh  = eventData.etaGapHigh_;
 
         int multiplicityTracksGen = eventData.multiplicityTracksGen_;
         double sumPtTracksGen = eventData.sumPtTracksGen_;
 
         double sumEHEGen_plus = eventData.sumEnergyHEPlusGen_;
         double sumEHEGen_minus = eventData.sumEnergyHEMinusGen_;
         double sumEHFGen_plus = eventData.sumEnergyHFPlusGen_;
         double sumEHFGen_minus = eventData.sumEnergyHFMinusGen_;
 
         histosTH1F["etaMaxGen"]->Fill(etaMaxGen,eventWeight);
         histosTH1F["etaMinGen"]->Fill(etaMinGen,eventWeight);
         histosTH2F["etaMaxFromPFCandsVsetaMaxGen"]->Fill(etaMaxGen,etaMaxFromPFCands,eventWeight);
         histosTH2F["etaMinFromPFCandsVsetaMinGen"]->Fill(etaMinGen,etaMinFromPFCands,eventWeight);

         histosTH1F["deltaEtaGen"]->Fill(deltaEtaGen,eventWeight);
         histosTH1F["etaGapLow"]->Fill(etaGapLow,eventWeight);
         histosTH1F["etaGapHigh"]->Fill(etaGapHigh,eventWeight);

         histosTH1F["multiplicityTracksGen"]->Fill(multiplicityTracksGen,eventWeight);
         histosTH1F["sumPtTracksGen"]->Fill(sumPtTracksGen,eventWeight);
         histosTH2F["multiplicityTracksVsGen"]->Fill(multiplicityTracksGen,multiplicityTracks,eventWeight);
         histosTH2F["sumPtTracksVsGen"]->Fill(sumPtTracksGen,sumPtTracks,eventWeight); 

         histosTH1F["sumEnergyHEPlusGen"]->Fill(sumEHEGen_plus,eventWeight);
         histosTH1F["sumEnergyHEMinusGen"]->Fill(sumEHEGen_minus,eventWeight);
         histosTH1F["sumEnergyHFPlusGen"]->Fill(sumEHFGen_plus,eventWeight);
         histosTH1F["sumEnergyHFMinusGen"]->Fill(sumEHFGen_minus,eventWeight);
         
         /*// xi <= 0.1
         double xigen_plus = eventData.xiGenPlus_;
         double xigen_minus = eventData.xiGenMinus_;*/
         /*double xigen_plus = -1.;
         double xigen_minus = -1.;
         double xi_gen = (eventData.MxGenDiss_*eventData.MxGenDiss_);
         xi_gen /= 7000.*7000.;
         std::vector<int> procIdsSDPlus,procIdsSDMinus;
         if(genType == PYTHIA){
            getSelectedProcIdsPYTHIA(SDPlus,procIdsSDPlus);
            getSelectedProcIdsPYTHIA(SDMinus,procIdsSDMinus);
         } else if(genType == PYTHIA8){
            getSelectedProcIdsPYTHIA8(SDPlus,procIdsSDPlus);
            getSelectedProcIdsPYTHIA8(SDMinus,procIdsSDMinus);
         }
         // SDPlus   
         if( eventData.processId_ == *(procIdsSDPlus.begin()) )  xigen_plus = xi_gen;
         if( eventData.processId_ == *(procIdsSDMinus.begin()) ) xigen_minus = xi_gen;*/
         // Xi hadron level definition
         double xigen_plus = (eventData.MxGenMinus_*eventData.MxGenMinus_)/(4*EBeam*EBeam);
         double xigen_minus = (eventData.MxGenPlus_*eventData.MxGenPlus_)/(4*EBeam*EBeam);
         if(xigen_plus > 0.){ 
            histosTH1F["xiGenPlus"]->Fill(xigen_plus,eventWeight);
            histosTH1F["logXiGenPlus"]->Fill( log10(xigen_plus),eventWeight );
            histosTH1F["logXiGenPlusVarBin"]->Fill( log10(xigen_plus),eventWeight );
            histosTH1F["ResXiPlusFromTowers"]->Fill((xiPlusFromTowers - xigen_plus)/xigen_plus,eventWeight);
            histosTH1F["ResXiPlusFromPFCands"]->Fill((xiPlusFromPFCands - xigen_plus)/xigen_plus,eventWeight);
            histosTH2F["xiFromTowersVsxiGenPlus"]->Fill(xigen_plus,xiPlusFromTowers,eventWeight);
            histosTH2F["xiFromPFCandsVsxiGenPlus"]->Fill(xigen_plus,xiPlusFromPFCands,eventWeight);
            histosTH2F["xiFromTowersVslogXiGenPlus"]->Fill( log10(xigen_plus),xiPlusFromTowers,eventWeight );
            histosTH2F["xiFromPFCandsVslogXiGenPlus"]->Fill( log10(xigen_plus),xiPlusFromPFCands,eventWeight );
            if(xiPlusFromPFCands > 0.){
               histosTH1F["ResLogXiPlusFromPFCands"]->Fill( log10(xiPlusFromPFCands) - log10(xigen_plus),eventWeight );
 
               histosTH2F["logXiFromPFCandsVslogXiGenPlus"]->Fill( log10(xigen_plus),log10(xiPlusFromPFCands),eventWeight );
               bool sameBin = ( histosTH1F["logXiPlusFromPFCands"]->FindBin(log10(xiPlusFromPFCands)) == histosTH1F["logXiGenPlus"]->FindBin(log10(xigen_plus)) );
               if(sameBin) histosTH1F["logXiFromPFCandsANDXiGenPlus"]->Fill(log10(xigen_plus),eventWeight);

               bool sameBinVarBin = ( histosTH1F["logXiPlusFromPFCandsVarBin"]->FindBin(log10(xiPlusFromPFCands)) == histosTH1F["logXiGenPlusVarBin"]->FindBin(log10(xigen_plus)) );
               if(sameBinVarBin) histosTH1F["logXiFromPFCandsANDXiGenPlusVarBin"]->Fill(log10(xigen_plus),eventWeight);
            }
         }
         if(xigen_minus > 0.){ 
            histosTH1F["xiGenMinus"]->Fill(xigen_minus,eventWeight);
            histosTH1F["logXiGenMinus"]->Fill( log10(xigen_minus),eventWeight );
            histosTH1F["logXiGenMinusVarBin"]->Fill( log10(xigen_minus),eventWeight );
            histosTH1F["ResXiMinusFromTowers"]->Fill((xiMinusFromTowers - xigen_minus)/xigen_minus,eventWeight);
            histosTH1F["ResXiMinusFromPFCands"]->Fill((xiMinusFromPFCands - xigen_minus)/xigen_minus,eventWeight);
            histosTH2F["xiFromTowersVslogXiGenMinus"]->Fill( log10(xigen_minus),xiMinusFromTowers,eventWeight );
            histosTH2F["xiFromPFCandsVslogXiGenMinus"]->Fill( log10(xigen_minus),xiMinusFromPFCands,eventWeight );
            if(xiMinusFromPFCands > 0.){
               histosTH1F["ResLogXiMinusFromPFCands"]->Fill( log10(xiMinusFromPFCands) - log10(xigen_minus),eventWeight ); 
               histosTH2F["logXiFromPFCandsVslogXiGenMinus"]->Fill( log10(xigen_minus),log10(xiMinusFromPFCands),eventWeight );
               bool sameBin = ( histosTH1F["logXiMinusFromPFCands"]->FindBin(log10(xiMinusFromPFCands)) == histosTH1F["logXiGenMinus"]->FindBin(log10(xigen_minus)) );
               if(sameBin) histosTH1F["logXiFromPFCandsANDXiGenMinus"]->Fill(log10(xigen_minus),eventWeight);
               bool sameBinVarBin = ( histosTH1F["logXiMinusFromPFCandsVarBin"]->FindBin(log10(xiMinusFromPFCands)) == histosTH1F["logXiGenMinusVarBin"]->FindBin(log10(xigen_minus)) );
               if(sameBinVarBin) histosTH1F["logXiFromPFCandsANDXiGenMinusVarBin"]->Fill(log10(xigen_minus),eventWeight);
            }
         }
 
         // Histos as a function of Ntrk
         if(multiplicityTracks >= 0 && multiplicityTracks <= 5) histosTH1F["sumEnergyHFPlusGen_Ntrk0_5"]->Fill(sumEHFGen_plus,eventWeight);
         else if(multiplicityTracks > 5 && multiplicityTracks <= 10) histosTH1F["sumEnergyHFPlusGen_Ntrk6_10"]->Fill(sumEHFGen_plus,eventWeight);
         else if(multiplicityTracks > 10 && multiplicityTracks <= 15) histosTH1F["sumEnergyHFPlusGen_Ntrk11_15"]->Fill(sumEHFGen_plus,eventWeight);
         else if(multiplicityTracks > 15 && multiplicityTracks <= 25) histosTH1F["sumEnergyHFPlusGen_Ntrk16_25"]->Fill(sumEHFGen_plus,eventWeight);
         else if(multiplicityTracks > 25 && multiplicityTracks <= 100) histosTH1F["sumEnergyHFPlusGen_Ntrk26_100"]->Fill(sumEHFGen_plus,eventWeight);

         // Histos as a function of xi bin
         for(size_t ibinLogXi = 0; ibinLogXi < (vec_binningLogXi.size() - 1); ++ibinLogXi){
            std::stringstream histoNameGenXiPlus,
                              histoNameXiGenPlus,histoNameGenXiGenPlus,
                              histoNameGenXiMinus,
                              histoNameXiGenMinus,histoNameGenXiGenMinus,
                              histoNameRecVsGenXiPlus,
                              histoNameRecVsGenXiMinus,
                              histoNameRecVsGenXiGenPlus,
                              histoNameRecVsGenXiGenMinus;
            //histoNameXiPlus << "multiplicityTracks_LogXiPlus_" << ibinLogXi;
            histoNameGenXiPlus << "multiplicityTracksGen_LogXiPlus_" << ibinLogXi; 
            histoNameXiGenPlus << "multiplicityTracks_LogXiGenPlus_" << ibinLogXi;
            histoNameGenXiGenPlus << "multiplicityTracksGen_LogXiGenPlus_" << ibinLogXi;
            //histoNameXiMinus << "multiplicityTracks_LogXiMinus_" << ibinLogXi;
            histoNameGenXiMinus << "multiplicityTracksGen_LogXiMinus_" << ibinLogXi;
            histoNameXiGenMinus << "multiplicityTracks_LogXiGenMinus_" << ibinLogXi;
            histoNameGenXiGenMinus << "multiplicityTracksGen_LogXiGenMinus_" << ibinLogXi; 
            histoNameRecVsGenXiPlus << "multiplicityTracksVsGen_LogXiPlus_" << ibinLogXi;
            histoNameRecVsGenXiMinus << "multiplicityTracksVsGen_LogXiMinus_" << ibinLogXi;
            histoNameRecVsGenXiGenPlus << "multiplicityTracksVsGen_LogXiGenPlus_" << ibinLogXi;
            histoNameRecVsGenXiGenMinus << "multiplicityTracksVsGen_LogXiGenMinus_" << ibinLogXi;
            if(xiPlusFromPFCands > 0.){
               if( (log10(xiPlusFromPFCands) >= vec_binningLogXi[ibinLogXi]) && 
                   (log10(xiPlusFromPFCands) < vec_binningLogXi[ibinLogXi+1]) ){
                  histosTH1F[histoNameGenXiPlus.str()]->Fill(multiplicityTracksGen,eventWeight);
                  histosTH2F[histoNameRecVsGenXiPlus.str()]->Fill(multiplicityTracksGen,multiplicityTracks,eventWeight); 
               } 
            }
            if(xiMinusFromPFCands > 0.){
               if( (log10(xiMinusFromPFCands) >= vec_binningLogXi[ibinLogXi]) &&
                   (log10(xiMinusFromPFCands) < vec_binningLogXi[ibinLogXi+1]) ){
                  histosTH1F[histoNameGenXiMinus.str()]->Fill(multiplicityTracksGen,eventWeight);
                  histosTH2F[histoNameRecVsGenXiMinus.str()]->Fill(multiplicityTracksGen,multiplicityTracks,eventWeight);
               } 
            }
            if(xigen_plus > 0.){
               if( (log10(xigen_plus) >= vec_binningLogXi[ibinLogXi]) &&
                   (log10(xigen_plus) < vec_binningLogXi[ibinLogXi+1]) ){
                  histosTH1F[histoNameXiGenPlus.str()]->Fill(multiplicityTracks,eventWeight);
                  histosTH1F[histoNameGenXiGenPlus.str()]->Fill(multiplicityTracksGen,eventWeight);
                  histosTH2F[histoNameRecVsGenXiGenPlus.str()]->Fill(multiplicityTracksGen,multiplicityTracks,eventWeight);
               }
            }
            if(xigen_minus > 0.){
               if( (log10(xigen_minus) >= vec_binningLogXi[ibinLogXi]) &&
                   (log10(xigen_minus) < vec_binningLogXi[ibinLogXi+1]) ){
                  histosTH1F[histoNameXiGenMinus.str()]->Fill(multiplicityTracks,eventWeight);
                  histosTH1F[histoNameGenXiGenMinus.str()]->Fill(multiplicityTracksGen,eventWeight);
                  histosTH2F[histoNameRecVsGenXiGenMinus.str()]->Fill(multiplicityTracksGen,multiplicityTracks,eventWeight); 
               }
            }
         } 
      }
     
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
   
   /*histosTH1F["EPlusPzFromTowersVarBin"]->Scale(1.,"width");
   histosTH1F["EMinusPzFromTowersVarBin"]->Scale(1.,"width");
   histosTH1F["sumEnergyHFPlusVarBin"]->Scale(1.,"width");
   histosTH1F["sumEnergyHFMinusVarBin"]->Scale(1.,"width");
   histosTH1F["multiplicityHFPlusVarBin"]->Scale(1.,"width");
   histosTH1F["multiplicityHFMinusVarBin"]->Scale(1.,"width");*/

   hfile->Write();
   hfile->Close();
}
