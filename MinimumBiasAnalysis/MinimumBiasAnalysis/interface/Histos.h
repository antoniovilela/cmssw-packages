#ifndef MinimumBiasAnalysis_Histos_h
#define MinimumBiasAnalysis_Histos_h

#include <cmath>
#include <map>

#include "TH1F.h"
#include "TH2F.h"

namespace minimumBiasAnalysis {
  // Histograms used for analysis
  typedef std::map<std::string,TH1F*> HistoMapTH1F;
  typedef std::map<std::string,TH2F*> HistoMapTH2F;

  // Book histos
  // FIXME: configure which histos to add
  template <class Adaptor>
  void bookHistos(HistoMapTH1F& histos, const Adaptor& adaptor){
     //pMakeTH1F = &Adaptor::template make<TH1F>;

     histos["nVertex"] = adaptor.template make<TH1F>("nVertex","Nr. of offline primary vertexes",10,0,10);
     histos["posXPrimVtx"] = adaptor.template make<TH1F>("posXPrimVtx","x position of primary vertexes",100,-1.,1.);
     histos["posYPrimVtx"] = adaptor.template make<TH1F>("posYPrimVtx","y position of primary vertexes",100,-1.,1.);
     histos["posZPrimVtx"] = adaptor.template make<TH1F>("posZPrimVtx","z position of primary vertexes",100,-30.,30.);
     histos["posRPrimVtx"] = adaptor.template make<TH1F>("posRPrimVtx","rho position of primary vertexes",100,0.,5.);
     histos["leadingJetPt"] = adaptor.template make<TH1F>("leadingJetPt","leadingJetPt",100,0.,100.);
     histos["leadingJetEta"] = adaptor.template make<TH1F>("leadingJetEta","leadingJetEta",100,-5.,5.);
     histos["leadingJetPhi"] = adaptor.template make<TH1F>("leadingJetPhi","leadingJetPhi",100,-1.1*M_PI,1.1*M_PI);
     histos["trackMultiplicity"] = adaptor.template make<TH1F>("trackMultiplicity","trackMultiplicity",20,0,20);
     histos["trackMultiplicityAssociatedToPV"] = adaptor.template make<TH1F>("trackMultiplicityAssociatedToPV","trackMultiplicityAssociatedToPV",20,0,20);
     histos["multiplicityHFPlus"] = adaptor.template make<TH1F>("multiplicityHFPlus","multiplicityHFPlus",20,0,20);
     histos["multiplicityHFMinus"] = adaptor.template make<TH1F>("multiplicityHFMinus","multiplicityHFMinus",20,0,20);
     histos["sumEnergyHFPlus"] = adaptor.template make<TH1F>("sumEnergyHFPlus","sumEnergyHFPlus",100,0.,100.);
     histos["sumEnergyHFMinus"] = adaptor.template make<TH1F>("sumEnergyHFMinus","sumEnergyHFMinus",100,0.,100.);
     histos["xiTowerPlus"] = adaptor.template make<TH1F>("xiTowerPlus","xiTowerPlus",200,0.,1.);
     histos["xiTowerMinus"] = adaptor.template make<TH1F>("xiTowerMinus","xiTowerMinus",200,0.,1.);
     histos["xiPlusFromJets"] = adaptor.template make<TH1F>("xiPlusFromJets","xiPlusFromJets",200,0.,1.);
     histos["xiMinusFromJets"] = adaptor.template make<TH1F>("xiMinusFromJets","xiMinusFromJets",200,0.,1.);
     histos["xiPlusFromTowers"] = adaptor.template make<TH1F>("xiPlusFromTowers","xiPlusFromTowers",200,0.,1.);
     histos["xiMinusFromTowers"] = adaptor.template make<TH1F>("xiMinusFromTowers","xiMinusFromTowers",200,0.,1.);
     histos["xiPlusFromPFCands"] = adaptor.template make<TH1F>("xiPlusFromPFCands","xiPlusFromPFCands",200,0.,1.);
     histos["xiMinusFromPFCands"] = adaptor.template make<TH1F>("xiMinusFromPFCands","xiMinusFromPFCands",200,0.,1.);
     histos["missingMassFromXiTower"] = adaptor.template make<TH1F>("missingMassFromXiTower","missingMassFromXiTower",200,-10.,800.);
     histos["missingMassFromXiFromJets"] = adaptor.template make<TH1F>("missingMassFromXiFromJets","missingMassFromXiFromJets",200,-10.,800.);
     histos["missingMassFromXiFromPFCands"] = adaptor.template make<TH1F>("missingMassFromXiFromPFCands","missingMassFromXiFromPFCands",200,-10.,800.);
     histos["MxFromJets"] = adaptor.template make<TH1F>("MxFromJets","MxFromJets",200,-10.,400.);
     histos["MxFromTowers"] = adaptor.template make<TH1F>("MxFromTowers","MxFromTowers",200,-10.,400.);
     histos["MxFromPFCands"] = adaptor.template make<TH1F>("MxFromPFCands","MxFromPFCands",200,-10.,400.);
     histos["towerEcalTime"] = adaptor.template make<TH1F>("towerEcalTime","towerEcalTime",200,-100.,100.);
     histos["towerHcalTime"] = adaptor.template make<TH1F>("towerHcalTime","towerHcalTime",200,-100.,100.);
     histos["energySumVsEcalTime"] = adaptor.template make<TH1F>("energySumVsEcalTime","energySumVsEcalTime",200,-100.,100.);
     histos["energySumVsHcalTime"] = adaptor.template make<TH1F>("energySumVsHcalTime","energySumVsHcalTime",200,-100.,100.);
     histos["sumET"] = adaptor.template make<TH1F>("sumET","sumET",200,0.,400.);
     histos["EPlusPzFromTowers"] = adaptor.template make<TH1F>("EPlusPzFromTowers","EPlusPzFromTowers",200,0.,600.);  
     histos["EMinusPzFromTowers"] = adaptor.template make<TH1F>("EMinusPzFromTowers","EMinusPzFromTowers",200,0.,600.);
     histos["EPlusPzFromPFCands"] = adaptor.template make<TH1F>("EPlusPzFromPFCands","EPlusPzFromPFCands",200,0.,600.);
     histos["EMinusPzFromPFCands"] = adaptor.template make<TH1F>("EMinusPzFromPFCands","EMinusPzFromPFCands",200,0.,600.);

     histos["MxGen"] = adaptor.template make<TH1F>("MxGen","MxGen",200,-10.,400.);
     histos["xiGenPlus"] = adaptor.template make<TH1F>("xiGenPlus","xiGenPlus",200,0.,1.);
     histos["xiGenMinus"] = adaptor.template make<TH1F>("xiGenMinus","xiGenMinus",200,0.,1.);
     histos["ResMx"] = adaptor.template make<TH1F>("ResMx","ResMx",100,-50.,50.);
     histos["ResXiPlus"] = adaptor.template make<TH1F>("ResXiPlus","ResXiPlus",100,-0.1,0.1);
     histos["ResXiMinus"] = adaptor.template make<TH1F>("ResXiMinus","ResXiMinus",100,-0.1,0.1);

     histos["BeamHaloId"] = adaptor.template make<TH1F>("BeamHaloId","BeamHaloId",2,0,2);
     histos["BeamHaloId"]->GetXaxis()->SetBinLabel(9,"BeamHaloLooseId");
     histos["BeamHaloId"]->GetXaxis()->SetBinLabel(10,"BeamHaloTightId");

     histos["HcalNoiseId"] = adaptor.template make<TH1F>("HcalNoiseId","HcalNoiseId",2,0,2);
     histos["HcalNoiseId"]->GetXaxis()->SetBinLabel(1,"LooseNoiseFilter");
     histos["HcalNoiseId"]->GetXaxis()->SetBinLabel(2,"TightNoiseFilter");

     histos["EventSelection"] = adaptor.template make<TH1F>("EventSelection","EventSelection",12,0,12);
     histos["EventSelection"]->GetXaxis()->SetBinLabel(1,"All");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(2,"ProcessIdOrRunSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(3,"TriggerSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(4,"HcalNoise");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(5,"BeamHaloId");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(6,"GoodVertexFilter");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(7,"HighQualityTracks");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(8,"VertexSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(9,"SumETSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(10,"HcalTimingSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(11,"MxSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(12,"HFSelection");
  }

  template <class Adaptor>
  void bookHistos(HistoMapTH2F& histos, const Adaptor& adaptor){
  }

} // namespace

#endif