#include "TROOT.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"
#include "Utilities/PlottingTools/interface/Plotter.h"

#include <iostream>
#include <vector>
#include <string>
#include <utility>

void plotEventSelection(char const* drawOption = ""){

  std::vector<std::string> variables;
  variables.push_back("EventSelection");

  std::string rootDir = "root/7TeV/Data/Run132605/eventSelection/test_031210";
  std::vector<std::pair<std::string,std::string> > fileNames;
  //fileNames.push_back(std::make_pair("All",rootDir + "/" + "analysisMinBiasTTree_MinimumBias_7TeV_minimumBiasTTreeAnalysisAll_histos.root"));
  //fileNames.push_back(std::make_pair("HLT",rootDir + "/" + "analysisMinBiasTTree_MinimumBias_7TeV_minimumBiasTTreeAnalysishltBscMinBiasORBptxPlusORMinus_histos.root"));
  //fileNames.push_back(std::make_pair("BPTX Coinc.",rootDir + "/" + "analysisMinBiasTTree_MinimumBias_7TeV_minimumBiasTTreeAnalysisBPTX_histos.root"));
  //fileNames.push_back(std::make_pair("BSC OR",rootDir + "/" + "analysisMinBiasTTree_MinimumBias_7TeV_minimumBiasTTreeAnalysisBscOr_histos.root"));
  fileNames.push_back(std::make_pair("Vertex",rootDir + "/" + "analysisMinBiasTTree_MinimumBias_7TeV_minimumBiasTTreeAnalysisVertexFilter_histos.root"));
  //fileNames.push_back(std::make_pair("Beam-halo veto",rootDir + "/" + "analysisMinBiasTTree_MinimumBias_7TeV_minimumBiasTTreeAnalysisBeamHaloVeto_histos.root"));
  //fileNames.push_back(std::make_pair("Scraping events",rootDir + "/" + "analysisMinBiasTTree_MinimumBias_7TeV_minimumBiasTTreeAnalysisFilterScraping_histos.root"));
  fileNames.push_back(std::make_pair("HB/HE Noise",rootDir + "/" + "analysisMinBiasTTree_MinimumBias_7TeV_minimumBiasTTreeAnalysisHBHENoiseFilter_histos.root"));

  std::vector<std::pair<std::string,TDirectory*> > dirs;
  for(size_t k = 0; k < fileNames.size(); ++k) 
     dirs.push_back( std::make_pair(fileNames[k].first,TFile::Open(fileNames[k].second.c_str(),"read") ) );

  Plotter<DefaultNorm> plotter;
  plotter.SetVerbose(true);
  plotter.SetStats(false);

  int markerstyles[] = {20,21,25,22,26,20,24,27};
  std::vector<int> histMarkerStyles(markerstyles,markerstyles + sizeof(markerstyles)/sizeof(int));
  plotter.SetMarkerStyles(histMarkerStyles);
  plotter.SetMarkerSizes(std::vector<float>(1,1.4));
  plotter.SetMarkerColors(std::vector<int>(1,1));
  plotter.SetLineColors(std::vector<int>(1,1));
  plotter.SetLineStyles(std::vector<int>(1,1));
  plotter.SetFillColors(std::vector<int>(1,0));
  plotter.SetFillStyles(std::vector<int>(1,0));
  plotter.plot(variables,dirs,drawOption);
}
