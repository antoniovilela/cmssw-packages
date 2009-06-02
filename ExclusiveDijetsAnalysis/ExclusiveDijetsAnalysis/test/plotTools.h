class TH1F;
class TFile;
class TDirectory;

#include <vector>
#include <string>
#include <map>

TH1F* getHisto(TFile*, const string&);
TH1F* getHisto(TDirectory*, const string&);
void scaleHisto(TH1F* histo, double scale = 1., int line = 1, int color = 1, int rebin = 1);
std::map<std::string,std::vector<std::string> > buildVarMap(const std::vector<std::string>& varNames,const std::vector<std::string>& triggerBits);

/*template<typename KeyType,typename ValueType>
std::map<KeyType,ValueType> makeMap(const std::vector<KeyType>& keys,const std::vector<ValueType>& values);*/

void plot(std::map<std::string,std::vector<std::string> >& variablesMap, TDirectory* dir, bool Norm = false);
void plot(std::vector<std::string>& variables, std::vector<std::pair<std::string,TDirectory*> >& directories, bool Norm = false);
