#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"

#include <vector>

void plotHistos(TString fileName){

  TFile* _file0 = TFile::Open(fileName.Data());

  // Tracks Reconstruction efficiency
  TH1F* hTrkRecEffvsPt = static_cast<TH1F*>(_file0->Get("trkEffAnalysis/RecoEfficiencyvsPt"));
  TH1F* hTrkRecEffvsPtBx0Signal = static_cast<TH1F*>(_file0->Get("trkEffAnalysis/RecoEfficiencyvsPtBx0Signal"));
  TH1F* hTrkRecEffvsPtBx0PileUp = static_cast<TH1F*>(_file0->Get("trkEffAnalysis/RecoEfficiencyvsPtBx0PileUp"));

  hTrkRecEffvsPt->GetXaxis()->SetTitle("tracks Pt (GeV)");
  hTrkRecEffvsPt->GetYaxis()->SetTitle("Efficiency");
  hTrkRecEffvsPt->SetLineWidth(2);
  hTrkRecEffvsPtBx0Signal->GetXaxis()->SetTitle("tracks Pt (GeV)");
  hTrkRecEffvsPtBx0Signal->GetYaxis()->SetTitle("Efficiency");
  hTrkRecEffvsPtBx0Signal->SetLineColor(kBlue);
  hTrkRecEffvsPtBx0Signal->SetLineWidth(2);
  hTrkRecEffvsPtBx0PileUp->GetXaxis()->SetTitle("tracks Pt (GeV)");
  hTrkRecEffvsPtBx0PileUp->GetYaxis()->SetTitle("Efficiency");
  hTrkRecEffvsPtBx0PileUp->SetLineWidth(2);
  hTrkRecEffvsPtBx0PileUp->SetLineColor(kRed);

  TCanvas* canvas1  = new TCanvas("canvas1");
  canvas1->cd();
  hTrkRecEffvsPtBx0Signal->Draw();
  hTrkRecEffvsPtBx0PileUp->Draw("same");
  hTrkRecEffvsPt->Draw("same");

  TH1F* hTrkRecEffvsEta = static_cast<TH1F*>(_file0->Get("trkEffAnalysis/RecoEfficiencyvsEta"));
  TH1F* hTrkRecEffvsEtaBx0Signal = static_cast<TH1F*>(_file0->Get("trkEffAnalysis/RecoEfficiencyvsEtaBx0Signal"));
  TH1F* hTrkRecEffvsEtaBx0PileUp = static_cast<TH1F*>(_file0->Get("trkEffAnalysis/RecoEfficiencyvsEtaBx0PileUp"));

  hTrkRecEffvsEta->GetXaxis()->SetTitle("tracks Eta");
  hTrkRecEffvsEta->GetYaxis()->SetTitle("Efficiency");
  hTrkRecEffvsEta->SetLineWidth(2);
  hTrkRecEffvsEtaBx0Signal->GetXaxis()->SetTitle("tracks Eta");
  hTrkRecEffvsEtaBx0Signal->GetYaxis()->SetTitle("Efficiency");
  hTrkRecEffvsEtaBx0Signal->SetLineWidth(2);
  hTrkRecEffvsEtaBx0Signal->SetLineColor(kBlue);
  hTrkRecEffvsEtaBx0PileUp->GetXaxis()->SetTitle("tracks Eta");
  hTrkRecEffvsEtaBx0PileUp->GetYaxis()->SetTitle("Efficiency");
  hTrkRecEffvsEtaBx0PileUp->SetLineWidth(2);
  hTrkRecEffvsEtaBx0PileUp->SetLineColor(kRed);

  TCanvas* canvas2  = new TCanvas("canvas2");
  canvas2->cd();
  hTrkRecEffvsEtaBx0Signal->Draw();
  hTrkRecEffvsEtaBx0PileUp->Draw("same");
  hTrkRecEffvsEta->Draw("same");

  TH1F* hSimTracksPt = static_cast<TH1F*>(_file0->Get("trkEffAnalysis/SimTracksPt"));
  TH1F* hSimTracksPtBx0Signal = static_cast<TH1F*>(_file0->Get("trkEffAnalysis/SimTracksPtBx0Signal"));
  TH1F* hSimTracksPtBx0PileUp = static_cast<TH1F*>(_file0->Get("trkEffAnalysis/SimTracksPtBx0PileUp"));

  hSimTracksPt->GetXaxis()->SetTitle("tracks Pt (GeV)");
  hSimTracksPt->GetYaxis()->SetTitle("Density");
  hSimTracksPt->SetLineWidth(2);
  hSimTracksPtBx0Signal->GetXaxis()->SetTitle("tracks Pt (GeV)");
  hSimTracksPtBx0Signal->GetYaxis()->SetTitle("Density");
  hSimTracksPtBx0Signal->SetLineWidth(2);
  hSimTracksPtBx0Signal->SetLineColor(kBlue);
  hSimTracksPtBx0PileUp->GetXaxis()->SetTitle("tracks Pt (GeV)");
  hSimTracksPtBx0PileUp->GetYaxis()->SetTitle("Density");
  hSimTracksPtBx0PileUp->SetLineWidth(2);
  hSimTracksPtBx0PileUp->SetLineColor(kRed);

  TCanvas* canvas3  = new TCanvas("canvas3");
  canvas3->cd();
  hSimTracksPtBx0PileUp->DrawNormalized();
  hSimTracksPtBx0Signal->DrawNormalized("same");
  hSimTracksPt->DrawNormalized("same");

  TH1F* hSimTracksEta = static_cast<TH1F*>(_file0->Get("trkEffAnalysis/SimTracksEta"));
  TH1F* hSimTracksEtaBx0Signal = static_cast<TH1F*>(_file0->Get("trkEffAnalysis/SimTracksEtaBx0Signal"));
  TH1F* hSimTracksEtaBx0PileUp = static_cast<TH1F*>(_file0->Get("trkEffAnalysis/SimTracksEtaBx0PileUp"));

  hSimTracksEta->GetXaxis()->SetTitle("tracks Eta");
  hSimTracksEta->GetYaxis()->SetTitle("Density");
  hSimTracksEta->SetLineWidth(2);
  hSimTracksEta->Rebin();
  hSimTracksEtaBx0Signal->GetXaxis()->SetTitle("tracks Eta");
  hSimTracksEtaBx0Signal->GetYaxis()->SetTitle("Density");
  hSimTracksEtaBx0Signal->SetLineWidth(2);
  hSimTracksEtaBx0Signal->SetLineColor(kBlue);
  hSimTracksEtaBx0Signal->Rebin();
  hSimTracksEtaBx0PileUp->GetXaxis()->SetTitle("tracks Eta");
  hSimTracksEtaBx0PileUp->GetYaxis()->SetTitle("Density");
  hSimTracksEtaBx0PileUp->SetLineWidth(2);
  hSimTracksEtaBx0PileUp->SetLineColor(kRed);
  hSimTracksEtaBx0PileUp->Rebin();

  TCanvas* canvas4  = new TCanvas("canvas4");
  canvas4->cd();
  hSimTracksEtaBx0Signal->DrawNormalized();
  hSimTracksEtaBx0PileUp->DrawNormalized("same");
  hSimTracksEta->DrawNormalized("same");

  // Pile-up histos
  TH2F* hNPUBx0vsNPV = static_cast<TH2F*>(_file0->Get("pileUpAnalysis/NPUBx0vsNPV"));
  hNPUBx0vsNPV->GetXaxis()->SetTitle("N primary vertices");
  hNPUBx0vsNPV->GetYaxis()->SetTitle("N pile-up (Bx == 0)");
  hNPUBx0vsNPV->SetLineWidth(2);

  TCanvas* canvas5  = new TCanvas("canvas5");
  canvas5->cd();
  hNPUBx0vsNPV->Draw("box");

  TTree* tree = static_cast<TTree*>(_file0->Get("pileUpAnalysis/data"));
  int nBunches;
  int nPileUp[9];
  tree->SetBranchAddress("nBunches",&nBunches);
  tree->SetBranchAddress("nPileUp",nPileUp);
  tree->GetEntry(1);
  vector<TH1F*> vecNPileUp(nBunches);
  char aux[20];
  for(int idx = 0; idx < nBunches; ++idx){
    sprintf(aux,"nPileUp_%d",idx);
    vecNPileUp[idx] = new TH1F(aux,aux,10,0,10);
  }

  TCanvas* canvas6  = new TCanvas("canvas6");
  canvas6->cd();
  size_t nBx = 0;
  for(vector<TH1F*>::const_iterator it = vecNPileUp.begin(); it != vecNPileUp.end(); ++it,++nBx){
    sprintf(aux,"nPileUp[%d]",nBx);
    tree->Project((*it)->GetName(),aux);
    (*it)->GetXaxis()->SetTitle("N pile-up");
    (*it)->GetYaxis()->SetTitle("N events");
    (*it)->SetLineWidth(2);
    (*it)->SetLineStyle(nBx + 1);
    (nBx)?(*it)->Draw("same"):(*it)->Draw();
  }

  // Tracks kinematics
  TH1F* hTrkDxySignal = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/TrackDxySignal"));
  TH1F* hTrkDxyPileUp = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/TrackDxyPileUp"));
  TH1F* hTrkDzSignal = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/TrackDzSignal"));
  TH1F* hTrkDzPileUp = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/TrackDzPileUp"));
  TH1F* hTrkVzSignal = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/TrackVzSignal"));
  TH1F* hTrkVzPileUp = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/TrackVzPileUp"));
  TH1F* hTrkSigxySignal = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/TrackSigxySignal"));
  TH1F* hTrkSigxyPileUp = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/TrackSigxyPileUp"));
  TH1F* hTrkSigzSignal = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/TrackSigzSignal"));
  TH1F* hTrkSigzPileUp = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/TrackSigzPileUp"));

  hTrkDxySignal->GetXaxis()->SetTitle("d_{xy} tracks (cm)");
  hTrkDxySignal->GetYaxis()->SetTitle("Density");
  hTrkDxySignal->SetLineWidth(2);
  //hTrkDxySignal->Rebin();
  hTrkDxyPileUp->GetXaxis()->SetTitle("d_{xy} tracks (cm)");
  hTrkDxyPileUp->GetYaxis()->SetTitle("Density");
  hTrkDxyPileUp->SetLineWidth(2);
  hTrkDxyPileUp->SetLineColor(kRed);
  //hTrkDxyPileUp->Rebin();

  hTrkDzSignal->GetXaxis()->SetTitle("d_{z} tracks (cm)");
  hTrkDzSignal->GetYaxis()->SetTitle("Density");
  hTrkDzSignal->SetLineWidth(2);
  hTrkDzSignal->Rebin();
  hTrkDzPileUp->GetXaxis()->SetTitle("d_{z} tracks (cm)");
  hTrkDzPileUp->GetYaxis()->SetTitle("Density");
  hTrkDzPileUp->SetLineWidth(2);
  hTrkDzPileUp->SetLineColor(kRed);
  hTrkDzPileUp->Rebin();
  
  hTrkVzSignal->GetXaxis()->SetTitle("v_{z} tracks (cm)");
  hTrkVzSignal->GetYaxis()->SetTitle("Density");
  hTrkVzSignal->SetLineWidth(2);
  hTrkVzSignal->Rebin();
  hTrkVzPileUp->GetXaxis()->SetTitle("v_{z} tracks (cm)");
  hTrkVzPileUp->GetYaxis()->SetTitle("Density");
  hTrkVzPileUp->SetLineWidth(2);
  hTrkVzPileUp->SetLineColor(kRed);
  hTrkVzPileUp->Rebin();

  hTrkSigxySignal->GetXaxis()->SetTitle("#sigma_{xy} tracks");
  hTrkSigxySignal->GetYaxis()->SetTitle("Density");
  hTrkSigxySignal->SetLineWidth(2);
  hTrkSigxySignal->Rebin();
  hTrkSigxyPileUp->GetXaxis()->SetTitle("#sigma_{xy} tracks");
  hTrkSigxyPileUp->GetYaxis()->SetTitle("Density");
  hTrkSigxyPileUp->SetLineWidth(2);
  hTrkSigxyPileUp->SetLineColor(kRed);
  hTrkSigxyPileUp->Rebin();

  hTrkSigzSignal->GetXaxis()->SetTitle("#sigma_{z} tracks");
  hTrkSigzSignal->GetYaxis()->SetTitle("Density");
  hTrkSigzSignal->SetLineWidth(2);
  hTrkSigzSignal->Rebin();
  hTrkSigzPileUp->GetXaxis()->SetTitle("#sigma_{z} tracks");
  hTrkSigzPileUp->GetYaxis()->SetTitle("Density");
  hTrkSigzPileUp->SetLineWidth(2);
  hTrkSigzPileUp->SetLineColor(kRed);
  hTrkSigzPileUp->Rebin();

  TCanvas* canvas71  = new TCanvas("canvas71");
  canvas71->cd();
  hTrkDxySignal->DrawNormalized();
  hTrkDxyPileUp->DrawNormalized("same");
  TCanvas* canvas72  = new TCanvas("canvas72");
  canvas72->cd();
  hTrkDzSignal->DrawNormalized();
  hTrkDzPileUp->DrawNormalized("same"); 
  TCanvas* canvas73  = new TCanvas("canvas73");
  canvas73->cd();
  hTrkVzSignal->DrawNormalized();
  hTrkVzPileUp->DrawNormalized("same");
  TCanvas* canvas74  = new TCanvas("canvas74");
  canvas74->cd();
  hTrkSigxySignal->DrawNormalized();
  hTrkSigxyPileUp->DrawNormalized("same");
  TCanvas* canvas75  = new TCanvas("canvas75");
  canvas75->cd();
  hTrkSigzSignal->DrawNormalized();
  hTrkSigzPileUp->DrawNormalized("same");

  // Primary vertices
  TH1F* hPrimVtxPosZBx0Signal = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/PrimVtxPosZBx0Signal"));
  TH1F* hPrimVtxPosZBx0PileUp = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/PrimVtxPosZBx0PileUp"));
  
  hPrimVtxPosZBx0Signal->GetXaxis()->SetTitle("v_{z} vertices (cm)");
  hPrimVtxPosZBx0Signal->GetYaxis()->SetTitle("Density");
  hPrimVtxPosZBx0Signal->SetLineWidth(2);
  hPrimVtxPosZBx0Signal->Rebin();
  hPrimVtxPosZBx0PileUp->GetXaxis()->SetTitle("v_{z} vertices (cm)");
  hPrimVtxPosZBx0PileUp->GetYaxis()->SetTitle("Density");
  hPrimVtxPosZBx0PileUp->SetLineWidth(2);
  hPrimVtxPosZBx0PileUp->SetLineColor(kRed);
  hPrimVtxPosZBx0PileUp->Rebin();

  TCanvas* canvas8  = new TCanvas("canvas8");
  canvas8->cd();
  hPrimVtxPosZBx0Signal->DrawNormalized();
  hPrimVtxPosZBx0PileUp->DrawNormalized("same");

  // Tracks vs Prim. vertex
  TH1F* hTrkDistPVBx0Signal = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/TrackDistPVBx0Signal"));
  TH1F* hTrkDistPVBx0PileUp = static_cast<TH1F*>(_file0->Get("pileUpAnalysis/TrackDistPVBx0PileUp"));

  hTrkDistPVBx0Signal->GetXaxis()->SetTitle("|z_{trk} - z_{vtx}| (cm)");
  hTrkDistPVBx0Signal->GetYaxis()->SetTitle("Density");
  hTrkDistPVBx0Signal->SetLineWidth(2);
  hTrkDistPVBx0PileUp->GetXaxis()->SetTitle("|z_{trk} - z_{vtx}| (cm)");
  hTrkDistPVBx0PileUp->GetYaxis()->SetTitle("Density");
  hTrkDistPVBx0PileUp->SetLineWidth(2);
  hTrkDistPVBx0PileUp->SetLineColor(kRed);

  TCanvas* canvas9  = new TCanvas("canvas9");
  canvas9->cd();
  canvas9->SetLogy();
  hTrkDistPVBx0Signal->DrawNormalized();
  hTrkDistPVBx0PileUp->DrawNormalized("same");
}
