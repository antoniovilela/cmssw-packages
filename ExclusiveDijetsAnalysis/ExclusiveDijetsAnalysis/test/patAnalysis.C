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
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/VertexReco/interface/Vertex.h" 
#endif

template <class Coll>
std::pair<double,double> xi(Coll&);

template <class JetColl,class PartColl>
double Rjj(JetColl&,PartColl&);

unsigned int nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapTreshToiEta, unsigned int thresholdHF, unsigned int ieta);

void patAnalysis(std::vector<std::string>& fileNames,int maxEvents = -1, bool verbose = false) {
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
   TFile* hfile = new TFile("analysisPAT_histos.root","recreate","data histograms");

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

   // Book Histograms
   TH1F* h_NPUBx0 = new TH1F("NPUBx0","NPUBx0",10,0,10);
   TH1F* h_NPrimVtx = new TH1F("NPrimVtx","NPrimVtx",10,0,10);
   TH2F* h_NPUBx0vsNPrimVtx = new TH2F("NPUBx0vsNPrimVtx","NPUBx0vsNPrimVtx",10,0,10,10,0,10);

   TH1F* h_leadingJetPt = new TH1F("leadingJetPt","leadingJetPt",100,0.,100.);
   TH1F* h_leadingJetEta = new TH1F("leadingJetEta","leadingJetEta",100,-5.,5.);
   TH1F* h_leadingJetPhi = new TH1F("leadingJetPhi","leadingJetPhi",100,-1.1*M_PI,1.1*M_PI);
   TH1F* h_leadingJetBDiscriminator = new TH1F("leadingJetBDiscriminator","leadingJetBDiscriminator",100,-10.,30.);

   TH1F* h_secondJetPt = new TH1F("secondJetPt","secondJetPt",100,0.,100.);
   TH1F* h_secondJetEta = new TH1F("secondJetEta","secondJetEta",100,-5.,5.);
   TH1F* h_secondJetPhi = new TH1F("secondJetPhi","secondJetPhi",100,-1.1*M_PI,1.1*M_PI);
   TH1F* h_secondJetBDiscriminator = new TH1F("secondJetBDiscriminator","secondJetBDiscriminator",100,-10.,30.);

   TH1F* h_thirdJetPt = new TH1F("thirdJetPt","thirdJetPt",100,0.,40.);
   TH1F* h_thirdJetEta = new TH1F("thirdJetEta","thirdJetEta",100,-5.,5.);

   TH1F* h_jetsAveEta = new TH1F("jetsAveEta","jetsAveEta",100,-5.,5.);

   TH1F* h_jetsDeltaEta = new TH1F("jetsDeltaEta","jetsDeltaEta",100,-5.,5.);
   TH1F* h_jetsDeltaPhi = new TH1F("jetsDeltaPhi","jetsDeltaPhi",100,-1.1*M_PI,1.1*M_PI);

   TH1F* h_trackMultiplicityAssociatedToPV = new TH1F("trackMultiplicityAssociatedToPV","trackMultiplicityAssociatedToPV",20,0,20);
   TH1F* h_trackMultiplicityOutsideJets = new TH1F("trackMultiplicityOutsideJets","trackMultiplicityOutsideJets",20,0,20);
   TH1F* h_trackMultiplicityTransverseRegion = new TH1F("trackMultiplicityTransverseRegion","trackMultiplicityTransverseRegion",20,0,20);
   TH1F* h_multiplicityHFPlus = new TH1F("multiplicityHFPlus","multiplicityHFPlus",20,0,20);
   TH1F* h_multiplicityHFMinus = new TH1F("multiplicityHFMinus","multiplicityHFMinus",20,0,20);

   TH2F* h_iEtaVsHFCountPlus = new TH2F("iEtaVsHFCountPlus","iEtaVsHFCountPlus",12,29,41,20,0,20);
   TH2F* h_iEtaVsHFCountMinus = new TH2F("iEtaVsHFCountMinus","iEtaVsHFCountMinus",12,29,41,20,0,20);
 
   TH1F* h_xiGenPlus = new TH1F("xiGenPlus","xiGenPlus",200,0.,1.);
   TH1F* h_xiGenMinus = new TH1F("xiGenMinus","xiGenMinus",200,0.,1.);
   TH1F* h_xiPlus = new TH1F("xiPlus","xiPlus",200,0.,1.);
   TH1F* h_xiMinus = new TH1F("xiMinus","xiMinus",200,0.,1.);
   TH2F* h_xiTowerVsxiGenPlus = new TH2F("xiTowerVsxiGenPlus","xiTowerVsxiGenPlus",100,0.,1.,100,0.,1.);
   TH2F* h_xiTowerVsxiGenMinus = new TH2F("xiTowerVsxiGenMinus","xiTowerVsxiGenMinus",100,0.,1.,100,0.,1.);
   TH1F* h_ResXiPlus = new TH1F("ResXiPlus","ResXiPlus",200,-1.,1.);
   TH1F* h_ResXiMinus = new TH1F("ResXiMinus","ResXiMinus",200,-1.,1.);

   TH1F* h_massDijets = new TH1F("massDijets","massDijets",200,-10.,400.);
   TH1F* h_missingMassFromXi = new TH1F("missingMassFromXi","missingMassFromXi",200,-10.,400.);
   TH1F* h_MxFromJets = new TH1F("MxFromJets","MxFromJets",200,-10.,400.);
   TH1F* h_RjjFromJets = new TH1F("RjjFromJets","RjjFromJets",200,-0.1,1.5);
   TH1F* h_MxFromPFCands = new TH1F("MxFromPFCands","MxFromPFCands",200,-10.,200.);
   TH1F* h_RjjFromPFCands = new TH1F("RjjFromPFCands","RjjFromPFCands",200,-0.1,1.5);
   
   TH1F* h_xiPlusFromJets = new TH1F("xiPlusFromJets","xiPlusFromJets",200,0.,1.);
   TH1F* h_xiMinusFromJets = new TH1F("xiMinusFromJets","xiMinusFromJets",200,0.,1.);
   TH1F* h_ResXiPlusFromJets = new TH1F("ResXiPlusFromJets","ResXiPlusFromJets",200,-1.,1.);
   TH1F* h_ResXiMinusFromJets = new TH1F("ResXiMinusFromJets","ResXiMinusFromJets",200,-1.,1.);

   TH1F* h_xiPlusFromPFCands = new TH1F("xiPlusFromPFCands","xiPlusFromPFCands",200,0.,1.);
   TH1F* h_xiMinusFromPFCands = new TH1F("xiMinusFromPFCands","xiMinusFromPFCands",200,0.,1.);
   TH1F* h_ResXiPlusFromPFCands = new TH1F("ResXiPlusFromPFCands","ResXiPlusFromPFCands",200,-1.,1.);
   TH1F* h_ResXiMinusFromPFCands = new TH1F("ResXiMinusFromPFCands","ResXiMinusFromPFCands",200,-1.,1.);

   TH1F* h_EnergyVsEta = new TH1F("EnergyVsEta","EnergyVsEta",300,-15.,15.);

   TH2F* h_RjjFromJetsVsBDicriminator = new TH2F("RjjFromJetsVsBDicriminator","RjjFromJetsVsBDicriminator",200,-0.1,1.5,200,-10.,30.);
   TH2F* h_RjjFromPFCandsVsBDicriminator = new TH2F("RjjFromPFCandsVsBDicriminator","RjjFromPFCandsVsBDicriminator",200,-0.1,1.5,200,-10.,30.);
   TH2F* h_RjjFromJetsVsThirdJetPt = new TH2F("RjjFromJetsVsThirdJetPt","RjjFromJetsVsThirdJetPt",200,-0.1,1.5,200,0.,80.);
   TH2F* h_RjjFromPFCandsVsThirdJetPt = new TH2F("RjjFromPFCandsVsThirdJetPt","RjjFromPFCandsVsThirdJetPt",200,-0.1,1.5,200,0.,80.);

   TH1F* h_xiPlusAfterSel = new TH1F("xiPlusAfterSel","xiPlusAfterSel",200,0.,1.);
   TH1F* h_xiMinusAfterSel = new TH1F("xiMinusAfterSel","xiMinusAfterSel",200,0.,1.);

   double xbins[8] = {0.,0.4,0.7,0.8,0.85,0.9,0.95,1.0};
   //TH1F* h_RjjAfterSel = new TH1F("RjjAfterSel","RjjAfterSel",200,-0.1,1.5);
   //TH1F* h_RjjFromPFAfterSel = new TH1F("RjjFromPFAfterSel","RjjFromPFAfterSel",200,-0.1,1.5);
   TH1F* h_RjjAfterSel = new TH1F("RjjAfterSel","RjjAfterSel",7,xbins);
   TH1F* h_RjjFromPFAfterSel = new TH1F("RjjFromPFAfterSel","RjjFromPFAfterSel",7,xbins);

   TH1F* h_forwardBackwardAsymmetryHFEnergy_ = new TH1F("forwardBackwardAsymmetryHFEnergy","forwardBackwardAsymmetryHFEnergy",200,-1.1,1.1);

   std::vector<TH1F*> histos_ResMassDijets;
   std::vector<TH1F*> histos_ResXiPlusFromJets;
   std::vector<TH1F*> histos_ResXiMinusFromJets;
   std::vector<TH1F*> histos_RjjFromJets;
   for(std::vector<std::string>::const_iterator it = jetColls.begin(); it != jetColls.end(); ++it){
      std::string hname = "ResMassDijets_" + *it;
      histos_ResMassDijets.push_back(new TH1F(hname.c_str(),hname.c_str(),200,-1.,1.));
      hname = "ResXiPlusFromJets_" + *it;
      histos_ResXiPlusFromJets.push_back(new TH1F(hname.c_str(),hname.c_str(),200,-1.,1.));
      hname = "ResXiMinusFromJets_" + *it;
      histos_ResXiMinusFromJets.push_back(new TH1F(hname.c_str(),hname.c_str(),200,-1.,1.));
      hname = "RjjFromJets_" + *it;
      histos_RjjFromJets.push_back(new TH1F(hname.c_str(),hname.c_str(),200,-0.1,1.5));
   }

   std::vector<TH1F*> histos_RjjFromPFCands;
   for(std::vector<std::string>::const_iterator it = jetCollsNonCorr.begin(); it != jetCollsNonCorr.end(); ++it){
      std::string hname = "RjjFromPFCands" + *it;
      histos_RjjFromPFCands.push_back(new TH1F(hname.c_str(),hname.c_str(),200,-0.1,1.5));
   }

   double Ebeam = 5000.;
   int thresholdHF = 16;// 2.4 GeV

   bool accessPileUp = true;
   bool selectPileUp = false;
   int nEventsPUBx0 = 0;

   bool accessEdmDump = true;

   // Event selection
   // Di-jet
   double ptmin = 50.;
   double etamax = 2.5;
   //B-tag
   bool doBTag = false;
   std::string bDiscriminatorName = "jetBProbabilityBJetTags";
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
     h_NPrimVtx->Fill(nGoodVertices);

     // Access pile-up info 
     if(accessPileUp) {
       fwlite::Handle<std::map<int,int> > pileUpMap;
       pileUpMap.getByLabel(ev,"pileUpInfo");

       std::map<int,int>::const_iterator bx0Iter = pileUpMap->find(0);
       int nPileUpBx0 = bx0Iter->second;

       h_NPUBx0->Fill(nPileUpBx0);
       h_NPUBx0vsNPrimVtx->Fill(nPileUpBx0,nGoodVertices);
       if(selectPileUp&&(nPileUpBx0 != nEventsPUBx0)) continue;
     }

     // Di-jets
     std::vector<fwlite::Handle<std::vector<pat::Jet> > > jetCollections(jetColls.size());
     for(size_t k = 0; k < jetColls.size(); ++k){
       jetCollections[k].getByLabel(ev,jetColls[k].c_str());
     }

     std::vector<fwlite::Handle<std::vector<reco::PFJet> > > jetCollectionsNonCorr(jetCollsNonCorr.size());
     for(size_t k = 0; k < jetCollsNonCorr.size(); ++k){
       jetCollectionsNonCorr[k].getByLabel(ev,jetCollsNonCorr[k].c_str());
     }

     const fwlite::Handle<std::vector<pat::Jet> >& jetCollection = jetCollections[0];
     const fwlite::Handle<std::vector<reco::PFJet> >& jetCollectionNonCorr = jetCollectionsNonCorr[0];  

     const pat::Jet& jet1 = (*jetCollection)[0];// they come out ordered right?
     const pat::Jet& jet2 = (*jetCollection)[1];

     /*if(verbose){
       std::cout << " JEC set: " << jet1.corrFactorSetLabel() << std::endl;
       std::cout << " JEC level: " << jet1.corrStep() << std::endl;
     }*/

     h_leadingJetPt->Fill(jet1.pt());
     h_secondJetPt->Fill(jet2.pt());
     if(jet1.pt() < ptmin) continue;
     if(jet2.pt() < ptmin) continue;

     h_leadingJetEta->Fill(jet1.eta());
     h_secondJetEta->Fill(jet2.eta());
     if(fabs(jet1.eta()) > etamax) continue;
     if(fabs(jet2.eta()) > etamax) continue; 

     h_jetsAveEta->Fill((jet1.eta() + jet2.eta())/2);
     
     h_leadingJetPhi->Fill(jet1.phi());
     h_secondJetPhi->Fill(jet2.phi());

     if(jetCollection->size() > 2){
        const pat::Jet& jet3 = (*jetCollection)[2];
        h_thirdJetPt->Fill(jet3.pt());
        h_thirdJetEta->Fill(jet3.eta());
     }

     h_jetsDeltaEta->Fill(jet1.eta() - jet2.eta());
     h_jetsDeltaPhi->Fill(jet1.phi() - jet2.phi());
 
     math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
     dijetSystem += jet1.p4();
     dijetSystem += jet2.p4();
     h_massDijets->Fill(dijetSystem.M());

     math::XYZTLorentzVector allJets(0.,0.,0.,0.);
     for(std::vector<pat::Jet>::const_iterator jet = jetCollection->begin();
                                               jet != jetCollection->end(); ++jet) allJets += jet->p4();

     h_MxFromJets->Fill(allJets.M());

     // PF candidates
     fwlite::Handle<std::vector<reco::PFCandidate> > pfCandCollection;
     pfCandCollection.getByLabel(ev,"particleFlow");

     math::XYZTLorentzVector allPFCands(0.,0.,0.,0.);
     for(std::vector<reco::PFCandidate>::const_iterator pfCand = pfCandCollection->begin();
                                                        pfCand != pfCandCollection->end();
                                                        ++pfCand) allPFCands += pfCand->p4();

     h_MxFromPFCands->Fill(allPFCands.M());

     // Compute Rjj
     double RjjFromJets = Rjj(*jetCollection,*jetCollection);

     double RjjFromPFCands = Rjj(*jetCollectionNonCorr,*pfCandCollection);

     // B-tag
     double bDiscJet1 = jet1.bDiscriminator(bDiscriminatorName);
     double bDiscJet2 = jet2.bDiscriminator(bDiscriminatorName);

     h_leadingJetBDiscriminator->Fill(bDiscJet1);
     h_secondJetBDiscriminator->Fill(bDiscJet2);

     h_RjjFromJetsVsBDicriminator->Fill(RjjFromJets,bDiscJet1);
     h_RjjFromPFCandsVsBDicriminator->Fill(RjjFromPFCands,bDiscJet1);

     bool singleBTag = ((bDiscJet1 > bDiscMinValue)||(bDiscJet2 > bDiscMinValue));
     bool doubleBTag = ((bDiscJet1 > bDiscMinValue)&&(bDiscJet2 > bDiscMinValue));

     if(doBTag && !doubleBTag) continue;

     h_RjjFromJets->Fill(RjjFromJets);

     h_RjjFromPFCands->Fill(RjjFromPFCands);

     double thirdJetPt = (jetCollection->size() > 2)?(*jetCollection)[2].pt():0.;

     h_RjjFromJetsVsThirdJetPt->Fill(RjjFromJets,thirdJetPt);
     h_RjjFromPFCandsVsThirdJetPt->Fill(RjjFromPFCands,thirdJetPt);

     std::pair<double,double> xiFromJets = xi(*jetCollection);
     h_xiPlusFromJets->Fill(xiFromJets.first);
     h_xiMinusFromJets->Fill(xiFromJets.second);

     std::pair<double,double> xiFromPFCands = xi(*pfCandCollection);
     h_xiPlusFromPFCands->Fill(xiFromPFCands.first);
     h_xiMinusFromPFCands->Fill(xiFromPFCands.second);

     // Gen particles
     fwlite::Handle<std::vector<reco::GenParticle> > genParticlesCollection;
     genParticlesCollection.getByLabel(ev,"genParticles");
     const reco::GenParticleCollection& genParticles = *genParticlesCollection;

     reco::GenParticleCollection::const_iterator proton1 = genParticles.end();
     reco::GenParticleCollection::const_iterator proton2 = genParticles.end();
     for(reco::GenParticleCollection::const_iterator genpart = genParticles.begin(); genpart != genParticles.end(); ++genpart){
       if(genpart->status() != 1) continue;
       h_EnergyVsEta->Fill(genpart->eta(),genpart->energy());	
		
       double pz = genpart->pz();
       if((proton1 == genParticles.end())&&(genpart->pdgId() == 2212)&&(pz > 0.75*Ebeam)) proton1 = genpart;
       else if((proton2 == genParticles.end())&&(genpart->pdgId() == 2212)&&(pz < -0.75*Ebeam)) proton2 = genpart;
     }
     if(verbose){
       std::cout << "Proton (z-plus): " << proton1->pt() << "  " << proton1->eta() << "  " << proton1->phi() << std::endl;
       std::cout << "Proton (z-minus): " << proton2->pt() << "  " << proton2->eta() << "  " << proton2->phi() << std::endl;
     }

     double xigen_plus = 1 - proton1->pz()/Ebeam;
     double xigen_minus = 1 + proton2->pz()/Ebeam;

     h_xiGenPlus->Fill(xigen_plus);
     h_xiGenMinus->Fill(xigen_minus); 

     h_ResXiPlusFromJets->Fill((xiFromJets.first - xigen_plus)/xigen_plus);
     h_ResXiMinusFromJets->Fill((xiFromJets.second - xigen_minus)/xigen_minus);
  
     h_ResXiPlusFromPFCands->Fill((xiFromPFCands.first - xigen_plus)/xigen_plus);
     h_ResXiMinusFromPFCands->Fill((xiFromPFCands.second - xigen_minus)/xigen_minus);

     // Access all jet collections
     for(size_t k = 0; k < jetColls.size(); ++k){
        const pat::Jet& jet1 = (jetCollections[k])->at(0);
        const pat::Jet& jet2 = (jetCollections[k])->at(1);

        math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
        dijetSystem += jet1.p4();
        dijetSystem += jet2.p4();

        const reco::GenJet* genJet1 = jet1.genJet();
        const reco::GenJet* genJet2 = jet2.genJet();

        if(genJet1 && genJet2){
          math::XYZTLorentzVector dijetGenSystem(0.,0.,0.,0.);
          dijetGenSystem += genJet1->p4();
          dijetGenSystem += genJet2->p4();
          double massGen = dijetGenSystem.M();
          (histos_ResMassDijets[k])->Fill((dijetSystem.M() - massGen)/massGen);
        }

        std::pair<double,double> xiFromJets = xi(*(jetCollections[k]));
        (histos_ResXiPlusFromJets[k])->Fill((xiFromJets.first - xigen_plus)/xigen_plus);
        (histos_ResXiMinusFromJets[k])->Fill((xiFromJets.second - xigen_minus)/xigen_minus);

        (histos_RjjFromJets[k])->Fill(Rjj(*(jetCollections[k]),*(jetCollections[k])));
     }

     for(size_t k = 0; k < jetCollsNonCorr.size(); ++k){
        (histos_RjjFromPFCands[k])->Fill(Rjj(*(jetCollectionsNonCorr[k]),*pfCandCollection));
     }
 
     if(!accessEdmDump) continue;

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

     unsigned int nTracksAssociatedToPV = *trackMultiplicityAssociatedToPV;
     unsigned int nTracksOutsideJets = *trackMultiplicityOutsideJets;
     unsigned int nTracksTransverseRegion = *trackMultiplicityTransverseRegion; 

     unsigned int nHF_plus = (*nHFPlus)[thresholdHF];
     unsigned int nHF_minus = (*nHFMinus)[thresholdHF];
 
     double xiTower_plus = *xiTowerPlus;
     double xiTower_minus = *xiTowerMinus;
 
     h_trackMultiplicityAssociatedToPV->Fill(nTracksAssociatedToPV);
     h_trackMultiplicityOutsideJets->Fill(nTracksOutsideJets);
     h_trackMultiplicityTransverseRegion->Fill(nTracksTransverseRegion);

     h_multiplicityHFPlus->Fill(nHF_plus);
     h_multiplicityHFMinus->Fill(nHF_minus);     

     for(unsigned int ieta = 29; ieta <= 41; ++ieta){
        //const std::map<unsigned int, std::vector<unsigned int> >& mapThreshToiEta_plus = *mapThreshToiEtaPlus;
        unsigned int nHFPlus_ieta = nHFSlice(*mapThreshToiEtaPlus,thresholdHF,ieta);
        h_iEtaVsHFCountPlus->Fill(ieta,nHFPlus_ieta);

        std::map<unsigned int, std::vector<unsigned int> >::const_iterator ieta_plus = iEtaHFMultiplicityPlus->find(ieta);
        unsigned int nHFPlus_ieta_v2 = (ieta_plus == iEtaHFMultiplicityPlus->end())?0:ieta_plus->second[thresholdHF]; 
        //std::cout << "ieta= " << ieta << " HF mult. plus= " << nHFPlus_ieta << ", " << nHFPlus_ieta_v2 << std::endl;
        if(nHFPlus_ieta != nHFPlus_ieta_v2) std::cout << "ieta= " << ieta << " different multiplicity plus " << nHFPlus_ieta << ", " << nHFPlus_ieta_v2 << std::endl;

        //const std::map<unsigned int, std::vector<unsigned int> >& mapThreshToiEta_minus = *mapThreshToiEtaMinus;
        unsigned int nHFMinus_ieta = nHFSlice(*mapThreshToiEtaMinus,thresholdHF,ieta);
        h_iEtaVsHFCountMinus->Fill(ieta,nHFMinus_ieta); 

        std::map<unsigned int, std::vector<unsigned int> >::const_iterator ieta_minus = iEtaHFMultiplicityMinus->find(ieta);
        unsigned int nHFMinus_ieta_v2 = (ieta_minus == iEtaHFMultiplicityMinus->end())?0:ieta_minus->second[thresholdHF]; 
        //std::cout << "ieta= " << ieta << " HF mult. minus= " << nHFMinus_ieta << ", " << nHFMinus_ieta_v2 << std::endl;
        if(nHFMinus_ieta != nHFMinus_ieta_v2) std::cout << "ieta= " << ieta << " different multiplicity minus " << nHFMinus_ieta << ", " << nHFMinus_ieta_v2 << std::endl;
     }
 
     h_xiPlus->Fill(xiTower_plus);
     h_xiMinus->Fill(xiTower_minus);

     h_xiTowerVsxiGenPlus->Fill(xigen_plus,xiTower_plus);
     h_xiTowerVsxiGenMinus->Fill(xigen_minus,xiTower_minus);

     h_ResXiPlus->Fill((xiTower_plus - xigen_plus)/xigen_plus);
     h_ResXiMinus->Fill((xiTower_minus - xigen_minus)/xigen_minus);

     double missingMass = 10000.*sqrt(xiTower_plus*xiTower_minus);
     h_missingMassFromXi->Fill(missingMass);

     // Selection
     if(doThirdJetSelection&&(thirdJetPt > thirdJetPtMax)) continue;
     if(doTrackSelection&&(nTracksOutsideJets > nTracksMax)) continue;
     if(doHFMultiplicitySelection&&(nHF_plus > nHFPlusMax)) continue;
     if(doHFMultiplicitySelection&&(nHF_minus > nHFMinusMax)) continue;

     h_xiPlusAfterSel->Fill(xiTower_plus);
     h_xiMinusAfterSel->Fill(xiTower_minus);
     h_RjjAfterSel->Fill(RjjFromJets);
     h_RjjFromPFAfterSel->Fill(RjjFromPFCands);
 
     fwlite::Handle<std::vector<double> > sumEHFplus;
     sumEHFplus.getByLabel(ev,"hfTower","sumEHFplus");
   
     fwlite::Handle<std::vector<double> > sumEHFminus;
     sumEHFminus.getByLabel(ev,"hfTower","sumEHFminus");

     double sumE_plus = (*sumEHFplus)[thresholdHF];
     double sumE_minus = (*sumEHFminus)[thresholdHF];

     double fbAsymmetryEnergy = ((sumE_plus + sumE_minus) > 0.)?((sumE_plus - sumE_minus)/(sumE_plus + sumE_minus)):0.;

     h_forwardBackwardAsymmetryHFEnergy_->Fill(fbAsymmetryEnergy);

   }  // End loop over events
  
   hfile->Write();
   hfile->Close();
}

template <class Coll>
std::pair<double,double> xi(Coll& partCollection){

   double xi_towers_plus = 0.;
   double xi_towers_minus = 0.;
   for(typename Coll::const_iterator part = partCollection.begin(); part != partCollection.end(); ++part){
     //double correction = (jetCorrector)?(jetCorrector->getCorrection(part->pt(),part->eta())):1.;
     xi_towers_plus += part->et()*TMath::Exp(part->eta());
     xi_towers_minus += part->et()*TMath::Exp(-part->eta());
   }

   xi_towers_plus /= 10000.;
   xi_towers_minus /= 10000.;
   
   return std::make_pair(xi_towers_plus,xi_towers_minus);
}

template <class JetColl,class PartColl>
double Rjj(JetColl& jetCollection,PartColl& partCollection){
   math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
   dijetSystem += (jetCollection[0]).p4();
   dijetSystem += (jetCollection[1]).p4(); 

   math::XYZTLorentzVector allCands(0.,0.,0.,0.);
   for(typename PartColl::const_iterator part = partCollection.begin();
                                         part != partCollection.end(); ++part) allCands += part->p4();

   return (dijetSystem.M()/allCands.M());
}

unsigned int nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapTreshToiEta, unsigned int thresholdHF, unsigned int ieta){
   const std::vector<unsigned int>& vec_iEta = mapTreshToiEta.find(thresholdHF)->second;

   // Count number of ieta entries in vector 
   int count_ieta = (int)std::count(vec_iEta.begin(),vec_iEta.end(),ieta);

   return count_ieta;
}
