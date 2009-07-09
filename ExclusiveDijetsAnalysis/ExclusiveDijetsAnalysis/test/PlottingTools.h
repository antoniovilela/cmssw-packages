#ifndef PlottingTools_h
#define PlottingTools_h

class TFile;
class TDirectory;

#include "TH1F.h"

#include <vector>
#include <string>
#include <map>

class DefaultNorm{
   public:
      DefaultNorm() {}

      double GetNormFactor(const TH1F* hist) {return hist->GetSumOfWeights();}
      double operator()(const TH1F* hist) {return GetNormFactor(hist);}
   private:
};

class ConstNorm{
   public:
      ConstNorm(double norm=1.):normalization_(norm) {}

      double GetNormFactor(const TH1F* hist) {return normalization_;}
      double operator()(const TH1F* hist) {return GetNormFactor(hist);}    
   private:
      double normalization_; 
};

class NumberEntriesNorm{
   public:
      NumberEntriesNorm() {}

      double operator()(const TH1F* hist) {return hist->GetSumOfWeights()/hist->GetEntries();}
   private:
};

TH1F* getHisto(TFile*, const std::string&); 
TH1F* getHisto(TDirectory*, const std::string&);
void scaleHisto(TH1F* histo, double scale = 1., int line = 1, int color = 1, int rebin = 1);
std::map<std::string,std::vector<std::string> > buildVarMap(const std::vector<std::string>& varNames,const std::vector<std::string>& triggerBits);

template<typename KeyType,typename ValueType>
std::map<KeyType,ValueType> makeMap(const std::vector<KeyType>& keys,const std::vector<ValueType>& values){
   std::map<KeyType,ValueType> res;
   typename std::vector<KeyType>::const_iterator key = keys.begin();
   typename std::vector<ValueType>::const_iterator value = values.begin(); 
   for(; key != keys.end() && value != values.end(); ++key,++value) res[*key] = *value;

   return res;
}

void plot(std::map<std::string,std::vector<std::string> >& variablesMap, TDirectory* dir, bool Norm = false);

//template <class NormPolicy>
//void plot(std::vector<std::string>& variables, std::vector<std::pair<std::string,TDirectory*> >& directories, NormPolicy& norm);
#endif
