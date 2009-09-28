#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TStyle.h"
#include "TH1D.h"
#include "TTree.h"

//#include "PlottingTools.h"
//#include "Plotter.h"

#include <iostream>
#include <vector>
#include <map>

void jetResponse(std::string fileName){
   TFile* file = TFile::Open(fileName.c_str(),"read");

   // Get TTree
   TTree* data = dynamic_cast<TTree*>(file->Get("jetResponseAnalysis/data"));
   if(!data){
      std::cout << "ERROR: Could not find jetResponseAnalysis/data" <<  std::endl;
      file->Close();
      return;
   }
 
   struct {
       double genJetPt_;
       double genJetEta_;
       double genJetPhi_;
       double recJetPt_;
       double recJetEta_;
       double recJetPhi_;
   } eventData;
 
   data->SetBranchAddress("genJetPt",&eventData.genJetPt_);
   data->SetBranchAddress("genJetEta",&eventData.genJetEta_);
   data->SetBranchAddress("genJetPhi",&eventData.genJetPhi_);
   data->SetBranchAddress("recJetPt",&eventData.recJetPt_);
   data->SetBranchAddress("recJetEta",&eventData.recJetEta_);
   data->SetBranchAddress("recJetPhi",&eventData.recJetPhi_);

   // Create output file
   TFile* hfile = new TFile("analysisJetResponse_histos.root","recreate","data histograms");

   // Book histos
   TH1::SetDefaultSumw2(true);

   double aux_EtaBoundaries[5] = {0.0,1.4,2.6,3.2,4.7};
   double aux_PtBoundaries[5] = {10.,30.,50.,100.,200.};
   std::vector<double> etaBoundaries(aux_EtaBoundaries,aux_EtaBoundaries + 5);
   std::vector<double> ptBoundaries(aux_PtBoundaries,aux_PtBoundaries + 5);

   std::map<std::pair<int,int>, std::vector<TH1D*> > histos;
   for(size_t i_eta = 0; i_eta < (etaBoundaries.size() - 1); ++i_eta){
      for(size_t i_pt = 0; i_pt < (ptBoundaries.size() - 1); ++i_pt){
         std::pair<int,int> index(i_eta,i_pt);
         histos[index] = std::vector<TH1D*>(4); 
    
         char title[30];
         char label[100];
         // Ratio of RecoJetpT and GenJetpT
         sprintf(title,"response_%i_%i",i_eta,i_pt);
         sprintf(label,"p_{T}(Reco)/p_{T}(Gen) #in %f < #eta < %f and #in p_{T} #left[%f , %f #right]GeV",etaBoundaries[i_eta],etaBoundaries[i_eta+1],ptBoundaries[i_pt],ptBoundaries[i_pt+1]);
         histos[index][0] = new TH1D(title,label,100,0.,2.);
	
         // RecoJetpT - GenJetpT
         sprintf(title,"pTdistribution_%i_%i",i_eta,i_pt);
         sprintf(label,"p_{T}(Reco) - p_{T}(Gen) #in %f < #eta < %f and #in p_{T} #left[%f , %f #right]GeV",etaBoundaries[i_eta],etaBoundaries[i_eta+1],ptBoundaries[i_pt],ptBoundaries[i_pt+1]);
         histos[index][1] = new TH1D(title,label,100,-100.,100.);

       	 // RecoJetPhi - GenJetPhi
         sprintf(title,"phidistribution_%i_%i",i_eta,i_pt);
         sprintf(label,"#phi_{Reco}-#phi_{Gen} #in %f < #eta < %f and #in p_{T} #left[%f , %f #right]GeV",etaBoundaries[i_eta],etaBoundaries[i_eta+1],ptBoundaries[i_pt],ptBoundaries[i_pt+1]);
         histos[index][2] = new TH1D(title,label,100,-1.0,1.0);

       	 // RecoJetEta - GenJetEta
         sprintf(title,"etadistribution_%i_%i",i_eta,i_pt);
         sprintf(label,"#eta_{Reco}-#eta_{Gen} #in %f < #eta < %f and #in p_{T} #left[%f , %f #right]GeV",etaBoundaries[i_eta],etaBoundaries[i_eta+1],ptBoundaries[i_pt],ptBoundaries[i_pt+1]);
         histos[index][3] = new TH1D(title,label,100,-0.5,0.5);
      }
   }

   // Loop over TTree
   int nEntries = data->GetEntries();
   for(int ientry = 0; ientry < nEntries; ++ientry){
      if(ientry%2000 == 0) std::cout << ">>> Analysing " << ientry << "th entry" << std::endl;

      data->GetEntry(ientry);

      double response = eventData.recJetPt_/eventData.genJetPt_;
      double ptDiff = eventData.recJetPt_ - eventData.genJetPt_;
      double phiDiff = eventData.recJetPhi_ - eventData.genJetPhi_;
      double etaDiff = fabs(eventData.recJetEta_) - fabs(eventData.genJetEta_);

      double refEta = eventData.recJetEta_;
      double refPt = eventData.genJetPt_;
      // Find eta/pt bin --> rec. eta and gen. pt 
      size_t i_eta = 0;
      for(; i_eta < (etaBoundaries.size() - 1); ++i_eta){
         if(fabs(refEta) >= etaBoundaries[i_eta] && fabs(refEta) < etaBoundaries[i_eta + 1]) break;
      }
      size_t i_pt = 0;
      for(; i_pt < (ptBoundaries.size() - 1); ++i_pt){
         if(fabs(refPt) >= ptBoundaries[i_pt] && fabs(refPt) < ptBoundaries[i_pt + 1]) break;
      }
      std::pair<int,int> index(i_eta,i_pt);
   
      std::map<std::pair<int,int>, std::vector<TH1D*> >::iterator it_histos = histos.find(index);
      if(it_histos != histos.end()){
         std::vector<TH1D*>& myhistos = it_histos->second;
         myhistos[0]->Fill(response);
         myhistos[1]->Fill(ptDiff);
         myhistos[2]->Fill(phiDiff);
         myhistos[3]->Fill(etaDiff);
      }
   }
   
   hfile->Write();
   hfile->Close();
}
