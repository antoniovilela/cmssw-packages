#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TDirectory.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TTree.h"

//#include "plotTools.h"
#include "PlottingTools.h"

#include <iostream>
#include <vector>
#include <map>

/*TH1F* getHisto(TFile*, const string&);
TH1F* getHisto(TDirectory*, const string&);
void scaleHisto(TH1F* histo, double scale = 1., int line = 1, int color = 1, int rebin = 1);*/

void plot(std::vector<string>& refVar, std::vector<std::string>& stackVars, std::map<std::string,TDirectory*>& dirMap, std::map<std::string,double>& scaleMap,bool Norm = false, int rebin = 1);

//template<typename KeyType,typename ValueType>
//std::map<KeyType,ValueType> makeMap(const std::vector<KeyType>& keys,const std::vector<ValueType>& values);

/*struct Result{
   double eff_HLT_;
   double nEvents_;
   double scale_;
};*/

void plot(bool Norm = false,int rebin = 1){

   std::vector<std::string> samples;
   /*samples.push_back("signal");
   samples.push_back("SDPlusDijets");
   samples.push_back("SDMinusDijets");
   samples.push_back("QCD100to250-madgraph");
   samples.push_back("DPEDijets");*/
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
   /*files.push_back(TFile::Open("~/scratch0/data/analysisDijets_PAT_CEPDijets_M100_histos.root"));
   files.push_back(TFile::Open("~/scratch0/data/analysisDijets_PAT_SDPlusDijets_Pt30_histos.root")); 
   files.push_back(TFile::Open("~/scratch0/data/analysisDijets_PAT_SDMinusDijets_Pt30_histos.root"));
   files.push_back(TFile::Open("~/scratch0/data/analysisDijets_PAT_QCD100to250-madgraph_histos.root"));
   files.push_back(TFile::Open("~/scratch0/data/analysisDijets_PAT_DPEDijets_Pt40_histos.root"));*/

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
         directories.push_back((*file)->GetDirectory("edmDumpAnalysis_singleVertexFilter"));
         directoriesHLT.push_back((*file)->GetDirectory("edmDumpAnalysis"));
      }
   }

   std::vector<double> sigmas;
   sigmas.push_back(250.);
   sigmas.push_back(0.12*1.1*300000./2);
   sigmas.push_back(0.12*1.1*300000./2);
   sigmas.push_back(0.12*1.1*15000000.);
   sigmas.push_back(4600.);

   std::vector<TFile*> filesEff;
   filesEff.push_back(TFile::Open("root/analysis_histos_CPEDijets.root"));
   filesEff.push_back(TFile::Open("root/analysis_histos_SDDijets.root"));
   filesEff.push_back(TFile::Open("root/analysis_histos_SDDijets.root"));
   filesEff.push_back(TFile::Open("root/analysis_histos_QCD100-madgraph.root"));
   filesEff.push_back(TFile::Open("root/analysis_histos_DPEDijets.root"));

   std::map<std::string,TDirectory*> dirMap = makeMap(samples,directories);
   std::map<std::string,TDirectory*> dirMapHLT = makeMap(samples,directoriesHLT);
   std::map<std::string,double> sigmaMap = makeMap(samples,sigmas);
   std::map<std::string,TFile*> fileEffMap = makeMap(samples,filesEff);
 
   for(std::map<std::string,double>::const_iterator it = sigmaMap.begin(); it != sigmaMap.end(); ++it)
               std::cout << " Cross-section " << it->first << ": " << it->second << std::endl;

   std::map<std::string,double> scaleMap;
   for(std::map<std::string,TFile*>::const_iterator it = fileEffMap.begin(); it != fileEffMap.end(); ++it){
      const TTree* data_before = static_cast<const TTree*>(it->second->Get("analysisBeforeSelection/data"));
      const TTree* data_after = static_cast<const TTree*>(it->second->Get("analysisAfterSelection/data"));

      double eff_HLT = (double)data_after->GetEntries()/(double)data_before->GetEntries();

      std::cout << " Efficiency for HLT selection " << it->first << ": " << eff_HLT << std::endl;

      const TH1F* h_leadingJetPt_HLT = static_cast<const TH1F*>(dirMapHLT[it->first]->Get("leadingJetPt"));
      const TH1F* h_leadingJetPt_Vtx = static_cast<const TH1F*>(dirMap[it->first]->Get("leadingJetPt"));
      
      double eff_Vtx = (double)h_leadingJetPt_Vtx->GetEntries()/(double)h_leadingJetPt_HLT->GetEntries();
 
      std::cout << " Efficiency for Vertex selection " << it->first << ": " << eff_Vtx << std::endl;
 
      //double nEvents = h_leadingJetPt_HLT->GetEntries();
      double nEvents = h_leadingJetPt_Vtx->GetEntries();
      double scale = sigmaMap[it->first]*eff_HLT*eff_Vtx/nEvents;
      //double scaleMap = 1./nEvents_back;
      scaleMap[it->first] = scale;

      std::cout << " N events after L1+HLT+Vtx " << it->first << ": " << nEvents << std::endl;
   }

   std::vector<string> refVar;
   /*refVar.push_back("leadingJetPt");
   refVar.push_back("leadingJetEta");
   refVar.push_back("leadingJetPhi");
   refVar.push_back("secondJetPt");
   refVar.push_back("secondJetEta");
   refVar.push_back("secondJetPhi");
   refVar.push_back("jetsDeltaEta");
   refVar.push_back("jetsDeltaPhi");
   refVar.push_back("trackMultiplicity");
   refVar.push_back("multiplicityHFPlus");
   refVar.push_back("multiplicityHFMinus");
   refVar.push_back("xiPlus");
   refVar.push_back("xiMinus");
   refVar.push_back("massDijets");
   refVar.push_back("missingMassFromXi");
   refVar.push_back("MxFromJets");*/
   refVar.push_back("RjjFromJets");
   refVar.push_back("RjjFromPFCands");
   refVar.push_back("xiPlusAfterSel");
   refVar.push_back("xiMinusAfterSel");
   refVar.push_back("RjjAfterSel");
   refVar.push_back("RjjFromPFAfterSel");

   std::vector<std::string> stackVars;
   stackVars.push_back("RjjFromJets");
   stackVars.push_back("RjjFromPFCands");
   stackVars.push_back("RjjAfterSel");
   stackVars.push_back("RjjFromPFAfterSel");

   plot(refVar,stackVars,dirMap,scaleMap,Norm,rebin);
}

void plot(std::vector<string>& refVar, std::vector<std::string>& stackVars, std::map<std::string,TDirectory*>& dirMap, std::map<std::string,double>& scaleMap, bool Norm, int rebin){
   std::map<std::string,TCanvas*> canvasesVar;
   std::map<std::string,TLegend*> legendsVar;
   //for(size_t k = 0; k < canvasesVar.size(); ++k) canvasesVar[k] = new TCanvas(refVar[k].c_str(),refVar[k].c_str());
   for(std::vector<std::string>::const_iterator var = refVar.begin(); var != refVar.end(); ++var){
      canvasesVar[*var] = new TCanvas(var->c_str(),var->c_str());
      legendsVar[*var] = new TLegend(0.4,0.7,0.95,0.8);
   }

   std::map<std::string,TCanvas*> canvasesStack;
   //std::map<std::string,TLegend*> legendsStack;
   //for(size_t k = 0; k < canvasesStack.size(); ++k) canvasesStack[k] = new TCanvas(("c_stack_" + stackVars[k]).c_str(),stackVars[k].c_str());
   for(std::vector<std::string>::const_iterator var = stackVars.begin(); var != stackVars.end(); ++var){   
      canvasesStack[*var] = new TCanvas(("c_stack_" + *var).c_str(),var->c_str());
      //legendsStack[*var] = new TLegend(0.4,0.7,0.95,0.8);
   }

   std::map<std::string,TH1F*> histos_signal;
   std::map<std::string,std::map<std::string,TH1F*> > histos_back;
   std::map<std::string,TH1F*> defmap;
   int colors[5] = {kRed,kBlue,kOrange,kMagenta,kYellow};
   int styles[5] = {3004,3005,3006,3007,3013};
   //for(size_t k = 0; k < refVar.size(); ++k){
   for(std::vector<std::string>::const_iterator var = refVar.begin(); var != refVar.end(); ++var){
      const std::string& varName = *var; 
      histos_signal[varName] = getHisto(dirMap["signal"],varName);
      scaleHisto(histos_signal[varName],scaleMap["signal"],1,kBlack,rebin);
      legendsVar[varName]->AddEntry(histos_signal[varName],"CEP di-jets","L");
 
      canvasesVar[varName]->cd();
      if(Norm) histos_signal[varName]->DrawNormalized("",histos_signal[varName]->GetSumOfWeights()/histos_signal[varName]->Integral(0,histos_signal[varName]->GetNbinsX() + 1));
      else histos_signal[varName]->Draw();
      int index = 0;
      for(std::map<std::string,TDirectory*>::const_iterator it = dirMap.begin(); it != dirMap.end(); ++it,++index){
         if(it->first == "signal") continue;
         if(histos_back.find(it->first) == histos_back.end()) histos_back[it->first] = defmap;
         histos_back[it->first][varName] = getHisto(it->second,varName); 
         scaleHisto(histos_back[it->first][varName],scaleMap[it->first],1,colors[index],rebin);
         histos_back[it->first][varName]->SetFillColor(colors[index]);
         histos_back[it->first][varName]->SetFillStyle(styles[index]);
         legendsVar[varName]->AddEntry(histos_back[it->first][varName],it->first.c_str(),"L"); 
         if(Norm) histos_back[it->first][varName]->DrawNormalized("same",histos_back[it->first][varName]->GetSumOfWeights()/histos_back[it->first][varName]->Integral(0,histos_back[it->first][varName]->GetNbinsX() + 1));
         else histos_back[it->first][varName]->Draw("same");
      }
      legendsVar[varName]->SetFillColor(0);
      legendsVar[varName]->Draw("same");
   }

   std::map<std::string,THStack*> stacks;
   //for(size_t k = 0; k < stackVars.size(); ++k){  
   for(std::vector<std::string>::const_iterator var = stackVars.begin(); var != stackVars.end(); ++var){
      const std::string& varName = *var;
      TH1F* histo_signal = histos_signal[varName];

      stacks[varName] = new THStack(("stack_" + varName).c_str(),varName.c_str());
      //stacks[varName]->Add(histo_signal);
      
      for(std::map<std::string,std::map<std::string,TH1F*> >::iterator it = histos_back.begin();
                                                                       it != histos_back.end(); ++it){
         std::map<std::string,TH1F*>& histoMap = it->second;
         stacks[varName]->Add(histoMap[varName]);
      }

      stacks[varName]->Add(histo_signal);

      canvasesStack[varName]->cd();
      stacks[varName]->Draw();
      legendsVar[varName]->Draw("same");
   }
}

/*template<typename KeyType,typename ValueType>
std::map<KeyType,ValueType> makeMap(const std::vector<KeyType>& keys,const std::vector<ValueType>& values){
   std::map<KeyType,ValueType> res;
   typename std::vector<KeyType>::const_iterator key = keys.begin();
   typename std::vector<ValueType>::const_iterator value = values.begin(); 
   for(; key != keys.end() && value != values.end(); ++key,++value) res[*key] = *value;

   return res;
}*/
