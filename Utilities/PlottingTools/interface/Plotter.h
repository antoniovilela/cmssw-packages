#ifndef ExclusiveDijetsAnalysis_Plotter_h
#define ExclusiveDijetsAnalysis_Plotter_h

#include "TH1F.h"
#include "TDirectory.h"
#include "THStack.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"

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
      typedef std::pair<TH1F*,std::string> HistDesc;
      typedef std::map<std::string, std::vector<HistDesc> > HistMap;

      Plotter(): verbose_(false),rebin_(1) {}
    
      void SetVerbose(bool verbose) {verbose_ = verbose;}
      void SetRebin(int rebin) {rebin_ = rebin;}

      void plot(std::vector<std::string>&, std::vector<std::pair<std::string,TDirectory*> >&, const char* drawOption = "");
      void plot(std::vector<std::string>&, std::vector<std::pair<std::string,TDirectory*> >&, const std::vector<double>&, const char* drawOption = "");

      void plot(std::map<std::string,std::vector<std::string> >&, TDirectory*, const char* drawOption = "");
      void plot(VarMap& variablesMap, const char* drawOption = "");

      void plotStack(VarMap const& variablesMap, const char* drawOption = "", bool firstAsRefHist);

      void plotStack(std::vector<std::string>& variables, std::vector<std::pair<std::string,TDirectory*> >& directories, const std::vector<double>& normFactors, const char* drawOption = "");
      void plotComponents(std::vector<std::string>& variables, std::vector<std::pair<std::string,TDirectory*> >& directories, const std::vector<double>& normFactors, const char* drawOption = "");
      //void plotComponents(VarMap const& variablesMap, const char* drawOption = "");

   private:
      TH1F* loadHisto(VarDesc const&);
      std::map<std::string,std::vector<HistDesc> > loadHistos(VarMap const&);
      THStack* buildTHStack(std::string const&, std::vector<HistDesc> const&);
      VarMap makeVarMap(std::vector<std::string>&, std::vector<std::pair<std::string,TDirectory*> >&, const std::vector<double>&);

      bool verbose_;
      int rebin_;
      //NormPolicy norm_;
};

#include "Plotter.icc"

#endif