#ifndef ExclusiveDijetsAnalysis_Histos_h
#define ExclusiveDijetsAnalysis_Histos_h

//#include "FWCore/ServiceRegistry/interface/Service.h"
//#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include <cmath>

#include "TH1F.h"
#include "TH2F.h"

namespace exclusiveDijetsAnalysis {
  // Histograms used for analysis
  struct Histos {
     TH1F* h_nPileUpBx0_;

     TH1F* h_nVertex_;

     TH1F* h_leadingJetPt_;
     TH1F* h_leadingJetEta_;
     TH1F* h_leadingJetPhi_;
     TH1F* h_leadingJetBDiscriminator_;
   
     TH1F* h_secondJetPt_;
     TH1F* h_secondJetEta_;
     TH1F* h_secondJetPhi_;
     TH1F* h_secondJetBDiscriminator_;

     TH1F* h_thirdJetPt_;
     TH1F* h_thirdJetEta_;

     TH1F* h_jetsAveEta_;
     TH1F* h_jetsDeltaEta_;
     TH1F* h_jetsDeltaPhi_;
     TH1F* h_jetsDeltaPt_;

     TH1F* h_trackMultiplicity_;
     TH1F* h_multiplicityHFPlus_;
     TH1F* h_multiplicityHFMinus_;

     TH2F* h_iEtaVsHFCountPlus_;
     TH2F* h_iEtaVsHFCountMinus_;
  
     TH1F* h_xiGenPlus_;
     TH1F* h_xiGenMinus_;
     TH1F* h_xiTowerPlus_;
     TH1F* h_xiTowerMinus_;
     TH1F* h_xiPlusFromJets_;
     TH1F* h_xiMinusFromJets_;
     TH1F* h_xiPlusFromPFCands_;
     TH1F* h_xiMinusFromPFCands_;
     TH1F* h_ResXiTowerPlus_;
     TH1F* h_ResXiTowerMinus_;
     TH1F* h_ResXiPlusFromJets_;
     TH1F* h_ResXiMinusFromJets_;
     TH1F* h_ResXiPlusFromPFCands_;
     TH1F* h_ResXiMinusFromPFCands_;
     TH2F* h_xiPlusVsxiGenPlus_;
     TH2F* h_xiMinusVsxiGenMinus_;
      
     TH1F* h_massDijets_;
     TH1F* h_massDijetsGen_; 
     TH1F* h_missingMassFromXi_;
     TH1F* h_MxFromJets_;
     TH1F* h_RjjFromJets_;
     TH1F* h_RjjFromPFCands_;
     TH1F* h_ResMassDijets_;

     TH1F* h_EnergyVsEta_;

     TH2F* h_RjjFromJetsVsBDicriminator_;
     TH2F* h_RjjFromPFCandsVsBDicriminator_;
  };

  // Book histos
  // FIXME: make this a template function allowing for different "adaptors"
  //void bookHistos(Histos& histos, edm::Service<TFileService>& adaptor){
  template <class Adaptor>
  void bookHistos(Histos& histos, Adaptor& adaptor){
     histos.h_nPileUpBx0_ = adaptor.template make<TH1F>("nPileUpBx0","Nr. of pile-up events in Bx0",10,0,10);

     histos.h_nVertex_ = adaptor.template make<TH1F>("nVertex","Nr. of offline primary vertexes",10,0,10);

     histos.h_leadingJetPt_ = adaptor.template make<TH1F>("leadingJetPt","leadingJetPt",100,0.,100.);
     histos.h_leadingJetEta_ = adaptor.template make<TH1F>("leadingJetEta","leadingJetEta",100,-5.,5.);
     histos.h_leadingJetPhi_ = adaptor.template make<TH1F>("leadingJetPhi","leadingJetPhi",100,-1.1*M_PI,1.1*M_PI);
     histos.h_leadingJetBDiscriminator_ = adaptor.template make<TH1F>("leadingJetBDiscriminator","leadingJetBDiscriminator",100,-10.,30.);
 
     histos.h_secondJetPt_ = adaptor.template make<TH1F>("secondJetPt","secondJetPt",100,0.,100.);
     histos.h_secondJetEta_ = adaptor.template make<TH1F>("secondJetEta","secondJetEta",100,-5.,5.);
     histos.h_secondJetPhi_ = adaptor.template make<TH1F>("secondJetPhi","secondJetPhi",100,-1.1*M_PI,1.1*M_PI);
     histos.h_secondJetBDiscriminator_ = adaptor.template make<TH1F>("secondJetBDiscriminator","secondJetBDiscriminator",100,-10.,30.);

     histos.h_thirdJetPt_ = adaptor.template make<TH1F>("thirdJetPt","thirdJetPt",100,0.,80.);
     histos.h_thirdJetEta_ = adaptor.template make<TH1F>("thirdJetEta","thirdJetEta",100,-5.,5.);

     histos.h_jetsAveEta_ = adaptor.template make<TH1F>("jetsAveEta","jetsAveEta",100,-5.,5.);
     histos.h_jetsDeltaEta_ = adaptor.template make<TH1F>("jetsDeltaEta","jetsDeltaEta",100,-5.,5.);
     histos.h_jetsDeltaPhi_ = adaptor.template make<TH1F>("jetsDeltaPhi","jetsDeltaPhi",100,0.,1.1*M_PI);
     histos.h_jetsDeltaPt_ = adaptor.template make<TH1F>("jetsDeltaPt","jetsDeltaPt",100,0.,10.);

     histos.h_trackMultiplicity_ = adaptor.template make<TH1F>("trackMultiplicity","trackMultiplicity",20,0,20);
     histos.h_multiplicityHFPlus_ = adaptor.template make<TH1F>("multiplicityHFPlus","multiplicityHFPlus",20,0,20);
     histos.h_multiplicityHFMinus_ = adaptor.template make<TH1F>("multiplicityHFMinus","multiplicityHFMinus",20,0,20);

     histos.h_iEtaVsHFCountPlus_ = adaptor.template make<TH2F>("iEtaVsHFCountPlus","iEtaVsHFCountPlus",11,30,41,20,0,20);
     histos.h_iEtaVsHFCountMinus_ = adaptor.template make<TH2F>("iEtaVsHFCountMinus","iEtaVsHFCountMinus",11,30,41,20,0,20);

     histos.h_xiGenPlus_ = adaptor.template make<TH1F>("xiGenPlus","xiGenPlus",200,0.,1.);
     histos.h_xiGenMinus_ = adaptor.template make<TH1F>("xiGenMinus","xiGenMinus",200,0.,1.);
     histos.h_xiTowerPlus_ = adaptor.template make<TH1F>("xiTowerPlus","xiTowerPlus",200,0.,1.);
     histos.h_xiTowerMinus_ = adaptor.template make<TH1F>("xiTowerMinus","xiTowerMinus",200,0.,1.);
     histos.h_xiPlusFromJets_ = adaptor.template make<TH1F>("xiPlusFromJets","xiPlusFromJets",200,0.,1.);
     histos.h_xiMinusFromJets_ = adaptor.template make<TH1F>("xiMinusFromJets","xiMinusFromJets",200,0.,1.);
     histos.h_xiPlusFromPFCands_ = adaptor.template make<TH1F>("xiPlusFromPFCands","xiPlusFromPFCands",200,0.,1.);
     histos.h_xiMinusFromPFCands_ = adaptor.template make<TH1F>("xiMinusFromPFCands","xiMinusFromPFCands",200,0.,1.);

     histos.h_xiPlusVsxiGenPlus_ = adaptor.template make<TH2F>("xiPlusVsxiGenPlus","xiPlusVsxiGenPlus",100,0.,1.,100,0.,1.);
     histos.h_xiMinusVsxiGenMinus_ = adaptor.template make<TH2F>("xiMinusVsxiGenMinus","xiMinusVsxiGenMinus",100,0.,1.,100,0.,1.);

     histos.h_ResXiTowerPlus_ = adaptor.template make<TH1F>("ResXiTowerPlus","ResXiTowerPlus",100,-1.,1.);
     histos.h_ResXiTowerMinus_ = adaptor.template make<TH1F>("ResXiTowerMinus","ResXiTowerMinus",100,-1.,1.);
     histos.h_ResXiPlusFromJets_ = adaptor.template make<TH1F>("ResXiPlusFromJets","ResXiPlusFromJets",100,-1.,1.);
     histos.h_ResXiMinusFromJets_ = adaptor.template make<TH1F>("ResXiMinusFromJets","ResXiMinusFromJets",100,-1.,1.);
     histos.h_ResXiPlusFromPFCands_ = adaptor.template make<TH1F>("ResXiPlusFromPFCands","ResXiPlusFromPFCands",100,-1.,1.);
     histos.h_ResXiMinusFromPFCands_ = adaptor.template make<TH1F>("ResXiMinusFromPFCands","ResXiMinusFromPFCands",100,-1.,1.);

     histos.h_massDijets_ = adaptor.template make<TH1F>("massDijets","massDijets",200,-10.,400.);
     histos.h_massDijetsGen_ = adaptor.template make<TH1F>("massDijetsGen","massDijetsGen",200,-10.,400.);
     histos.h_missingMassFromXi_ = adaptor.template make<TH1F>("missingMassFromXi","missingMassFromXi",200,-10.,400.);
     histos.h_MxFromJets_ = adaptor.template make<TH1F>("MxFromJets","MxFromJets",200,-10.,400.);
     histos.h_RjjFromJets_ = adaptor.template make<TH1F>("RjjFromJets","RjjFromJets",200,-0.1,1.5);
     histos.h_RjjFromPFCands_ = adaptor.template make<TH1F>("RjjFromPFCands","RjjFromPFCands",200,-0.1,1.5);
     histos.h_ResMassDijets_ = adaptor.template make<TH1F>("ResMassDijets","ResMassDijets",100,-1.,1.);

     histos.h_EnergyVsEta_ = adaptor.template make<TH1F>("EnergyVsEta","EnergyVsEta",300,-15.,15.);

     histos.h_RjjFromJetsVsBDicriminator_ = adaptor.template make<TH2F>("RjjFromJetsVsBDicriminator","RjjFromJetsVsBDicriminator",200,-0.1,1.5,200,-10.,30.);
     histos.h_RjjFromPFCandsVsBDicriminator_ = adaptor.template make<TH2F>("RjjFromPFCandsVsBDicriminator","RjjFromPFCandsVsBDicriminator",200,-0.1,1.5,200,-10.,30.);
  }

} // namespace

#endif
