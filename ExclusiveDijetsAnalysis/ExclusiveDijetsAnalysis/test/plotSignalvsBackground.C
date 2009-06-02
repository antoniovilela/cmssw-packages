#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TDirectory.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include <iostream>
#include <vector>
#include <map>

TH1F* getHisto(TFile*, const string&);
TH1F* getHisto(TDirectory*, const string&);
void scaleHisto(TH1F* histo, double scale, int line, int color, char* title);

void plot(){

   TFile* file_signal = TFile::Open("root/analysisDijets_CEPDijets_M100_histos_PU_nHFMax_1.root");
   double sigma_signal = 450.; //pb
   std::cout << " Cross-section (signal): " << sigma_signal << std::endl;

   TFile* file_eff_signal = TFile::Open("root/analysis_histos_CPEDijets.root");
   const TTree* data_before_signal = static_cast<const TTree*>(file_eff_signal->Get("analysisBeforeSelection/data"));
   const TTree* data_after_signal = static_cast<const TTree*>(file_eff_signal->Get("analysisAfterSelection/data"));

   double eff_HLT_signal = (double)data_after_signal->GetEntries()/(double)data_before_signal->GetEntries();

   std::cout << " Efficiency for HLT selection (signal): " << eff_HLT_signal << std::endl;

   const TH1F* h_leadingJetPt_signal = static_cast<const TH1F*>(file_signal->Get("edmDumpAnalysis/leadingJetPt"));
   double nEvents_signal = h_leadingJetPt_signal->GetEntries();
   double scale_signal = sigma_signal*eff_HLT_signal/nEvents_signal;

   std::cout << " N events after L1+HLT (signal): " << nEvents_signal << std::endl;

   std::vector<std::string> backgrounds;
   backgrounds.push_back("DPEDijets");
   backgrounds.push_back("SDDijets");

   std::map<std::string,TFile*> files_back;
   files_back["DPEDijets"] = TFile::Open("root/analysisDijets_DPEDijets_Pt40_histos_PU_nHFMax_1.root");
   files_back["SDDijets"] = TFile::Open("root/analysisDijets_SDDijets_Pt30_histos_noPU_nHFMax_1.root");

   std::map<std::string,double> sigma_back;
   sigma_back["DPEDijets"] = 4600.;
   sigma_back["SDDijets"] = 0.12*1.2*300000.;
 
   for(std::map<std::string,double>::const_iterator it = sigma_back.begin(); it != sigma_back.end(); ++it)
               std::cout << " Cross-section " << it->first << ": " << it->second << std::endl;

   std::map<std::string,TFile*> files_eff_back;
   files_eff_back["DPEDijets"] = TFile::Open("root/analysis_histos_DPEDijets.root");
   files_eff_back["SDDijets"] = TFile::Open("root/analysis_histos_SDDijets.root");
   
   std::map<std::string,double> scale_back;
   for(std::map<std::string,TFile*>::const_iterator it = files_eff_back.begin(); it != files_eff_back.end(); ++it){
      const TTree* data_before_back = static_cast<const TTree*>(it->second->Get("analysisBeforeSelection/data"));
      const TTree* data_after_back = static_cast<const TTree*>(it->second->Get("analysisAfterSelection/data"));

      double eff_HLT = (double)data_after_back->GetEntries()/(double)data_before_back->GetEntries();

      std::cout << " Efficiency for HLT selection " << it->first << ": " << eff_HLT << std::endl;

      const TH1F* h_leadingJetPt_back = static_cast<const TH1F*>(files_back[it->first]->Get("edmDumpAnalysis/leadingJetPt"));

      double nEvents = h_leadingJetPt_back->GetEntries();
      double scale = sigma_back[it->first]*eff_HLT/nEvents;
      //double scale_back = 1./nEvents_back;
      scale_back[it->first] = scale;

      std::cout << " N events after L1+HLT " << it->first << ": " << nEvents << std::endl;
   }

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
   refVar.push_back("xiPlus");
   refVar.push_back("xiMinus");
   refVar.push_back("massDijets");
   refVar.push_back("missingMassFromXi");
   refVar.push_back("MxFromJets");
   refVar.push_back("RjjFromJets");
   refVar.push_back("xiPlusAfterSel");
   refVar.push_back("xiMinusAfterSel");
   refVar.push_back("RjjAfterSel");

   std::vector<TCanvas*> canvasesVar(refVar.size());
   for(size_t k = 0; k < canvasesVar.size(); ++k) canvasesVar[k] = new TCanvas(refVar[k].c_str(),refVar[k].c_str());

   std::vector<TH1F*> histos_signal(refVar.size());
   std::map<std::string,std::vector<TH1F*> > histos_back;
   std::vector<TH1F*> defvector(refVar.size());
   for(size_t k = 0; k < refVar.size(); ++k){
      histos_signal[k] = getHisto(file_signal->GetDirectory("edmDumpAnalysis"),refVar[k]);
      scaleHisto(histos_signal[k],scale_signal,1,kBlack,"a.u.");

      canvasesVar[k]->cd();
      histos_signal[k]->Draw();
      for(std::map<std::string,TFile*>::const_iterator it = files_back.begin(); it != files_back.end(); ++it){
         if(histos_back.find(it->first) == histos_back.end()) histos_back[it->first] = defvector;
         histos_back[it->first][k] = getHisto(it->second->GetDirectory("edmDumpAnalysis"),refVar[k]); 
         scaleHisto(histos_back[it->first][k],scale_back[it->first],2,kRed,"a.u."); 
         histos_back[it->first][k]->Draw("same");
      }
   }  
}

TH1F* getHisto(TFile* file, const string& refVar){
   TH1F* hist = static_cast<TH1F*>(file->Get(refVar.c_str()));

   if(!hist) std::cout << ">>>> Error: histogram " << refVar.c_str() << " not found." << std::endl; 

   return hist;
} 

TH1F* getHisto(TDirectory* dir, const string& refVar){
   TH1F* hist = static_cast<TH1F*>(dir->Get(refVar.c_str()));

   if(!hist) std::cout << ">>>> Error: histogram " << refVar.c_str() << " not found." << std::endl;

   return hist;
}

void scaleHisto(TH1F* histo, double scale, int line, int color,char* title){

  histo->Scale(scale);

  histo->SetLineWidth(2);
  histo->SetLineStyle(line);
  histo->SetLineColor(color);

  histo->GetYaxis()->SetTitle(title);
}
