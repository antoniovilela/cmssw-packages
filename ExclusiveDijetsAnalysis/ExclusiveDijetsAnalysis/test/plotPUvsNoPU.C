#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include "plotTools.h"

#include <iostream>
#include <vector>

void plot(){
   TFile* file = TFile::Open("~/scratch0/data/analysisDijets_PAT_CEPDijets_M100_histos.root");

   //TDirectory* dir_0PU = file->GetDirectory("edmDumpAnalysis_filter0PU_singleVertexFilter");
   //TDirectory* dir_AvePU = file->GetDirectory("edmDumpAnalysis_singleVertexFilter");
   TDirectory* dir_0PU = file->GetDirectory("edmDumpAnalysis_filter0PU");
   TDirectory* dir_AvePU = file->GetDirectory("edmDumpAnalysis");

   double sigma = 250.; //pb

   TFile* file_eff = TFile::Open("root/analysis_histos_CPEDijets.root");

   const TTree* data_before = static_cast<const TTree*>(file_eff->Get("analysisBeforeSelection/data"));
   const TTree* data_after = static_cast<const TTree*>(file_eff->Get("analysisAfterSelection/data"));

   double eff_HLT = (double)data_after->GetEntries()/(double)data_before->GetEntries();

   cout << " Efficiency for HLT selection: " << eff_HLT << endl;

   const TH1F* h_leadingJetPt_AvePU = static_cast<const TH1F*>(dir_AvePU->Get("leadingJetPt"));

   double nEvents = h_leadingJetPt_AvePU->GetEntries();
   double scale = sigma*eff_HLT/nEvents;

   std::vector<string> refVar;
   refVar.push_back("leadingJetPt");
   refVar.push_back("leadingJetEta");
   refVar.push_back("leadingJetPhi");
   refVar.push_back("secondJetPt");
   refVar.push_back("secondJetEta");
   refVar.push_back("secondJetPhi");
   refVar.push_back("jetsDeltaEta");
   refVar.push_back("jetsDeltaPhi");
   refVar.push_back("trackMultiplicity");
   refVar.push_back("multiplicityHFPlus");
   refVar.push_back("multiplicityHFMinus");
   //refVar.push_back("xiGenPlus");
   //refVar.push_back("xiGenMinus");
   refVar.push_back("xiPlus");
   refVar.push_back("xiMinus");
   refVar.push_back("massDijets");
   refVar.push_back("missingMassFromXi");
   refVar.push_back("MxFromJets");
   refVar.push_back("RjjFromJets");
   refVar.push_back("RjjFromPFCands");
   refVar.push_back("xiPlusAfterSel");
   refVar.push_back("xiMinusAfterSel");
   refVar.push_back("RjjAfterSel");
   refVar.push_back("RjjFromPFAfterSel");

   /*std::vector<TCanvas*> canvasesVar(refVar.size());
   for(size_t k = 0; k < canvasesVar.size(); ++k) canvasesVar[k] = new TCanvas(refVar[k].c_str(),refVar[k].c_str());

   std::vector<TH1F*> histos_0PU(refVar.size());
   std::vector<TH1F*> histos_AvePU(refVar.size());
   for(size_t k = 0; k < refVar.size(); ++k){
      histos_0PU[k] = getHisto(dir_0PU,refVar[k]);
      histos_AvePU[k] = getHisto(dir_AvePU,refVar[k]); 

      scaleHisto(histos_AvePU[k],scale,1,kBlack);
      scaleHisto(histos_0PU[k],scale,2,kRed); 

      canvasesVar[k]->cd();
      histos_AvePU[k]->Draw();
      histos_0PU[k]->Draw("same");
   }*/

   std::vector<std::pair<std::string,TDirectory*> > dirs;
   dirs.push_back(std::make_pair("<N_{PU}> = 2.1",dir_AvePU));
   dirs.push_back(std::make_pair("0 PU",dir_0PU));  

   plot(refVar,dirs,false);
}
