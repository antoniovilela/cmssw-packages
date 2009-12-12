#ifndef ExclusiveDijetsAnalysis_Plotter_h
#define ExclusiveDijetsAnalysis_Plotter_h

#include "TDirectory.h"

#include "PlottingTools.h"

#include <vector>
#include <map>

class VarDesc{
   public:
      VarDesc(const std::string& name, TDirectory* dir, const std::string& label, double norm): name_(name),dir_(dir),label_(label),norm_(norm) {}
      const std::string& name() const {return name_;}
      TDirectory* dir() const {return dir_;}
      const std::string& label() const {return label_;}
      double norm() const {return norm_;}
   private:
      std::string name_;
      TDirectory* dir_;
      std::string label_;
      double norm_;
};

template <class NormPolicy=DefaultNorm>
class Plotter{
   public:
      //typedef std::pair<std::string,std::pair<std::string,TDirectory*> > VarDesc;
      //typedef std::pair<std::string,std::pair<TDirectory*,std::pair<std::string,double> > > VarDesc;
      typedef std::map<std::string, std::vector<VarDesc> > VarMap;

      Plotter(): verbose_(false),rebin_(1) {}
    
      void SetVerbose(bool verbose) {verbose_ = verbose;}
      void SetRebin(int rebin) {rebin_ = rebin;}

      void plot(std::vector<std::string>&, std::vector<std::pair<std::string,TDirectory*> >&, const char* drawOption = "");
      void plot(std::vector<std::string>&, std::vector<std::pair<std::string,TDirectory*> >&, const std::vector<double>&, const char* drawOption = "");

      void plot(std::map<std::string,std::vector<std::string> >&, TDirectory*, const char* drawOption = "");
      
      void plot(VarMap& variablesMap, const char* drawOption = "");
   private:
      bool verbose_;
      int rebin_;
      //NormPolicy norm_;
};

#include "Plotter.icc"

#endif
