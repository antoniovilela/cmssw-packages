#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TTree;
 
class HotTowerFinder : public edm::EDAnalyzer
{
 public:
  
  explicit HotTowerFinder(const edm::ParameterSet& conf);
  
  virtual ~HotTowerFinder();
  
  virtual void beginJob(edm::EventSetup const& iSetup);
  virtual void endJob();
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  
 private:

  edm::InputTag calotowersLabel_;

  TTree* data; 
  int tree_iEta;
  int tree_iPhi;
  double tree_Frac;

  double e_tresholdHF;
  double fracMaxHF;

  int nEvts;
  std::map<std::pair<int,int>,int> hotTowerMap;
};

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoCaloTowerCandidate.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"

#include "Geometry/CaloTopology/interface/HcalTopology.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <iostream>
#include "TFile.h"
#include "TTree.h"

using namespace reco;
 
HotTowerFinder::HotTowerFinder(const edm::ParameterSet& conf)
{
  calotowersLabel_=conf.getParameter<edm::InputTag>("CaloTowersLabel");

  e_tresholdHF = conf.getParameter<double>("TowerEnergyTresholdHF");
  fracMaxHF = conf.getParameter<double>("OccupancyFractionMaxHF");	
}  
  
HotTowerFinder::~HotTowerFinder()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

void HotTowerFinder::beginJob(edm::EventSetup const&iSetup){

  edm::Service<TFileService> fs;

  data = fs->make<TTree>("data","data");
  data->Branch("iEta",&tree_iEta,"iEta/I");
  data->Branch("iPhi",&tree_iPhi,"iPhi/I");
  data->Branch("Frac",&tree_Frac,"Frac/D");		

  nEvts = 0;
}     

void HotTowerFinder::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
	//Calo Towers
	edm::Handle<CandidateCollection> calotowercands;  
	iEvent.getByLabel(calotowersLabel_,calotowercands);

	edm::LogInfo("")<<"\n\n =================> Treating event "<<iEvent.id()
			<<" Number of Calo Towers "<<calotowercands.product()->size();

	//if(calotowercands->size() == 0) {LogTrace("") << ">>> Calo tower collection has zero size";return;}

	int sizeCaloTowers = calotowercands->size();

	if(sizeCaloTowers == 0) return;
	nEvts++;

	CandidateCollection::const_iterator cand = calotowercands->begin();
	CaloTowerRef towerRef = cand->get<CaloTowerRef>();
			
	const CaloTowerCollection *towerCollection = towerRef.product();

	for(CaloTowerCollection::const_iterator calotower = towerCollection->begin(); calotower != towerCollection->end(); ++calotower){
		bool hasHF = false;
		bool hasHE = false;
		bool hasHO = false;
		bool hasHB = false;
		bool hasECAL = false; 
		for(size_t iconst = 0; iconst < calotower->constituentsSize(); iconst++){
			DetId detId = calotower->constituent(iconst);
			if(detId.det()==DetId::Hcal){
				HcalDetId hcalDetId(detId);
				if(hcalDetId.subdet()==HcalForward) hasHF = true;		
				else if(hcalDetId.subdet()==HcalOuter) hasHO = true;
                                else if(hcalDetId.subdet()==HcalEndcap) hasHE = true;
				else if(hcalDetId.subdet()==HcalBarrel) hasHB = true;
			} else if(detId.det()==DetId::Ecal) hasECAL = true;
		}
				 	
			  	
		int zside = calotower->id().zside();
		int ieta = calotower->id().ieta();
		int iphi = calotower->id().iphi();
		double energy = calotower->energy();
		
		bool etreshHF = (energy >= e_tresholdHF);
		if(hasHF&&(!hasHE)){
			if(etreshHF){
				std::pair<int,int> aux_pair(ieta,iphi);
				std::map<std::pair<int,int>,int>::iterator iter = hotTowerMap.find(aux_pair);
				if(iter != hotTowerMap.end()) iter->second++;  
				else hotTowerMap.insert(make_pair(aux_pair,1));
			}
		}
	}

}

void HotTowerFinder::endJob(){
	for(std::map<std::pair<int,int>,int>::iterator it = hotTowerMap.begin(); it != hotTowerMap.end(); ++it){
		int count = it->second;
		double frac = ((double)count)/((double)nEvts);
		
		std::cout << "(" << (it->first).first << "," << (it->first).second << "): " << frac << std::endl;
		if(frac >= fracMaxHF){
			std::cout << ">>>>>>> Tower with very high activity" << std::endl;
			tree_iEta = (it->first).first;
			tree_iPhi = (it->first).second;
			tree_Frac = frac;
			data->Fill();
		}
	}
}

DEFINE_FWK_MODULE(HotTowerFinder);
