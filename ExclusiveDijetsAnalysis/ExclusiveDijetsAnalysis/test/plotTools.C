#include "plotTools.h"

#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TDirectory.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include <iostream>
#include <vector>
#include <map>

/*TH1F* getHisto(TFile*, const string&);
TH1F* getHisto(TDirectory*, const string&);
void scaleHisto(TH1F* histo, double scale = 1., int line = 1, int color = 1, int rebin = 1);
std::map<std::string,std::vector<std::string> > buildVarMap(const std::vector<std::string>& varNames,const std::vector<std::string>& triggerBits);

template<typename KeyType,typename ValueType>
std::map<KeyType,ValueType> makeMap(const std::vector<KeyType>& keys,const std::vector<ValueType>& values);

void plot(std::map<std::string,std::vector<std::string> >& variablesMap, TDirectory* dir, bool Norm = false);
void plot(std::vector<std::string>& variables, std::vector<std::pair<std::string,TDirectory*> >& directories, bool Norm = false);*/

void plot(std::map<std::string,std::vector<std::string> >& variablesMap, TDirectory* dir, bool Norm){
   std::map<std::string,TCanvas*> canvasesVar;
   std::map<std::string,TLegend*> legendsVar;
   for(std::map<std::string,std::vector<std::string> >::const_iterator cat = variablesMap.begin();
                                                                       cat != variablesMap.end(); ++cat){
      const std::string& name = cat->first;
      canvasesVar[name] = new TCanvas(name.c_str(),name.c_str());
      canvasesVar[name]->cd();

      legendsVar[name] = new TLegend(0.4,0.7,0.95,0.8);
    
      const std::vector<std::string>& vars = cat->second;
      size_t index = 0;
      for(std::vector<std::string>::const_iterator var = vars.begin(); var != vars.end(); ++var,++index){
         TH1F* histo = getHisto(dir,*var);

         scaleHisto(histo,1,1,(index + 1));
         legendsVar[name]->AddEntry(histo,var->c_str(),"L"); 
         if(Norm) histo->DrawNormalized("same");
         else histo->Draw("same");
      }
      legendsVar[name]->SetFillColor(0);
      legendsVar[name]->Draw("same");
   }
}

template <class NormPolicy>
void plot(std::vector<std::string>& variables, std::vector<std::pair<std::string,TDirectory*> >& directories, NormPolicy* norm){

   std::vector<TCanvas*> canvasesVar(variables.size());
   std::vector<TLegend*> legendsVar(variables.size());
   for(size_t k = 0; k < variables.size(); ++k){
      canvasesVar[k] = new TCanvas(variables[k].c_str(),variables[k].c_str());
      legendsVar[k] = new TLegend(0.4,0.7,0.95,0.8);
   }

   std::map<TDirectory*,std::vector<TH1F*> > histoMap;
   size_t index = 0;
   for(std::vector<std::pair<std::string,TDirectory*> >::iterator it = directories.begin(); it != directories.end(); ++it,++index){
      std::vector<TH1F*> histos(variables.size()); 
      for(size_t k = 0; k < variables.size(); ++k){
         canvasesVar[k]->cd();
      
         histos[k] = getHisto(it->second,variables[k]);

         scaleHisto(histos[k],1,1,(index + 1));

         legendsVar[k]->AddEntry(histos[k],it->first.c_str(),"L");
         /*//if(Norm) histos[k]->DrawNormalized("same",histos[k]->Integral()/histos[k]->GetEntries());
         //if(Norm) histos[k]->DrawNormalized("same");
         if(Norm) histos[k]->DrawNormalized("same",histos[k]->Integral()/histos[k]->GetBinContent(1));
         else histos[k]->Draw("same");
         //if(Norm) histos[k]->SetNormFactor(1./histos[k]->GetEntries());         
         //histos[k]->Draw("same");*/
         if(norm) histos[k]->DrawNormalized("same",norm->GetNormFactor(histos[k]));
         else histos[k]->Draw("same");
         histoMap[it->second] = histos;
      }
   }  

   for(size_t k = 0; k < variables.size(); ++k){
      canvasesVar[k]->cd();
      legendsVar[k]->SetFillColor(0);
      legendsVar[k]->Draw("same");
   }
}

std::map<std::string,std::vector<std::string> > buildVarMap(const std::vector<std::string>& varNames,const std::vector<std::string>& triggerBits){

  std::map<std::string,std::vector<std::string> > varMap;

  std::vector<std::string> def;
   for(std::vector<std::string>::const_iterator var = varNames.begin(); var != varNames.end(); ++var){
      varMap[*var] = def;
      std::vector<std::string>& variables = varMap[*var];
      variables.push_back(*var);
      for(std::vector<std::string>::const_iterator trigBit = triggerBits.begin(); trigBit != triggerBits.end(); ++trigBit){
         variables.push_back(*var + "_" + *trigBit);
      }
   }

   return varMap;
}

/*template<typename KeyType,typename ValueType>
std::map<KeyType,ValueType> makeMap(const std::vector<KeyType>& keys,const std::vector<ValueType>& values){
   std::map<KeyType,ValueType> res;
   typename std::vector<KeyType>::const_iterator key = keys.begin();
   typename std::vector<ValueType>::const_iterator value = values.begin(); 
   for(; key != keys.end() && value != values.end(); ++key,++value) res[*key] = *value;

   return res;
}*/

TH1F* getHisto(TFile* file, const string& refVar){
   TH1F* hist = static_cast<TH1F*>(file->Get(refVar.c_str()));

   if(!hist) std::cout << ">>>> Error: histogram " << refVar.c_str() << " not found." << std::endl; 

   return hist;
} 

TH1F* getHisto(TDirectory* dir, const string& refVar){
   TH1F* hist = static_cast<TH1F*>(dir->Get(refVar.c_str()));

   if(!hist) std::cout << ">>>> Error: histogram " << refVar.c_str() << " not found." << std::endl;

   return hist;
}

void scaleHisto(TH1F* histo, double scale, int line, int color, int rebin){

  histo->Scale(scale);

  histo->Rebin(rebin);
  histo->SetLineWidth(2);
  histo->SetLineStyle(line);
  histo->SetLineColor(color);

  histo->GetYaxis()->SetTitle("a.u.");
}
