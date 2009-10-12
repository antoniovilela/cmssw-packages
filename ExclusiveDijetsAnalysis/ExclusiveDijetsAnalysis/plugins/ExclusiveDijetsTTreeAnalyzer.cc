
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>

#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/EventData.h"
#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/Histos.h"
#include "ExclusiveDijetsAnalysis/ExclusiveDijetsAnalysis/interface/ExclusiveDijetsAnalysisImpl.h"

class TTree;
class TH1F;
class TH2F;

class JetCorrector;

class ExclusiveDijetsTTreeAnalyzer: public edm::EDAnalyzer
{
  public: 
    explicit ExclusiveDijetsTTreeAnalyzer(const edm::ParameterSet&);
    ~ExclusiveDijetsTTreeAnalyzer();

    virtual void beginJob(const edm::EventSetup&);
    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
  private:
    exclusiveDijetsAnalysis::ExclusiveDijetsAnalysisImpl exclusiveDijetsAnalysisImpl_;
    bool saveTTree_;

    TTree* data_; // not owned
    exclusiveDijetsAnalysis::EventData eventData_;
    exclusiveDijetsAnalysis::Histos histos_; // not owned
};

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace exclusiveDijetsAnalysis;

/*class TFileServiceAdaptor{
  public:
     TFileServiceAdaptor(TFileService& fs): fs_(fs) {}

     template <class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
     T* make(const Arg1 & a1, const Arg2 & a2, const Arg3 & a3, const Arg4 & a4, const Arg5 & a5){
        return fs_->make<T>(a1,a2,a3,a4,a5);
     }
      
  private:
     TFileService& fs_;
};*/

ExclusiveDijetsTTreeAnalyzer::ExclusiveDijetsTTreeAnalyzer(const edm::ParameterSet& pset):
  exclusiveDijetsAnalysisImpl_(pset),  
  saveTTree_(pset.getUntrackedParameter<bool>("SaveROOTTree",true)){}

ExclusiveDijetsTTreeAnalyzer::~ExclusiveDijetsTTreeAnalyzer(){}

void ExclusiveDijetsTTreeAnalyzer::beginJob(const edm::EventSetup& setup){
  edm::Service<TFileService> fs;

  if(saveTTree_){
    data_ = fs->make<TTree>("data","data");
    addTTreeBranches(*data_,eventData_);
  }

  bookHistos(histos_,*fs);
}

void ExclusiveDijetsTTreeAnalyzer::beginRun(const edm::Run& run, const edm::EventSetup& setup){
  exclusiveDijetsAnalysisImpl_.servicesBeginRun(run,setup);
}

void ExclusiveDijetsTTreeAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){

  exclusiveDijetsAnalysisImpl_.fillEventData(eventData_,event,setup);

  histos_.h_nPileUpBx0_->Fill(eventData_.nPileUpBx0_);
  histos_.h_nVertex_->Fill(eventData_.nVertex_); 

  histos_.h_leadingJetPt_->Fill(eventData_.leadingJetPt_);
  histos_.h_secondJetPt_->Fill(eventData_.secondJetPt_);
  histos_.h_leadingJetEta_->Fill(eventData_.leadingJetEta_);
  histos_.h_secondJetEta_->Fill(eventData_.secondJetEta_);
  histos_.h_leadingJetPhi_->Fill(eventData_.leadingJetPhi_);
  histos_.h_secondJetPhi_->Fill(eventData_.secondJetPhi_);  
  histos_.h_thirdJetPt_->Fill(eventData_.thirdJetPt_);
  histos_.h_thirdJetEta_->Fill(eventData_.thirdJetEta_);
  histos_.h_jetsAveEta_->Fill(eventData_.jetsAveEta_);
  histos_.h_jetsDeltaEta_->Fill(eventData_.jetsDeltaEta_);
  histos_.h_jetsDeltaPhi_->Fill(eventData_.jetsDeltaPhi_);
  histos_.h_jetsDeltaPt_->Fill(eventData_.jetsDeltaPt_);
  histos_.h_massDijets_->Fill(eventData_.massDijets_);
  histos_.h_MxFromJets_->Fill(eventData_.MxFromJets_);
  histos_.h_RjjFromJets_->Fill(eventData_.RjjFromJets_);
  histos_.h_RjjFromPFCands_->Fill(eventData_.RjjFromPFCands_);
  histos_.h_massDijetsGen_->Fill(eventData_.massDijetsGen_);
  histos_.h_leadingJetBDiscriminator_->Fill(eventData_.leadingJetBDiscriminator_);
  histos_.h_secondJetBDiscriminator_->Fill(eventData_.secondJetBDiscriminator_);

  histos_.h_trackMultiplicity_->Fill(eventData_.trackMultiplicity_);
  histos_.h_multiplicityHFPlus_->Fill(eventData_.multiplicityHFPlus_);
  histos_.h_multiplicityHFMinus_->Fill(eventData_.multiplicityHFMinus_); 
  
  histos_.h_xiGenPlus_->Fill(eventData_.xiGenPlus_);
  histos_.h_xiGenMinus_->Fill(eventData_.xiGenMinus_);
  histos_.h_xiTowerPlus_->Fill(eventData_.xiTowerPlus_);
  histos_.h_xiTowerMinus_->Fill(eventData_.xiTowerMinus_);
  histos_.h_xiPlusFromJets_->Fill(eventData_.xiPlusFromJets_);
  histos_.h_xiMinusFromJets_->Fill(eventData_.xiMinusFromJets_);
  histos_.h_xiPlusFromPFCands_->Fill(eventData_.xiPlusFromPFCands_);
  histos_.h_xiMinusFromPFCands_->Fill(eventData_.xiMinusFromPFCands_);
  histos_.h_missingMassFromXi_->Fill(eventData_.missingMassFromXi_);

  double resMassDijets = (eventData_.massDijets_ - eventData_.massDijetsGen_)/eventData_.massDijetsGen_;
  histos_.h_ResMassDijets_->Fill(resMassDijets);
  histos_.h_RjjFromJetsVsBDicriminator_->Fill(eventData_.RjjFromJets_,eventData_.leadingJetBDiscriminator_);
  histos_.h_RjjFromPFCandsVsBDicriminator_->Fill(eventData_.RjjFromPFCands_,eventData_.leadingJetBDiscriminator_);

  //histos_.h_iEtaVsHFCountPlus_->Fill(ieta,nHFPlus_ieta);
  //histos_.h_iEtaVsHFCountMinus_->Fill(ieta,nHFMinus_ieta);

  double resXiTowerPlus = (eventData_.xiTowerPlus_ - eventData_.xiGenPlus_)/eventData_.xiGenPlus_;
  histos_.h_ResXiTowerPlus_->Fill(resXiTowerPlus);
  double resXiTowerMinus = (eventData_.xiTowerMinus_ - eventData_.xiGenMinus_)/eventData_.xiGenMinus_; 
  histos_.h_ResXiTowerMinus_->Fill(resXiTowerMinus);
  double resXiPlusFromJets = (eventData_.xiPlusFromJets_ - eventData_.xiGenPlus_)/eventData_.xiGenPlus_;
  histos_.h_ResXiPlusFromJets_->Fill(resXiPlusFromJets);
  double resXiMinusFromJets = (eventData_.xiMinusFromJets_ - eventData_.xiGenMinus_)/eventData_.xiGenMinus_;
  histos_.h_ResXiMinusFromJets_->Fill(resXiMinusFromJets);
  double resXiPlusFromPFCands = (eventData_.xiPlusFromPFCands_ - eventData_.xiGenPlus_)/eventData_.xiGenPlus_;
  histos_.h_ResXiPlusFromPFCands_->Fill(resXiPlusFromPFCands);
  double resXiMinusFromPFCands = (eventData_.xiMinusFromPFCands_ - eventData_.xiGenMinus_)/eventData_.xiGenMinus_;
  histos_.h_ResXiMinusFromPFCands_->Fill(resXiMinusFromPFCands);

  histos_.h_xiPlusVsxiGenPlus_->Fill(eventData_.xiGenPlus_,eventData_.xiPlusFromPFCands_);
  histos_.h_xiMinusVsxiGenMinus_->Fill(eventData_.xiGenMinus_,eventData_.xiMinusFromPFCands_);

  // Fill TTree
  if(saveTTree_) data_->Fill();
}

DEFINE_FWK_MODULE(ExclusiveDijetsTTreeAnalyzer);
