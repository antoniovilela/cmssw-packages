#ifndef ExclusiveDijetsAnalysis_Histos_h
#define ExclusiveDijetsAnalysis_Histos_h

//#include "FWCore/ServiceRegistry/interface/Service.h"
//#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include <cmath>
#include <map>

#include "TH1F.h"
#include "TH2F.h"

namespace exclusiveDijetsAnalysis {
  // Histograms used for analysis
  typedef std::map<std::string,TH1F*> HistoMapTH1F;
  typedef std::map<std::string,TH2F*> HistoMapTH2F;

  // Book histos
  // FIXME: configure which histos to add
  template <class Adaptor>
  void bookHistos(HistoMapTH1F& histos, const Adaptor& adaptor){
     //pMakeTH1F = &Adaptor::template make<TH1F>;

     histos["nPileUpBx0"] = adaptor.template make<TH1F>("nPileUpBx0","Nr. of pile-up events in Bx0",10,0,10);
     histos["nVertex"] = adaptor.template make<TH1F>("nVertex","Nr. of offline primary vertexes",10,0,10);

     histos["leadingJetPt"] = adaptor.template make<TH1F>("leadingJetPt","leadingJetPt",100,0.,100.);
     histos["leadingJetEta"] = adaptor.template make<TH1F>("leadingJetEta","leadingJetEta",100,-5.,5.);
     histos["leadingJetPhi"] = adaptor.template make<TH1F>("leadingJetPhi","leadingJetPhi",100,-1.1*M_PI,1.1*M_PI);
     histos["leadingJetBDiscriminator"] = adaptor.template make<TH1F>("leadingJetBDiscriminator","leadingJetBDiscriminator",100,-10.,30.);
 
     histos["secondJetPt"] = adaptor.template make<TH1F>("secondJetPt","secondJetPt",100,0.,100.);
     histos["secondJetEta"] = adaptor.template make<TH1F>("secondJetEta","secondJetEta",100,-5.,5.);
     histos["secondJetPhi"] = adaptor.template make<TH1F>("secondJetPhi","secondJetPhi",100,-1.1*M_PI,1.1*M_PI);
     histos["secondJetBDiscriminator"] = adaptor.template make<TH1F>("secondJetBDiscriminator","secondJetBDiscriminator",100,-10.,30.);

     histos["thirdJetPt"] = adaptor.template make<TH1F>("thirdJetPt","thirdJetPt",100,0.,80.);
     histos["thirdJetEta"] = adaptor.template make<TH1F>("thirdJetEta","thirdJetEta",100,-5.,5.);

     histos["jetsAveEta"] = adaptor.template make<TH1F>("jetsAveEta","jetsAveEta",100,-5.,5.);
     histos["jetsDeltaEta"] = adaptor.template make<TH1F>("jetsDeltaEta","jetsDeltaEta",100,-5.,5.);
     histos["jetsDeltaPhi"] = adaptor.template make<TH1F>("jetsDeltaPhi","jetsDeltaPhi",100,0.,1.1*M_PI);
     histos["jetsDeltaPt"] = adaptor.template make<TH1F>("jetsDeltaPt","jetsDeltaPt",100,0.,10.);

     histos["trackMultiplicity"] = adaptor.template make<TH1F>("trackMultiplicity","trackMultiplicity",20,0,20);
     histos["multiplicityHFPlus"] = adaptor.template make<TH1F>("multiplicityHFPlus","multiplicityHFPlus",20,0,20);
     histos["multiplicityHFMinus"] = adaptor.template make<TH1F>("multiplicityHFMinus","multiplicityHFMinus",20,0,20);

     histos["xiGenPlus"] = adaptor.template make<TH1F>("xiGenPlus","xiGenPlus",200,0.,1.);
     histos["xiGenMinus"] = adaptor.template make<TH1F>("xiGenMinus","xiGenMinus",200,0.,1.);
     histos["xiTowerPlus"] = adaptor.template make<TH1F>("xiTowerPlus","xiTowerPlus",200,0.,1.);
     histos["xiTowerMinus"] = adaptor.template make<TH1F>("xiTowerMinus","xiTowerMinus",200,0.,1.);
     histos["xiPlusFromJets"] = adaptor.template make<TH1F>("xiPlusFromJets","xiPlusFromJets",200,0.,1.);
     histos["xiMinusFromJets"] = adaptor.template make<TH1F>("xiMinusFromJets","xiMinusFromJets",200,0.,1.);
     histos["xiPlusFromPFCands"] = adaptor.template make<TH1F>("xiPlusFromPFCands","xiPlusFromPFCands",200,0.,1.);
     histos["xiMinusFromPFCands"] = adaptor.template make<TH1F>("xiMinusFromPFCands","xiMinusFromPFCands",200,0.,1.);

     histos["ResXiTowerPlus"] = adaptor.template make<TH1F>("ResXiTowerPlus","ResXiTowerPlus",100,-1.,1.);
     histos["ResXiTowerMinus"] = adaptor.template make<TH1F>("ResXiTowerMinus","ResXiTowerMinus",100,-1.,1.);
     histos["ResXiPlusFromJets"] = adaptor.template make<TH1F>("ResXiPlusFromJets","ResXiPlusFromJets",100,-1.,1.);
     histos["ResXiMinusFromJets"] = adaptor.template make<TH1F>("ResXiMinusFromJets","ResXiMinusFromJets",100,-1.,1.);
     histos["ResXiPlusFromPFCands"] = adaptor.template make<TH1F>("ResXiPlusFromPFCands","ResXiPlusFromPFCands",100,-1.,1.);
     histos["ResXiMinusFromPFCands"] = adaptor.template make<TH1F>("ResXiMinusFromPFCands","ResXiMinusFromPFCands",100,-1.,1.);

     histos["massDijets"] = adaptor.template make<TH1F>("massDijets","massDijets",200,-10.,400.);
     histos["massDijetsGen"] = adaptor.template make<TH1F>("massDijetsGen","massDijetsGen",200,-10.,400.);
     histos["missingMassFromXi"] = adaptor.template make<TH1F>("missingMassFromXi","missingMassFromXi",200,-10.,400.);
     histos["MxFromJets"] = adaptor.template make<TH1F>("MxFromJets","MxFromJets",200,-10.,400.);
     histos["RjjFromJets"] = adaptor.template make<TH1F>("RjjFromJets","RjjFromJets",200,-0.1,1.5);
     histos["RjjFromPFCands"] = adaptor.template make<TH1F>("RjjFromPFCands","RjjFromPFCands",200,-0.1,1.5);
     histos["ResMassDijets"] = adaptor.template make<TH1F>("ResMassDijets","ResMassDijets",100,-1.,1.);

     histos["EnergyVsEta"] = adaptor.template make<TH1F>("EnergyVsEta","EnergyVsEta",300,-15.,15.);
  }

  template <class Adaptor>
  void bookHistos(HistoMapTH2F& histos, const Adaptor& adaptor){
     histos["iEtaVsHFCountPlus"] = adaptor.template make<TH2F>("iEtaVsHFCountPlus","iEtaVsHFCountPlus",11,30,41,20,0,20);
     histos["iEtaVsHFCountMinus"] = adaptor.template make<TH2F>("iEtaVsHFCountMinus","iEtaVsHFCountMinus",11,30,41,20,0,20);
     histos["HFRingCountPlus"] = adaptor.template make<TH2F>("HFRingCountPlus","HFRingCountPlus",13,1,14,20,0,20);
     histos["HFRingCountMinus"] = adaptor.template make<TH2F>("HFRingCountMinus","HFRingCountMinus",13,1,14,20,0,20);

     histos["xiPlusVsxiGenPlus"] = adaptor.template make<TH2F>("xiPlusVsxiGenPlus","xiPlusVsxiGenPlus",100,0.,1.,100,0.,1.);
     histos["xiMinusVsxiGenMinus"] = adaptor.template make<TH2F>("xiMinusVsxiGenMinus","xiMinusVsxiGenMinus",100,0.,1.,100,0.,1.);

     histos["RjjFromJetsVsBDicriminator"] = adaptor.template make<TH2F>("RjjFromJetsVsBDicriminator","RjjFromJetsVsBDicriminator",200,-0.1,1.5,200,-10.,30.);
     histos["RjjFromPFCandsVsBDicriminator"] = adaptor.template make<TH2F>("RjjFromPFCandsVsBDicriminator","RjjFromPFCandsVsBDicriminator",200,-0.1,1.5,200,-10.,30.);
  }

} // namespace

#endif
