#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TDirectory.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TTree.h"

#include "PlottingTools.h"

#include <iostream>
#include <vector>
#include <map>

void plot(std::vector<string>& refVar, std::vector<std::string>& stackVars, std::map<std::string,TDirectory*>& dirMap, std::map<std::string,double>& scaleMap,bool Norm, int rebinFactor, bool variableBin, std::vector<int> const& groups);

TH1F* scaleHisto(TH1F* histo, double scale, int line, int color, int rebinFactor, bool variableBin, std::vector<int> const& groups){
   if(!variableBin){
      scaleHisto(histo,scale,line,color,rebinFactor);
      return 0;
   } else{ 
      scaleHisto(histo,scale,line,color,1);
      TH1F* histo_new = rebinHisto(histo,groups);
      return histo_new;
   }
}

void plot(bool Norm = false, int rebinFactor = 1, bool variableBin = false, std::vector<int> groups = std::vector<int>(0)){

   std::vector<std::string> samples;
   samples.push_back("signal");
   samples.push_back("SD-plus di-jets");
   samples.push_back("SD-minus di-jets");
   samples.push_back("QCD non-diffractive");
   samples.push_back("DPE di-jets");

   std::vector<TFile*> files;
   files.push_back(TFile::Open("analysisDijetsTTree_histos_CEPDijets_M100_noPU.root"));
   files.push_back(TFile::Open("analysisDijetsTTree_histos_SDPlusDijets_Pt30.root")); 
   files.push_back(TFile::Open("analysisDijetsTTree_histos_SDMinusDijets_Pt30.root"));
   files.push_back(TFile::Open("analysisDijetsTTree_histos_QCD100to250-madgraph.root"));
   files.push_back(TFile::Open("analysisDijetsTTree_histos_DPEDijets_Pt40_noPU.root"));

   std::vector<TDirectory*> directories;

   size_t index = 0;
   for(std::vector<TFile*>::const_iterator file = files.begin(); file != files.end(); ++file,++index){
      directories.push_back(static_cast<TDirectory*>(*file));
   }

   std::vector<double> sigmas;
   sigmas.push_back(250.);
   sigmas.push_back(300000./2);
   sigmas.push_back(300000./2);
   sigmas.push_back(15000000.);
   sigmas.push_back(4600.);

   std::map<std::string,TDirectory*> dirMap = makeMap(samples,directories);
   std::map<std::string,double> sigmaMap = makeMap(samples,sigmas);
 
   for(std::map<std::string,double>::const_iterator it = sigmaMap.begin(); it != sigmaMap.end(); ++it)
               std::cout << " Cross-section " << it->first << ": " << it->second << std::endl;

   std::map<std::string,double> scaleMap;
   for(std::map<std::string,TDirectory*>::const_iterator it = dirMap.begin(); it != dirMap.end(); ++it){

      const TH1F* h_SelEff = static_cast<const TH1F*>(it->second->Get("SelectionEff"));
      const TH1F* h_leadingJetPt_dijetsSelection = static_cast<const TH1F*>(it->second->Get("leadingJetPt"));

      double eff_HLT = h_SelEff->GetBinContent(1);
      double eff_Vtx = 1.;
      double eff_dijetsSelection = h_SelEff->GetBinContent(2);

      std::cout << " Efficiency for HLT selection " << it->first << ": " << eff_HLT << std::endl;
      std::cout << " Efficiency for Vertex selection " << it->first << ": " << eff_Vtx << std::endl;
      std::cout << " Efficiency for Di-jets selection " << it->first << ": " << eff_dijetsSelection << std::endl;

      double nEventsAfterSel = h_leadingJetPt_dijetsSelection->GetEntries();
      double scale = sigmaMap[it->first]*eff_HLT*eff_Vtx*eff_dijetsSelection/nEventsAfterSel;
      scaleMap[it->first] = scale;

      std::cout << " N events after L1 + HLT + Vtx + Di-jets selection " << it->first << ": " << nEventsAfterSel << std::endl;
   }

   std::vector<string> refVar;
   refVar.push_back("leadingJetPt");
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
   refVar.push_back("xiPlusFromPFCands");
   refVar.push_back("xiMinusFromPFCands");
   refVar.push_back("massDijets");
   refVar.push_back("missingMassFromXi");
   refVar.push_back("MxFromJets");
   refVar.push_back("xiPlusFromPFCandsAfterSel");
   refVar.push_back("xiMinusFromPFCandsAfterSel");
   refVar.push_back("RjjFromJets");
   refVar.push_back("RjjFromPFCands");
   refVar.push_back("RjjFromJetsAfterSel");
   refVar.push_back("RjjFromPFCandsAfterSel");

   std::vector<std::string> stackVars;
   stackVars.push_back("RjjFromJets");
   stackVars.push_back("RjjFromPFCands");
   stackVars.push_back("RjjFromJetsAfterSel");
   stackVars.push_back("RjjFromPFCandsAfterSel");

   //plot(refVar,stackVars,dirMap,scaleMap,Norm,ngroups,xbins);
   plot(refVar,stackVars,dirMap,scaleMap,Norm,rebinFactor,variableBin,groups);
}

void plot(std::vector<string>& refVar, std::vector<std::string>& stackVars, std::map<std::string,TDirectory*>& dirMap, std::map<std::string,double>& scaleMap, bool Norm, int rebinFactor, bool variableBin, std::vector<int> const& groups){
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
      TH1F* histo_new = scaleHisto(histos_signal[varName],scaleMap["signal"],1,kBlack,rebinFactor,variableBin,groups);
      if(histo_new) histos_signal[varName] = histo_new;
      legendsVar[varName]->AddEntry(histos_signal[varName],"CEP di-jets","L");
 
      canvasesVar[varName]->cd();
      if(Norm) histos_signal[varName]->DrawNormalized("HIST",histos_signal[varName]->GetSumOfWeights()/histos_signal[varName]->Integral(0,histos_signal[varName]->GetNbinsX() + 1));
      else histos_signal[varName]->Draw("HIST");
      int index = 0;
      for(std::map<std::string,TDirectory*>::const_iterator it = dirMap.begin(); it != dirMap.end(); ++it,++index){
         if(it->first == "signal") continue;
         if(histos_back.find(it->first) == histos_back.end()) histos_back[it->first] = defmap;
         histos_back[it->first][varName] = getHisto(it->second,varName); 
         TH1F* histo_new = scaleHisto(histos_back[it->first][varName],scaleMap[it->first],1,colors[index],rebinFactor,variableBin,groups);
         if(histo_new) histos_back[it->first][varName] = histo_new; 
         histos_back[it->first][varName]->SetFillColor(colors[index]);
         histos_back[it->first][varName]->SetFillStyle(styles[index]);
         legendsVar[varName]->AddEntry(histos_back[it->first][varName],it->first.c_str(),"L"); 
         if(Norm) histos_back[it->first][varName]->DrawNormalized("HIST SAME",histos_back[it->first][varName]->GetSumOfWeights()/histos_back[it->first][varName]->Integral(0,histos_back[it->first][varName]->GetNbinsX() + 1));
         else histos_back[it->first][varName]->Draw("HIST SAME");
      }
      legendsVar[varName]->SetFillColor(0);
      legendsVar[varName]->Draw("SAME");
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
      stacks[varName]->Draw("HIST");
      legendsVar[varName]->Draw("SAME");
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
