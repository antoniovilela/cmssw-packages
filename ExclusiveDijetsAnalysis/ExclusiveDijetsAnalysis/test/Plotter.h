#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TDirectory.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include "PlottingTools.h"

#include <iostream>
#include <vector>
#include <map>

template <class NormPolicy=DefaultNorm>
class Plotter{
   public:
      typedef std::pair<std::string,std::pair<std::string,TDirectory*> > VarDesc;
      typedef std::map<std::string, std::vector<VarDesc> > VarMap;

      Plotter() {}
    
      //void plot(std::map<std::string,std::vector<std::string> >&, TDirectory*);
      void plot(std::vector<std::string>&, std::vector<std::pair<std::string,TDirectory*> >&);
      
      void plot(VarMap& variablesMap);
   private:
      NormPolicy norm_;
};

/*template <class NormPolicy>
void Plotter<NormPolicy>::plot(std::map<std::string,std::vector<std::string> >& variablesMap, TDirectory* dir){
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
         histo->DrawNormalized("same",norm_(histos[k]));
         //if(Norm) histo->DrawNormalized("same");
         //else histo->Draw("same");
      }
      legendsVar[name]->SetFillColor(0);
      legendsVar[name]->Draw("same");
   }
}*/

template <class NormPolicy>
void Plotter<NormPolicy>::plot(VarMap& variablesMap){

   std::map<std::string,TCanvas*> canvasesVar;
   std::map<std::string,TLegend*> legendsVar;
   for(VarMap::iterator cat = variablesMap.begin(); cat != variablesMap.end(); ++cat){
      const std::string& name = cat->first;
      canvasesVar[name] = new TCanvas(name.c_str(),name.c_str());
      canvasesVar[name]->cd();

      legendsVar[name] = new TLegend(0.4,0.7,0.95,0.8);
    
      std::vector<VarDesc>& vars = cat->second;
      size_t index = 0;
      for(std::vector<VarDesc>::iterator var = vars.begin(); var != vars.end(); ++var,++index){

         const std::string& name = var->first;
         const std::string& desc = var->second.first;         
         TDirectory* dir = var->second.second;
 
         TH1F* histo = getHisto(dir,name);

         scaleHisto(histo,1,1,(index + 1));

         legendsVar[name]->AddEntry(histo,desc.c_str(),"L"); 
         histo->DrawNormalized("same",norm_(histo));
      }
      legendsVar[name]->SetFillColor(0);
      legendsVar[name]->Draw("same");
   }
}

template <class NormPolicy>
void Plotter<NormPolicy>::plot(std::vector<std::string>& variables, std::vector<std::pair<std::string,TDirectory*> >& directories){
   VarMap variablesMap;
   for(std::vector<std::string>::const_iterator var = variables.begin(); var != variables.end(); ++var){
      std::vector<VarDesc> varVector;
      for(std::vector<std::pair<std::string,TDirectory*> >::const_iterator dir = directories.begin(); dir != directories.end(); ++dir){
         varVector.push_back(std::make_pair(*var,*dir));
      }
      variablesMap[*var] = varVector;
   }   
   plot(variablesMap);  
}

/*template <class NormPolicy>
void Plotter<NormPolicy>::plot(std::vector<std::string>& variables, std::vector<std::pair<std::string,TDirectory*> >& directories){

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

         //histos[k]->DrawNormalized("same",norm_.GetNormFactor(histos[k]));
         histos[k]->DrawNormalized("same",norm_(histos[k]));
         histoMap[it->second] = histos;
      }
   }

   for(size_t k = 0; k < variables.size(); ++k){
      canvasesVar[k]->cd();
      legendsVar[k]->SetFillColor(0);
      legendsVar[k]->Draw("same");
   }
}*/
