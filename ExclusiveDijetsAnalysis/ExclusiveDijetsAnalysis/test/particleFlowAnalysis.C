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
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#endif

//typedef std::vector<std::string> (*GetFiles)();
 
template <class Coll>
std::pair<double,double> xi(Coll&);

void particleFlowAnalysis(std::vector<std::string>& fileNames,int maxEvents = -1, bool verbose = false) {
   // Create a vector of input files
   //vector<string> fileNames = func();
   if(verbose){
     std::cout << ">>> Reading files: " << std::endl;
     for(vector<string>::const_iterator it = fileNames.begin(); it != fileNames.end(); ++it)
               std::cout << "  " << *it << std::endl; 
   } 

   // Chain the input files
   fwlite::ChainEvent ev(fileNames);
   
   // Create output file
   TFile* hfile = new TFile("analysisPF_histos.root","recreate","data histograms");

   // Book Histograms
   TH1F* h_leadingJetPt = new TH1F("leadingJetPt","leadingJetPt",100,0.,100.);
   TH1F* h_leadingJetEta = new TH1F("leadingJetEta","leadingJetEta",100,-5.,5.);
   TH1F* h_leadingJetPhi = new TH1F("leadingJetPhi","leadingJetPhi",100,-1.1*M_PI,1.1*M_PI);

   TH1F* h_secondJetPt = new TH1F("secondJetPt","secondJetPt",100,0.,100.);
   TH1F* h_secondJetEta = new TH1F("secondJetEta","secondJetEta",100,-5.,5.);
   TH1F* h_secondJetPhi = new TH1F("secondJetPhi","secondJetPhi",100,-1.1*M_PI,1.1*M_PI);

   TH1F* h_jetsDeltaEta = new TH1F("jetsDeltaEta","jetsDeltaEta",100,-5.,5.);
   TH1F* h_jetsDeltaPhi = new TH1F("jetsDeltaPhi","jetsDeltaPhi",100,-1.1*M_PI,1.1*M_PI);

   TH1F* h_trackMultiplicityAssociatedToPV = new TH1F("trackMultiplicityAssociatedToPV","trackMultiplicityAssociatedToPV",20,0,20);
   TH1F* h_trackMultiplicityOutsideJets = new TH1F("trackMultiplicityOutsideJets","trackMultiplicityOutsideJets",20,0,20);
   TH1F* h_trackMultiplicityTransverseRegion = new TH1F("trackMultiplicityTransverseRegion","trackMultiplicityTransverseRegion",20,0,20);
   TH1F* h_multiplicityHFPlus = new TH1F("multiplicityHFPlus","multiplicityHFPlus",20,0,20);
   TH1F* h_multiplicityHFMinus = new TH1F("multiplicityHFMinus","multiplicityHFMinus",20,0,20);

   TH1F* h_xiGenPlus = new TH1F("xiGenPlus","xiGenPlus",200,0.,1.);
   TH1F* h_xiGenMinus = new TH1F("xiGenMinus","xiGenMinus",200,0.,1.);
   TH1F* h_xiPlus = new TH1F("xiPlus","xiPlus",200,0.,1.);
   TH1F* h_xiMinus = new TH1F("xiMinus","xiMinus",200,0.,1.);
   TH2F* h_xiTowerVsxiGenPlus = new TH2F("xiTowerVsxiGenPlus","xiTowerVsxiGenPlus",100,0.,1.,100,0.,1.);
   TH2F* h_xiTowerVsxiGenMinus = new TH2F("xiTowerVsxiGenMinus","xiTowerVsxiGenMinus",100,0.,1.,100,0.,1.);
   TH1F* h_ResXiPlus = new TH1F("ResXiPlus","ResXiPlus",200,-1.,1.);
   TH1F* h_ResXiMinus = new TH1F("ResXiMinus","ResXiMinus",200,-1.,1.);

   TH1F* h_massDijets = new TH1F("massDijets","massDijets",200,-10.,200.);
   TH1F* h_missingMassFromXi = new TH1F("missingMassFromXi","missingMassFromXi",200,-10.,200.);
   TH1F* h_MxFromJets = new TH1F("MxFromJets","MxFromJets",200,-10.,200.);
   TH1F* h_RjjFromJets = new TH1F("RjjFromJets","RjjFromJets",200,-0.1,1.5);
   TH1F* h_MxFromPFCands = new TH1F("MxFromPFCands","MxFromPFCands",200,-10.,200.);
   TH1F* h_RjjFromPFCands = new TH1F("RjjFromPFCands","RjjFromPFCands",200,-0.1,1.5);

   TH1F* h_xiPlusFromJets = new TH1F("xiPlusFromJets","xiPlusFromJets",200,0.,1.);
   TH1F* h_xiMinusFromJets = new TH1F("xiMinusFromJets","xiMinusFromJets",200,0.,1.);
   TH1F* h_ResXiPlusFromJets = new TH1F("ResXiPlusFromJets","ResXiPlusFromJets",200,-1.,1.);
   TH1F* h_ResXiMinusFromJets = new TH1F("ResXiMinusFromJets","ResXiMinusFromJets",200,-1.,1.);

   TH1F* h_xiPlusFromPFCands = new TH1F("xiPlusFromPFCands","xiPlusFromPFCands",200,0.,1.);
   TH1F* h_xiMinusFromPFCands = new TH1F("xiMinusFromPFCands","xiMinusFromPFCands",200,0.,1.);

   TH1F* h_EnergyVsEta = new TH1F("EnergyVsEta","EnergyVsEta",300,-15.,15.);

   TH1F* h_xiPlusAfterSel = new TH1F("xiPlusAfterSel","xiPlusAfterSel",200,0.,1.);
   TH1F* h_xiMinusAfterSel = new TH1F("xiMinusAfterSel","xiMinusAfterSel",200,0.,1.);
   TH1F* h_RjjAfterSel = new TH1F("RjjAfterSel","RjjAfterSel",200,-0.1,1.5);

   double Ebeam = 5000.;
   int thresholdHF = 10;// 2 GeV

   bool selectPileUp = true;
   int nEventsPUBx0 = 0;

   bool accessEdmDump = true;

   // Event selection
   // Di-jet
   double ptmin = 50.;
   double etamax = 2.5;
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
     // Access pile-up info 
     if(selectPileUp) {
       fwlite::Handle<std::map<int,int> > pileUpMap;
       pileUpMap.getByLabel(ev,"pileUpInfo");

       std::map<int,int>::const_iterator bx0Iter = pileUpMap->find(0);
       int nPileUpBx0 = bx0Iter->second;

       if(selectPileUp&&(nPileUpBx0 != nEventsPUBx0)) continue;
     }

     // Di-jets
     fwlite::Handle<std::vector<reco::PFJet> > jetCollection;
     //jetCollection.getByLabel(ev,"L2L3CorJetSC7PF");
     jetCollection.getByLabel(ev,"sisCone7PFJets");

     const reco::PFJet& jet1 = (*jetCollection)[0];// they come out ordered right?
     const reco::PFJet& jet2 = (*jetCollection)[1];

     h_leadingJetPt->Fill(jet1.pt());
     h_secondJetPt->Fill(jet2.pt());
     if(jet1.pt() < ptmin) continue;
     if(jet2.pt() < ptmin) continue;

     h_leadingJetEta->Fill(jet1.eta());
     h_secondJetEta->Fill(jet2.eta());
     if(fabs(jet1.eta()) > etamax) continue;
     if(fabs(jet2.eta()) > etamax) continue; 

     h_leadingJetPhi->Fill(jet1.phi());
     h_secondJetPhi->Fill(jet2.phi());

     h_jetsDeltaEta->Fill(jet1.eta() - jet2.eta());
     h_jetsDeltaPhi->Fill(jet1.phi() - jet2.phi());
 
     math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
     dijetSystem += jet1.p4();
     dijetSystem += jet2.p4();
     h_massDijets->Fill(dijetSystem.M());

     math::XYZTLorentzVector allJets(0.,0.,0.,0.);
     for(std::vector<reco::PFJet>::const_iterator jet = jetCollection->begin();
                                                  jet != jetCollection->end(); ++jet) allJets += jet->p4();

     h_MxFromJets->Fill(allJets.M());

     double RjjFromJets = dijetSystem.M()/allJets.M();
     h_RjjFromJets->Fill(RjjFromJets);
     
     // PF candidates
     fwlite::Handle<std::vector<reco::PFCandidate> > pfCandCollection;
     pfCandCollection.getByLabel(ev,"particleFlow");

     math::XYZTLorentzVector allPFCands(0.,0.,0.,0.);
     for(std::vector<reco::PFCandidate>::const_iterator pfCand = pfCandCollection->begin();
                                                        pfCand != pfCandCollection->end();
                                                        ++pfCand) allPFCands += pfCand->p4();

     h_MxFromPFCands->Fill(allPFCands.M());

     double RjjFromPFCands = dijetSystem.M()/allPFCands.M();
     h_RjjFromPFCands->Fill(RjjFromPFCands);

     std::pair<double,double> xiFromJets = xi(*jetCollection);
     h_xiPlusFromJets->Fill(xiFromJets.first);
     h_xiMinusFromJets->Fill(xiFromJets.second);
     //std::cout << "xi plus from jets= " << xiFromJets.first << std::endl;
     //std::cout << "xi minus from jets= " << xiFromJets.second << std::endl;

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

     h_xiPlus->Fill(xiTower_plus);
     h_xiMinus->Fill(xiTower_minus);

     h_xiTowerVsxiGenPlus->Fill(xigen_plus,xiTower_plus);
     h_xiTowerVsxiGenMinus->Fill(xigen_minus,xiTower_minus);

     h_ResXiPlus->Fill((xiTower_plus - xigen_plus)/xigen_plus);
     h_ResXiMinus->Fill((xiTower_minus - xigen_minus)/xigen_minus);

     double missingMass = 10000.*sqrt(xiTower_plus*xiTower_minus);
     h_missingMassFromXi->Fill(missingMass);

     // Selection
     if(doTrackSelection&&(nTracksOutsideJets > nTracksMax)) continue;
     if(doHFMultiplicitySelection&&(nHF_plus > nHFPlusMax)) continue;
     if(doHFMultiplicitySelection&&(nHF_minus > nHFMinusMax)) continue;

     h_xiPlusAfterSel->Fill(xiTower_plus);
     h_xiMinusAfterSel->Fill(xiTower_minus);
     h_RjjAfterSel->Fill(RjjFromJets);
 
   }  // End loop over events
  
   hfile->Write();
   hfile->Close();
}

template <class Coll>
std::pair<double,double> xi(Coll& partCollection){

   double xi_towers_plus = 0.;
   double xi_towers_minus = 0.;
   for(typename Coll::const_iterator part = partCollection.begin(); part != partCollection.end(); ++part){
     xi_towers_plus += part->et()*TMath::Exp(part->eta());
     xi_towers_minus += part->et()*TMath::Exp(-part->eta());
   }

   xi_towers_plus /= 10000.;
   xi_towers_minus /= 10000.;
   
   return std::make_pair(xi_towers_plus,xi_towers_minus);
}
