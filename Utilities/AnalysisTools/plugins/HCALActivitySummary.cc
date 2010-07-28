
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include <vector>

class CaloTower;

class HCALActivitySummary : public edm::EDProducer {
public:
  HCALActivitySummary( edm::ParameterSet const& );
private:
  void produce( edm::Event &, edm::EventSetup const& );
  bool checkTowerFlagsHB( CaloTower const& );
  bool checkTowerFlagsHE( CaloTower const& );
  bool checkTowerFlagsHF( CaloTower const&, double );

  edm::InputTag caloTowersTag_;

  double eThresholdHBMin_;
  double eThresholdHBMax_;
  double eThresholdHEMin_;
  double eThresholdHEMax_;
  double eThresholdHFMin_;
  double eThresholdHFMax_;
  unsigned int nThresholdIter_;
  bool discardFlaggedTowers_;
  bool applyEnergyScale_;
  double energyScaleHB_;
  double energyScaleHE_;
  double energyScaleHF_;
  std::vector<double> thresholdsHB_;
  std::vector<double> thresholdsHE_;
  std::vector<double> thresholdsHF_;
};

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#include "DataFormats/Candidate/interface/Candidate.h"
//#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
//#include "DataFormats/RecoCandidate/interface/RecoCaloTowerCandidate.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h" 
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

using namespace reco;

HCALActivitySummary::HCALActivitySummary(edm::ParameterSet const& pset): 
  caloTowersTag_(pset.getParameter<edm::InputTag>("CaloTowersTag")),
  eThresholdHBMin_(pset.getParameter<double>("TowerEnergyThresholdHBMin")),
  eThresholdHBMax_(pset.getParameter<double>("TowerEnergyThresholdHBMax")),
  eThresholdHEMin_(pset.getParameter<double>("TowerEnergyThresholdHEMin")),
  eThresholdHEMax_(pset.getParameter<double>("TowerEnergyThresholdHEMax")),
  eThresholdHFMin_(pset.getParameter<double>("TowerEnergyThresholdHFMin")),
  eThresholdHFMax_(pset.getParameter<double>("TowerEnergyThresholdHFMax")),
  nThresholdIter_(pset.getParameter<unsigned int>("NumberOfThresholds")),   
  discardFlaggedTowers_(pset.getParameter<bool>("DiscardFlaggedTowers")),
  applyEnergyScale_(pset.getParameter<bool>("ApplyEnergyScale")),
  energyScaleHB_(1.0),
  energyScaleHE_(1.0),
  energyScaleHF_(1.0) {

  if(applyEnergyScale_){
     energyScaleHB_ = pset.getParameter<double>("EnergyScaleFactorHB");
     energyScaleHE_ = pset.getParameter<double>("EnergyScaleFactorHE");
     energyScaleHF_ = pset.getParameter<double>("EnergyScaleFactorHF");
  }

  thresholdsHB_ = std::vector<double>(nThresholdIter_);
  thresholdsHE_ = std::vector<double>(nThresholdIter_);
  thresholdsHF_ = std::vector<double>(nThresholdIter_);
  for(size_t k = 0; k < nThresholdIter_; ++k){
     thresholdsHB_[k] = eThresholdHBMin_ + k*((eThresholdHBMax_ - eThresholdHBMin_)/nThresholdIter_);
     thresholdsHE_[k] = eThresholdHEMin_ + k*((eThresholdHEMax_ - eThresholdHEMin_)/nThresholdIter_);
     thresholdsHF_[k] = eThresholdHFMin_ + k*((eThresholdHFMax_ - eThresholdHFMin_)/nThresholdIter_);
  }
    
  std::string alias;
  // HB
  produces<std::vector<double> >( alias = "thresholdsHB" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nHBplus" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nHBminus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEHBplus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEHBminus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHBMultiplicityPlus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHBMultiplicityMinus" ).setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHBEnergySumPlus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHBEnergySumMinus").setBranchAlias( alias );
  // HE
  produces<std::vector<double> >( alias = "thresholdsHE" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nHEplus" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nHEminus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEHEplus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEHEminus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHEMultiplicityPlus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHEMultiplicityMinus" ).setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHEEnergySumPlus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHEEnergySumMinus").setBranchAlias( alias );
  // HF
  produces<std::vector<double> >( alias = "thresholdsHF" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nHFplus" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nHFminus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEHFplus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEHFminus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHFMultiplicityPlus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHFMultiplicityMinus" ).setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHFEnergySumPlus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHFEnergySumMinus").setBranchAlias( alias );
}

void HCALActivitySummary::produce(edm::Event& event, edm::EventSetup const& setup) {

  edm::Handle<CaloTowerCollection> towerCollectionH;
  event.getByLabel(caloTowersTag_,towerCollectionH);
  const CaloTowerCollection& towerCollection = *(towerCollectionH.product());
	
  // Loop over CaloTowers
  std::map<int,std::vector<unsigned int> > iEtaHBMultiplicity;
  std::map<int,std::vector<double> > iEtaHBEnergySum;
  std::map<int,std::vector<unsigned int> > iEtaHEMultiplicity;
  std::map<int,std::vector<double> > iEtaHEEnergySum;
  std::map<int,std::vector<unsigned int> > iEtaHFMultiplicity;
  std::map<int,std::vector<double> > iEtaHFEnergySum;

  CaloTowerCollection::const_iterator calotower = towerCollection.begin();
  CaloTowerCollection::const_iterator calotowers_end = towerCollection.end(); 
  for(; calotower != calotowers_end; ++calotower) {	
     bool hasHF = false;
     bool hasHE = false;
     bool hasHO = false;
     bool hasHB = false;
     bool hasECAL = false; 
     for(size_t iconst = 0; iconst < calotower->constituentsSize(); iconst++){
        DetId detId = calotower->constituent(iconst);
        if( detId.det() == DetId::Hcal ){
           HcalDetId hcalDetId(detId);
           if( hcalDetId.subdet() == HcalForward) hasHF = true;
           else if( hcalDetId.subdet() == HcalEndcap ) hasHE = true;
           else if( hcalDetId.subdet() == HcalBarrel ) hasHB = true;
           else if( hcalDetId.subdet() == HcalOuter ) hasHO = true;
        } else if( detId.det() == DetId::Ecal ){
           hasECAL = true;
        }	 	  
     }
				 	
     //int zside = calotower->id().zside();
     //double eta = calotower->eta();
     int ieta = calotower->id().ieta();
     double energy = calotower->energy();

     bool isHBTower = hasHB && (!hasHE);
     bool isHETower = hasHE && (!hasHB) && (!hasHF);
     bool isHFTower = hasHF && (!hasHE);
     if(isHBTower){
        bool acceptHBTower = true;
        if(discardFlaggedTowers_ && checkTowerFlagsHB(*calotower)) acceptHBTower = false; 
        if(!acceptHBTower) continue;

        if(applyEnergyScale_) energy *= energyScaleHB_;
     } else if(isHETower){
        bool acceptHETower = true;
        if(discardFlaggedTowers_ && checkTowerFlagsHE(*calotower)) acceptHETower = false;     
        if(!acceptHETower) continue;

        if(applyEnergyScale_) energy *= energyScaleHE_;
     } else if(isHFTower){
        bool acceptHFTower = true; 
        if(discardFlaggedTowers_ && checkTowerFlagsHF(*calotower,0.99)) acceptHFTower = false;
        if(!acceptHFTower) continue;

        if(applyEnergyScale_) energy *= energyScaleHF_;
     }
 
     // Loop over tower thresholds
     for(size_t i_threshold = 0; i_threshold < nThresholdIter_; ++i_threshold){ 
        if(isHBTower){
           if(energy < thresholdsHB_[i_threshold]) continue;
    
           if(iEtaHBMultiplicity.find(ieta) == iEtaHBMultiplicity.end()){
              iEtaHBMultiplicity[ieta] = std::vector<unsigned int>(nThresholdIter_);
              iEtaHBEnergySum[ieta] = std::vector<double>(nThresholdIter_);
           }
           ++iEtaHBMultiplicity[ieta][i_threshold];
           iEtaHBEnergySum[ieta][i_threshold] += energy;
        } else if(isHETower){
           if(energy < thresholdsHE_[i_threshold]) continue;

           if(iEtaHEMultiplicity.find(ieta) == iEtaHEMultiplicity.end()){
              iEtaHEMultiplicity[ieta] = std::vector<unsigned int>(nThresholdIter_);
              iEtaHEEnergySum[ieta] = std::vector<double>(nThresholdIter_);
           }
           ++iEtaHEMultiplicity[ieta][i_threshold];
           iEtaHEEnergySum[ieta][i_threshold] += energy;
        } else if(isHFTower){
           if(energy < thresholdsHF_[i_threshold]) continue;

           if(iEtaHFMultiplicity.find(ieta) == iEtaHFMultiplicity.end()){
              iEtaHFMultiplicity[ieta] = std::vector<unsigned int>(nThresholdIter_);
              iEtaHFEnergySum[ieta] = std::vector<double>(nThresholdIter_);
           }
           ++iEtaHFMultiplicity[ieta][i_threshold];
           iEtaHFEnergySum[ieta][i_threshold] += energy;
        }

     }
  }

  // HB
  std::vector<unsigned int> nHB_plus(nThresholdIter_,0);
  std::vector<unsigned int> nHB_minus(nThresholdIter_,0);
  std::vector<double> sumEHB_plus(nThresholdIter_,0.);
  std::vector<double> sumEHB_minus(nThresholdIter_,0.);
  std::map<unsigned int,std::vector<unsigned int> > iEtaHBMultiplicity_plus;
  std::map<unsigned int,std::vector<unsigned int> > iEtaHBMultiplicity_minus;
  std::map<unsigned int,std::vector<double> > iEtaHBEnergySum_plus;
  std::map<unsigned int,std::vector<double> > iEtaHBEnergySum_minus;

  std::map<int,std::vector<unsigned int> >::const_iterator itHBMult = iEtaHBMultiplicity.begin();
  std::map<int,std::vector<unsigned int> >::const_iterator itHBMult_end = iEtaHBMultiplicity.end();
  for(; itHBMult != itHBMult_end; ++itHBMult){
     int ieta = itHBMult->first;
     std::vector<unsigned int> const& vec_mult = iEtaHBMultiplicity[ieta];
     std::vector<double> const& vec_esum = iEtaHBEnergySum[ieta];  
     for(size_t i_threshold = 0; i_threshold < nThresholdIter_; ++i_threshold){
        if(ieta >= 0){
           nHB_plus[i_threshold] += vec_mult[i_threshold];
           sumEHB_plus[i_threshold] += vec_esum[i_threshold];
        } else{
           nHB_minus[i_threshold] += vec_mult[i_threshold];
           sumEHB_minus[i_threshold] += vec_esum[i_threshold];
        } 
     }

     unsigned int abs_ieta = (ieta >= 0) ? ieta : -ieta;
     if(ieta >= 0){
        iEtaHBMultiplicity_plus[abs_ieta] = vec_mult;
        iEtaHBEnergySum_plus[abs_ieta] = vec_esum;
     } else{
        iEtaHBMultiplicity_minus[abs_ieta] = vec_mult;
        iEtaHBEnergySum_minus[abs_ieta] = vec_esum;
     }
  }

  // HE
  std::vector<unsigned int> nHE_plus(nThresholdIter_,0);
  std::vector<unsigned int> nHE_minus(nThresholdIter_,0);
  std::vector<double> sumEHE_plus(nThresholdIter_,0.);
  std::vector<double> sumEHE_minus(nThresholdIter_,0.);
  std::map<unsigned int,std::vector<unsigned int> > iEtaHEMultiplicity_plus;
  std::map<unsigned int,std::vector<unsigned int> > iEtaHEMultiplicity_minus;
  std::map<unsigned int,std::vector<double> > iEtaHEEnergySum_plus;
  std::map<unsigned int,std::vector<double> > iEtaHEEnergySum_minus;

  std::map<int,std::vector<unsigned int> >::const_iterator itHEMult = iEtaHEMultiplicity.begin();
  std::map<int,std::vector<unsigned int> >::const_iterator itHEMult_end = iEtaHEMultiplicity.end();
  for(; itHEMult != itHEMult_end; ++itHEMult){
     int ieta = itHEMult->first;
     std::vector<unsigned int> const& vec_mult = iEtaHEMultiplicity[ieta];
     std::vector<double> const& vec_esum = iEtaHEEnergySum[ieta];
     for(size_t i_threshold = 0; i_threshold < nThresholdIter_; ++i_threshold){
        if(ieta >= 0){
           nHE_plus[i_threshold] += vec_mult[i_threshold];
           sumEHE_plus[i_threshold] += vec_esum[i_threshold];
        } else{
           nHE_minus[i_threshold] += vec_mult[i_threshold];
           sumEHE_minus[i_threshold] += vec_esum[i_threshold];
        }
     } 

     unsigned int abs_ieta = (ieta >= 0) ? ieta : -ieta;
     if(ieta >= 0){
        iEtaHEMultiplicity_plus[abs_ieta] = vec_mult;
        iEtaHEEnergySum_plus[abs_ieta] = vec_esum;
     } else{
        iEtaHEMultiplicity_minus[abs_ieta] = vec_mult;
        iEtaHEEnergySum_minus[abs_ieta] = vec_esum;
     }
  }  

  // HF
  std::vector<unsigned int> nHF_plus(nThresholdIter_,0);
  std::vector<unsigned int> nHF_minus(nThresholdIter_,0);
  std::vector<double> sumEHF_plus(nThresholdIter_,0.);
  std::vector<double> sumEHF_minus(nThresholdIter_,0.);
  std::map<unsigned int,std::vector<unsigned int> > iEtaHFMultiplicity_plus;
  std::map<unsigned int,std::vector<unsigned int> > iEtaHFMultiplicity_minus;
  std::map<unsigned int,std::vector<double> > iEtaHFEnergySum_plus;
  std::map<unsigned int,std::vector<double> > iEtaHFEnergySum_minus;

  std::map<int,std::vector<unsigned int> >::const_iterator itHFMult = iEtaHFMultiplicity.begin();
  std::map<int,std::vector<unsigned int> >::const_iterator itHFMult_end = iEtaHFMultiplicity.end();
  for(; itHFMult != itHFMult_end; ++itHFMult){
     int ieta = itHFMult->first;
     std::vector<unsigned int> const& vec_mult = iEtaHFMultiplicity[ieta];
     std::vector<double> const& vec_esum = iEtaHFEnergySum[ieta];
     for(size_t i_threshold = 0; i_threshold < nThresholdIter_; ++i_threshold){
        if(ieta >= 0){
           nHF_plus[i_threshold] += vec_mult[i_threshold];
           sumEHF_plus[i_threshold] += vec_esum[i_threshold];
        } else{
           nHF_minus[i_threshold] += vec_mult[i_threshold];
           sumEHF_minus[i_threshold] += vec_esum[i_threshold];
        }
     }

     unsigned int abs_ieta = (ieta >= 0) ? ieta : -ieta;
     if(ieta >= 0){
        iEtaHFMultiplicity_plus[abs_ieta] = vec_mult;
        iEtaHFEnergySum_plus[abs_ieta] = vec_esum;
     } else{
        iEtaHFMultiplicity_minus[abs_ieta] = vec_mult;
        iEtaHFEnergySum_minus[abs_ieta] = vec_esum;
     } 
  }

  std::auto_ptr<std::vector<double> > thresholdsHB_ptr( new std::vector<double>(thresholdsHB_) );
  std::auto_ptr<std::vector<unsigned int> > nHBplus_ptr( new std::vector<unsigned int>(nHB_plus) );
  std::auto_ptr<std::vector<unsigned int> > nHBminus_ptr( new std::vector<unsigned int>(nHB_minus) );
  std::auto_ptr<std::vector<double> > sumEHBplus_ptr( new std::vector<double>(sumEHB_plus) );
  std::auto_ptr<std::vector<double> > sumEHBminus_ptr( new std::vector<double>(sumEHB_minus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHBMultiplicityPlus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaHBMultiplicity_plus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHBMultiplicityMinus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaHBMultiplicity_minus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHBEnergySumPlus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHBEnergySum_plus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHBEnergySumMinus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHBEnergySum_minus) );

  std::auto_ptr<std::vector<double> > thresholdsHE_ptr( new std::vector<double>(thresholdsHE_) );
  std::auto_ptr<std::vector<unsigned int> > nHEplus_ptr( new std::vector<unsigned int>(nHE_plus) );
  std::auto_ptr<std::vector<unsigned int> > nHEminus_ptr( new std::vector<unsigned int>(nHE_minus) );
  std::auto_ptr<std::vector<double> > sumEHEplus_ptr( new std::vector<double>(sumEHE_plus) );
  std::auto_ptr<std::vector<double> > sumEHEminus_ptr( new std::vector<double>(sumEHE_minus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHEMultiplicityPlus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaHEMultiplicity_plus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHEMultiplicityMinus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaHEMultiplicity_minus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHEEnergySumPlus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHEEnergySum_plus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHEEnergySumMinus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHEEnergySum_minus) );  

  std::auto_ptr<std::vector<double> > thresholdsHF_ptr( new std::vector<double>(thresholdsHF_) );
  std::auto_ptr<std::vector<unsigned int> > nHFplus_ptr( new std::vector<unsigned int>(nHF_plus) );
  std::auto_ptr<std::vector<unsigned int> > nHFminus_ptr( new std::vector<unsigned int>(nHF_minus) );
  std::auto_ptr<std::vector<double> > sumEHFplus_ptr( new std::vector<double>(sumEHF_plus) );
  std::auto_ptr<std::vector<double> > sumEHFminus_ptr( new std::vector<double>(sumEHF_minus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityPlus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaHFMultiplicity_plus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityMinus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaHFMultiplicity_minus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHFEnergySumPlus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHFEnergySum_plus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHFEnergySumMinus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHFEnergySum_minus) );

  event.put( thresholdsHB_ptr, "thresholdsHB" );
  event.put( nHBplus_ptr, "nHBplus" );
  event.put( nHBminus_ptr, "nHBminus" );
  event.put( sumEHBplus_ptr, "sumEHBplus" );
  event.put( sumEHBminus_ptr, "sumEHBminus" );
  event.put( iEtaHBMultiplicityPlus_ptr, "iEtaHBMultiplicityPlus" );
  event.put( iEtaHBMultiplicityMinus_ptr, "iEtaHBMultiplicityMinus" );
  event.put( iEtaHBEnergySumPlus_ptr, "iEtaHBEnergySumPlus" );
  event.put( iEtaHBEnergySumMinus_ptr, "iEtaHBEnergySumMinus" );

  event.put( thresholdsHE_ptr, "thresholdsHE" );
  event.put( nHEplus_ptr, "nHEplus" );
  event.put( nHEminus_ptr, "nHEminus" );
  event.put( sumEHEplus_ptr, "sumEHEplus" );
  event.put( sumEHEminus_ptr, "sumEHEminus" );
  event.put( iEtaHEMultiplicityPlus_ptr, "iEtaHEMultiplicityPlus" );
  event.put( iEtaHEMultiplicityMinus_ptr, "iEtaHEMultiplicityMinus" );
  event.put( iEtaHEEnergySumPlus_ptr, "iEtaHEEnergySumPlus" );
  event.put( iEtaHEEnergySumMinus_ptr, "iEtaHEEnergySumMinus" );

  event.put( thresholdsHF_ptr, "thresholdsHF" );
  event.put( nHFplus_ptr, "nHFplus" );
  event.put( nHFminus_ptr, "nHFminus" );
  event.put( sumEHFplus_ptr, "sumEHFplus" );
  event.put( sumEHFminus_ptr, "sumEHFminus" );
  event.put( iEtaHFMultiplicityPlus_ptr, "iEtaHFMultiplicityPlus" );
  event.put( iEtaHFMultiplicityMinus_ptr, "iEtaHFMultiplicityMinus" );
  event.put( iEtaHFEnergySumPlus_ptr, "iEtaHFEnergySumPlus" );
  event.put( iEtaHFEnergySumMinus_ptr, "iEtaHFEnergySumMinus" );
}

bool HCALActivitySummary::checkTowerFlagsHB(CaloTower const& tower){ return false; }

bool HCALActivitySummary::checkTowerFlagsHE(CaloTower const& tower){ return false; }

bool HCALActivitySummary::checkTowerFlagsHF(CaloTower const& tower, double emFracThreshold){
  bool flagged = false;

  double emEnergy = tower.emEnergy();
  double hadEnergy = tower.hadEnergy();
  double eTot = emEnergy + hadEnergy;
  double emFrac = fabs(emEnergy/eTot);
  if(eTot > 50.0 && emFrac > emFracThreshold) flagged = true;

  return flagged;  
}

DEFINE_FWK_MODULE(HCALActivitySummary);
