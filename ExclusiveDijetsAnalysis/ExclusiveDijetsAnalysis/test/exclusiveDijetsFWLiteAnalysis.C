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

#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/Histos.h"
#include "StdAllocatorAdaptor.h"

// Trick to make CINT happy
//#include "DataFormats/FWLite/interface/Handle.h"
//#include "DataFormats/FWLite/interface/Event.h"
#if !defined(__CINT__) && !defined(__MAKECINT__)
//Headers for the data items
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/VertexReco/interface/Vertex.h" 
#endif

using namespace exclusiveDijetsAnalysis;

template <class PartColl>
double MassColl(PartColl&);

template <class Coll>
std::pair<double,double> xi(Coll&,double);

template <class JetColl,class PartColl>
double Rjj(JetColl&,PartColl&);

unsigned int nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapTreshToiEta, unsigned int thresholdHF, unsigned int ieta);

void exclusiveDijetsFWLiteAnalysis(std::vector<std::string>& fileNames,
                                   std::string const& outFileName = "analysisDijetsFWLite_histos.root",
                                   int maxEvents = -1, bool verbose = false) {
   // Create a vector of input files
   //vector<string> fileNames = func();
   if(verbose){
     std::cout << ">>> Reading files: " << std::endl;
     for(std::vector<std::string>::const_iterator it = fileNames.begin(); it != fileNames.end(); ++it)
               std::cout << "  " << *it << std::endl; 
   } 

   // Chain the input files
   fwlite::ChainEvent ev(fileNames);
   
   // Create output file
   TFile* hfile = new TFile(outFileName.c_str(),"recreate","data histograms");

   std::vector<std::string> jetColls;
   jetColls.push_back("selectedLayer1JetsSC7PF");
   //jetColls.push_back("selectedLayer1JetsSC7Calo");
   jetColls.push_back("selectedLayer1JetsSC5PF");
   //jetColls.push_back("selectedLayer1JetsSC5Calo");
   //jetColls.push_back("selectedLayer1JetsKT6PF");
   //jetColls.push_back("selectedLayer1JetsKT6Calo");

   std::vector<std::string> jetCollsNonCorr;
   jetCollsNonCorr.push_back("sisCone7PFJets");
   jetCollsNonCorr.push_back("sisCone5PFJets");

   std::vector<std::string> genJetColls;
   genJetColls.push_back("sisCone7GenJets");
   genJetColls.push_back("sisCone5GenJets");
 
   // Book Histograms
   //TH1::SetDefaultSumw2(true);

   HistoMapTH1F histosTH1F;
   HistoMapTH2F histosTH2F; 
   bookHistos(histosTH1F,StdAllocatorAdaptor());
   bookHistos(histosTH2F,StdAllocatorAdaptor());
   histosTH1F["trackMultiplicityAssociatedToPV"] = new TH1F("trackMultiplicityAssociatedToPV","trackMultiplicityAssociatedToPV",20,0,20);
   histosTH1F["trackMultiplicityOutsideJets"] = new TH1F("trackMultiplicityOutsideJets","trackMultiplicityOutsideJets",20,0,20);
   histosTH1F["trackMultiplicityTransverseRegion"] = new TH1F("trackMultiplicityTransverseRegion","trackMultiplicityTransverseRegion",20,0,20);
   histosTH1F["forwardBackwardAsymmetryHFEnergy"] = new TH1F("forwardBackwardAsymmetryHFEnergy","forwardBackwardAsymmetryHFEnergy",200,-1.1,1.1);
   //double xbins[8] = {0.,0.4,0.7,0.8,0.85,0.9,0.95,1.0};
   /*double xbins[10] = {-0.1,0.40,0.60,0.80,0.84,0.88,0.92,0.96,1.0,1.5};
   histosTH1F["RjjFromJetsAfterSelCustomBin"] = new TH1F("RjjFromJetsAfterSelCustomBin","RjjFromJetsAfterSelCustomBin",9,xbins);
   histosTH1F["RjjFromPFCandsAfterSelCustomBin"] = new TH1F("RjjFromPFCandsAfterSelCustomBin","RjjFromPFCandsAfterSelCustomBin",9,xbins);*/
   histosTH1F["RjjGenMatchFromRec"] = static_cast<TH1F*>(histosTH1F["RjjGen"]->Clone("RjjGenMatchFromRec"));

   std::vector<TH1F*> histos_ResMassDijets;
   std::vector<TH1F*> histos_ResXiPlusFromJets;
   std::vector<TH1F*> histos_ResXiMinusFromJets;
   std::vector<TH1F*> histos_RjjFromJets;
   std::vector<TH1F*> histos_RjjGenMatchFromRec;
   std::vector<TH1F*> histos_ResRjjFromJets;
   for(std::vector<std::string>::const_iterator it = jetColls.begin(); it != jetColls.end(); ++it){
      std::string hname = "ResMassDijets_" + *it;
      histos_ResMassDijets.push_back(static_cast<TH1F*>(histosTH1F["ResMassDijets"]->Clone(hname.c_str())));
      hname = "ResXiPlusFromJets_" + *it;
      histos_ResXiPlusFromJets.push_back(static_cast<TH1F*>(histosTH1F["ResXiPlusFromJets"]->Clone(hname.c_str())));
      hname = "ResXiMinusFromJets_" + *it;
      histos_ResXiMinusFromJets.push_back(static_cast<TH1F*>(histosTH1F["ResXiMinusFromJets"]->Clone(hname.c_str())));
      hname = "RjjFromJets_" + *it;
      histos_RjjFromJets.push_back(static_cast<TH1F*>(histosTH1F["RjjFromJets"]->Clone(hname.c_str())));
      hname = "RjjGenMatchFromRec_" + *it;
      histos_RjjGenMatchFromRec.push_back(static_cast<TH1F*>(histosTH1F["RjjGen"]->Clone(hname.c_str())));
      hname = "ResRjjFromJets_" + *it;
      histos_ResRjjFromJets.push_back(static_cast<TH1F*>(histosTH1F["ResRjjFromJets"]->Clone(hname.c_str())));
   }

   std::vector<TH1F*> histos_RjjFromPFCands;
   for(std::vector<std::string>::const_iterator it = jetCollsNonCorr.begin(); it != jetCollsNonCorr.end(); ++it){
      std::string hname = "RjjFromPFCands_" + *it;
      histos_RjjFromPFCands.push_back(static_cast<TH1F*>(histosTH1F["RjjFromPFCands"]->Clone(hname.c_str())));
   }

   double Ebeam = 5000.;
   int thresholdHF = 15;// 0.2 GeV/bin

   bool accessPileUp = false;
   bool selectPileUp = false;
   int nEventsPUBx0 = 0;

   bool useHFTowerWeighted = false;

   // Event selection
   // Prim. vertices
   bool doVertexSelection = true;
   // Di-jet
   double ptMin = 50.;
   double etaMax = 2.5;

   double deltaEtaMax = 2.0;
   double deltaPhiMax = 0.4;
   double deltaPtMax = 999.;
   // B-tag
   bool doBTag = false;
   std::string bDiscriminatorName = "jetBProbabilityBJetTags";
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
   int nHFPlusMax = 0;
   int nHFMinusMax = 0;

   // Loop over the events
   int nEvts = 0;
   for( ev.toBegin(); ! ev.atEnd(); ++ev) {

     if((maxEvents > 0)&&(nEvts == maxEvents)) break;
	
     ++nEvts;
     if(verbose) std::cout << ">>> Event number: " << nEvts << endl;

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

     // Access pile-up info 
     if(accessPileUp) {
       fwlite::Handle<std::map<int,int> > pileUpMap;
       pileUpMap.getByLabel(ev,"pileUpInfo");

       std::map<int,int>::const_iterator bx0Iter = pileUpMap->find(0);
       int nPileUpBx0 = bx0Iter->second;

       histosTH1F["nPileUpBx0"]->Fill(nPileUpBx0);
       histosTH2F["nPUBx0vsnVtx"]->Fill(nPileUpBx0,nGoodVertices);
       if(selectPileUp&&(nPileUpBx0 != nEventsPUBx0)) continue;
     }

     // Single-vertex
     histosTH1F["nVertex"]->Fill(nGoodVertices);
     if(doVertexSelection&&(nGoodVertices != 1)) continue;

     // Di-jets
     std::vector<fwlite::Handle<std::vector<pat::Jet> > > jetCollections(jetColls.size());
     for(size_t k = 0; k < jetColls.size(); ++k){
       jetCollections[k].getByLabel(ev,jetColls[k].c_str());
     }

     std::vector<fwlite::Handle<std::vector<reco::PFJet> > > jetCollectionsNonCorr(jetCollsNonCorr.size());
     for(size_t k = 0; k < jetCollsNonCorr.size(); ++k){
       jetCollectionsNonCorr[k].getByLabel(ev,jetCollsNonCorr[k].c_str());
     }
     
     /*std::vector<fwlite::Handle<std::vector<reco::GenJet> > > genJetCollections(genJetColls.size());
     for(size_t k = 0; k < genJetColls.size(); ++k){
       genJetCollections[k].getByLabel(ev,genJetColls[k].c_str());
     }*/

     const fwlite::Handle<std::vector<pat::Jet> >& jetCollection = jetCollections[0];
     const fwlite::Handle<std::vector<reco::PFJet> >& jetCollectionNonCorr = jetCollectionsNonCorr[0];  
     //const fwlite::Handle<std::vector<reco::GenJet> >& genJetCollection = genJetCollections[0];
     // PF candidates
     fwlite::Handle<std::vector<reco::PFCandidate> > pfCandCollection;
     pfCandCollection.getByLabel(ev,"particleFlow");

     const pat::Jet& jet1 = (*jetCollection)[0];// they come out ordered right?
     const pat::Jet& jet2 = (*jetCollection)[1];

     /*if(verbose){
       std::cout << " JEC set: " << jet1.corrFactorSetLabel() << std::endl;
       std::cout << " JEC level: " << jet1.corrStep() << std::endl;
     }*/

     histosTH1F["leadingJetPt"]->Fill(jet1.pt());
     histosTH1F["secondJetPt"]->Fill(jet2.pt());
     if(jet1.pt() < ptMin) continue;
     if(jet2.pt() < ptMin) continue;

     histosTH1F["leadingJetEta"]->Fill(jet1.eta());
     histosTH1F["secondJetEta"]->Fill(jet2.eta());
     if(fabs(jet1.eta()) > etaMax) continue;
     if(fabs(jet2.eta()) > etaMax) continue; 

     histosTH1F["jetsAveEta"]->Fill((jet1.eta() + jet2.eta())/2);
     
     histosTH1F["leadingJetPhi"]->Fill(jet1.phi());
     histosTH1F["secondJetPhi"]->Fill(jet2.phi());

     histosTH1F["jetsDeltaEta"]->Fill(jet1.eta() - jet2.eta());
     histosTH1F["jetsDeltaPhi"]->Fill(M_PI - fabs(jet1.phi() - jet2.phi()));
     histosTH1F["jetsDeltaPt"]->Fill(fabs(jet1.pt() - jet2.pt()));

     if(fabs(jet1.eta() - jet2.eta()) > deltaEtaMax) continue;
     if((M_PI - fabs(jet1.phi() - jet2.phi())) > deltaPhiMax) continue;
     if(fabs(jet1.pt() - jet2.pt()) > deltaPtMax) continue;

     math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
     dijetSystem += jet1.p4();
     dijetSystem += jet2.p4();
     double massDijets = dijetSystem.M();
     histosTH1F["massDijets"]->Fill(massDijets);

     // Compute Rjj
     double RjjFromJets = Rjj(*jetCollection,*jetCollection);
     double RjjFromPFCands = Rjj(*jetCollectionNonCorr,*pfCandCollection);

     // B-tag
     double bDiscJet1 = jet1.bDiscriminator(bDiscriminatorName);
     double bDiscJet2 = jet2.bDiscriminator(bDiscriminatorName);

     histosTH1F["leadingJetBDiscriminator"]->Fill(bDiscJet1);
     histosTH1F["secondJetBDiscriminator"]->Fill(bDiscJet2);

     histosTH2F["RjjFromJetsVsBDicriminator"]->Fill(RjjFromJets,bDiscJet1);
     histosTH2F["RjjFromPFCandsVsBDicriminator"]->Fill(RjjFromPFCands,bDiscJet1);

     bool singleBTag = ((bDiscJet1 > bDiscMinValue)||(bDiscJet2 > bDiscMinValue));
     bool doubleBTag = ((bDiscJet1 > bDiscMinValue)&&(bDiscJet2 > bDiscMinValue));

     //if(doBTag && !doubleBTag) continue;
     if(doBTag){
        if(!singleOrDoubleBTag && !singleBTag) continue; 
        if(singleOrDoubleBTag && !doubleBTag) continue;
     }

     // Compute Mx
     double MxFromJets = MassColl(*jetCollection);
     double MxFromPFCands = MassColl(*pfCandCollection);
     
     histosTH1F["MxFromJets"]->Fill(MxFromJets);
     histosTH1F["MxFromPFCands"] ->Fill(MxFromPFCands);
     histosTH1F["RjjFromJets"]->Fill(RjjFromJets);
     histosTH1F["RjjFromPFCands"]->Fill(RjjFromPFCands);

     // Gen particles
     fwlite::Handle<std::vector<reco::GenParticle> > genParticlesCollection;
     genParticlesCollection.getByLabel(ev,"genParticles");
     const reco::GenParticleCollection& genParticles = *genParticlesCollection;

     math::XYZTLorentzVector allGenParticles(0.,0.,0.,0.);

     reco::GenParticleCollection::const_iterator proton1 = genParticles.end();
     reco::GenParticleCollection::const_iterator proton2 = genParticles.end();
     for(reco::GenParticleCollection::const_iterator genpart = genParticles.begin(); genpart != genParticles.end(); ++genpart){
       if(genpart->status() != 1) continue;
       histosTH1F["EnergyVsEta"]->Fill(genpart->eta(),genpart->energy());	
       allGenParticles += genpart->p4();
		
       double pz = genpart->pz();
       if((proton1 == genParticles.end())&&(genpart->pdgId() == 2212)&&(pz > 0.75*Ebeam)) proton1 = genpart;
       else if((proton2 == genParticles.end())&&(genpart->pdgId() == 2212)&&(pz < -0.75*Ebeam)) proton2 = genpart;
     }
     if(verbose){
       if(proton1 != genParticles.end()) std::cout << "Proton (z-plus): " << proton1->pt() << "  "
                                                   << proton1->eta() << "  " << proton1->phi() << std::endl;
       if(proton2 != genParticles.end()) std::cout << "Proton (z-minus): " << proton2->pt() << "  "
                                                   << proton2->eta() << "  " << proton2->phi() << std::endl;
     }
     // Subtract the two protons' momenta
     if(proton1 != genParticles.end()) allGenParticles -= proton1->p4();
     if(proton2 != genParticles.end()) allGenParticles -= proton2->p4();
     histosTH1F["MxGen"]->Fill(allGenParticles.M());
     histosTH1F["ResMxFromJets"]->Fill(MxFromJets - allGenParticles.M());
     histosTH1F["ResMxFromPFCands"]->Fill(MxFromPFCands - allGenParticles.M());

     /*math::XYZTLorentzVector genDijetSystem(0.,0.,0.,0.);
     genDijetSystem += ((*genJetCollection)[0]).p4();
     genDijetSystem += ((*genJetCollection)[1]).p4();
     // Rjj gen
     double RjjGen = genDijetSystem.M()/allGenParticles.M();
     histosTH1F["RjjGen"]->Fill(RjjGen);*/
     
     // Xi info
     std::pair<double,double> xiFromJets = xi(*jetCollection,Ebeam);
     histosTH1F["xiPlusFromJets"]->Fill(xiFromJets.first);
     histosTH1F["xiMinusFromJets"]->Fill(xiFromJets.second);

     std::pair<double,double> xiFromPFCands = xi(*pfCandCollection,Ebeam);
     histosTH1F["xiPlusFromPFCands"]->Fill(xiFromPFCands.first);
     histosTH1F["xiMinusFromPFCands"]->Fill(xiFromPFCands.second);

     // Xi gen
     double xigen_plus = (proton1 != genParticles.end()) ? (1 - proton1->pz()/Ebeam) : 1.;
     double xigen_minus = (proton2 != genParticles.end()) ? (1 + proton2->pz()/Ebeam) : 1.;

     histosTH1F["xiGenPlus"]->Fill(xigen_plus);
     histosTH1F["xiGenMinus"]->Fill(xigen_minus); 

     histosTH2F["xiFromJetsVsxiGenPlus"]->Fill(xigen_plus,xiFromJets.first);
     histosTH2F["xiFromJetsVsxiGenMinus"]->Fill(xigen_minus,xiFromJets.second);

     //histosTH1F["ResXiPlusFromJets"]->Fill((xiFromJets.first - xigen_plus)/xigen_plus);
     //histosTH1F["ResXiMinusFromJets"]->Fill((xiFromJets.second - xigen_minus)/xigen_minus);
     histosTH1F["ResXiPlusFromJets"]->Fill(xiFromJets.first - xigen_plus);
     histosTH1F["ResXiMinusFromJets"]->Fill(xiFromJets.second - xigen_minus);
  
     histosTH2F["xiFromPFCandsVsxiGenPlus"]->Fill(xigen_plus,xiFromPFCands.first);
     histosTH2F["xiFromPFCandsVsxiGenMinus"]->Fill(xigen_minus,xiFromPFCands.second);

     //histosTH1F["ResXiPlusFromPFCands"]->Fill((xiFromPFCands.first - xigen_plus)/xigen_plus);
     //histosTH1F["ResXiMinusFromPFCands"]->Fill((xiFromPFCands.second - xigen_minus)/xigen_minus);
     histosTH1F["ResXiPlusFromPFCands"]->Fill(xiFromPFCands.first - xigen_plus);
     histosTH1F["ResXiMinusFromPFCands"]->Fill(xiFromPFCands.second - xigen_minus);

     // Gen jets
     const reco::GenJet* genJet1 = jet1.genJet();
     const reco::GenJet* genJet2 = jet2.genJet();

     if(genJet1 && genJet2){
        math::XYZTLorentzVector dijetGenSystem(0.,0.,0.,0.);
        dijetGenSystem += genJet1->p4();
        dijetGenSystem += genJet2->p4();
        double massGen = dijetGenSystem.M();

        histosTH1F["massDijetsGen"]->Fill(massGen);
        //histosTH1F["ResMassDijets"]->Fill((massDijets - massGen)/massGen);
        histosTH1F["ResMassDijets"]->Fill(massDijets - massGen);

        // Rjj gen
        double RjjGenMatchFromRec = massGen/allGenParticles.M();
        histosTH1F["RjjGenMatchFromRec"]->Fill(RjjGenMatchFromRec);
        //histosTH1F["ResRjjFromJets"]->Fill((RjjFromJets - RjjGen)/RjjGen);
        //histosTH1F["ResRjjFromPFCands"]->Fill((RjjFromPFCands - RjjGen)/RjjGen);
        histosTH1F["ResRjjFromJets"]->Fill(RjjFromJets - RjjGenMatchFromRec);
        histosTH1F["ResRjjFromPFCands"]->Fill(RjjFromPFCands - RjjGenMatchFromRec);
     }

     // Access all jet collections
     for(size_t k = 0; k < jetColls.size(); ++k){
        const pat::Jet& jet1 = (jetCollections[k])->at(0);
        const pat::Jet& jet2 = (jetCollections[k])->at(1);

        math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
        dijetSystem += jet1.p4();
        dijetSystem += jet2.p4();

        double RjjFromJets = Rjj(*(jetCollections[k]),*(jetCollections[k]));
        (histos_RjjFromJets[k])->Fill(RjjFromJets);

        const reco::GenJet* genJet1 = jet1.genJet();
        const reco::GenJet* genJet2 = jet2.genJet();

        if(genJet1 && genJet2){
          math::XYZTLorentzVector dijetGenSystem(0.,0.,0.,0.);
          dijetGenSystem += genJet1->p4();
          dijetGenSystem += genJet2->p4();
          double massGen = dijetGenSystem.M();
          //(histos_ResMassDijets[k])->Fill((dijetSystem.M() - massGen)/massGen);
          (histos_ResMassDijets[k])->Fill(dijetSystem.M() - massGen);

          // Rjj gen
          double RjjGenMatchFromRec = massGen/allGenParticles.M();
          histos_RjjGenMatchFromRec[k]->Fill(RjjGenMatchFromRec);
          histos_ResRjjFromJets[k]->Fill(RjjFromJets - RjjGenMatchFromRec);
        }

        // Xi
        std::pair<double,double> xiFromJets = xi(*(jetCollections[k]),Ebeam);
        //(histos_ResXiPlusFromJets[k])->Fill((xiFromJets.first - xigen_plus)/xigen_plus);
        //(histos_ResXiMinusFromJets[k])->Fill((xiFromJets.second - xigen_minus)/xigen_minus);
        (histos_ResXiPlusFromJets[k])->Fill(xiFromJets.first - xigen_plus);
        (histos_ResXiMinusFromJets[k])->Fill(xiFromJets.second - xigen_minus);
     }

     for(size_t k = 0; k < jetCollsNonCorr.size(); ++k){
        (histos_RjjFromPFCands[k])->Fill(Rjj(*(jetCollectionsNonCorr[k]),*pfCandCollection));
     }
 
     // Access multiplicities
     fwlite::Handle<unsigned int> trackMultiplicityAssociatedToPV; 
     trackMultiplicityAssociatedToPV.getByLabel(ev,"trackMultiplicityAssociatedToPV","trackMultiplicity");

     fwlite::Handle<unsigned int> trackMultiplicityOutsideJets;
     trackMultiplicityOutsideJets.getByLabel(ev,"trackMultiplicityOutsideJets","trackMultiplicity");

     fwlite::Handle<unsigned int> trackMultiplicityTransverseRegion;
     trackMultiplicityTransverseRegion.getByLabel(ev,"trackMultiplicityTransverseRegion","trackMultiplicity");

     fwlite::Handle<std::vector<unsigned int> > nHFPlus;
     nHFPlus.getByLabel(ev,"hfTower","nHFplus");
  
     fwlite::Handle<std::vector<unsigned int> > nHFMinus;
     nHFMinus.getByLabel(ev,"hfTower","nHFminus");

     fwlite::Handle<std::vector<double> > sumWeightsHFplus;
     fwlite::Handle<std::vector<double> > sumWeightsHFminus;
     if(useHFTowerWeighted){
        sumWeightsHFplus.getByLabel(ev,"hfTower","sumWeightsHFplus");
        sumWeightsHFminus.getByLabel(ev,"hfTower","sumWeightsHFminus");
     }

     fwlite::Handle<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaPlus;
     mapThreshToiEtaPlus.getByLabel(ev,"hfTower","mapTreshToiEtaplus");

     fwlite::Handle<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaMinus;
     mapThreshToiEtaMinus.getByLabel(ev,"hfTower","mapTreshToiEtaminus");

     fwlite::Handle<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityPlus;
     iEtaHFMultiplicityPlus.getByLabel(ev,"hfTower","iEtaHFMultiplicityPlus");

     fwlite::Handle<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityMinus;
     iEtaHFMultiplicityMinus.getByLabel(ev,"hfTower","iEtaHFMultiplicityMinus");

     fwlite::Handle<std::vector<double> > sumEHFplus;
     sumEHFplus.getByLabel(ev,"hfTower","sumEHFplus");

     fwlite::Handle<std::vector<double> > sumEHFminus;
     sumEHFminus.getByLabel(ev,"hfTower","sumEHFminus");

     fwlite::Handle<std::vector<double> > sumEWeightedHFplus;
     fwlite::Handle<std::vector<double> > sumEWeightedHFminus; 
     if(useHFTowerWeighted){
        sumEWeightedHFplus.getByLabel(ev,"hfTower","sumEWeightedHFplus");
        sumEWeightedHFminus.getByLabel(ev,"hfTower","sumEWeightedHFminus");
     }

     fwlite::Handle<double> xiTowerPlus;
     xiTowerPlus.getByLabel(ev,"xiTower","xiTowerplus");

     fwlite::Handle<double> xiTowerMinus;
     xiTowerMinus.getByLabel(ev,"xiTower","xiTowerminus");

     double thirdJetPt = 0.;
     if(jetCollection->size() > 2){
        const pat::Jet& jet3 = (*jetCollection)[2];
        thirdJetPt = jet3.pt();
        histosTH1F["thirdJetEta"]->Fill(jet3.eta());
     }
     //double thirdJetPt = (jetCollection->size() > 2)?(*jetCollection)[2].pt():0.;

     unsigned int nTracksAssociatedToPV = *trackMultiplicityAssociatedToPV;
     unsigned int nTracksOutsideJets = *trackMultiplicityOutsideJets;
     unsigned int nTracksTransverseRegion = *trackMultiplicityTransverseRegion; 

     //unsigned int nHF_plus = (*nHFPlus)[thresholdHF];
     //unsigned int nHF_minus = (*nHFMinus)[thresholdHF];
     double nHF_plus = useHFTowerWeighted ? (*sumWeightsHFplus)[thresholdHF] : (*nHFPlus)[thresholdHF];
     double nHF_minus = useHFTowerWeighted ? (*sumWeightsHFminus)[thresholdHF] : (*nHFMinus)[thresholdHF];

     //double sumE_plus = (*sumEHFplus)[thresholdHF];
     //double sumE_minus = (*sumEHFminus)[thresholdHF]; 
     double sumE_plus = useHFTowerWeighted ? (*sumEWeightedHFplus)[thresholdHF] : (*sumEHFplus)[thresholdHF];
     double sumE_minus = useHFTowerWeighted ? (*sumEWeightedHFminus)[thresholdHF] : (*sumEHFminus)[thresholdHF];

     double xiTower_plus = *xiTowerPlus;
     double xiTower_minus = *xiTowerMinus;
 
     histosTH1F["thirdJetPt"]->Fill(thirdJetPt);
     histosTH2F["RjjFromJetsVsThirdJetPt"]->Fill(RjjFromJets,thirdJetPt);
     histosTH2F["RjjFromPFCandsVsThirdJetPt"]->Fill(RjjFromPFCands,thirdJetPt);

     histosTH1F["trackMultiplicityAssociatedToPV"]->Fill(nTracksAssociatedToPV);
     histosTH1F["trackMultiplicityOutsideJets"]->Fill(nTracksOutsideJets);
     histosTH1F["trackMultiplicityTransverseRegion"]->Fill(nTracksTransverseRegion);

     histosTH1F["multiplicityHFPlus"]->Fill(nHF_plus);
     histosTH1F["multiplicityHFMinus"]->Fill(nHF_minus);     

     histosTH1F["sumEnergyHFPlus"]->Fill(sumE_plus);
     histosTH1F["sumEnergyHFMinus"]->Fill(sumE_minus);

     for(unsigned int ieta = 29; ieta <= 41; ++ieta){
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

     histosTH2F["xiTowerVsxiGenPlus"]->Fill(xigen_plus,xiTower_plus);
     histosTH2F["xiTowerVsxiGenMinus"]->Fill(xigen_minus,xiTower_minus);

     //histosTH1F["ResXiTowerPlus"]->Fill((xiTower_plus - xigen_plus)/xigen_plus);
     //histosTH1F["ResXiTowerMinus"]->Fill((xiTower_minus - xigen_minus)/xigen_minus);
     histosTH1F["ResXiTowerPlus"]->Fill(xiTower_plus - xigen_plus);
     histosTH1F["ResXiTowerMinus"]->Fill(xiTower_minus - xigen_minus);

     double missingMass = 2*Ebeam*sqrt(xiTower_plus*xiTower_minus);
     histosTH1F["missingMassFromXi"]->Fill(missingMass);

     // Selection
     if(doThirdJetSelection&&(thirdJetPt > thirdJetPtMax)) continue;
     if(doTrackSelection&&(nTracksOutsideJets > nTracksMax)) continue;
     if(doHFMultiplicitySelection&&(nHF_plus > nHFPlusMax)) continue;
     if(doHFMultiplicitySelection&&(nHF_minus > nHFMinusMax)) continue;

     histosTH1F["xiPlusFromJetsAfterSel"]->Fill(xiFromJets.first);
     histosTH1F["xiMinusFromJetsAfterSel"]->Fill(xiFromJets.second);
     histosTH1F["xiPlusFromPFCandsAfterSel"]->Fill(xiFromPFCands.first);
     histosTH1F["xiMinusFromPFCandsAfterSel"]->Fill(xiFromPFCands.second);
     histosTH1F["RjjFromJetsAfterSel"]->Fill(RjjFromJets);
     histosTH1F["RjjFromPFCandsAfterSel"]->Fill(RjjFromPFCands);
     histosTH1F["RjjFromJetsAfterSelCustomBin"]->Fill(RjjFromJets);
     histosTH1F["RjjFromPFCandsAfterSelCustomBin"]->Fill(RjjFromPFCands);
 
     double fbAsymmetryEnergy = ((sumE_plus + sumE_minus) > 0.)?((sumE_plus - sumE_minus)/(sumE_plus + sumE_minus)):0.;

     histosTH1F["forwardBackwardAsymmetryHFEnergy"]->Fill(fbAsymmetryEnergy);

   }  // End loop over events
  
   hfile->Write();
   hfile->Close();
}

template <class PartColl>
double MassColl(PartColl& partCollection){
   math::XYZTLorentzVector allCands(0.,0.,0.,0.);
   for(typename PartColl::const_iterator part = partCollection.begin();
                                         part != partCollection.end(); ++part) allCands += part->p4();

   return allCands.M();
}

template <class Coll>
std::pair<double,double> xi(Coll& partCollection, double Ebeam){

   double xi_towers_plus = 0.;
   double xi_towers_minus = 0.;
   for(typename Coll::const_iterator part = partCollection.begin(); part != partCollection.end(); ++part){
     //double correction = (jetCorrector)?(jetCorrector->getCorrection(part->pt(),part->eta())):1.;
     xi_towers_plus += part->et()*TMath::Exp(part->eta());
     xi_towers_minus += part->et()*TMath::Exp(-part->eta());
   }

   xi_towers_plus /= 2*Ebeam;
   xi_towers_minus /= 2*Ebeam;
   
   return std::make_pair(xi_towers_plus,xi_towers_minus);
}

template <class JetColl,class PartColl>
double Rjj(JetColl& jetCollection,PartColl& partCollection){
   math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
   dijetSystem += (jetCollection[0]).p4();
   dijetSystem += (jetCollection[1]).p4(); 

   double Mx = MassColl(partCollection);

   return (dijetSystem.M()/Mx);
}

unsigned int nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapTreshToiEta, unsigned int thresholdHF, unsigned int ieta){
   const std::vector<unsigned int>& vec_iEta = mapTreshToiEta.find(thresholdHF)->second;

   // Count number of ieta entries in vector 
   int count_ieta = (int)std::count(vec_iEta.begin(),vec_iEta.end(),ieta);

   return count_ieta;
}
