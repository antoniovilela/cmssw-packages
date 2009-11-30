#include <TFile.h>
#include "TDirectory.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include "PlottingTools.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

std::map<std::string,std::vector<double> > getEvents(std::vector<string>& vars, std::map<std::string,TDirectory*>& dirMap);

std::vector<std::pair<std::string,TFile*> > getAllSamples(){
   std::vector<std::pair<std::string,TFile*> > files;
   /*files.push_back(std::make_pair("CEP di-jets",TFile::Open("analysisDijetsTTree_histos_CEPDijets_M100_noPU_nHFMax_0.root")));
   files.push_back(std::make_pair("SD-plus di-jets",TFile::Open("analysisDijetsTTree_histos_SDPlusDijets_Pt30_nHFMax_0.root")));
   files.push_back(std::make_pair("SD-minus di-jets",TFile::Open("analysisDijetsTTree_histos_SDMinusDijets_Pt30_nHFMax_0.root")));
   files.push_back(std::make_pair("QCD non-diffractive",TFile::Open("analysisDijetsTTree_histos_QCD100to250-madgraph_nHFMax_0.root")));
   files.push_back(std::make_pair("DPE di-jets",TFile::Open("analysisDijetsTTree_histos_DPEDijets_Pt40_noPU_nHFMax_0.root")));*/
   files.push_back(std::make_pair("CEP di-jets",TFile::Open("analysisDijetsTTree_histos_CEPDijets_M100_noPU.root"))); 
   files.push_back(std::make_pair("SD-plus di-jets",TFile::Open("analysisDijetsTTree_histos_SDPlusDijets_Pt30_noPU.root")));
   files.push_back(std::make_pair("SD-minus di-jets",TFile::Open("analysisDijetsTTree_histos_SDMinusDijets_Pt30_noPU.root"))); 
   files.push_back(std::make_pair("DPE di-jets",TFile::Open("analysisDijetsTTree_histos_DPEDijets_Pt40_noPU.root")));
   files.push_back(std::make_pair("QCD non-diffractive",TFile::Open("analysisDijetsTTree_histos_QCD100to250-madgraph_noPU_AOD.root")));

   return files;
}

std::vector<std::pair<std::string,TFile*> > getSystSamples(){
   std::vector<std::pair<std::string,TFile*> > files;
   files.push_back(std::make_pair("Ref",TFile::Open("analysisDijetsTTree_histos_CEPDijets_M100_PU_ref.root")));
   files.push_back(std::make_pair("JES +10%",TFile::Open("analysisDijetsTTree_histos_CEPDijets_M100_PU_scaledJES1pt1.root")));
   files.push_back(std::make_pair("JES -10%",TFile::Open("analysisDijetsTTree_histos_CEPDijets_M100_PU_scaledJES0pt9.root")));

   return files;
}

void plot(){

   /*std::vector<std::string> samples;
   samples.push_back("signal");
   samples.push_back("SD-plus di-jets");
   samples.push_back("SD-minus di-jets");
   samples.push_back("QCD non-diffractive");
   samples.push_back("DPE di-jets");

   std::vector<TFile*> files;
   files.push_back(TFile::Open("analysisDijetsTTree_histos_CEPDijets_M100_noPU_nHFMax_0.root"));
   files.push_back(TFile::Open("analysisDijetsTTree_histos_SDPlusDijets_Pt30_nHFMax_0.root")); 
   files.push_back(TFile::Open("analysisDijetsTTree_histos_SDMinusDijets_Pt30_nHFMax_0.root"));
   files.push_back(TFile::Open("analysisDijetsTTree_histos_QCD100to250-madgraph_nHFMax_0.root"));
   files.push_back(TFile::Open("analysisDijetsTTree_histos_DPEDijets_Pt40_noPU_nHFMax_0.root"));

   std::vector<TDirectory*> directories;
   size_t index = 0;
   for(std::vector<TFile*>::const_iterator file = files.begin(); file != files.end(); ++file,++index){
      directories.push_back(static_cast<TDirectory*>(*file));
   }

   std::map<std::string,TDirectory*> dirMap = makeMap(samples,directories);*/
 
   std::vector<std::pair<std::string,TFile*> > files = getAllSamples();
   //std::vector<std::pair<std::string,TFile*> > files = getSystSamples();

   std::map<std::string,TDirectory*> dirMap;
   for(std::vector<std::pair<std::string,TFile*> >::const_iterator it = files.begin(); it != files.end(); ++it){
      dirMap[it->first] = static_cast<TDirectory*>(it->second);
   }
 
   std::vector<string> variablesForEff;
   variablesForEff.push_back("leadingJetPt");
   variablesForEff.push_back("leadingJetEta");
   variablesForEff.push_back("jetsAveEta");
   variablesForEff.push_back("massDijets");
   variablesForEff.push_back("RjjFromJets");
   variablesForEff.push_back("trackMultiplicity");
   variablesForEff.push_back("RjjFromJetsAfterSel");

   std::map<std::string,std::vector<double> > nEvents = getEvents(variablesForEff,dirMap);

    for(std::map<std::string,TDirectory*>::const_iterator it = dirMap.begin(); it != dirMap.end(); ++it){

      const TH1F* h_SelEff = static_cast<const TH1F*>(it->second->Get("SelectionEff"));
      const TH1F* h_leadingJetPt_dijetsSelection = static_cast<const TH1F*>(it->second->Get("leadingJetPt"));

      double eff_HLT = h_SelEff->GetBinContent(1);
      double eff_Vtx = h_SelEff->GetBinContent(2);
      double eff_dijetsSelection = h_SelEff->GetBinContent(3);

      std::cout << " Efficiency for HLT selection " << it->first << ": " << eff_HLT << std::endl;
      std::cout << " Efficiency for Vertex selection " << it->first << ": " << eff_Vtx << std::endl;
      std::cout << " Efficiency for Di-jets selection " << it->first << ": " << eff_dijetsSelection << std::endl;

      double nEventsAfterSel = h_leadingJetPt_dijetsSelection->GetEntries();
 
      std::vector<double>& myvec = nEvents[it->first];
      //myvec.insert(myvec.begin(),(double)h_leadingJetPt_HLT->GetEntries());
      //double nHLT = myvec[0];
      //std::transform(myvec.begin(),myvec.end(),myvec.begin(),std::bind1st(std::multiplies<double>(),eff_HLT/nHLT));

      std::transform(myvec.begin(),myvec.end(),myvec.begin(),std::bind1st(std::multiplies<double>(),eff_HLT*eff_Vtx*eff_dijetsSelection/nEventsAfterSel));
      myvec.insert(myvec.begin(),eff_HLT*eff_Vtx);
      myvec.insert(myvec.begin(),eff_HLT);
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
