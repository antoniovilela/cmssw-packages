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

      Plotter(): verbose_(false) {}
    
      void SetVerbose(bool verbose) {verbose_ = verbose;}

      void plot(std::vector<std::string>&, std::vector<std::pair<std::string,TDirectory*> >&);
      void plot(std::map<std::string,std::vector<std::string> >&, TDirectory*);
      
      void plot(VarMap& variablesMap);
   private:
      bool verbose_;
      //NormPolicy norm_;
};

template <class NormPolicy>
void Plotter<NormPolicy>::plot(VarMap& variablesMap){

   std::map<std::string,TCanvas*> canvasesVar;
   std::map<std::string,TLegend*> legendsVar;
   for(VarMap::iterator cat = variablesMap.begin(); cat != variablesMap.end(); ++cat){
      const std::string& catName = cat->first;
      canvasesVar[catName] = new TCanvas(catName.c_str(),catName.c_str());
      canvasesVar[catName]->cd();

      legendsVar[catName] = new TLegend(0.4,0.7,0.95,0.8);
    
      std::vector<VarDesc>& vars = cat->second;
      size_t index = 0;
      for(std::vector<VarDesc>::iterator var = vars.begin(); var != vars.end(); ++var,++index){

         const std::string& name = var->first;
         const std::string& desc = var->second.first;         
         TDirectory* dir = var->second.second;
 
         if(verbose_) std::cout << " Accessing " << name << " in " << dir->GetName() << std::endl;
  
         TH1F* histo = getHisto(dir,name);

         scaleHisto(histo,1,1,(index + 1));

         legendsVar[catName]->AddEntry(histo,desc.c_str(),"L");
         if(var == vars.begin()) histo->DrawNormalized("",NormPolicy::GetNorm(histo));   
         else histo->DrawNormalized("same",NormPolicy::GetNorm(histo));
      }
      legendsVar[catName]->SetFillColor(0);
      legendsVar[catName]->Draw("same");
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

template <class NormPolicy>
void Plotter<NormPolicy>::plot(std::map<std::string,std::vector<std::string> >& variables, TDirectory* dir){
   VarMap variablesMap;
   for(std::map<std::string,std::vector<std::string> >::const_iterator var = variables.begin();
                                                                       var != variables.end(); ++var){

      std::vector<VarDesc> varVector;
      for(std::vector<std::string>::const_iterator var_name = var->second.begin(); var_name != var->second.end(); ++var_name){
         varVector.push_back(std::make_pair(*var_name,std::make_pair(*var_name,dir)));
      }
      variablesMap[var->first] = varVector;
   }
   plot(variablesMap);
}
