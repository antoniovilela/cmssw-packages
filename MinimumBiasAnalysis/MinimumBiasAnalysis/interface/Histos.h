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

     /*double etaBinsHCALBoundaries[] = {0.000, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696,
                                       0.783, 0.870, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392,
                                       1.479, 1.566, 1.653, 1.740, 1.830, 1.930, 2.043, 2.172, 2.322,
                                       2.500, 2.650, 2.868, 3.000,
                                       3.152, 3.327, 3.503, 3.677, 3.853, 4.027, 4.204, 4.377, 4.552,
                                       4.730, 4.903, 5.205}; // 41 bins*/
     float etaBinsHCALBoundaries[] = {-5.205, -4.903, -4.730,
                                       -4.552, -4.377, -4.204, -4.027, -3.853, -3.677, -3.503, -3.327, -3.152,
                                       -3.000, -2.868, -2.650, -2.500, 
                                       -2.322, -2.172, -2.043, -1.930, -1.830, -1.740, -1.653, -1.566, -1.479,
                                       -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.870, -0.783, 
                                       -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087,
                                       0.000, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696,
                                       0.783, 0.870, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392,
                                       1.479, 1.566, 1.653, 1.740, 1.830, 1.930, 2.043, 2.172, 2.322,
                                       2.500, 2.650, 2.868, 3.000,
                                       3.152, 3.327, 3.503, 3.677, 3.853, 4.027, 4.204, 4.377, 4.552,
                                       4.730, 4.903, 5.205}; // 41 + 41 bins

     float minEVarBin = 0.; 
     float binningEPlusPz[]={minEVarBin,5.,10.,15.,20.,25.,30.,40.,50.,60.,70.,80.,90.,
                               100.,120.,140.,160.,180.,200.,225.,250.,
                               275.,300.,350.,400.}; // 24 bins
     float binningESumHF[]={minEVarBin,5.,10.,15.,20.,25.,30.,40.,50.,60.,70.,80.,90.,
                               100.,120.,140.,160.,180.,200.}; // 18 bins
     int nBinsLogXi = 7;
     float binningLogXi[] = {-5.,-4.5,-4.,-3.5,-3.,-2.5,-2.0,0.}; // 7 bins 

     histos["nVertex"] = adaptor.template make<TH1F>("nVertex","Nr. of offline primary vertexes",10,0,10);
     histos["posXPrimVtx"] = adaptor.template make<TH1F>("posXPrimVtx","x position of primary vertexes",100,-1.,1.);
     histos["posYPrimVtx"] = adaptor.template make<TH1F>("posYPrimVtx","y position of primary vertexes",100,-1.,1.);
     histos["posZPrimVtx"] = adaptor.template make<TH1F>("posZPrimVtx","z position of primary vertexes",100,-30.,30.);
     histos["posRPrimVtx"] = adaptor.template make<TH1F>("posRPrimVtx","rho position of primary vertexes",100,0.,5.);
     histos["multiplicityTracks"] = adaptor.template make<TH1F>("multiplicityTracks","multiplicityTracks",40,0,40);
     histos["sumPtTracks"] = adaptor.template make<TH1F>("sumPtTracks","sumPtTracks",100,0.,50.);
     histos["leadingJetPt"] = adaptor.template make<TH1F>("leadingJetPt","leadingJetPt",100,0.,100.);
     histos["leadingJetEta"] = adaptor.template make<TH1F>("leadingJetEta","leadingJetEta",100,-5.,5.);
     histos["leadingJetPhi"] = adaptor.template make<TH1F>("leadingJetPhi","leadingJetPhi",100,-1.1*M_PI,1.1*M_PI);
     histos["trackMultiplicity"] = adaptor.template make<TH1F>("trackMultiplicity","trackMultiplicity",20,0,20);
     histos["trackMultiplicityAssociatedToPV"] = adaptor.template make<TH1F>("trackMultiplicityAssociatedToPV","trackMultiplicityAssociatedToPV",20,0,20);

     histos["multiplicityHEPlus"] = adaptor.template make<TH1F>("multiplicityHEPlus","multiplicityHEPlus",20,0,20);
     histos["multiplicityHEMinus"] = adaptor.template make<TH1F>("multiplicityHEMinus","multiplicityHEMinus",20,0,20);
     histos["sumEnergyHEPlus"] = adaptor.template make<TH1F>("sumEnergyHEPlus","sumEnergyHEPlus",100,0.,100.);
     histos["sumEnergyHEMinus"] = adaptor.template make<TH1F>("sumEnergyHEMinus","sumEnergyHEMinus",100,0.,100.);
     histos["multiplicityHFPlus"] = adaptor.template make<TH1F>("multiplicityHFPlus","multiplicityHFPlus",20,0,20);
     histos["multiplicityHFMinus"] = adaptor.template make<TH1F>("multiplicityHFMinus","multiplicityHFMinus",20,0,20);
     histos["sumEnergyHFPlus"] = adaptor.template make<TH1F>("sumEnergyHFPlus","sumEnergyHFPlus",100,0.,100.);
     histos["sumEnergyHFMinus"] = adaptor.template make<TH1F>("sumEnergyHFMinus","sumEnergyHFMinus",100,0.,100.);
     histos["sumEnergyHFPlusVarBin"] = adaptor.template make<TH1F>("sumEnergyHFPlusVarBin","sumEnergyHFPlusVarBin",18,binningESumHF);
     histos["sumEnergyHFMinusVarBin"] = adaptor.template make<TH1F>("sumEnergyHFMinusVarBin","sumEnergyHFMinusVarBin",18,binningESumHF);
     histos["multiplicityHFPlusVarBin"] = adaptor.template make<TH1F>("multiplicityHFPlusVarBin","multiplicityHFPlusVarBin",20,-0.5,19.5);
     histos["multiplicityHFMinusVarBin"] = adaptor.template make<TH1F>("multiplicityHFMinusVarBin","multiplicityHFMinusVarBin",20,-0.5,19.5);
     histos["sumEnergyCASTOR"] = adaptor.template make<TH1F>("sumEnergyCASTOR","sumEnergyHFPlus",800,0.,4000.);

     histos["xiTowerPlus"] = adaptor.template make<TH1F>("xiTowerPlus","xiTowerPlus",200,0.,1.);
     histos["xiTowerMinus"] = adaptor.template make<TH1F>("xiTowerMinus","xiTowerMinus",200,0.,1.);
     histos["xiPlusFromJets"] = adaptor.template make<TH1F>("xiPlusFromJets","xiPlusFromJets",200,0.,1.);
     histos["xiMinusFromJets"] = adaptor.template make<TH1F>("xiMinusFromJets","xiMinusFromJets",200,0.,1.);
     histos["xiPlusFromTowers"] = adaptor.template make<TH1F>("xiPlusFromTowers","xiPlusFromTowers",200,0.,1.);
     histos["xiMinusFromTowers"] = adaptor.template make<TH1F>("xiMinusFromTowers","xiMinusFromTowers",200,0.,1.);
     histos["xiPlusFromPFCands"] = adaptor.template make<TH1F>("xiPlusFromPFCands","xiPlusFromPFCands",200,0.,1.);
     histos["xiMinusFromPFCands"] = adaptor.template make<TH1F>("xiMinusFromPFCands","xiMinusFromPFCands",200,0.,1.);

     histos["missingMassFromXiTower"] = adaptor.template make<TH1F>("missingMassFromXiTower","missingMassFromXiTower",200,-10.,800.);
     histos["missingMassFromXiFromTowers"] = adaptor.template make<TH1F>("missingMassFromXiFromTowers","missingMassFromXiFromTowers",200,-10.,800.);
     histos["missingMassFromXiFromJets"] = adaptor.template make<TH1F>("missingMassFromXiFromJets","missingMassFromXiFromJets",200,-10.,800.);
     histos["missingMassFromXiFromPFCands"] = adaptor.template make<TH1F>("missingMassFromXiFromPFCands","missingMassFromXiFromPFCands",200,-10.,800.);
     histos["MxFromJets"] = adaptor.template make<TH1F>("MxFromJets","MxFromJets",200,-10.,400.);
     histos["MxFromTowers"] = adaptor.template make<TH1F>("MxFromTowers","MxFromTowers",200,-10.,400.);
     histos["MxFromPFCands"] = adaptor.template make<TH1F>("MxFromPFCands","MxFromPFCands",200,-10.,400.);

     histos["etaMaxFromPFCands"] = adaptor.template make<TH1F>("etaMaxFromPFCands","etaMaxFromPFCands",200,-7.,7.);
     histos["etaMinFromPFCands"] = adaptor.template make<TH1F>("etaMinFromPFCands","etaMinFromPFCands",200,-7.,7.);
     histos["etaMaxFromPFCandsVarBin"] = adaptor.template make<TH1F>("etaMaxFromPFCandsVarBin","etaMaxFromPFCandsVarBin",82,etaBinsHCALBoundaries);
     histos["etaMinFromPFCandsVarBin"] = adaptor.template make<TH1F>("etaMinFromPFCandsVarBin","etaMinFromPFCandsVarBin",82,etaBinsHCALBoundaries); 

     /*histos["towerEcalTime"] = adaptor.template make<TH1F>("towerEcalTime","towerEcalTime",200,-100.,100.);
     histos["towerHcalTime"] = adaptor.template make<TH1F>("towerHcalTime","towerHcalTime",200,-100.,100.);
     histos["energySumVsEcalTime"] = adaptor.template make<TH1F>("energySumVsEcalTime","energySumVsEcalTime",200,-100.,100.);
     histos["energySumVsHcalTime"] = adaptor.template make<TH1F>("energySumVsHcalTime","energySumVsHcalTime",200,-100.,100.);*/
     histos["sumET"] = adaptor.template make<TH1F>("sumET","sumET",200,0.,100.);
     histos["EPlusPzFromTowers"] = adaptor.template make<TH1F>("EPlusPzFromTowers","EPlusPzFromTowers",200,0.,600.);  
     histos["EMinusPzFromTowers"] = adaptor.template make<TH1F>("EMinusPzFromTowers","EMinusPzFromTowers",200,0.,600.);
     histos["EPlusPzFromPFCands"] = adaptor.template make<TH1F>("EPlusPzFromPFCands","EPlusPzFromPFCands",200,0.,600.);
     histos["EMinusPzFromPFCands"] = adaptor.template make<TH1F>("EMinusPzFromPFCands","EMinusPzFromPFCands",200,0.,600.);
     histos["EPlusPzFromTowersVarBin"] = adaptor.template make<TH1F>("EPlusPzFromTowersVarBin","EPlusPzFromTowersVarBin",24,binningEPlusPz);
     histos["EMinusPzFromTowersVarBin"] = adaptor.template make<TH1F>("EMinusPzFromTowersVarBin","EMinusPzFromTowersVarBin",24,binningEPlusPz);

     histos["MxGen"] = adaptor.template make<TH1F>("MxGen","MxGen",200,-10.,400.);
     histos["xiGenPlus"] = adaptor.template make<TH1F>("xiGenPlus","xiGenPlus",500,0.,1.);
     histos["xiGenMinus"] = adaptor.template make<TH1F>("xiGenMinus","xiGenMinus",500,0.,1.);
     histos["etaMaxGen"] = adaptor.template make<TH1F>("etaMaxGen","etaMaxGen",200,-10.,10.);
     histos["etaMinGen"] = adaptor.template make<TH1F>("etaMinGen","etaMinGen",200,-10.,10.);

     histos["logXiPlusFromPFCands"] = adaptor.template make<TH1F>("logXiPlusFromPFCands","logXiPlusFromPFCands",200,-5.,0.);
     histos["logXiMinusFromPFCands"] = adaptor.template make<TH1F>("logXiMinusFromPFCands","logXiMinusFromPFCands",200,-5.,0.);
     histos["logXiGenPlus"] = adaptor.template make<TH1F>("logXiGenPlus","logXiGenPlus",200,-5.,0.);
     histos["logXiGenMinus"] = adaptor.template make<TH1F>("logXiGenMinus","logXiGenMinus",200,-5.,0.);
     histos["logXiFromPFCandsANDXiGenPlus"] = adaptor.template make<TH1F>("logXiFromPFCandsANDXiGenPlus","logXiFromPFCandsANDXiGenPlus",200,-5.,0.);
     histos["logXiFromPFCandsANDXiGenMinus"] = adaptor.template make<TH1F>("logXiFromPFCandsANDXiGenMinus","logXiFromPFCandsANDXiGenMinus",200,-5.,0.); 
     histos["logXiPlusFromPFCandsVarBin"] = adaptor.template make<TH1F>("logXiPlusFromPFCandsVarBin","logXiPlusFromPFCandsVarBin",nBinsLogXi,binningLogXi);
     histos["logXiMinusFromPFCandsVarBin"] = adaptor.template make<TH1F>("logXiMinusFromPFCandsVarBin","logXiMinusFromPFCandsVarBin",nBinsLogXi,binningLogXi);
     histos["logXiGenPlusVarBin"] = adaptor.template make<TH1F>("logXiGenPlusVarBin","logXiGenPlusVarBin",nBinsLogXi,binningLogXi);
     histos["logXiGenMinusVarBin"] = adaptor.template make<TH1F>("logXiGenMinusVarBin","logXiGenMinusVarBin",nBinsLogXi,binningLogXi);
     histos["logXiFromPFCandsANDXiGenPlusVarBin"] = adaptor.template make<TH1F>("logXiFromPFCandsANDXiGenPlusVarBin","logXiFromPFCandsANDXiGenPlusVarBin",nBinsLogXi,binningLogXi);
     histos["logXiFromPFCandsANDXiGenMinusVarBin"] = adaptor.template make<TH1F>("logXiFromPFCandsANDXiGenMinusVarBin","logXiFromPFCandsANDXiGenMinusVarBin",nBinsLogXi,binningLogXi);
 
     histos["multiplicityTracksGen"] = adaptor.template make<TH1F>("multiplicityTracksGen","multiplicityTracksGen",40,0,40);
     histos["sumPtTracksGen"] = adaptor.template make<TH1F>("sumPtTracksGen","sumPtTracksGen",100,0.,50.);
     histos["sumEnergyHEPlusGen"] = adaptor.template make<TH1F>("sumEnergyHEPlusGen","sumEnergyHEPlusGen",100,0.,100.);
     histos["sumEnergyHEMinusGen"] = adaptor.template make<TH1F>("sumEnergyHEMinusGen","sumEnergyHEMinusGen",100,0.,100.);
     histos["sumEnergyHFPlusGen"] = adaptor.template make<TH1F>("sumEnergyHFPlusGen","sumEnergyHFPlusGen",100,0.,100.);
     histos["sumEnergyHFMinusGen"] = adaptor.template make<TH1F>("sumEnergyHFMinusGen","sumEnergyHFMinusGen",100,0.,100.);
     /*histos["sumEnergyHFPlusGen_Ntrk0_5"] = adaptor.template make<TH1F>("sumEnergyHFPlusGen_Ntrk0_5","sumEnergyHFPlusGen_Ntrk0_5",100,0.,100.);
     histos["sumEnergyHFPlusGen_Ntrk6_10"] = adaptor.template make<TH1F>("sumEnergyHFPlusGen_Ntrk6_10","sumEnergyHFPlusGen_Ntrk6_10",100,0.,100.);
     histos["sumEnergyHFPlusGen_Ntrk11_15"] = adaptor.template make<TH1F>("sumEnergyHFPlusGen_Ntrk11_15","sumEnergyHFPlusGen_Ntrk11_15",100,0.,100.);
     histos["sumEnergyHFPlusGen_Ntrk16_25"] = adaptor.template make<TH1F>("sumEnergyHFPlusGen_Ntrk16_25","sumEnergyHFPlusGen_Ntrk16_25",100,0.,100.);
     histos["sumEnergyHFPlusGen_Ntrk26_100"] = adaptor.template make<TH1F>("sumEnergyHFPlusGen_Ntrk26_100","sumEnergyHFPlusGen_Ntrk26_100",100,0.,100.);*/
     histos["ResMxFromTowers"] = adaptor.template make<TH1F>("ResMxFromTowers","ResMxFromTowers",200,-2.5,2.5);
     histos["ResMxFromPFCands"] = adaptor.template make<TH1F>("ResMxFromPFCands","ResMxFromPFCands",200,-2.5,2.5);
     histos["ResXiPlusFromTowers"] = adaptor.template make<TH1F>("ResXiPlusFromTowers","ResXiPlusFromTowers",200,-2.0,2.0);
     histos["ResXiMinusFromTowers"] = adaptor.template make<TH1F>("ResXiMinusFromTowers","ResXiMinusFromTowers",200,-2.0,2.0);
     histos["ResXiPlusFromPFCands"] = adaptor.template make<TH1F>("ResXiPlusFromPFCands","ResXiPlusFromPFCands",200,-2.0,2.0);
     histos["ResXiMinusFromPFCands"] = adaptor.template make<TH1F>("ResXiMinusFromPFCands","ResXiMinusFromPFCands",200,-2.0,2.0);

     histos["BeamHaloId"] = adaptor.template make<TH1F>("BeamHaloId","BeamHaloId",2,0,2);
     histos["BeamHaloId"]->GetXaxis()->SetBinLabel(1,"BeamHaloLooseId");
     histos["BeamHaloId"]->GetXaxis()->SetBinLabel(2,"BeamHaloTightId");

     histos["HcalNoiseId"] = adaptor.template make<TH1F>("HcalNoiseId","HcalNoiseId",2,0,2);
     histos["HcalNoiseId"]->GetXaxis()->SetBinLabel(1,"LooseNoiseFilter");
     histos["HcalNoiseId"]->GetXaxis()->SetBinLabel(2,"TightNoiseFilter");

     histos["EventSelection"] = adaptor.template make<TH1F>("EventSelection","EventSelection",15,0,15);
     histos["EventSelection"]->GetXaxis()->SetBinLabel(1,"All");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(2,"ProcessIdOrRunSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(3,"TriggerSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(4,"HcalNoise");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(5,"BeamHaloId");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(6,"GoodVertexFilter");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(7,"HighQualityTracks");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(8,"VertexSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(9,"HCALSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(10,"SumETSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(11,"MxSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(12,"XiPlusSelection"); 
     histos["EventSelection"]->GetXaxis()->SetBinLabel(13,"XiMinusSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(14,"EtaMaxSelection");
     histos["EventSelection"]->GetXaxis()->SetBinLabel(15,"EtaMinSelection");
  }

  template <class Adaptor>
  void bookHistos(HistoMapTH2F& histos, const Adaptor& adaptor){
     histos["multiplicityHFVsiEtaPlus"] = adaptor.template make<TH2F>("multiplicityHFVsiEtaPlus","multiplicityHFVsiEtaPlus",13,29,42,20,0,20);
     histos["multiplicityHFVsiEtaMinus"] = adaptor.template make<TH2F>("multiplicityHFVsiEtaMinus","multiplicityHFVsiEtaMinus",13,29,42,20,0,20);
     histos["sumEnergyHFVsiEtaPlus"] = adaptor.template make<TH2F>("sumEnergyHFVsiEtaPlus","sumEnergyHFVsiEtaPlus",13,29,42,100,0.,100.);
     histos["sumEnergyHFVsiEtaMinus"] = adaptor.template make<TH2F>("sumEnergyHFVsiEtaMinus","sumEnergyHFVsiEtaMinus",13,29,42,100,0.,100.);
     histos["sumETHFVsiEtaPlus"] = adaptor.template make<TH2F>("sumETHFVsiEtaPlus","sumETHFVsiEtaPlus",13,29,42,100,0.,50.);
     histos["sumETHFVsiEtaMinus"] = adaptor.template make<TH2F>("sumETHFVsiEtaMinus","sumETHFVsiEtaMinus",13,29,42,100,0.,50.);

     histos["multiplicityHFVsHEPlus"] = adaptor.template make<TH2F>("multiplicityHFvsHEPlus","multiplicityHFvsHEPlus",20,0,20,20,0,20);
     histos["multiplicityHFVsHEMinus"] = adaptor.template make<TH2F>("multiplicityHFvsHEMinus","multiplicityHFvsHEMinus",20,0,20,20,0,20);
     
     histos["multiplicityTracksVsGen"] = adaptor.template make<TH2F>("multiplicityTracksVsGen","multiplicityTracksVsGen",40,0,40,40,0,40);
     histos["sumPtTracksVsGen"] = adaptor.template make<TH2F>("sumPtTracksVsGen","sumPtTracksVsGen",100,0.,50.,100,0.,50.);

     histos["xiFromTowersVsxiGenPlus"] = adaptor.template make<TH2F>("xiFromTowersVsxiGenPlus","xiFromTowersVsxiGenPlus",500,0.,0.2,100,0.,0.2);
     histos["xiFromTowersVsxiGenMinus"] = adaptor.template make<TH2F>("xiFromTowersVsxiGenMinus","xiFromTowersVsxiGenMinus",500,0.,0.2,100,0.,0.2);
     histos["xiFromPFCandsVsxiGenPlus"] = adaptor.template make<TH2F>("xiFromPFCandsVsxiGenPlus","xiFromPFCandsVsxiGenPlus",500,0.,0.2,100,0.,0.2);
     histos["xiFromPFCandsVsxiGenMinus"] = adaptor.template make<TH2F>("xiFromPFCandsVsxiGenMinus","xiFromPFCandsVsxiGenMinus",500,0.,0.2,100,0.,0.2);
     histos["xiFromTowersVslogXiGenPlus"] = adaptor.template make<TH2F>("xiFromTowersVslogXiGenPlus","xiFromTowersVslogXiGenPlus",200,-5.,0.,100,0.,0.2);
     histos["xiFromTowersVslogXiGenMinus"] = adaptor.template make<TH2F>("xiFromTowersVslogXiGenMinus","xiFromTowersVslogXiGenMinus",200,-5.,0.,100,0.,0.2);
     histos["xiFromPFCandsVslogXiGenPlus"] = adaptor.template make<TH2F>("xiFromPFCandsVslogXiGenPlus","xiFromPFCandsVslogXiGenPlus",200,-5.,0.,100,0.,0.2);
     histos["xiFromPFCandsVslogXiGenMinus"] = adaptor.template make<TH2F>("xiFromPFCandsVslogXiGenMinus","xiFromPFCandsVslogXiGenMinus",200,-5.,0.,100,0.,0.2);
     histos["logXiFromPFCandsVslogXiGenPlus"] = adaptor.template make<TH2F>("logXiFromPFCandsVslogXiGenPlus","logXiFromPFCandsVslogXiGenPlus",200,-5.,0.,200,-5.,0.);
     histos["logXiFromPFCandsVslogXiGenMinus"] = adaptor.template make<TH2F>("logXiFromPFCandsVslogXiGenMinus","logXiFromPFCandsVslogXiGenMinus",200,-5.,0.,200,-5.,0.);

     histos["etaMaxFromPFCandsVsetaMaxGen"] = adaptor.template make<TH2F>("etaMaxFromPFCandsVsetaMaxGen","etaMaxFromPFCandsVsetaMaxGen",200,-10.,10.,200,-7.,7.);
     histos["etaMinFromPFCandsVsetaMinGen"] = adaptor.template make<TH2F>("etaMinFromPFCandsVsetaMinGen","etaMinFromPFCandsVsetaMinGen",200,-10.,10.,200,-7.,7.);
 
  }

} // namespace

#endif
