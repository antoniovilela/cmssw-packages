#include "PlottingTools.h"

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

TH1F* getHisto(TFile* file, const std::string& refVar){
   TH1F* hist = static_cast<TH1F*>(file->Get(refVar.c_str()));

   if(!hist) std::cout << ">>>> Error: histogram " << refVar.c_str() << " not found." << std::endl; 

   return hist;
} 

TH1F* getHisto(TDirectory* dir, const std::string& refVar){
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
