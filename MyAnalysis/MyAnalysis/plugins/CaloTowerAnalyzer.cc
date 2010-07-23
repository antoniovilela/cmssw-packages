// user include files
#include "CaloTowerAnalyzer.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoCaloTowerCandidate.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"

#include "Geometry/CaloTopology/interface/HcalTopology.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandGaussQ.h"

#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h" 
#include "TTree.h"

using namespace reco;
 
CaloTowerAnalyzer::CaloTowerAnalyzer(const edm::ParameterSet& conf):
  calotowersTag_(conf.getParameter<edm::InputTag>("CaloTowersTag")),
  accessRecHits_(conf.getUntrackedParameter<bool>("AccessRecHits",false)),
  //excludeHotTowers_(conf.getParameter<bool>("ExcludeHotTowers")), 
  nThresholdIter_(conf.getParameter<unsigned int>("NumberOfTresholds")),
  eThresholdHBMin_(conf.getParameter<double>("TowerEnergyTresholdHBMin")),
  eThresholdHBMax_(conf.getParameter<double>("TowerEnergyTresholdHBMax")),
  eThresholdHEMin_(conf.getParameter<double>("TowerEnergyTresholdHEMin")),
  eThresholdHEMax_(conf.getParameter<double>("TowerEnergyTresholdHEMax")),
  eThresholdHFMin_(conf.getParameter<double>("TowerEnergyTresholdHFMin")),
  eThresholdHFMax_(conf.getParameter<double>("TowerEnergyTresholdHFMax")),
  nBinsHB_(conf.getUntrackedParameter<int>("NBinsHB",20)),
  nBinsHE_(conf.getUntrackedParameter<int>("NBinsHE",20)),
  nBinsHF_(conf.getUntrackedParameter<int>("NBinsHF",20)),	
  reweightHFTower_(conf.getParameter<bool>("ReweightHFTower")),
  applyEnergyOffset_(conf.getParameter<bool>("ApplyEnergyOffset")),
  eThresholdHB_(std::vector<double>()),
  eThresholdHE_(std::vector<double>()),
  eThresholdHF_(std::vector<double>()) {

  if(accessRecHits_) hfRecHitsTag_ = conf.getUntrackedParameter<edm::InputTag>("HFRecHitsLabel",edm::InputTag("hfreco"));

  // reweightHistoName_[0] --> file name
  // reweightHistoName_[1] --> histo path in file
  if(reweightHFTower_){
     reweightHistoName_ = conf.getParameter<std::vector<std::string> >("ReweightHistoName");

     // Access and close file; keep hard copy of histo
     edm::LogVerbatim("Analysis") << "Accessing file " << reweightHistoName_[0] << " histo " << reweightHistoName_[1];
     TFile file(reweightHistoName_[0].c_str(),"read");
     TH1F* histo = static_cast<TH1F*>(file.Get(reweightHistoName_[1].c_str())); 
     reweightHisto_ = *histo;
  }
 
  if(applyEnergyOffset_){
     edm::Service<edm::RandomNumberGenerator> rng;
     CLHEP::HepRandomEngine& engine = rng->getEngine();
     gauss_ = std::auto_ptr<CLHEP::RandGaussQ>(new CLHEP::RandGaussQ(engine));
     edm::ParameterSet energyOffsetPSet = conf.getParameter<edm::ParameterSet>("EnergyOffsetParameters");
     /*sigmaShort_ = energyOffsetPSet.getParameter<double>("sigmaShort");
     sigmaLong_ = energyOffsetPSet.getParameter<double>("sigmaLong");
     nIncrements_ = energyOffsetPSet.getParameter<int>("nIncrements");*/
     meanHFEnergy_ = energyOffsetPSet.getParameter<double>("meanHFEnergy");
     sigmaHFEnergy_ = energyOffsetPSet.getParameter<double>("sigmaHFEnergy");
     edm::LogVerbatim("Analysis") << "Using \n"
                                  << "  meanHFEnergy=    " << meanHFEnergy_
                                  << "\n  sigmaHFEnergy=   " << sigmaHFEnergy_;
  }  
}  
  
CaloTowerAnalyzer::~CaloTowerAnalyzer()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

void CaloTowerAnalyzer::beginJob(){

  edm::Service<TFileService> fs;
  TH1::SetDefaultSumw2(true);

  histosTH1F_["NEvents"] = fs->make<TH1F>("NEvents","NEvents",1,0,1);

  char hname[50];
  for(unsigned int i = 0; i < nThresholdIter_; ++i){
     eThresholdHB_.push_back(eThresholdHBMin_ + i*((eThresholdHBMax_ - eThresholdHBMin_)/((double)nThresholdIter_))); 
     eThresholdHE_.push_back(eThresholdHEMin_ + i*((eThresholdHEMax_ - eThresholdHEMin_)/((double)nThresholdIter_)));
     eThresholdHF_.push_back(eThresholdHFMin_ + i*((eThresholdHFMax_ - eThresholdHFMin_)/((double)nThresholdIter_)));

     edm::LogVerbatim("Analysis") << "Threshold HB" << i << ": " << eThresholdHB_[i] << "\n"
                                  << "Threshold HE" << i << ": " << eThresholdHE_[i] << "\n"
                                  << "Threshold HF" << i << ": " << eThresholdHF_[i];
     // HB
     sprintf(hname,"nHBPlus_%d",i);
     histosNHBPlus_.push_back(fs->make<TH1F>(hname,"Towers mult. HB plus",nBinsHB_,0,nBinsHB_));
     sprintf(hname,"nHBMinus_%d",i);
     histosNHBMinus_.push_back(fs->make<TH1F>(hname,"Towers mult. HB minus",nBinsHB_,0,nBinsHB_));
     sprintf(hname,"sumEHBplus_%d",i);
     histosSumEHBPlus_.push_back(fs->make<TH1F>(hname,"Energy sum HB plus",100,0.,200.));
     sprintf(hname,"sumEHBminus_%d",i);
     histosSumEHBMinus_.push_back(fs->make<TH1F>(hname,"Energy sum HB minus",100,0.,200.));
     // HE
     sprintf(hname,"nHEplus_%d",i);
     histosNHEPlus_.push_back(fs->make<TH1F>(hname,"Towers mult. HE plus",nBinsHE_,0,nBinsHE_));
     sprintf(hname,"nHEminus_%d",i);
     histosNHEMinus_.push_back(fs->make<TH1F>(hname,"Towers mult. HE minus",nBinsHE_,0,nBinsHE_));
     sprintf(hname,"sumEHEplus_%d",i);
     histosSumEHEPlus_.push_back(fs->make<TH1F>(hname,"Energy sum HE plus",100,0.,200.));
     sprintf(hname,"sumEHEminus_%d",i);
     histosSumEHEMinus_.push_back(fs->make<TH1F>(hname,"Energy sum HE minus",100,0.,200.));
     // HF
     sprintf(hname,"nHFplus_%d",i);
     histosNHFPlus_.push_back(fs->make<TH1F>(hname,"Towers mult. HF plus",nBinsHF_,0,nBinsHF_));
     sprintf(hname,"nHFminus_%d",i);
     histosNHFMinus_.push_back(fs->make<TH1F>(hname,"Towers mult. HF minus",nBinsHF_,0,nBinsHF_));	
     sprintf(hname,"sumEHFplus_%d",i);
     histosSumEHFPlus_.push_back(fs->make<TH1F>(hname,"Energy sum HF plus",100,0.,200.));   
     sprintf(hname,"sumEHFminus_%d",i);
     histosSumEHFMinus_.push_back(fs->make<TH1F>(hname,"Energy sum HF minus",100,0.,200.));
     /*sprintf(hname,"nhflow_%d",i);
     histosnhflow_.push_back(fs->make<TH1F>(hname,"Towers mult. HF low",nBinsHF_,0,nBinsHF_));
     sprintf(hname,"nhfhigh_%d",i);
     histosnhfhigh_.push_back(fs->make<TH1F>(hname,"Towers mult. HF high",nBinsHF_,0,nBinsHF_));*/
  }

  histosTH1F_["energyHB"] = fs->make<TH1F>("energyHB","Tower Energy HB",200,0.,15.0);
  histosTH1F_["energyHBplus"] = fs->make<TH1F>("energyHBplus","Tower Energy HB-plus",200,0.,15.0);
  histosTH1F_["energyHBminus"] = fs->make<TH1F>("energyHBminus","Tower Energy HB-minus",200,0.,15.0);
  histosTH1F_["emEnergyHB"] = fs->make<TH1F>("emEnergyHB","EM Tower Energy HB",200,0.,15.0);
  histosTH1F_["hadEnergyHB"] = fs->make<TH1F>("hadEnergyHB","HAD Tower Energy HB",200,0.,15.0);
  histosTH1F_["emFractionHB"] = fs->make<TH1F>("emFractionHB","EM energy fraction HB",200,0.,1.1);
  histosTH1F_["energyHE"] = fs->make<TH1F>("energyHE","Tower Energy HE",200,0.,15.0);
  histosTH1F_["energyHEplus"] = fs->make<TH1F>("energyHEplus","Tower Energy HE-plus",200,0.,15.0);
  histosTH1F_["energyHEminus"] = fs->make<TH1F>("energyHEminus","Tower Energy HE-minus",200,0.,15.0);
  histosTH1F_["emEnergyHE"] = fs->make<TH1F>("emEnergyHE","EM Tower Energy HE",200,0.,15.0);
  histosTH1F_["hadEnergyHE"] = fs->make<TH1F>("hadEnergyHE","HAD Tower Energy HE",200,0.,15.0);
  histosTH1F_["emFractionHE"] = fs->make<TH1F>("emFractionHE","EM energy fraction HE",200,0.,1.1);
  histosTH1F_["energyHF"] = fs->make<TH1F>("energyHF","Tower Energy HF",200,0.,15.0);
  histosTH1F_["energyHFplus"] = fs->make<TH1F>("energyHFplus","Tower Energy HF-plus",200,0.,15.0);
  histosTH1F_["energyHFminus"] = fs->make<TH1F>("energyHFminus","Tower Energy HF-minus",200,0.,15.0);
  histosTH1F_["emEnergyHF"] = fs->make<TH1F>("emEnergyHF","EM Tower Energy HF",200,0.,15.0); 
  histosTH1F_["hadEnergyHF"] = fs->make<TH1F>("hadEnergyHF","HAD Tower Energy HF",200,0.,15.0);
  histosTH1F_["emFractionHF"] = fs->make<TH1F>("emFractionHF","EM energy fraction HF",200,0.,1.1);

  histos_.histenergyvseta_ = fs->make<TH1F>("energyvseta","Tower Energy vs eta",100,-5.1,5.1);
  histos_.histetavsphi_ = fs->make<TH2F>("etavsphi","etavsphi",100,-5.,5.,100,-3.1416,3.1416);
  histos_.histietavsiphi_ = fs->make<TH2F>("ietavsiphi","ietavsiphi",85,-42,43,75,0,75);
  histos_.histetavsphiweighted_ = fs->make<TH2F>("etavsphiweighted","etavsphiweighted",100,-5.,5.,100,-3.1416,3.1416);
  histos_.histietavsiphiweighted_ = fs->make<TH2F>("ietavsiphiweighted","ietavsiphiweighted",85,-42,43,75,0,75);
	
  histos_.histenergyvsetaHF_ = fs->make<TH1F>("energyvsetaHF","Tower Energy vs eta",100,-5.1,5.1);
  histos_.histetavsphiHF_ = fs->make<TH2F>("etavsphiHF","Tower Energy vs eta x phi",100,-5.,5.,100,-3.1416,3.1416);
  histos_.histetavsphiHFweighted_ = fs->make<TH2F>("etavsphiHFweighted","etavsphiHFweighted",100,-5.,5.,100,-3.1416,3.1416);
  
  histos_.histenergyvsetaHFMax_ = fs->make<TH1F>("energyvsetaHFMax","Tower Energy vs eta",100,-5.1,5.1);
  histos_.histetavsphiHFMax_ = fs->make<TH2F>("etavsphiHFMax","Tower Energy vs eta x phi",100,-5.,5.,100,-3.1416,3.1416);
  	
  histos_.histhbtiming_ = fs->make<TH1F>("hbtiming","HBHE Timing",10,-0.5,9.5);
  histos_.histhfhitenergy_ = fs->make<TH1F>("hfhitenergy","HF RecHit energy",100,-2.,20.);
  histos_.histhflongenergy_ = fs->make<TH1F>("hflongenergy","HF RecHit Long Fiber energy",100,-2.,20.);
  histos_.histhfshortenergy_ = fs->make<TH1F>("hfshortenergy","HF RecHit Short Fiber energy",100,-2.,20.);
  histos_.histhfhitenergyplus_ = fs->make<TH1F>("hfhitenergyplus","HF RecHit energy",100,-2.,20.);
  histos_.histhflongenergyplus_ = fs->make<TH1F>("hflongenergyplus","HF RecHit Long Fiber energy",100,-2.,20.);
  histos_.histhfshortenergyplus_ = fs->make<TH1F>("hfshortenergyplus","HF RecHit Short Fiber energy",100,-2.,20.);
  histos_.histhfhitenergyminus_ = fs->make<TH1F>("hfhitenergyminus","HF RecHit energy",100,-2.,20.);
  histos_.histhflongenergyminus_ = fs->make<TH1F>("hflongenergyminus","HF RecHit Long Fiber energy",100,-2.,20.);
  histos_.histhfshortenergyminus_ = fs->make<TH1F>("hfshortenergyminus","HF RecHit Short Fiber energy",100,-2.,20.);
  histos_.histhfenergyvstime_ = fs->make<TH1F>("hfenergyvstime","HF time",100,-100.,150.);

  histos_.histhflongenergyFromTwr_ = fs->make<TH1F>("hflongenergyFromTwr","HF RecHit Long Fiber energy",100,-2.,20.);
  histos_.histhfshortenergyFromTwr_ = fs->make<TH1F>("hfshortenergyFromTwr","HF RecHit Short Fiber energy",100,-2.,20.);
  // Exclude list
  //(40,63): 0.245
  //>>>>>>> Tower with very high activity
  //(40,67): 0.218
  //>>>>>>> Tower with very high activity
  /*excludeList = std::vector<std::pair<int,int> >();
  std::pair<int,int> aux_pair(40,63);	
  excludeList.push_back(aux_pair);
  aux_pair = std::pair<int,int>(40,67);	
  excludeList.push_back(aux_pair);

  for(std::vector<std::pair<int,int> >::const_iterator it = excludeList.begin(); it != excludeList.end(); ++it){
	std::cout << "Excluding (iEta,iPhi) " << "(" << it->first << "," << it->second << ")" << std::endl;	
  }*/
}     

void CaloTowerAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& iSetup) {
  //Calo Towers
  edm::Handle<CaloTowerCollection> towerCollection;  
  event.getByLabel(calotowersTag_,towerCollection);

  histosTH1F_["NEvents"]->Fill(0.);

  int sizeCaloTowers = towerCollection->size();

  LogTrace("Analysis") << " =================> Treating event " << event.id()
                       << " Number of Calo Towers " << sizeCaloTowers;

  if(sizeCaloTowers == 0) return;

  std::vector<int> nhb_plus(nThresholdIter_,0);
  std::vector<int> nhb_minus(nThresholdIter_,0);
  std::vector<double> sumw_hb_plus(nThresholdIter_,0.);
  std::vector<double> sumw_hb_minus(nThresholdIter_,0.);
  std::vector<double> sume_hb_plus(nThresholdIter_,0.);
  std::vector<double> sume_hb_minus(nThresholdIter_,0.);
  std::vector<double> sumew_hb_plus(nThresholdIter_,0.);
  std::vector<double> sumew_hb_minus(nThresholdIter_,0.);
  std::vector<int> nhe_plus(nThresholdIter_,0);
  std::vector<int> nhe_minus(nThresholdIter_,0);
  std::vector<double> sumw_he_plus(nThresholdIter_,0.);
  std::vector<double> sumw_he_minus(nThresholdIter_,0.);
  std::vector<double> sume_he_plus(nThresholdIter_,0.);
  std::vector<double> sume_he_minus(nThresholdIter_,0.);
  std::vector<double> sumew_he_plus(nThresholdIter_,0.);
  std::vector<double> sumew_he_minus(nThresholdIter_,0.);
  std::vector<int> nhf_plus(nThresholdIter_,0);
  std::vector<int> nhf_minus(nThresholdIter_,0);
  std::vector<double> sumw_hf_plus(nThresholdIter_,0.);
  std::vector<double> sumw_hf_minus(nThresholdIter_,0.); 
  std::vector<double> sume_hf_plus(nThresholdIter_,0.);
  std::vector<double> sume_hf_minus(nThresholdIter_,0.);
  std::vector<double> sumew_hf_plus(nThresholdIter_,0.);
  std::vector<double> sumew_hf_minus(nThresholdIter_,0.); 

  /*CandidateCollection::const_iterator cand = calotowercands->begin();
  CaloTowerRef towerRef = cand->get<CaloTowerRef>();
		
  const CaloTowerCollection *towerCollection = towerRef.product();*/

  double calotwrMaxEnergyHF = 0.;
  CaloTowerCollection::const_iterator calotwrMaxHF = towerCollection->end();
  CaloTowerCollection::const_iterator calotower = towerCollection->begin(); 
  CaloTowerCollection::const_iterator calotowers_end = towerCollection->end();
  for(; calotower != calotowers_end; ++calotower){
     //const CaloTower* calotower = &(*towerCollection)[idx];
     /*std::cout << "Eta: " << calotower->eta() << std::endl
                 << "Phi: " << calotower->phi() << std::endl
                 << "Et: " << calotower->et() << std::endl
                 << "Energy: " << calotower->energy() << std::endl
	         << "EM energy: " << calotower->emEnergy() << std::endl
	         << "HAD energy: " << calotower->hadEnergy() << std::endl
	         << "iEta: " << calotower->id().ieta() << std::endl	
	         << "iPhi: " << calotower->id().iphi() << std::endl
	         << "Detector: " << calotower->id().det() << std::endl
	         << "Sub Det Id: " << calotower->id().subdetId() << std::endl
	         << "Raw Id: " << calotower->id().rawId() << std::endl
		 << "Constituents: " << std::endl;*/
     int ieta = calotower->id().ieta();
     int iphi = calotower->id().iphi();
     //bool hotTower = false;
     /*for(std::vector<std::pair<int,int> >::const_iterator it = excludeList.begin(); it != excludeList.end(); ++it){
        if((ieta == it->first)&&(iphi == it->second)) hotTower = true;
     }*/
     //if(excludeHotTowers_&&hotTower) continue;
		
     bool hasHF = false;
     bool hasHE = false;
     bool hasHO = false;
     bool hasHB = false;
     bool hasECAL = false; 
     for(size_t iconst = 0; iconst < calotower->constituentsSize(); iconst++){
        /*std::cout << "    " << iconst << ":  " << std::endl
		    << "    Detector: " << calotower->constituent(iconst).det() << std::endl
		    << "    Sub Det Id: " << calotower->constituent(iconst).subdetId() << std::endl
                    << "    Raw Id: " << calotower->constituent(iconst).rawId() << std::endl;*/
	DetId detId = calotower->constituent(iconst);
	if(detId.det()==DetId::Hcal){
	   HcalDetId hcalDetId(detId);
	   if(hcalDetId.subdet()==HcalForward) hasHF = true;		
	   else if(hcalDetId.subdet()==HcalOuter) hasHO = true;
           else if(hcalDetId.subdet()==HcalEndcap) hasHE = true;
	   else if(hcalDetId.subdet()==HcalBarrel) hasHB = true;
        } else if(detId.det()==DetId::Ecal){
           hasECAL = true;
        }
     }
				 	
     int zside = calotower->id().zside();
     double eta = calotower->eta();
     double phi = calotower->phi();

     double energy = calotower->energy();
     double weightHB = 1.0;
     double weightHE = 1.0;
     double weightHF = 1.0;
     // HF
     if(hasHF&&(!hasHE)){
        //Valid for HF emEnergy = E(L) - E(S); hadEnergy = 2*E(S)
        double energyShort = calotower->hadEnergy()/2.;
        double energyLong = calotower->emEnergy() + energyShort;
        if(applyEnergyOffset_){
           /*energyShort += gauss_->fire(0.,sigmaShort_);
           energyLong += gauss_->fire(0.,sigmaLong_);
           energy = energyShort + energyLong;*/
           //energy += nIncrements_*sigmaLong_; 
           /*double deltaEShort = std::max(gauss_->fire(0.,sigmaShort_),0.);
           double deltaELong = std::max(gauss_->fire(0.,sigmaLong_),0.);
           energyShort += deltaEShort;
           energyLong += deltaELong; 
           energy = energyShort + energyLong;*/
           double deltaEnergy = std::max(gauss_->fire(meanHFEnergy_,sigmaHFEnergy_),0.);
           energy += deltaEnergy;
        }

        if(reweightHFTower_){
           int xbin = reweightHisto_.GetXaxis()->FindBin(energy);
           int nBins = reweightHisto_.GetNbinsX(); // 1 <= xbin <= nBins
           //weight = (xbin <= nBins) ? reweightHisto_.GetBinContent(xbin) : reweightHisto_.GetBinContent(nBins);
           weightHF = (xbin <= nBins) ? reweightHisto_.GetBinContent(xbin) : 1.;
        }
 
        histos_.histhflongenergyFromTwr_->Fill(energyLong,weightHF);
        histos_.histhfshortenergyFromTwr_->Fill(energyShort,weightHF);
        double emEnergy = calotower->emEnergy();
        double hadEnergy = calotower->hadEnergy(); 
        double eTot = emEnergy + hadEnergy;
        histosTH1F_["emEnergyHF"]->Fill(emEnergy,weightHF);
        histosTH1F_["hadEnergyHF"]->Fill(hadEnergy,weightHF);
        if(eTot > 50.0) histosTH1F_["emFractionHF"]->Fill(fabs(emEnergy/eTot),weightHF);
     }

     histos_.histenergyvseta_->Fill(eta,energy*weightHF);
     histos_.histetavsphi_->Fill(eta,phi,weightHF);
     histos_.histietavsiphi_->Fill(ieta,iphi,weightHF);
     /*histos_.histetavsphiweighted_->Fill(eta,phi,energy*weightHF);
     histos_.histietavsiphiweighted_->Fill(ieta,iphi,energy*weightHF);*/
	
     for(unsigned int i = 0; i < nThresholdIter_; ++i){
        bool ethreshHB = (energy >= eThresholdHB_[i]);
        bool ethreshHE = (energy >= eThresholdHE_[i]);
        bool ethreshHF = (energy >= eThresholdHF_[i]);
        if( ethreshHB && ( hasHB && (!hasHE) ) ){
           if(zside >= 0){
              nhb_plus[i]++; sumw_hb_plus[i] += weightHB;
              sume_hb_plus[i] += energy; sumew_hb_plus[i] += energy*weightHB;
           } else{
              nhb_minus[i]++; sumw_hb_minus[i] += weightHB;
              sume_hb_minus[i] += energy; sumew_hb_minus[i] += energy*weightHB;
           }
        } else if( ethreshHE && ( hasHE && (!hasHB) && (!hasHF) ) ){
           if(zside > 0){
              nhe_plus[i]++; sumw_he_plus[i] += weightHE;
              sume_he_plus[i] += energy; sumew_he_plus[i] += energy*weightHE;
           } else{
              nhe_minus[i]++; sumw_he_minus[i] += weightHE;
              sume_he_minus[i] += energy; sumew_he_minus[i] += energy*weightHE;
           }
        } else if( ethreshHF && ( hasHF && (!hasHE) ) ){
	   if(zside > 0){
              nhf_plus[i]++; sumw_hf_plus[i] += weightHF;
              sume_hf_plus[i] += energy; sumew_hf_plus[i] += energy*weightHF;
           } else{
              nhf_minus[i]++; sumw_hf_minus[i] += weightHF;
              sume_hf_minus[i] += energy; sumew_hf_minus[i] += energy*weightHF;  
           }
	}
     }
 
     
     if( hasHB && (!hasHE) ){
        histosTH1F_["energyHB"]->Fill(energy,weightHB);
        if(zside >= 0) histosTH1F_["energyHBplus"]->Fill(energy,weightHB);
        else histosTH1F_["energyHBminus"]->Fill(energy,weightHB);
     } else if( hasHE && (!hasHB) && (!hasHF) ){
        histosTH1F_["energyHE"]->Fill(energy,weightHE);
        if(zside > 0) histosTH1F_["energyHEplus"]->Fill(energy,weightHE);
        else histosTH1F_["energyHEminus"]->Fill(energy,weightHE);
     } else if( hasHF && (!hasHE) ){
        histosTH1F_["energyHF"]->Fill(energy,weightHF);
	if(zside > 0) histosTH1F_["energyHFplus"]->Fill(energy,weightHF);
	else histosTH1F_["energyHFminus"]->Fill(energy,weightHF);

        histos_.histenergyvsetaHF_->Fill(eta,energy*weightHF);
        histos_.histetavsphiHF_->Fill(eta,phi,weightHF);
        //histos_.histetavsphiHFweighted_->Fill(eta,phi,energy*weightHF);

	if(energy > calotwrMaxEnergyHF){
	   calotwrMaxEnergyHF = energy;
	   calotwrMaxHF = calotower;
	}
     }
  }

  if(calotwrMaxHF != calotowers_end){
     histos_.histenergyvsetaHFMax_->Fill(calotwrMaxHF->eta(),calotwrMaxHF->energy());
     histos_.histetavsphiHFMax_->Fill(calotwrMaxHF->eta(),calotwrMaxHF->phi(),calotwrMaxHF->energy());
  }

  for(unsigned int i = 0; i < nThresholdIter_; ++i){	
     histosNHBPlus_[i]->Fill(sumw_hb_plus[i]);
     histosNHBMinus_[i]->Fill(sumw_hb_minus[i]);
     histosSumEHBPlus_[i]->Fill(sumew_hb_plus[i]);
     histosSumEHBMinus_[i]->Fill(sumew_hb_minus[i]);

     histosNHEPlus_[i]->Fill(sumw_he_plus[i]);
     histosNHEMinus_[i]->Fill(sumw_he_minus[i]);
     histosSumEHEPlus_[i]->Fill(sumew_he_plus[i]);
     histosSumEHEMinus_[i]->Fill(sumew_he_minus[i]);

     histosNHFPlus_[i]->Fill(sumw_hf_plus[i]);
     histosNHFMinus_[i]->Fill(sumw_hf_minus[i]);
     /*double sumw_hf_low = (sumw_hf_minus < sumw_hf_plus)?sumw_hf_minus[i]:sumw_hf_plus[i];
     double sumw_hf_high = sumw_hf_minus[i] + sumw_hf_plus[i] - sumw_hf_low;
     histosnhflow_[i]->Fill(sumw_hf_low);
     histosnhfhigh_[i]->Fill(sumw_hf_high);*/
     histosSumEHFPlus_[i]->Fill(sumew_hf_plus[i]);
     histosSumEHFMinus_[i]->Fill(sumew_hf_minus[i]);
  }

  if(accessRecHits_){
     edm::Handle<HFRecHitCollection> hfrh;
     event.getByLabel(hfRecHitsTag_,hfrh);
     const HFRecHitCollection Hithf = *(hfrh.product());

     for(HFRecHitCollection::const_iterator hhit=Hithf.begin(); hhit!=Hithf.end(); hhit++) {
        //std::cout << "rec hit energy,time= " << hhit->energy() << "  " << hhit->time() << std::endl; 
	if(hhit->energy() > 0.6) histos_.histhfenergyvstime_->Fill(hhit->time(),hhit->energy());
        HcalDetId hcalDetId(hhit->detid());
	histos_.histhfhitenergy_->Fill(hhit->energy());
	if(hcalDetId.zside() > 0) histos_.histhfhitenergyplus_->Fill(hhit->energy());
	else histos_.histhfhitenergyminus_->Fill(hhit->energy());
	if(hcalDetId.depth() == 1){
	   histos_.histhflongenergy_->Fill(hhit->energy());
	   if(hcalDetId.zside() > 0) histos_.histhflongenergyplus_->Fill(hhit->energy());
	   else histos_.histhflongenergyminus_->Fill(hhit->energy());
	} else{
	   histos_.histhfshortenergy_->Fill(hhit->energy());
	   if(hcalDetId.zside() > 0) histos_.histhfshortenergyplus_->Fill(hhit->energy());
           else histos_.histhfshortenergyminus_->Fill(hhit->energy());
	}
     }

     /*edm::Handle<HBHEDigiCollection> hbhe_digi; 
     //event.getByLabel("hcalZeroSuppressedDigis",hbhe_digi);
     event.getByLabel("hcalDigis",hbhe_digi);

     if(!hbhe_digi.failedToGet()){
        int adcs[10] = {};
     
        //CORRECT:  Timing plot should be done using linearized ADC's!
        for (HBHEDigiCollection::const_iterator j=hbhe_digi->begin(); j!=hbhe_digi->end(); j++){
           const HBHEDataFrame digi = (const HBHEDataFrame)(*j);
	   HcalDetId id = digi.id();
	   if (id.subdet() != 1) continue; 
	   int maxadc = 0;
	   for (int TS = 0; TS < 10 && TS < digi.size(); ++TS){     
	      adcs[TS] = digi[TS].adc();
	      if (digi[TS].adc() > maxadc) maxadc = digi[TS].adc();
	   }
	   std::cout << "maxadc= " << maxadc << std::endl;
	   for (int TS = 0; TS < 10 && TS < digi.size(); ++TS){     
	      if (maxadc > 10) histhbtiming_->Fill(TS,adcs[TS]);
	   }
        }
     }*/
  }
}

DEFINE_FWK_MODULE(CaloTowerAnalyzer);
