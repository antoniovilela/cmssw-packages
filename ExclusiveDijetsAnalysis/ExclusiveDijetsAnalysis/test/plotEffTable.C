#include <TFile.h>
#include "TDirectory.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

//#include "plotTools.h"
#include "PlottingTools.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

std::map<std::string,std::vector<double> > getEvents(std::vector<string>& vars, std::map<std::string,TDirectory*>& dirMap);

class Mult{
   public: 
      Mult(double x): val_(x) {}
      double operator()(double x) {return x*val_;}
   private:
      double val_;
};

void plot(){

   std::vector<std::string> samples;
   samples.push_back("signal");
   samples.push_back("SD-plus di-jets");
   samples.push_back("SD-minus di-jets");
   samples.push_back("QCD non-diffractive");
   samples.push_back("DPE di-jets");

   std::vector<TFile*> files;
   files.push_back(TFile::Open("root/analysisDijets_PAT_CEPDijets_M100_histos.root"));
   files.push_back(TFile::Open("root/analysisDijets_PAT_SDPlusDijets_Pt30-FastSim_histos.root")); 
   files.push_back(TFile::Open("root/analysisDijets_PAT_SDMinusDijets_Pt30-FastSim_histos.root"));
   files.push_back(TFile::Open("root/analysisDijets_PAT_QCD100to250-madgraph-FastSim_histos.root"));
   files.push_back(TFile::Open("root/analysisDijets_PAT_DPEDijets_Pt40_histos.root"));

   std::vector<TDirectory*> directories;
   std::vector<TDirectory*> directoriesHLT;
   size_t index = 0;
   std::vector<std::string> samplesFastSim;
   samplesFastSim.push_back("SD-plus di-jets");
   samplesFastSim.push_back("SD-minus di-jets");
   samplesFastSim.push_back("QCD non-diffractive");
   for(std::vector<TFile*>::const_iterator file = files.begin(); file != files.end(); ++file,++index){
      if(find(samplesFastSim.begin(),samplesFastSim.end(),samples[index]) != samplesFastSim.end()){
         directories.push_back((*file)->GetDirectory("edmDumpAnalysis_singleVtx"));
         directoriesHLT.push_back((*file)->GetDirectory("edmDumpAnalysis_hlt"));
      } else{
         //directories.push_back((*file)->GetDirectory("edmDumpAnalysis_singleVertexFilter"));
         directories.push_back((*file)->GetDirectory("edmDumpAnalysis_filter0PU_singleVertexFilter"));
         directoriesHLT.push_back((*file)->GetDirectory("edmDumpAnalysis_filter0PU"));
      }
   }

   std::vector<TFile*> filesEff;
   filesEff.push_back(TFile::Open("root/analysis_histos_CPEDijets.root"));
   filesEff.push_back(TFile::Open("root/analysis_histos_SDDijets.root"));
   filesEff.push_back(TFile::Open("root/analysis_histos_SDDijets.root"));
   filesEff.push_back(TFile::Open("root/analysis_histos_QCD100-madgraph.root"));
   filesEff.push_back(TFile::Open("root/analysis_histos_DPEDijets.root"));

   std::map<std::string,TDirectory*> dirMap = makeMap(samples,directories);
   std::map<std::string,TDirectory*> dirMapHLT = makeMap(samples,directoriesHLT);
   std::map<std::string,TFile*> fileEffMap = makeMap(samples,filesEff);
 
   std::vector<string> variablesForEff;
   variablesForEff.push_back("leadingJetPt");
   variablesForEff.push_back("leadingJetEta");
   variablesForEff.push_back("jetsAveEta");
   variablesForEff.push_back("massDijets");
   variablesForEff.push_back("RjjFromJets");
   variablesForEff.push_back("trackMultiplicity");
   variablesForEff.push_back("RjjAfterSel");

   std::map<std::string,std::vector<double> > nEvents = getEvents(variablesForEff,dirMap);

   for(std::map<std::string,TFile*>::const_iterator it = fileEffMap.begin(); it != fileEffMap.end(); ++it){
      const TTree* data_before = static_cast<const TTree*>(it->second->Get("analysisBeforeSelection/data"));
      const TTree* data_after = static_cast<const TTree*>(it->second->Get("analysisAfterSelection/data"));

      double eff_HLT = (double)data_after->GetEntries()/(double)data_before->GetEntries();
      //std::cout << " Efficiency for HLT selection " << it->first << ": " << eff_HLT << std::endl;

      const TH1F* h_leadingJetPt_HLT = static_cast<const TH1F*>(dirMapHLT[it->first]->Get("leadingJetPt"));
      const TH1F* h_leadingJetPt_Vtx = static_cast<const TH1F*>(dirMap[it->first]->Get("leadingJetPt"));
      
      double eff_Vtx = (double)h_leadingJetPt_Vtx->GetEntries()/(double)h_leadingJetPt_HLT->GetEntries();
      //std::cout << " Efficiency for Vertex selection " << it->first << ": " << eff_Vtx << std::endl;

      std::vector<double>& myvec = nEvents[it->first];
      myvec.insert(myvec.begin(),(double)h_leadingJetPt_HLT->GetEntries());
      double nHLT = myvec[0];
      std::transform(myvec.begin(),myvec.end(),myvec.begin(),Mult(eff_HLT/nHLT));
   }

   std::map<std::string,std::vector<double> >::const_iterator nEvents_end = nEvents.end();
   for(std::map<std::string,std::vector<double> >::const_iterator it = nEvents.begin(); it != nEvents_end; ++it){
      std::cout << " Efficiencies for " << it->first << std::endl;
      std::vector<double> const& myvec = it->second;
      for(size_t idx = 0; idx < myvec.size(); ++idx){
         size_t idx_previous = idx ? (idx - 1) : 0;
         std::cout << idx << ": " << myvec[idx] << "  wrt previous " << myvec[idx]/myvec[idx_previous] << std::endl;
      } 
   }
}

std::map<std::string,std::vector<double> > getEvents(std::vector<string>& vars, std::map<std::string,TDirectory*>& dirMap){

   std::map<std::string,std::vector<double> > nEvents; 
   std::vector<std::string>::const_iterator vars_end = vars.end(); 
   std::map<std::string,TDirectory*>::const_iterator dirs_begin = dirMap.begin();
   std::map<std::string,TDirectory*>::const_iterator dirs_end = dirMap.end();
   for(std::vector<std::string>::const_iterator var = vars.begin(); var != vars_end; ++var){
      const std::string& varName = *var; 
      for(std::map<std::string,TDirectory*>::const_iterator it = dirs_begin; it != dirs_end; ++it){
         if(nEvents.find(it->first) == nEvents.end()) nEvents[it->first] = std::vector<double>();
         TH1F* histo = getHisto(it->second,varName);
         nEvents[it->first].push_back(histo->GetEntries());
      }
   }

   return nEvents;
}
