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
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h" 
#include "TTree.h"

using namespace reco;
 
CaloTowerAnalyzer::CaloTowerAnalyzer(const edm::ParameterSet& conf)
{
  calotowersTag_ = conf.getParameter<edm::InputTag>("CaloTowersLabel");
  accessRecHits_ = conf.getUntrackedParameter<bool>("AccessRecHits",true);
  if(accessRecHits_) hfRecHitsTag_ = conf.getUntrackedParameter<edm::InputTag>("HFRecHitsLabel",edm::InputTag("hfreco"));
  //excludeHotTowers_ = conf.getParameter<bool>("ExcludeHotTowers"); 
  nThresholdIter_ = conf.getParameter<unsigned int>("NumberOfTresholds");
  eThresholdHFMin_ = conf.getParameter<double>("TowerEnergyTresholdHFMin");
  eThresholdHFMax_ = conf.getParameter<double>("TowerEnergyTresholdHFMax");

  nBinsHF_ = conf.getUntrackedParameter<int>("NBinsHF",20);	
}  
  
CaloTowerAnalyzer::~CaloTowerAnalyzer()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

void CaloTowerAnalyzer::beginJob(edm::EventSetup const&iSetup){

  edm::Service<TFileService> fs;

  histNEvents_ = fs->make<TH1F>("NEvents","NEvents",1,0,1);

  char hname[50];
  for(unsigned int i = 0; i < nThresholdIter_; ++i){
	eThreshold_.push_back(eThresholdHFMin_ + i*((eThresholdHFMax_ - eThresholdHFMin_)/((double)nThresholdIter_)));
	std::cout << "Threshold " << i << ": " << eThreshold_[i] << std::endl;
	sprintf(hname,"nhfplus_%d",i);	
	histosnhfplus_.push_back(fs->make<TH1F>(hname,"Towers mult. HF plus",nBinsHF_,0,nBinsHF_));
	sprintf(hname,"nhfminus_%d",i);
	histosnhfminus_.push_back(fs->make<TH1F>(hname,"Towers mult. HF minus",nBinsHF_,0,nBinsHF_));	
	sprintf(hname,"nhflow_%d",i);
	histosnhflow_.push_back(fs->make<TH1F>(hname,"Towers mult. HF low",nBinsHF_,0,nBinsHF_));
	sprintf(hname,"nhfhigh_%d",i);
	histosnhfhigh_.push_back(fs->make<TH1F>(hname,"Towers mult. HF high",nBinsHF_,0,nBinsHF_));
  }
  histenergyHF_ = fs->make<TH1F>("energyHF","Tower Energy HF",100,0.,15.0);
  histenergyHFplus_ = fs->make<TH1F>("energyHFplus","Tower Energy HF-plus",100,0.,15.0);
  histenergyHFminus_ = fs->make<TH1F>("energyHFminus","Tower Energy HF-minus",100,0.,15.0);
  histenergyvseta_ = fs->make<TH1F>("energyvseta","Tower Energy vs eta",100,-5.1,5.1);
  histetavsphi_ = fs->make<TH2F>("etavsphi","etavsphi",100,-5.,5.,100,-3.1416,3.1416);
  histietavsiphi_ = fs->make<TH2F>("ietavsiphi","ietavsiphi",85,-42,43,75,0,75);
  histetavsphiweighted_ = fs->make<TH2F>("etavsphiweighted","etavsphiweighted",100,-5.,5.,100,-3.1416,3.1416);
  histietavsiphiweighted_ = fs->make<TH2F>("ietavsiphiweighted","ietavsiphiweighted",85,-42,43,75,0,75);
	
  histenergyvsetaHF_ = fs->make<TH1F>("energyvsetaHF","Tower Energy vs eta",100,-5.1,5.1);
  histetavsphiHF_ = fs->make<TH2F>("etavsphiHF","Tower Energy vs eta x phi",100,-5.,5.,100,-3.1416,3.1416);
  histetavsphiHFweighted_ = fs->make<TH2F>("etavsphiHFweighted","etavsphiHFweighted",100,-5.,5.,100,-3.1416,3.1416);
  
  histenergyvsetaHFMax_ = fs->make<TH1F>("energyvsetaHFMax","Tower Energy vs eta",100,-5.1,5.1);
  histetavsphiHFMax_ = fs->make<TH2F>("etavsphiHFMax","Tower Energy vs eta x phi",100,-5.,5.,100,-3.1416,3.1416);
  	
  histhbtiming_ = fs->make<TH1F>("hbtiming","HBHE Timing",10,-0.5,9.5);
  histhfhitenergy_ = fs->make<TH1F>("hfhitenergy","HF RecHit energy",100,-2.,20.);
  histhflongenergy_ = fs->make<TH1F>("hflongenergy","HF RecHit Long Fiber energy",100,-2.,20.);
  histhfshortenergy_ = fs->make<TH1F>("hfshortenergy","HF RecHit Short Fiber energy",100,-2.,20.);
  histhfhitenergyplus_ = fs->make<TH1F>("hfhitenergyplus","HF RecHit energy",100,-2.,20.);
  histhflongenergyplus_ = fs->make<TH1F>("hflongenergyplus","HF RecHit Long Fiber energy",100,-2.,20.);
  histhfshortenergyplus_ = fs->make<TH1F>("hfshortenergyplus","HF RecHit Short Fiber energy",100,-2.,20.);
  histhfhitenergyminus_ = fs->make<TH1F>("hfhitenergyminus","HF RecHit energy",100,-2.,20.);
  histhflongenergyminus_ = fs->make<TH1F>("hflongenergyminus","HF RecHit Long Fiber energy",100,-2.,20.);
  histhfshortenergyminus_ = fs->make<TH1F>("hfshortenergyminus","HF RecHit Short Fiber energy",100,-2.,20.);
  histhfenergyvstime_ = fs->make<TH1F>("hfenergyvstime","HF time",100,-100.,150.);

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

void CaloTowerAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
	//Calo Towers
	/*edm::Handle<CaloTowerCollection> calotowercands;  
	iEvent.getByLabel(calotowersLabel_,calotowercands);

	edm::LogInfo("")<<"\n\n =================> Treating event "<<iEvent.id()
			<<" Number of Calo Towers "<<calotowercands.product()->size();

	//if(calotowercands->size() == 0) {LogTrace("") << ">>> Calo tower collection has zero size";return;}

	int sizeCaloTowers = calotowercands->size();*/

	edm::Handle<CaloTowerCollection> towerCollection;  
        iEvent.getByLabel(calotowersTag_,towerCollection);

        histNEvents_->Fill(0.);

	int sizeCaloTowers = towerCollection->size();

        edm::LogInfo("")<< "\n\n =================> Treating event " << iEvent.id()
                        << " Number of Calo Towers " << sizeCaloTowers;

	if(sizeCaloTowers == 0) return;

	std::vector<int> nhf_plus(nThresholdIter_,0);
        std::vector<int> nhf_minus(nThresholdIter_,0);
 
	/*CandidateCollection::const_iterator cand = calotowercands->begin();
	CaloTowerRef towerRef = cand->get<CaloTowerRef>();
			
	const CaloTowerCollection *towerCollection = towerRef.product();*/

	double calotwrMaxEnergy = 0.;
	//int calotwrMaxIdx = -1;
	CaloTowerCollection::const_iterator calotwrMax = towerCollection->end();
	//for(size_t idx = 0; idx < towerCollection->size(); ++idx){
	for(CaloTowerCollection::const_iterator calotower = towerCollection->begin(); calotower != towerCollection->end(); ++calotower){
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
		bool hotTower = false;
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
					    << "    Sub Det Id: " << calotower->constituent(iconst).subdetId() << std::endl	     					 << "    Raw Id: " << calotower->constituent(iconst).rawId() << std::endl;*/
			DetId detId = calotower->constituent(iconst);
			//std::cout << "    " << iconst << ":  " << std::endl;
			if(detId.det()==DetId::Hcal){
				HcalDetId hcalDetId(detId);
				if(hcalDetId.subdet()==HcalForward){  
					  /*std::cout << "     HF constituent" << std::endl			
						    << "     iEta: " << hcalDetId.ieta() << std::endl
						    << "     iPhi: " << hcalDetId.iphi() << std::endl
						    << "     Depth: " << hcalDetId.depth() << std::endl;*/
					 hasHF = true;		
				} else if(hcalDetId.subdet()==HcalOuter){
					  /*std::cout << "     HO constituent" << std::endl
                                                    << "     iEta: " << hcalDetId.ieta() << std::endl
                                                    << "     iPhi: " << hcalDetId.iphi() << std::endl
                                                    << "     Depth: " << hcalDetId.depth() << std::endl;*/
					 hasHO = true;
                                } else if(hcalDetId.subdet()==HcalEndcap){
					  /*std::cout << "     HE constituent" << std::endl
                                                    << "     iEta: " << hcalDetId.ieta() << std::endl
                                                    << "     iPhi: " << hcalDetId.iphi() << std::endl
                                                    << "     Depth: " << hcalDetId.depth() << std::endl;*/
					 hasHE = true;
				} else if(hcalDetId.subdet()==HcalBarrel){
                                          /*std::cout << "     HB constituent" << std::endl
                                                    << "     iEta: " << hcalDetId.ieta() << std::endl
                                                    << "     iPhi: " << hcalDetId.iphi() << std::endl
                                                    << "     Depth: " << hcalDetId.depth() << std::endl;*/
					 hasHB = true;
                                } else{
					  /*std::cout << "     HcalSubdetector: " << hcalDetId.subdet() << std::endl
                                                    << "     iEta: " << hcalDetId.ieta() << std::endl
                                                    << "     iPhi: " << hcalDetId.iphi() << std::endl
                                                    << "     Depth: " << hcalDetId.depth() << std::endl;*/
				} 
			} else if(detId.det()==DetId::Ecal){
				//std::cout << "     ECAL constituent" << std::endl;
				hasECAL = true;
			}	 	  
		}
				 	
			  	
		int zside = calotower->id().zside();
		double eta = calotower->eta();
		double phi = calotower->phi();
		double energy = calotower->energy();
		
		histenergyvseta_->Fill(eta,energy);
		histetavsphi_->Fill(eta,phi);
		histietavsiphi_->Fill(ieta,iphi);
		histetavsphiweighted_->Fill(eta,phi,energy);
		histietavsiphiweighted_->Fill(ieta,iphi,energy);
	
		for(unsigned int i = 0; i < nThresholdIter_; ++i){
			bool ethreshHF = (energy >= eThreshold_[i]);
			if(ethreshHF&&(hasHF&&(!hasHE))){
				if(zside > 0) nhf_plus[i]++;
				else nhf_minus[i]++;
				histenergyvsetaHF_->Fill(eta,energy); 
				histetavsphiHF_->Fill(eta,phi);
				histetavsphiHFweighted_->Fill(eta,phi,energy);
			}
		}

		if(hasHF&&(!hasHE)){
			histenergyHF_->Fill(energy);
			if(zside > 0) histenergyHFplus_->Fill(energy);
			else histenergyHFminus_->Fill(energy);
			if(energy > calotwrMaxEnergy){
				calotwrMaxEnergy = energy;
				calotwrMax = calotower;
			}
		}
	}

	if(calotwrMax != towerCollection->end()){
		histenergyvsetaHFMax_->Fill(calotwrMax->eta(),calotwrMax->energy());
		histetavsphiHFMax_->Fill(calotwrMax->eta(),calotwrMax->phi(),calotwrMax->energy());
	}

	for(unsigned int i = 0; i < nThresholdIter_; ++i){	
		histosnhfplus_[i]->Fill(nhf_plus[i]);
                histosnhfminus_[i]->Fill(nhf_minus[i]);
		int nhflow = (nhf_minus < nhf_plus)?nhf_minus[i]:nhf_plus[i];
		int nhfhigh = nhf_minus[i] + nhf_plus[i] - nhflow;
		histosnhflow_[i]->Fill(nhflow);
		histosnhfhigh_[i]->Fill(nhfhigh);
	}

        if(accessRecHits_){
	   edm::Handle<HFRecHitCollection> hfrh;
	   iEvent.getByLabel(hfRecHitsTag_,hfrh);
	   const HFRecHitCollection Hithf = *(hfrh.product());

	   for(HFRecHitCollection::const_iterator hhit=Hithf.begin(); hhit!=Hithf.end(); hhit++) {
		//std::cout << "rec hit energy,time= " << hhit->energy() << "  " << hhit->time() << std::endl; 
		if(hhit->energy() > 0.6) histhfenergyvstime_->Fill(hhit->time(),hhit->energy());
		HcalDetId hcalDetId(hhit->detid());
		histhfhitenergy_->Fill(hhit->energy());
		if(hcalDetId.zside() > 0) histhfhitenergyplus_->Fill(hhit->energy());
		else histhfhitenergyminus_->Fill(hhit->energy());
		if(hcalDetId.depth() == 1){
			histhflongenergy_->Fill(hhit->energy());
			if(hcalDetId.zside() > 0) histhflongenergyplus_->Fill(hhit->energy());
			else histhflongenergyminus_->Fill(hhit->energy());
		} else{
			histhfshortenergy_->Fill(hhit->energy());
			if(hcalDetId.zside() > 0) histhfshortenergyplus_->Fill(hhit->energy());
                        else histhfshortenergyminus_->Fill(hhit->energy());
		}
	   }

	   /*edm::Handle<HBHEDigiCollection> hbhe_digi; 
	   //iEvent.getByLabel("hcalZeroSuppressedDigis",hbhe_digi);
	   iEvent.getByLabel("hcalDigis",hbhe_digi);

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
