#ifndef ExclusiveDijetsAnalysis_Plotter_h
#define ExclusiveDijetsAnalysis_Plotter_h

#include "TDirectory.h"

#include "PlottingTools.h"

#include <vector>
#include <map>

template <class NormPolicy=DefaultNorm>
class Plotter{
   public:
      typedef std::pair<std::string,std::pair<std::string,TDirectory*> > VarDesc;
      typedef std::map<std::string, std::vector<VarDesc> > VarMap;

      Plotter(): verbose_(false),rebin_(1) {}
    
      void SetVerbose(bool verbose) {verbose_ = verbose;}
      void SetRebin(int rebin) {rebin_ = rebin;}

      void plot(std::vector<std::string>&, std::vector<std::pair<std::string,TDirectory*> >&, const char* drawOption = "");
      void plot(std::map<std::string,std::vector<std::string> >&, TDirectory*, const char* drawOption = "");
      
      void plot(VarMap& variablesMap, const char* drawOption = "");
   private:
      bool verbose_;
      int rebin_;
      //NormPolicy norm_;
};

#include "Plotter.icc"

#endif
