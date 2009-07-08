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

template <class NormPolicy>
class Plotter{
   public:
      Plotter() {}
      void plot(std::vector<std::string>&, std::vector<std::pair<std::string,TDirectory*> >&);
   private:
      NormPolicy norm_;
};

template <class NormPolicy>
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

         histos[k]->DrawNormalized("same",norm_.GetNormFactor(histos[k]));
         histoMap[it->second] = histos;
      }
   }

   for(size_t k = 0; k < variables.size(); ++k){
      canvasesVar[k]->cd();
      legendsVar[k]->SetFillColor(0);
      legendsVar[k]->Draw("same");
   }
}
