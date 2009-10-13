#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TTree.h"

#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/EventData.h"
#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/Histos.h" 

#include <iostream>
#include <vector>
#include <string>

using namespace exclusiveDijetsAnalysis;

class CreateHisto{
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
};

void exclusiveDijetsTTreeAnalysis(std::string const& fileName, std::string const& treeName, int maxEvents = -1, bool verbose = false){
   if(verbose) std::cout << ">>> Reading file: " << fileName << std::endl;

   TFile* file = TFile::Open(fileName.c_str(),"read");

   // Get TTree
   TTree* data = dynamic_cast<TTree*>(file->Get(treeName.c_str()));
   if(!data){
      std::cout << "ERROR: Could not find " << treeName << std::endl;
      file->Close();
      return;
   }
   
   EventData eventData;
   setTTreeBranches(*data,eventData);

   // Create output file
   TFile* hfile = new TFile("analysisDijetsTTree_histos.root","recreate","data histograms");

   // Book Histograms
   TH1::SetDefaultSumw2(true);

   HistoMapTH1F histosTH1F;
   HistoMapTH2F histosTH2F; 
   bookHistos(histosTH1F,CreateHisto());
   bookHistos(histosTH2F,CreateHisto());
   histosTH2F["nPUBx0vsnVtx"] = new TH2F("nPUBx0vsnVtx","nPUBx0vsnVtx",10,0,10,10,0,10);   
   histosTH2F["RjjFromJetsVsThirdJetPt"] = new TH2F("RjjFromJetsVsThirdJetPt","RjjFromJetsVsThirdJetPt",200,-0.1,1.5,200,0.,80.);
   histosTH2F["RjjFromPFCandsVsThirdJetPt"] = new TH2F("RjjFromPFCandsVsThirdJetPt","RjjFromPFCandsVsThirdJetPt",200,-0.1,1.5,200,0.,80.);
   histosTH2F["xiTowerVsxiGenPlus"] = new TH2F("xiTowerVsxiGenPlus","xiTowerVsxiGenPlus",100,0.,1.,100,0.,1.);
   histosTH2F["xiTowerVsxiGenMinus"] = new TH2F("xiTowerVsxiGenMinus","",100,0.,1.,100,0.,1.);
   histosTH2F["xiFromJetsVsxiGenPlus"] = new TH2F("xiFromJetsVsxiGenPlus","xiFromJetsVsxiGenPlus",100,0.,1.,100,0.,1.);
   histosTH2F["xiFromJetsVsxiGenMinus"] = new TH2F("xiFromJetsVsxiGenMinus","xiFromJetsVsxiGenMinus",100,0.,1.,100,0.,1.);
   histosTH2F["xiFromPFCandsVsxiGenPlus"] = new TH2F("xiFromPFCandsVsxiGenPlus","xiFromPFCandsVsxiGenPlus",100,0.,1.,100,0.,1.);
   histosTH2F["xiFromPFCandsVsxiGenMinus"] = new TH2F("xiFromPFCandsVsxiGenMinus","xiFromPFCandsVsxiGenMinus",100,0.,1.,100,0.,1.);
   histosTH1F["xiPlusFromJetsAfterSel"] = new TH1F("xiPlusFromJetsAfterSel","xiPlusFromJetsAfterSel",200,0.,1.);
   histosTH1F["xiMinusFromJetsAfterSel"] = new TH1F("xiMinusFromJetsAfterSel","xiMinusFromJetsAfterSel",200,0.,1.);
   histosTH1F["xiPlusFromPFCandsAfterSel"] = new TH1F("xiPlusFromPFCandsAfterSel","xiPlusFromPFCandsAfterSel",200,0.,1.);
   histosTH1F["xiMinusFromPFCandsAfterSel"] = new TH1F("xiMinusFromPFCandsAfterSel","xiMinusFromPFCandsAfterSel",200,0.,1.);
   histosTH1F["RjjFromJetsAfterSel"] = new TH1F("RjjFromJetsAfterSel","RjjFromJetsAfterSel",200,-0.1,1.5);
   histosTH1F["RjjFromPFCandsAfterSel"] = new TH1F("RjjFromPFCandsAfterSel","RjjFromPFCandsAfterSel",200,-0.1,1.5);

   bool selectPileUp = false;
   int nEventsPUBx0 = 0;

   // Event selection
   // Prim. vertices
   bool doVertexSelection = true;
   // Di-jet
   double ptmin = 50.;
   double etamax = 2.5;
   // B-tag
   bool doBTag = false;
   bool singleOrDoubleBTag = true; // false --> single, true --> double
   double bDiscMinValue = 3.0;
   // Third jet
   bool doThirdJetSelection = true;
   double thirdJetPtMax = 15.; 
   // Track multiplicity
   bool doTrackSelection = true; 
   int nTracksMax = 5;
   // HF-multiplicity
   bool doHFMultiplicitySelection = true; 
   int nHFPlusMax = 0;
   int nHFMinusMax = 0;

   // Loop over TTree
   int nEntries = data->GetEntries();
   for(int ientry = 0; ientry < nEntries; ++ientry){
      if((maxEvents > 0)&&(ientry == maxEvents)) break;
      if(verbose && ientry%2000 == 0) std::cout << ">>> Analysing " << ientry << "th entry" << std::endl;

      data->GetEntry(ientry);

      // Pile-up
      int nPileUpBx0 = eventData.nPileUpBx0_; 
      histosTH1F["nPileUpBx0"]->Fill(nPileUpBx0);
      histosTH2F["nPUBx0vsnVtx"]->Fill(nPileUpBx0,eventData.nVertex_);
      if(selectPileUp&&(eventData.nPileUpBx0_ != nEventsPUBx0)) continue;

      // Single-vertex
      histosTH1F["nVertex"]->Fill(eventData.nVertex_);
      if(doVertexSelection&&(eventData.nVertex_ != 1)) continue;

      // Di-jets
      histosTH1F["leadingJetPt"]->Fill(eventData.leadingJetPt_);
      histosTH1F["secondJetPt"]->Fill(eventData.secondJetPt_);
      if(eventData.leadingJetPt_ < ptmin) continue;
      if(eventData.secondJetPt_ < ptmin) continue;

      histosTH1F["leadingJetEta"]->Fill(eventData.leadingJetEta_);
      histosTH1F["secondJetEta"]->Fill(eventData.secondJetEta_);
      if(fabs(eventData.leadingJetEta_) > etamax) continue;
      if(fabs(eventData.secondJetEta_) > etamax) continue; 

      histosTH1F["jetsAveEta"]->Fill(eventData.jetsAveEta_);

      histosTH1F["leadingJetPhi"]->Fill(eventData.leadingJetPhi_);
      histosTH1F["secondJetPhi"]->Fill(eventData.secondJetPhi_);  

      histosTH1F["thirdJetPt"]->Fill(eventData.thirdJetPt_);
      histosTH1F["thirdJetEta"]->Fill(eventData.thirdJetEta_);

      histosTH1F["jetsDeltaEta"]->Fill(eventData.jetsDeltaEta_);
      histosTH1F["jetsDeltaPhi"]->Fill(eventData.jetsDeltaPhi_);
 
      histosTH1F["massDijets"]->Fill(eventData.massDijets_);
      histosTH1F["massDijetsGen"]->Fill(eventData.massDijetsGen_);
      histosTH1F["ResMassDijets"]->Fill((eventData.massDijets_ - eventData.massDijetsGen_)/eventData.massDijets_);

      histosTH1F["MxFromJets"]->Fill(eventData.MxFromJets_); 

      // Rjj
      double RjjFromJets = eventData.RjjFromJets_;
      double RjjFromPFCands = eventData.RjjFromPFCands_;

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

      histosTH1F["RjjFromJets"]->Fill(RjjFromJets);
      histosTH1F["RjjFromPFCands"]->Fill(RjjFromPFCands);

      histosTH2F["RjjFromJetsVsThirdJetPt"]->Fill(RjjFromJets,eventData.thirdJetPt_);
      histosTH2F["RjjFromPFCandsVsThirdJetPt"]->Fill(RjjFromPFCands,eventData.thirdJetPt_);

      histosTH1F["xiTowerPlus"]->Fill(eventData.xiTowerPlus_);
      histosTH1F["xiTowerMinus"]->Fill(eventData.xiTowerMinus_);
      histosTH1F["xiPlusFromJets"]->Fill(eventData.xiPlusFromJets_);
      histosTH1F["xiMinusFromJets"]->Fill(eventData.xiMinusFromJets_);
      histosTH1F["xiPlusFromPFCands"]->Fill(eventData.xiPlusFromPFCands_);
      histosTH1F["xiMinusFromPFCands"]->Fill(eventData.xiMinusFromPFCands_);
      histosTH1F["xiGenPlus"]->Fill(eventData.xiGenPlus_);
      histosTH1F["xiGenMinus"]->Fill(eventData.xiGenMinus_);
 
      histosTH1F["ResXiTowerPlus"]->Fill((eventData.xiTowerPlus_ - eventData.xiGenPlus_)/eventData.xiGenPlus_);
      histosTH1F["ResXiTowerMinus"]->Fill((eventData.xiTowerMinus_ - eventData.xiGenPlus_)/eventData.xiGenPlus_);
      histosTH1F["ResXiPlusFromJets"]->Fill((eventData.xiPlusFromJets_ - eventData.xiGenPlus_)/eventData.xiGenPlus_);
      histosTH1F["ResXiMinusFromJets"]->Fill((eventData.xiMinusFromJets_ - eventData.xiGenMinus_)/eventData.xiGenMinus_);
      histosTH1F["ResXiPlusFromPFCands"]->Fill((eventData.xiPlusFromPFCands_ - eventData.xiGenPlus_)/eventData.xiGenPlus_);
      histosTH1F["ResXiMinusFromPFCands"]->Fill((eventData.xiMinusFromPFCands_ - eventData.xiGenMinus_)/eventData.xiGenMinus_);

      //histosTH2F["xiPlusVsxiGenPlus"]->Fill();
      //histosTH2F["xiMinusVsxiGenMinus"]->Fill();
      histosTH2F["xiTowerVsxiGenPlus"]->Fill(eventData.xiGenPlus_,eventData.xiTowerPlus_);
      histosTH2F["xiTowerVsxiGenMinus"]->Fill(eventData.xiGenMinus_,eventData.xiTowerMinus_);
      histosTH2F["xiFromJetsVsxiGenPlus"]->Fill(eventData.xiGenPlus_,eventData.xiPlusFromJets_);
      histosTH2F["xiFromJetsVsxiGenMinus"]->Fill(eventData.xiGenMinus_,eventData.xiMinusFromJets_);
      histosTH2F["xiFromPFCandsVsxiGenPlus"]->Fill(eventData.xiGenPlus_,eventData.xiPlusFromPFCands_);
      histosTH2F["xiFromPFCandsVsxiGenMinus"]->Fill(eventData.xiGenMinus_,eventData.xiMinusFromPFCands_);
 
      histosTH1F["missingMassFromXi"]->Fill(eventData.missingMassFromXi_);

      // Access multiplicities
      int nTracks = eventData.trackMultiplicity_;
      int nHF_plus = eventData.multiplicityHFPlus_;
      int nHF_minus = eventData.multiplicityHFMinus_;
 
      histosTH1F["trackMultiplicity"]->Fill(nTracks);
      histosTH1F["multiplicityHFPlus"]->Fill(nHF_plus);
      histosTH1F["multiplicityHFMinus"]->Fill(nHF_minus);

      //histos["iEtaVsHFCountPlus"]  
      //histos["iEtaVsHFCountPlus"]
      for(int iring = 0; iring < 13; ++iring){
         histosTH2F["HFRingCountPlus"]->Fill(iring + 1, eventData.multiplicityHFPlusVsiEta_[iring]);
         histosTH2F["HFRingCountMinus"]->Fill(iring + 1, eventData.multiplicityHFMinusVsiEta_[iring]);
      }

      // Selection
      if(doThirdJetSelection&&(eventData.thirdJetPt_ > thirdJetPtMax)) continue;
      if(doTrackSelection&&(nTracks > nTracksMax)) continue;
      if(doHFMultiplicitySelection&&(nHF_plus > nHFPlusMax)) continue;
      if(doHFMultiplicitySelection&&(nHF_minus > nHFMinusMax)) continue; 

      histosTH1F["xiPlusFromJetsAfterSel"]->Fill(eventData.xiPlusFromJets_);
      histosTH1F["xiMinusFromJetsAfterSel"]->Fill(eventData.xiMinusFromJets_);
      histosTH1F["xiPlusFromPFCandsAfterSel"]->Fill(eventData.xiPlusFromPFCands_);
      histosTH1F["xiMinusFromPFCandsAfterSel"]->Fill(eventData.xiMinusFromPFCands_);
      histosTH1F["RjjFromJetsAfterSel"]->Fill(RjjFromJets);
      histosTH1F["RjjFromPFCandsAfterSel"]->Fill(RjjFromPFCands);
   }  // End loop over events
  
   hfile->Write();
   hfile->Close();
}
