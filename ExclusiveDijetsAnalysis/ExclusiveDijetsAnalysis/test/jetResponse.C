#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TStyle.h"
#include "TH1F.h"
#include "TTree.h"
#include "TF1.h"

//#include "PlottingTools.h"
//#include "Plotter.h"

#include <iostream>
#include <vector>
#include <map>

void jetResponse(std::string const& fileName, std::string const& treeName = "jetResponseAnalysis/data"){
   TFile* file = TFile::Open(fileName.c_str(),"read");

   // Get TTree
   TTree* data = dynamic_cast<TTree*>(file->Get(treeName.c_str()));
   if(!data){
      std::cout << "ERROR: Could not find " << treeName << std::endl;
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
   //TFile* hfile = new TFile("analysisJetResponse_histos.root","recreate","data histograms");

   // Book histos
   TH1::SetDefaultSumw2(true);

   double aux_EtaBoundaries[5] = {0.0,1.4,2.6,3.2,4.7};
   double aux_PtBoundaries[5] = {10.,30.,50.,100.,200.};
   std::vector<double> etaBoundaries(aux_EtaBoundaries,aux_EtaBoundaries + 5);
   std::vector<double> ptBoundaries(aux_PtBoundaries,aux_PtBoundaries + 5);

   std::map<std::pair<int,int>, std::vector<TH1F*> > histos;
   for(size_t i_eta = 0; i_eta < (etaBoundaries.size() - 1); ++i_eta){
      for(size_t i_pt = 0; i_pt < (ptBoundaries.size() - 1); ++i_pt){
         std::pair<int,int> index(i_eta,i_pt);
         histos[index] = std::vector<TH1F*>(4); 
    
         char hname[50];
         char title[200];
         // Ratio of RecoJetpT and GenJetpT
         sprintf(hname,"response_%i_%i",i_eta,i_pt);
         sprintf(title,"p_{T}(Reco)/p_{T}(Gen) #in %f < #||{#eta} < %f and #in p_{T} #left[%f , %f #right]GeV",etaBoundaries[i_eta],etaBoundaries[i_eta+1],ptBoundaries[i_pt],ptBoundaries[i_pt+1]);
         std::cout << hname << std::endl;
         histos[index][0] = new TH1F(hname,title,100,0.,2.);
	
         // RecoJetpT - GenJetpT
         sprintf(hname,"pTdistribution_%i_%i",i_eta,i_pt);
         sprintf(title,"p_{T}(Reco) - p_{T}(Gen) #in %f < #||{#eta} < %f and #in p_{T} #left[%f , %f #right]GeV",etaBoundaries[i_eta],etaBoundaries[i_eta+1],ptBoundaries[i_pt],ptBoundaries[i_pt+1]);
         std::cout << hname << std::endl;
         histos[index][1] = new TH1F(hname,title,100,-100.,100.);

       	 // RecoJetPhi - GenJetPhi
         sprintf(hname,"phidistribution_%i_%i",i_eta,i_pt);
         sprintf(title,"#phi_{Reco}-#phi_{Gen} #in %f < #||{#eta} < %f and #in p_{T} #left[%f , %f #right]GeV",etaBoundaries[i_eta],etaBoundaries[i_eta+1],ptBoundaries[i_pt],ptBoundaries[i_pt+1]);
         std::cout << hname << std::endl;
         histos[index][2] = new TH1F(hname,title,100,-1.0,1.0);

       	 // RecoJetEta - GenJetEta
         sprintf(hname,"etadistribution_%i_%i",i_eta,i_pt);
         sprintf(title,"#||{#eta_{Reco}}-#||{#eta_{Gen}} #in %f < #||{#eta} < %f and #in p_{T} #left[%f , %f #right]GeV",etaBoundaries[i_eta],etaBoundaries[i_eta+1],ptBoundaries[i_pt],ptBoundaries[i_pt+1]);
         std::cout << hname << std::endl;
         histos[index][3] = new TH1F(hname,title,100,-0.5,0.5);

         /*// Ratio RecoJetpT/GenJetpT vs pt
         sprintf(hname,"resp_%i_%i",i_eta,i_pt);
         histos[index][4] = new TH1F(hname,hname,(ptBoundaries.size() - 1),aux_PtBoundaries);*/
      }
   }

   std::map<int, std::vector<TH1F*> > histosVsPt;
   for(size_t i_eta = 0; i_eta < (etaBoundaries.size() - 1); ++i_eta){
      histosVsPt[i_eta] = std::vector<TH1F*>(2);

      char hname[30];
      char title[100];
      sprintf(hname,"meanVsPt_%i",i_eta);
      sprintf(title,"<response> vs p^{GenJet}_{T} - %f < #||{#eta} < %f",etaBoundaries[i_eta],etaBoundaries[i_eta+1]);
      histosVsPt[i_eta][0] = new TH1F(hname,title,4,aux_PtBoundaries);

      sprintf(hname,"sigmaVsPt_%i",i_eta);
      sprintf(title,"Jet p^{GenJet}_{T} resolution - %f < #||{#eta} < %f",etaBoundaries[i_eta],etaBoundaries[i_eta+1]);
      histosVsPt[i_eta][1] = new TH1F(hname,title,4,aux_PtBoundaries);
   }

   std::map<int, std::vector<TH1F*> > histosVsEta;
   for(size_t i_pt = 0; i_pt < (ptBoundaries.size() - 1); ++i_pt){
      histosVsEta[i_pt] = std::vector<TH1F*>(2);

      char hname[30];
      char title[100];
      sprintf(hname,"meanVsEta_%i",i_pt);
      sprintf(title,"<response> vs #eta^{RecoJet} #in p_{T} #left[%f , %f #right]GeV",ptBoundaries[i_pt],ptBoundaries[i_pt+1]);
      histosVsEta[i_pt][0] = new TH1F(hname,title,4,aux_EtaBoundaries);

      sprintf(hname,"sigmaVsEta_%i",i_pt);
      sprintf(title,"Jet p^{GenJet}_{T} resolution #in p_{T} #left[%f , %f #right]GeV",ptBoundaries[i_pt],ptBoundaries[i_pt+1]);
      histosVsEta[i_pt][1] = new TH1F(hname,title,4,aux_EtaBoundaries);
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
   
      std::map<std::pair<int,int>, std::vector<TH1F*> >::iterator it_histos = histos.find(index);
      if(it_histos != histos.end()){
         std::vector<TH1F*>& myhistos = it_histos->second;
         myhistos[0]->Fill(response);
         myhistos[1]->Fill(ptDiff);
         myhistos[2]->Fill(phiDiff);
         myhistos[3]->Fill(etaDiff);
      }
   }

   // Loop over histos
   double gausswidth = 1.5;
   std::map<std::pair<int,int>, std::vector<TCanvas*> > canvases;
   for(size_t i_eta = 0; i_eta < (etaBoundaries.size() - 1); ++i_eta){
      for(size_t i_pt = 0; i_pt < (ptBoundaries.size() - 1); ++i_pt){
         std::pair<int,int> index(i_eta,i_pt);
 
         std::map<std::pair<int,int>, std::vector<TH1F*> >::iterator it_histos = histos.find(index);
         if(it_histos == histos.end()){
            std::cout << "ERROR: Could not find histos (" << index.first << "," << index.second << ")" << std::endl;
            return;
         } 

         std::vector<TH1F*>& myhistos = it_histos->second;

         canvases[index] = std::vector<TCanvas*>(4);
 
         // Draw response histo
         char cname[30];
         sprintf(cname,"c_response_%i_%i",i_eta,i_pt);
         canvases[index][0] = new TCanvas(cname,cname,900,700);
         canvases[index][0]->cd(); 

         TH1F* h_response = myhistos[0];
         h_response->GetXaxis()->SetTitle("p^{RecoJet}_{T}/ p^{GenJet}_{T}");
         h_response->GetYaxis()->SetTitle("Events");
         h_response->GetYaxis()->SetLabelSize(0.04);
         h_response->GetXaxis()->SetLabelSize(0.04);
         h_response->Draw();

         // Fit response histo
         double fitMin = h_response->GetMean() - gausswidth*h_response->GetRMS();
         double fitMax = h_response->GetMean() + gausswidth*h_response->GetRMS(); 
         h_response->Fit("gaus","RIE","",fitMin,fitMax);
         const TF1* fit = static_cast<TF1*>(h_response->GetFunction("gaus"));

         double mean = fit->GetParameter(1);
         double meanErr = fit->GetParError(1);
         double sigma = fit->GetParameter(2);
         double sigmaErr = fit->GetParError(2);
         double x_binPt = (ptBoundaries[i_pt] + ptBoundaries[i_pt+1])/2;
         double x_binEta = (etaBoundaries[i_eta] + etaBoundaries[i_eta+1])/2;
        
         // Fill histos vs pt
         std::map<int, std::vector<TH1F*> >::iterator it_histosVsPt = histosVsPt.find(i_eta);
         if(it_histosVsPt == histosVsPt.end()){
            std::cout << "ERROR: Could not find histos i_eta= " << i_eta << std::endl;
            return;
         }
         std::vector<TH1F*>& myhistosVsPt = it_histosVsPt->second;

         TH1F* h_meanVsPt = myhistosVsPt[0];
         TH1F* h_sigmaVsPt = myhistosVsPt[1];  
         h_sigmaVsPt->SetBinContent(h_sigmaVsPt->GetXaxis()->FindBin(x_binPt),sigma/mean);
         h_sigmaVsPt->SetBinError(h_sigmaVsPt->GetXaxis()->FindBin(x_binPt),sigmaErr/mean);
         h_meanVsPt->SetBinContent(h_meanVsPt->GetXaxis()->FindBin(x_binPt),mean);
         h_meanVsPt->SetBinError(h_meanVsPt->GetXaxis()->FindBin(x_binPt),meanErr);

         // Fill histos vs eta
         std::map<int, std::vector<TH1F*> >::iterator it_histosVsEta = histosVsEta.find(i_pt);
         if(it_histosVsEta == histosVsEta.end()){
            std::cout << "ERROR: Could not find histos i_pt= " << i_pt << std::endl;
            return;
         }
         std::vector<TH1F*>& myhistosVsEta = it_histosVsEta->second;

         TH1F* h_meanVsEta = myhistosVsEta[0];
         TH1F* h_sigmaVsEta = myhistosVsEta[1];

         h_sigmaVsEta->SetBinContent(h_sigmaVsEta->GetXaxis()->FindBin(x_binEta),sigma/mean);
         h_sigmaVsEta->SetBinError(h_sigmaVsEta->GetXaxis()->FindBin(x_binEta),sigmaErr/mean);
         h_meanVsEta->SetBinContent(h_meanVsEta->GetXaxis()->FindBin(x_binEta),mean);
         h_meanVsEta->SetBinError(h_meanVsEta->GetXaxis()->FindBin(x_binEta),meanErr);

         // Draw ptDiff histo
         sprintf(cname,"c_pTdistribution_%i_%i",i_eta,i_pt);
         canvases[index][1] = new TCanvas(cname,cname,900,700);
         canvases[index][1]->cd();

         TH1F* h_ptDiff = myhistos[1];
         h_ptDiff->GetXaxis()->SetTitle("p^{RecoJet}_{T} - p^{GenJet}_{T}");
         h_ptDiff->GetYaxis()->SetTitle("Events");
         h_ptDiff->GetYaxis()->SetLabelSize(0.04);
         h_ptDiff->GetXaxis()->SetLabelSize(0.04);
         h_ptDiff->Draw();

         // Draw phiDiff histo
         sprintf(cname,"c_phidistribution_%i_%i",i_eta,i_pt);
         canvases[index][2] = new TCanvas(cname,cname,900,700);
         canvases[index][2]->cd();

         TH1F* h_phiDiff = myhistos[2];
         h_phiDiff->GetXaxis()->SetTitle("#phi^{RecoJet} - #phi^{GenJet}");
         h_phiDiff->GetYaxis()->SetTitle("Events");
         h_phiDiff->GetYaxis()->SetLabelSize(0.04);
         h_phiDiff->GetXaxis()->SetLabelSize(0.04);
         h_phiDiff->Draw();

         // Draw etaDiff histo
         sprintf(cname,"c_etadistribution_%i_%i",i_eta,i_pt);
         canvases[index][3] = new TCanvas(cname,cname,900,700);
         canvases[index][3]->cd();

         TH1F* h_etaDiff = myhistos[3];
         h_etaDiff->GetXaxis()->SetTitle("#||{#eta^{RecoJet}} - #||{#eta^{GenJet}}");
         h_etaDiff->GetYaxis()->SetTitle("Events");
         h_etaDiff->GetYaxis()->SetLabelSize(0.04);
         h_etaDiff->GetXaxis()->SetLabelSize(0.04);
         h_etaDiff->Draw();
      }
   }
   
   std::map<int, std::vector<TCanvas*> > canvasesVsPt;
   std::map<int, TF1*> fitResolutionVsPt; 
   for(size_t i_eta = 0; i_eta < (etaBoundaries.size() - 1); ++i_eta){
      std::map<int, std::vector<TH1F*> >::iterator it_histosVsPt = histosVsPt.find(i_eta);
      if(it_histosVsPt == histosVsPt.end()){
            std::cout << "ERROR: Could not find histos i_eta= " << i_eta << std::endl;
            return;
      }
      std::vector<TH1F*>& myhistosVsPt = it_histosVsPt->second;

      canvasesVsPt[i_eta] = std::vector<TCanvas*>(2);
 
      char cname[30];
      sprintf(cname,"c_meanVsPt_%i",i_eta);
      canvasesVsPt[i_eta][0] = new TCanvas(cname,cname,900,700);
      canvasesVsPt[i_eta][0]->cd(); 

      TH1F* h_mean = myhistosVsPt[0];
      h_mean->GetXaxis()->SetRangeUser(20.,100.);
      h_mean->GetYaxis()->SetRangeUser(0.,1.5);
      h_mean->GetXaxis()->SetTitle("p^{GenJet}_{T}#left[GeV#right]");
      h_mean->GetYaxis()->SetTitle("< #frac{p^{RecoJet}_{T}}{p^{GenJet}_{T}} >");
      h_mean->Draw();

      sprintf(cname,"c_sigmaVsPt_%i",i_eta);
      canvasesVsPt[i_eta][1] = new TCanvas(cname,cname,900,700);
      canvasesVsPt[i_eta][1]->cd();

      TH1F* h_sigma = myhistosVsPt[1];
      h_sigma->GetXaxis()->SetRangeUser(20.,100.);
      h_sigma->GetYaxis()->SetRangeUser(0.,0.35);
      h_sigma->GetXaxis()->SetTitle("p^{GenJet}_{T}#left[GeV#right]");
      h_sigma->GetYaxis()->SetTitle("#sigma(#frac{p^{RecoJet}_{T}}{p^{GenJet}_{T}})");

      // Fit on resolution
      double fitMin = 20.;
      double fitMax = 100.;
      char funcname[30];
      char formula[100];
      sprintf(funcname,"fitResolutionVsPt_%i",i_eta);
      sprintf(formula,"sqrt(([0]/x)*([0]/x)+([1]/sqrt(x))*([1]/sqrt(x))+[2]*[2])");
      fitResolutionVsPt[i_eta] = new TF1(funcname,formula,fitMin,fitMax);
      fitResolutionVsPt[i_eta]->SetLineColor(2);
      fitResolutionVsPt[i_eta]->SetLineWidth(1);

      h_sigma->Fit(fitResolutionVsPt[i_eta],"RIE+","",fitMin,fitMax);

      h_sigma->SetMarkerStyle(21);
      h_sigma->SetMarkerColor(2);
      h_sigma->SetLineColor(2);
      h_sigma->GetYaxis()->SetLabelSize(0.03);  
      h_sigma->Draw();
   }

   std::map<int, std::vector<TCanvas*> > canvasesVsEta;
   //std::map<int, TF1*> fitResolutionVsEta;
   for(size_t i_pt = 0; i_pt < (ptBoundaries.size() - 1); ++i_pt){
      std::map<int, std::vector<TH1F*> >::iterator it_histosVsEta = histosVsEta.find(i_pt);
      if(it_histosVsEta == histosVsEta.end()){
            std::cout << "ERROR: Could not find histos i_pt= " << i_pt << std::endl;
            return;
      }
      std::vector<TH1F*>& myhistosVsEta = it_histosVsEta->second;

      canvasesVsEta[i_pt] = std::vector<TCanvas*>(2);

      char cname[30];
      sprintf(cname,"c_meanVsEta_%i",i_pt);
      canvasesVsEta[i_pt][0] = new TCanvas(cname,cname,900,700);
      canvasesVsEta[i_pt][0]->cd();

      TH1F* h_mean = myhistosVsEta[0];
      h_mean->GetXaxis()->SetRangeUser(0.,5.);
      h_mean->GetYaxis()->SetRangeUser(0.0,1.5);
      h_mean->GetXaxis()->SetTitle("#eta^{RecoJet}");
      h_mean->GetYaxis()->SetTitle("< #frac{p^{RecoJet}_{T}}{p^{GenJet}_{T}} >");
      h_mean->Draw();

      sprintf(cname,"c_sigmaVsEta_%i",i_pt);
      canvasesVsEta[i_pt][1] = new TCanvas(cname,cname,900,700);
      canvasesVsEta[i_pt][1]->cd();
  
      TH1F* h_sigma = myhistosVsEta[1];
      h_sigma->GetXaxis()->SetRangeUser(0.,5.);
      h_sigma->GetYaxis()->SetRangeUser(0.,0.35);
      h_sigma->GetXaxis()->SetTitle("#eta^{RecoJet}");
      h_sigma->GetYaxis()->SetTitle("#sigma(#frac{p^{RecoJet}_{T}}{p^{GenJet}_{T}})");

      /*// Fit on resolution
      double fitMin = 0.;
      double fitMax = 5.;
      char funcname[30];
      char formula[100];
      sprintf(funcname,"fitResolutionVsEta_%i",i_pt);
      sprintf(formula,"sqrt(([0]/x)*([0]/x)+([1]/sqrt(x))*([1]/sqrt(x))+[2]*[2])");
      fitResolutionVsEta[i_pt] = new TF1(funcname,formula,fitMin,fitMax);
      fitResolutionVsEta[i_pt]->SetLineColor(2);
      fitResolutionVsEta[i_pt]->SetLineWidth(1);

      h_sigma->Fit(fitResolutionVsEta[i_pt],"RIE+","",fitMin,fitMax);
  
      h_sigma->SetMarkerStyle(21);
      h_sigma->SetMarkerColor(2);
      h_sigma->SetLineColor(2);
      h_sigma->GetYaxis()->SetLabelSize(0.03);*/
      h_sigma->Draw();     
   }

   //hfile->Write();
   //hfile->Close();
}
