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

TH1F* getHisto(TFile*, const string&);
TH1F* getHisto(TDirectory*, const string&);
void scaleHisto(TH1F* histo, double scale, int line, int color, char* title);

void plot(){
   TFile* file_signal = TFile::Open("root/analysisDijets_CEPDijets_M100_histos_PU_nHFMax_1.root");
   TFile* file_back = TFile::Open("root/analysisDijets_DPEDijets_Pt40_histos_PU_nHFMax_1.root");

   double sigma_signal = 450.; //pb
   double sigma_back = 4600.;
 
   std::cout << " Cross-section (signal): " << sigma_signal << std::endl;
   std::cout << " Cross-section (background): " << sigma_back << std::endl;

   TFile* file_eff_signal = TFile::Open("root/analysis_histos_CPEDijets.root");
   TFile* file_eff_back = TFile::Open("root/analysis_histos_DPEDijets.root");
   
   const TTree* data_before_signal = static_cast<const TTree*>(file_eff_signal->Get("analysisBeforeSelection/data"));
   const TTree* data_after_signal = static_cast<const TTree*>(file_eff_signal->Get("analysisAfterSelection/data"));

   double eff_HLT_signal = (double)data_after_signal->GetEntries()/(double)data_before_signal->GetEntries();

   std::cout << " Efficiency for HLT selection (signal): " << eff_HLT_signal << std::endl;

   const TTree* data_before_back = static_cast<const TTree*>(file_eff_back->Get("analysisBeforeSelection/data"));
   const TTree* data_after_back = static_cast<const TTree*>(file_eff_back->Get("analysisAfterSelection/data"));

   double eff_HLT_back = (double)data_after_back->GetEntries()/(double)data_before_back->GetEntries();

   std::cout << " Efficiency for HLT selection (background): " << eff_HLT_back << std::endl;

   const TH1F* h_leadingJetPt_signal = static_cast<const TH1F*>(file_signal->Get("edmDumpAnalysis/leadingJetPt"));
   const TH1F* h_leadingJetPt_back = static_cast<const TH1F*>(file_back->Get("analysis/leadingJetPt"));

   double nEvents_signal = h_leadingJetPt_signal->GetEntries();
   double scale_signal = sigma_signal*eff_HLT_signal/nEvents_signal;
   //double scale_signal = 1./nEvents_signal;

   double nEvents_back = h_leadingJetPt_back->GetEntries();
   double scale_back = sigma_back*eff_HLT_back/nEvents_back;
   //double scale_back = 1./nEvents_back;

   std::cout << " N events after L1+HLT (signal): " << nEvents_signal << std::endl;
   std::cout << " N events after L1+HLT (background): " << nEvents_back << std::endl;
   
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
   std::vector<TH1F*> histos_back(refVar.size());
   for(size_t k = 0; k < refVar.size(); ++k){
      histos_signal[k] = getHisto(file_signal->GetDirectory("edmDumpAnalysis"),refVar[k]);
      histos_back[k] = getHisto(file_back->GetDirectory("analysis"),refVar[k]); 

      scaleHisto(histos_signal[k],scale_signal,1,kBlack,"a.u.");
      scaleHisto(histos_back[k],scale_back,2,kRed,"a.u."); 

      canvasesVar[k]->cd();
      histos_signal[k]->Draw();
      histos_back[k]->Draw("same");
      //histos_signal[k]->Rebin(4);
      //histos_back[k]->Rebin(8);
      //histos_signal[k]->DrawNormalized();
      //histos_back[k]->DrawNormalized("same");
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
