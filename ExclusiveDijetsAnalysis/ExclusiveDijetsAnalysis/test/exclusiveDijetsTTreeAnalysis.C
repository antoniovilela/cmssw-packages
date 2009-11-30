#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TTree.h"

#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/EventData.h"
#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/Histos.h" 

#include "StdAllocatorAdaptor.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <string>

using namespace exclusiveDijetsAnalysis;

/*class CreateHisto{
  public:
     template <class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
     T* make(const Arg1 & a1, const Arg2 & a2, const Arg3 & a3, const Arg4 & a4, const Arg5 & a5) const{
        return new T(a1,a2,a3,a4,a5); 
     }

     template <class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
                        typename Arg5, typename Arg6, typename Arg7, typename Arg8>
     T* make(const Arg1 & a1, const Arg2 & a2, const Arg3 & a3, const Arg4 & a4, const Arg5 & a5, const Arg6 & a6, const Arg7 & a7, const Arg8 & a8) const{
        return new T(a1,a2,a3,a4,a5,a6,a7,a8);
     }
};*/

void exclusiveDijetsTTreeAnalysis(std::string const& fileName,
                                  std::string const& treeName,
                                  std::string const& singleVtxFilterName,
                                  std::string const& dijetsFilterName,
                                  std::string const& fileHLTName, 
                                  std::string const& outFileName = "analysisDijetsTTree_histos.root",
                                  int maxEvents = -1, bool verbose = false){

   if(verbose) std::cout << ">>> Reading file: " << fileName << std::endl;

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

   // For efficiencies
   TH1F* h_nVertexFilter = dynamic_cast<TH1F*>(file->Get((singleVtxFilterName + "/nVertex").c_str()));
   if(!h_nVertexFilter){
      std::cout << "ERROR: Could not find reference histo " << singleVtxFilterName << " in " << fileName << std::endl;
      file->Close();
      return;
   }

   TH1F* h_leadingJetPtFilter = dynamic_cast<TH1F*>(file->Get((dijetsFilterName + "/leadingJetPt").c_str()));
   if(!h_leadingJetPtFilter){
      std::cout << "ERROR: Could not find reference histo " << dijetsFilterName << " in " << fileName << std::endl;
      file->Close();
      return;
   }
   
   TFile* fileHLTEff = TFile::Open(fileHLTName.c_str(),"read");
   if(!fileHLTEff){
      std::cout << "ERROR: Could not find " << fileHLTName << std::endl;
      file->Close(); 
      return;
   }
   
   std::string treeNameHLT_before = "analysisBeforeSelection/data";
   std::string treeNameHLT_after = "analysisAfterSelection/data";
   TTree* dataHLT_before = dynamic_cast<TTree*>(fileHLTEff->Get(treeNameHLT_before.c_str()));
   if(!dataHLT_before){
      std::cout << "ERROR: Could not find " << treeNameHLT_before << " in " << fileHLTName << std::endl;
      fileHLTEff->Close();
      file->Close();
      return;
   }

   TTree* dataHLT_after = dynamic_cast<TTree*>(fileHLTEff->Get(treeNameHLT_after.c_str()));
   if(!dataHLT_after){
      std::cout << "ERROR: Could not find " << treeNameHLT_after << " in " << fileHLTName << std::endl;
      fileHLTEff->Close();
      file->Close();
      return;
   }

   EventData eventData;
   setTTreeBranches(*data,eventData);
   int nEntries = data->GetEntries(); 

   // Create output file
   TFile* hfile = new TFile(outFileName.c_str(),"recreate","data histograms");

   // Book Histograms
   //TH1::SetDefaultSumw2(true);

   HistoMapTH1F histosTH1F;
   HistoMapTH2F histosTH2F; 
   bookHistos(histosTH1F,StdAllocatorAdaptor());
   bookHistos(histosTH2F,StdAllocatorAdaptor());
   histosTH1F["LogRjjFromJetsAfterSel"] = new TH1F("LogRjjFromJetsAfterSel","LogRjjFromJetsAfterSel",200,-4.,0.);
   histosTH1F["LogRjjFromPFCandsAfterSel"] = new TH1F("LogRjjFromPFCandsAfterSel","LogRjjFromPFCandsAfterSel",200,-4.,0.);
   histosTH1F["SelectionEff"] = new TH1F("SelectionEff","SelectionEff",3,0,3);
   histosTH1F["SelectionEff"]->GetXaxis()->SetBinLabel(1,"HLT");
   histosTH1F["SelectionEff"]->GetXaxis()->SetBinLabel(2,"Single-vertex");
   histosTH1F["SelectionEff"]->GetXaxis()->SetBinLabel(3,"Di-jets pre-selection"); 

   // Fill efficiency histo
   double eff_HLT = (double)dataHLT_after->GetEntries()/(double)dataHLT_before->GetEntries();
   double eff_Vtx = (double)h_leadingJetPtFilter->GetEntries()/(double)h_nVertexFilter->GetEntries();
   double eff_dijetsSelection = (double)nEntries/(double)h_leadingJetPtFilter->GetEntries();
   histosTH1F["SelectionEff"]->SetBinContent(1,eff_HLT);
   histosTH1F["SelectionEff"]->SetBinContent(2,eff_Vtx);
   histosTH1F["SelectionEff"]->SetBinContent(3,eff_dijetsSelection);
   
   bool selectPileUp = false;
   int nEventsPUBx0 = 0;

   bool useHFTowerWeighted = true;

   // Event selection
   // Prim. vertices
   bool doVertexSelection = false;
   // Di-jet
   double ptMin = 50.;
   double etaMax = 2.5;
   
   double deltaEtaMax = 2.0;
   double deltaPhiMax = 0.4;
   double deltaPtMax = 999.;
   // B-tag
   bool doBTag = false;
   bool singleOrDoubleBTag = true; // false --> single, true --> double
   double bDiscMinValue = 3.0;
   // Third jet
   bool doThirdJetSelection = true;
   double thirdJetPtMax = 15.; 
   // Track multiplicity
   bool doTrackSelection = true; 
   int nTracksMax = 3;
   // HF-multiplicity
   bool doHFMultiplicitySelection = true; 
   int nHFPlusMax = 2;
   int nHFMinusMax = 2;

   // Loop over TTree
   //int nEntries = data->GetEntries();
   for(int ientry = 0; ientry < nEntries; ++ientry){
      if((maxEvents > 0)&&(ientry == maxEvents)) break;
      if(verbose && ientry%2000 == 0) std::cout << ">>> Analysing " << ientry << "th entry" << std::endl;

      data->GetEntry(ientry);

      // Pile-up
      int nPileUpBx0 = eventData.nPileUpBx0_;
      if(nPileUpBx0 < 0) nPileUpBx0 = 0;
 
      histosTH1F["nPileUpBx0"]->Fill(nPileUpBx0);
      histosTH2F["nPUBx0vsnVtx"]->Fill(nPileUpBx0,eventData.nVertex_);
      if(selectPileUp&&(eventData.nPileUpBx0_ != nEventsPUBx0)) continue;

      // Single-vertex
      histosTH1F["nVertex"]->Fill(eventData.nVertex_);
      if(doVertexSelection&&(eventData.nVertex_ != 1)) continue;

      // Di-jets
      histosTH1F["leadingJetPt"]->Fill(eventData.leadingJetPt_);
      histosTH1F["secondJetPt"]->Fill(eventData.secondJetPt_);
      if(eventData.leadingJetPt_ < ptMin) continue;
      if(eventData.secondJetPt_ < ptMin) continue;

      histosTH1F["leadingJetEta"]->Fill(eventData.leadingJetEta_);
      histosTH1F["secondJetEta"]->Fill(eventData.secondJetEta_);
      if(fabs(eventData.leadingJetEta_) > etaMax) continue;
      if(fabs(eventData.secondJetEta_) > etaMax) continue; 

      histosTH1F["jetsAveEta"]->Fill(eventData.jetsAveEta_);

      histosTH1F["leadingJetPhi"]->Fill(eventData.leadingJetPhi_);
      histosTH1F["secondJetPhi"]->Fill(eventData.secondJetPhi_);  

      histosTH1F["jetsDeltaEta"]->Fill(eventData.jetsDeltaEta_);
      histosTH1F["jetsDeltaPhi"]->Fill(eventData.jetsDeltaPhi_);
      histosTH1F["jetsDeltaPt"]->Fill(eventData.jetsDeltaPt_);
 
      if(eventData.jetsDeltaEta_ > deltaEtaMax) continue;
      if(eventData.jetsDeltaPhi_ > deltaPhiMax) continue;
      if(eventData.jetsDeltaPt_ > deltaPtMax) continue;

      histosTH1F["massDijets"]->Fill(eventData.massDijets_);
      histosTH1F["massDijetsGen"]->Fill(eventData.massDijetsGen_);
      //histosTH1F["ResMassDijets"]->Fill((eventData.massDijets_ - eventData.massDijetsGen_)/eventData.massDijets_);
      histosTH1F["ResMassDijets"]->Fill(eventData.massDijets_ - eventData.massDijetsGen_); 

      // Rjj
      double RjjFromJets = eventData.RjjFromJets_;
      double RjjFromPFCands = eventData.RjjFromPFCands_;
      double RjjGen = eventData.RjjGen_;

      // B-tag
      double bDiscJet1 = eventData.leadingJetBDiscriminator_;
      double bDiscJet2 = eventData.secondJetBDiscriminator_;
      histosTH1F["leadingJetBDiscriminator"]->Fill(bDiscJet1);
      histosTH1F["secondJetBDiscriminator"]->Fill(bDiscJet2);

      histosTH2F["RjjFromJetsVsBDicriminator"]->Fill(RjjFromJets,bDiscJet1);
      histosTH2F["RjjFromPFCandsVsBDicriminator"]->Fill(RjjFromPFCands,bDiscJet1);

      bool singleBTag = ((bDiscJet1 > bDiscMinValue)||(bDiscJet2 > bDiscMinValue));
      bool doubleBTag = ((bDiscJet1 > bDiscMinValue)&&(bDiscJet2 > bDiscMinValue));

      if(doBTag){
         if(!singleOrDoubleBTag && !singleBTag) continue; 
         if(singleOrDoubleBTag && !doubleBTag) continue;
      }

      histosTH1F["MxFromJets"]->Fill(eventData.MxFromJets_);
      histosTH1F["MxFromPFCands"]->Fill(eventData.MxFromPFCands_);
      histosTH1F["MxGen"]->Fill(eventData.MxGen_);
      histosTH1F["ResMxFromJets"]->Fill(eventData.MxFromJets_ - eventData.MxGen_);
      histosTH1F["ResMxFromPFCands"]->Fill(eventData.MxFromPFCands_ - eventData.MxGen_);

      histosTH1F["RjjFromJets"]->Fill(RjjFromJets);
      histosTH1F["RjjFromPFCands"]->Fill(RjjFromPFCands);
      histosTH1F["RjjGen"]->Fill(RjjGen);  
      histosTH1F["ResRjjFromJets"]->Fill(RjjFromJets - RjjGen);
      histosTH1F["ResRjjFromPFCands"]->Fill(RjjFromPFCands - RjjGen); 

      histosTH1F["xiTowerPlus"]->Fill(eventData.xiTowerPlus_);
      histosTH1F["xiTowerMinus"]->Fill(eventData.xiTowerMinus_);
      histosTH1F["xiPlusFromJets"]->Fill(eventData.xiPlusFromJets_);
      histosTH1F["xiMinusFromJets"]->Fill(eventData.xiMinusFromJets_);
      histosTH1F["xiPlusFromPFCands"]->Fill(eventData.xiPlusFromPFCands_);
      histosTH1F["xiMinusFromPFCands"]->Fill(eventData.xiMinusFromPFCands_);
      histosTH1F["xiGenPlus"]->Fill(eventData.xiGenPlus_);
      histosTH1F["xiGenMinus"]->Fill(eventData.xiGenMinus_);
 
      /*histosTH1F["ResXiTowerPlus"]->Fill((eventData.xiTowerPlus_ - eventData.xiGenPlus_)/eventData.xiGenPlus_);
      histosTH1F["ResXiTowerMinus"]->Fill((eventData.xiTowerMinus_ - eventData.xiGenMinus_)/eventData.xiGenMinus_);
      histosTH1F["ResXiPlusFromJets"]->Fill((eventData.xiPlusFromJets_ - eventData.xiGenPlus_)/eventData.xiGenPlus_);
      histosTH1F["ResXiMinusFromJets"]->Fill((eventData.xiMinusFromJets_ - eventData.xiGenMinus_)/eventData.xiGenMinus_);
      histosTH1F["ResXiPlusFromPFCands"]->Fill((eventData.xiPlusFromPFCands_ - eventData.xiGenPlus_)/eventData.xiGenPlus_);
      histosTH1F["ResXiMinusFromPFCands"]->Fill((eventData.xiMinusFromPFCands_ - eventData.xiGenMinus_)/eventData.xiGenMinus_);*/
      histosTH1F["ResXiTowerPlus"]->Fill(eventData.xiTowerPlus_ - eventData.xiGenPlus_);
      histosTH1F["ResXiTowerMinus"]->Fill(eventData.xiTowerMinus_ - eventData.xiGenMinus_);
      histosTH1F["ResXiPlusFromJets"]->Fill(eventData.xiPlusFromJets_ - eventData.xiGenPlus_);
      histosTH1F["ResXiMinusFromJets"]->Fill(eventData.xiMinusFromJets_ - eventData.xiGenMinus_);
      histosTH1F["ResXiPlusFromPFCands"]->Fill(eventData.xiPlusFromPFCands_ - eventData.xiGenPlus_);
      histosTH1F["ResXiMinusFromPFCands"]->Fill(eventData.xiMinusFromPFCands_ - eventData.xiGenMinus_);

      //histosTH2F["xiPlusVsxiGenPlus"]->Fill();
      //histosTH2F["xiMinusVsxiGenMinus"]->Fill();
      histosTH2F["xiTowerVsxiGenPlus"]->Fill(eventData.xiGenPlus_,eventData.xiTowerPlus_);
      histosTH2F["xiTowerVsxiGenMinus"]->Fill(eventData.xiGenMinus_,eventData.xiTowerMinus_);
      histosTH2F["xiFromJetsVsxiGenPlus"]->Fill(eventData.xiGenPlus_,eventData.xiPlusFromJets_);
      histosTH2F["xiFromJetsVsxiGenMinus"]->Fill(eventData.xiGenMinus_,eventData.xiMinusFromJets_);
      histosTH2F["xiFromPFCandsVsxiGenPlus"]->Fill(eventData.xiGenPlus_,eventData.xiPlusFromPFCands_);
      histosTH2F["xiFromPFCandsVsxiGenMinus"]->Fill(eventData.xiGenMinus_,eventData.xiMinusFromPFCands_);
 
      histosTH1F["missingMassFromXi"]->Fill(eventData.missingMassFromXi_);

      // Exclusivity
      double thirdJetPt = (eventData.thirdJetPt_ > 0.) ? eventData.thirdJetPt_ : 0.;

      int nTracks = eventData.trackMultiplicity_;
      //int nHF_plus = eventData.multiplicityHFPlus_;
      //int nHF_minus = eventData.multiplicityHFMinus_;
      double nHF_plus = useHFTowerWeighted ? eventData.sumWeightsHFPlus_ : eventData.multiplicityHFPlus_;
      double nHF_minus = useHFTowerWeighted ? eventData.sumWeightsHFMinus_ : eventData.multiplicityHFMinus_;
 
      double sumE_plus = useHFTowerWeighted ? eventData.sumEnergyWeightedHFPlus_ : eventData.sumEnergyHFPlus_;
      double sumE_minus = useHFTowerWeighted ? eventData.sumEnergyWeightedHFMinus_ : eventData.sumEnergyHFMinus_;

      histosTH1F["thirdJetPt"]->Fill(thirdJetPt);
      if(eventData.thirdJetPt_ > 0.) histosTH1F["thirdJetEta"]->Fill(eventData.thirdJetEta_);
      histosTH2F["RjjFromJetsVsThirdJetPt"]->Fill(RjjFromJets,thirdJetPt);
      histosTH2F["RjjFromPFCandsVsThirdJetPt"]->Fill(RjjFromPFCands,thirdJetPt);

      histosTH1F["trackMultiplicity"]->Fill(nTracks);
      histosTH1F["multiplicityHFPlus"]->Fill(nHF_plus);
      histosTH1F["multiplicityHFMinus"]->Fill(nHF_minus);
      histosTH1F["sumEnergyHFPlus"]->Fill(sumE_plus);
      histosTH1F["sumEnergyHFMinus"]->Fill(sumE_minus); 

      //histos["iEtaVsHFCountPlus"]  
      //histos["iEtaVsHFCountMinus"]
      for(int iring = 0; iring < 13; ++iring){
         int multiplicityHFPlus = eventData.multiplicityHFPlusVsiEta_[iring];
         int multiplicityHFMinus = eventData.multiplicityHFMinusVsiEta_[iring];   
         histosTH2F["HFRingCountPlus"]->Fill(iring + 1, multiplicityHFPlus);
         histosTH2F["HFRingCountMinus"]->Fill(iring + 1, multiplicityHFMinus);
         histosTH2F["iEtaVsHFCountPlus"]->Fill(29 + iring, multiplicityHFPlus);
         histosTH2F["iEtaVsHFCountMinus"]->Fill(29 + iring, multiplicityHFMinus);
      }

      // Selection
      if(doThirdJetSelection&&(thirdJetPt > thirdJetPtMax)) continue;
      if(doTrackSelection&&(nTracks > nTracksMax)) continue;
      if(doHFMultiplicitySelection&&(nHF_plus > nHFPlusMax)) continue;
      if(doHFMultiplicitySelection&&(nHF_minus > nHFMinusMax)) continue; 

      histosTH1F["xiGenPlusAfterSel"]->Fill(eventData.xiGenPlus_);
      histosTH1F["xiGenMinusAfterSel"]->Fill(eventData.xiGenMinus_);
      histosTH1F["xiPlusFromJetsAfterSel"]->Fill(eventData.xiPlusFromJets_);
      histosTH1F["xiMinusFromJetsAfterSel"]->Fill(eventData.xiMinusFromJets_);
      histosTH1F["xiPlusFromPFCandsAfterSel"]->Fill(eventData.xiPlusFromPFCands_);
      histosTH1F["xiMinusFromPFCandsAfterSel"]->Fill(eventData.xiMinusFromPFCands_);
      histosTH1F["RjjFromJetsAfterSel"]->Fill(RjjFromJets);
      histosTH1F["RjjFromPFCandsAfterSel"]->Fill(RjjFromPFCands);

      histosTH1F["RjjFromJetsAfterSelCustomBin"]->Fill(RjjFromJets);
      histosTH1F["RjjFromPFCandsAfterSelCustomBin"]->Fill(RjjFromPFCands);
      histosTH1F["LogRjjFromJetsAfterSel"]->Fill(log10(1. - RjjFromJets));
      histosTH1F["LogRjjFromPFCandsAfterSel"]->Fill(log10(1. - RjjFromPFCands));
   }  // End loop over events
  
   hfile->Write();
   hfile->Close();
}
