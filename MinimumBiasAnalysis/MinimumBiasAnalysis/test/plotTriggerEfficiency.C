#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"

#include <iostream>
#include <string>
#include <vector>

void plotTriggerEfficiency(std::string const& fileName, std::string const& treeName){

  TFile* file = TFile::Open(fileName.c_str());

  TTree* data = static_cast<TTree*>(file->Get(treeName.c_str()));

  TH2F* hHLTBSCORVsTrackMult = new TH2F("HLTBSCORVsTrackMult","HLTBSCORVsTrackMult",50,0,50,300,0.,1.1);
  data->Project("HLTBSCORVsTrackMult","HLT_MinBiasBSCOR:trackMultiplicityAssociatedToPV");

  TProfile* profileHLTBSCORVsTrackMult = hHLTBSCORVsTrackMult->ProfileX();
  TCanvas* canvas1 = new TCanvas("canvas_HLTBSCORVsTrackMult","HLTBSCORVsTrackMult");
  profileHLTBSCORVsTrackMult->Draw();
}
