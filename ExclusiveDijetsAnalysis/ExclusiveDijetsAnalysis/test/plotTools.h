//class TH1F;
class TFile;
class TDirectory;

#include "TH1F.h"

#include <vector>
#include <string>
#include <map>

// Leave this for now here
class BaseNorm{
   public:
      BaseNorm() {}
      virtual ~BaseNorm() {}
      virtual double GetNormFactor(const TH1F*) = 0;
   private:
};

class DefaultNorm: public BaseNorm{
   public:
      DefaultNorm() {}

      double GetNormFactor(const TH1F* hist) {return hist->GetSumOfWeights();}
   private:
};

class ConstNorm: public BaseNorm{
   public:
      ConstNorm(double norm=1.):BaseNorm(),normalization_(norm) {}

      double GetNormFactor(const TH1F* hist) {return normalization_;}   
   private:
      double normalization_; 
};

TH1F* getHisto(TFile*, const std::string&);
TH1F* getHisto(TDirectory*, const std::string&);
void scaleHisto(TH1F* histo, double scale = 1., int line = 1, int color = 1, int rebin = 1);
std::map<std::string,std::vector<std::string> > buildVarMap(const std::vector<std::string>& varNames,const std::vector<std::string>& triggerBits);

/*template<typename KeyType,typename ValueType>
std::map<KeyType,ValueType> makeMap(const std::vector<KeyType>& keys,const std::vector<ValueType>& values);*/

void plot(std::map<std::string,std::vector<std::string> >& variablesMap, TDirectory* dir, bool Norm = false);

void plot(std::vector<std::string>& variables, std::vector<std::pair<std::string,TDirectory*> >& directories, BaseNorm* norm=0);
