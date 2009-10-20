
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
    exclusiveDijetsAnalysis::HistoMapTH1F histosTH1F_; // histos owned by Service
    exclusiveDijetsAnalysis::HistoMapTH2F histosTH2F_;
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

  bookHistos(histosTH1F_,*fs);
  bookHistos(histosTH2F_,*fs);
}

void ExclusiveDijetsTTreeAnalyzer::beginRun(const edm::Run& run, const edm::EventSetup& setup){
  exclusiveDijetsAnalysisImpl_.setBeginRun(run,setup);
}

void ExclusiveDijetsTTreeAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){

  exclusiveDijetsAnalysisImpl_.fillEventData(eventData_,event,setup);

  histosTH1F_["nPileUpBx0"]->Fill(eventData_.nPileUpBx0_);
  histosTH1F_["nVertex"]->Fill(eventData_.nVertex_); 

  histosTH1F_["leadingJetPt"]->Fill(eventData_.leadingJetPt_);
  histosTH1F_["secondJetPt"]->Fill(eventData_.secondJetPt_);
  histosTH1F_["leadingJetEta"]->Fill(eventData_.leadingJetEta_);
  histosTH1F_["secondJetEta"]->Fill(eventData_.secondJetEta_);
  histosTH1F_["leadingJetPhi"]->Fill(eventData_.leadingJetPhi_);
  histosTH1F_["secondJetPhi"]->Fill(eventData_.secondJetPhi_);  
  histosTH1F_["leadingJetBDiscriminator"]->Fill(eventData_.leadingJetBDiscriminator_);
  histosTH1F_["secondJetBDiscriminator"]->Fill(eventData_.secondJetBDiscriminator_);

  histosTH1F_["thirdJetPt"]->Fill(eventData_.thirdJetPt_);
  histosTH1F_["thirdJetEta"]->Fill(eventData_.thirdJetEta_);
  histosTH1F_["jetsAveEta"]->Fill(eventData_.jetsAveEta_);
  histosTH1F_["jetsDeltaEta"]->Fill(eventData_.jetsDeltaEta_);
  histosTH1F_["jetsDeltaPhi"]->Fill(eventData_.jetsDeltaPhi_);
  histosTH1F_["jetsDeltaPt"]->Fill(eventData_.jetsDeltaPt_);

  histosTH1F_["massDijets"]->Fill(eventData_.massDijets_);
  histosTH1F_["massDijetsGen"]->Fill(eventData_.massDijetsGen_);
  histosTH1F_["MxFromJets"]->Fill(eventData_.MxFromJets_);
  histosTH1F_["MxFromPFCands"]->Fill(eventData_.MxFromPFCands_);
  histosTH1F_["MxGen"]->Fill(eventData_.MxGen_);
  histosTH1F_["RjjFromJets"]->Fill(eventData_.RjjFromJets_);
  histosTH1F_["RjjFromPFCands"]->Fill(eventData_.RjjFromPFCands_);
  histosTH1F_["RjjGen"]->Fill(eventData_.RjjGen_);

  histosTH1F_["trackMultiplicity"]->Fill(eventData_.trackMultiplicity_);
  histosTH1F_["multiplicityHFPlus"]->Fill(eventData_.multiplicityHFPlus_);
  histosTH1F_["multiplicityHFMinus"]->Fill(eventData_.multiplicityHFMinus_); 
  
  histosTH1F_["xiGenPlus"]->Fill(eventData_.xiGenPlus_);
  histosTH1F_["xiGenMinus"]->Fill(eventData_.xiGenMinus_);
  histosTH1F_["xiTowerPlus"]->Fill(eventData_.xiTowerPlus_);
  histosTH1F_["xiTowerMinus"]->Fill(eventData_.xiTowerMinus_);
  histosTH1F_["xiPlusFromJets"]->Fill(eventData_.xiPlusFromJets_);
  histosTH1F_["xiMinusFromJets"]->Fill(eventData_.xiMinusFromJets_);
  histosTH1F_["xiPlusFromPFCands"]->Fill(eventData_.xiPlusFromPFCands_);
  histosTH1F_["xiMinusFromPFCands"]->Fill(eventData_.xiMinusFromPFCands_);
  histosTH1F_["missingMassFromXi"]->Fill(eventData_.missingMassFromXi_);

  //double resMassDijets = (eventData_.massDijets_ - eventData_.massDijetsGen_)/eventData_.massDijetsGen_;
  double resMassDijets = eventData_.massDijets_ - eventData_.massDijetsGen_;
  histosTH1F_["ResMassDijets"]->Fill(resMassDijets);

  double resMxFromJets = eventData_.MxFromJets_ - eventData_.MxGen_;
  histosTH1F_["ResMxFromJets"]->Fill(resMxFromJets);
  double resMxFromPFCands = eventData_.MxFromPFCands_ - eventData_.MxGen_;
  histosTH1F_["ResMxFromPFCands"]->Fill(resMxFromPFCands);
  
  double resRjjFromJets = eventData_.RjjFromJets_ - eventData_.RjjGen_;
  histosTH1F_["ResRjjFromJets"]->Fill(resRjjFromJets);
  double resRjjFromPFCands = eventData_.RjjFromPFCands_ - eventData_.RjjGen_;
  histosTH1F_["ResRjjFromPFCands"]->Fill(resRjjFromPFCands);

  //double resXiTowerPlus = (eventData_.xiTowerPlus_ - eventData_.xiGenPlus_)/eventData_.xiGenPlus_;
  double resXiTowerPlus = eventData_.xiTowerPlus_ - eventData_.xiGenPlus_;
  histosTH1F_["ResXiTowerPlus"]->Fill(resXiTowerPlus);
  //double resXiTowerMinus = (eventData_.xiTowerMinus_ - eventData_.xiGenMinus_)/eventData_.xiGenMinus_; 
  double resXiTowerMinus = eventData_.xiTowerMinus_ - eventData_.xiGenMinus_;
  histosTH1F_["ResXiTowerMinus"]->Fill(resXiTowerMinus);
  //double resXiPlusFromJets = (eventData_.xiPlusFromJets_ - eventData_.xiGenPlus_)/eventData_.xiGenPlus_;
  double resXiPlusFromJets = eventData_.xiPlusFromJets_ - eventData_.xiGenPlus_;
  histosTH1F_["ResXiPlusFromJets"]->Fill(resXiPlusFromJets);
  //double resXiMinusFromJets = (eventData_.xiMinusFromJets_ - eventData_.xiGenMinus_)/eventData_.xiGenMinus_;
  double resXiMinusFromJets = eventData_.xiMinusFromJets_ - eventData_.xiGenMinus_;
  histosTH1F_["ResXiMinusFromJets"]->Fill(resXiMinusFromJets);
  //double resXiPlusFromPFCands = (eventData_.xiPlusFromPFCands_ - eventData_.xiGenPlus_)/eventData_.xiGenPlus_;
  double resXiPlusFromPFCands = eventData_.xiPlusFromPFCands_ - eventData_.xiGenPlus_;
  histosTH1F_["ResXiPlusFromPFCands"]->Fill(resXiPlusFromPFCands);
  //double resXiMinusFromPFCands = (eventData_.xiMinusFromPFCands_ - eventData_.xiGenMinus_)/eventData_.xiGenMinus_;
  double resXiMinusFromPFCands = eventData_.xiMinusFromPFCands_ - eventData_.xiGenMinus_;
  histosTH1F_["ResXiMinusFromPFCands"]->Fill(resXiMinusFromPFCands);

  histosTH2F_["RjjFromJetsVsBDicriminator"]->Fill(eventData_.RjjFromJets_,eventData_.leadingJetBDiscriminator_);
  histosTH2F_["RjjFromPFCandsVsBDicriminator"]->Fill(eventData_.RjjFromPFCands_,eventData_.leadingJetBDiscriminator_);

  //histosTH2F_["iEtaVsHFCountPlus"]->Fill(ieta,nHFPlus_ieta);
  //histosTH2F_["iEtaVsHFCountMinus"]->Fill(ieta,nHFMinus_ieta);

  histosTH2F_["xiPlusVsxiGenPlus"]->Fill(eventData_.xiGenPlus_,eventData_.xiPlusFromPFCands_);
  histosTH2F_["xiMinusVsxiGenMinus"]->Fill(eventData_.xiGenMinus_,eventData_.xiMinusFromPFCands_);

  // Fill TTree
  if(saveTTree_) data_->Fill();
}

DEFINE_FWK_MODULE(ExclusiveDijetsTTreeAnalyzer);
