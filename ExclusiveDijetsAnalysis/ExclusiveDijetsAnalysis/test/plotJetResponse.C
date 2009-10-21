
#include "PlottingTools.h"
#include "Plotter.h"

#include <vector>
#include <string>

void plot(){
   std::vector<TDirectory*> dirs;
   //dirs.push_back(TFile::Open("analysisJetResponse_histos_IC5Calo.root"));
   //dirs.push_back(TFile::Open("analysisJetResponse_histos_KT4Calo.root"));
   //dirs.push_back(TFile::Open("analysisJetResponse_histos_KT6Calo.root"));
   dirs.push_back(TFile::Open("analysisJetResponse_histos_SC5Calo.root"));
   dirs.push_back(TFile::Open("analysisJetResponse_histos_SC7Calo.root"));
   //dirs.push_back(TFile::Open("analysisJetResponse_histos_IC5PF.root"));
   //dirs.push_back(TFile::Open("analysisJetResponse_histos_KT4PF.root"));
   dirs.push_back(TFile::Open("analysisJetResponse_histos_SC5PF.root"));
   dirs.push_back(TFile::Open("analysisJetResponse_histos_SC7PF.root"));
   //dirs.push_back(TFile::Open("analysisJetResponse_histos_KT6PF.root"));
   
   std::vector<std::string> labels;
   //labels.push_back("IC5 Calo");
   //labels.push_back("KT4 Calo");
   //labels.push_back("KT6 Calo");
   labels.push_back("SC5 Calo");
   labels.push_back("SC7 Calo");
   //labels.push_back("IC5 PF");
   //labels.push_back("KT4 PF");
   labels.push_back("SC5 PF");
   labels.push_back("SC7 PF");
   //labels.push_back("KT6 PF");
 
   std::vector<std::pair<std::string,TDirectory*> > dirsDesc;

   std::vector<TDirectory*>::const_iterator it_dir = dirs.begin();
   std::vector<std::string>::const_iterator it_label = labels.begin();

   std::vector<TDirectory*>::const_iterator dirs_end = dirs.end();
   for(; it_dir != dirs_end; ++it_dir, ++it_label){
      dirsDesc.push_back(std::make_pair(*it_label,*it_dir)); 
   }

   std::vector<string> variables;
   variables.push_back("meanRespVsPt_0");
   variables.push_back("meanRespVsPt_1");
   variables.push_back("meanRespVsPt_2");
   variables.push_back("meanRespVsPt_3");
   variables.push_back("sigmaRespVsPt_0");
   variables.push_back("sigmaRespVsPt_1");
   variables.push_back("sigmaRespVsPt_2");
   variables.push_back("sigmaRespVsPt_3");
   variables.push_back("meanRespVsEta_0");
   variables.push_back("meanRespVsEta_1");
   variables.push_back("meanRespVsEta_2");
   variables.push_back("sigmaRespVsEta_0");
   variables.push_back("sigmaRespVsEta_1");
   variables.push_back("sigmaRespVsEta_2");
   variables.push_back("sigmaEtaVsPt_0");
   variables.push_back("sigmaEtaVsPt_1");
   variables.push_back("sigmaEtaVsPt_2");
   variables.push_back("sigmaEtaVsPt_3");
   variables.push_back("sigmaPhiVsPt_0");
   variables.push_back("sigmaPhiVsPt_1");
   variables.push_back("sigmaPhiVsPt_2");
   variables.push_back("sigmaPhiVsPt_3");

   Plotter<DefaultNorm> plotter;
   plotter.plot(variables,dirsDesc);
}
