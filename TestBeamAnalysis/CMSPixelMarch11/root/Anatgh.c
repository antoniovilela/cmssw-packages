#include <fstream>
#include <iomanip>
#include <TCanvas.h>
#include <TGraph.h>
#include <Riostream.h>

using namespace std;
void Anatgh(TString const& baseName)
{
  gStyle->SetCanvasColor(0);
  gROOT->SetStyle("Plain");
  gStyle->SetCanvasColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(111111);
  
 // TH1F *hp0 = new TH1F("hp0","P0", 25,0.00005,0.00015);
  TH1F *hp0 = new TH1F("hp0","P0", 100,500,700);
  TH1F *hp1 = new TH1F("hp1","P1", 100,50,250);
  TH1F *hp2 = new TH1F("hp2","P2", 30,0.01,0.06);
  TH1F *hp3 = new TH1F("hp3","P3", 100,-3,0);

  TH2F *hmap_p0 = new TH2F("hmap_p0","P0", 52,0,52,80,0,80);
  TH2F *hmap_p1 = new TH2F("hmap_p1","P1", 52,0,52,80,0,80);
  TH2F *hmap_p2 = new TH2F("hmap_p2","P2", 52,0,52,80,0,80);
  TH2F *hmap_p3 = new TH2F("hmap_p3","P3", 52,0,52,80,0,80);

//  Int_t pr1[4160];
//  Float_t fpc1[4160];
//  Double_t noise[4];
//  Double_t bv[4] = {70.0,100.0,130.0,160.0};
 
  Int_t dummy;
  Int_t St;
  Int_t Pl;
  Int_t Chip;
  Int_t Row;
  Int_t Col;
  Float_t P_P0;
  Float_t P_P1;
  Float_t P_P2;
  Float_t P_P3;
  
  TString dummy1; 
  TString dummy2; 
  TString dummy3; 
  TString dummy4; 
  TString dummy9; 
  TString dummy5; 
  TString dummy6; 
  TString dummy7; 
  TString dummy8; 
  TString dummy9; 
  TString dummy10; 

  //TString BaseName = "TELESCOPE_CAL_3_24_2011_0_0_7";
//    TString BaseName ;
//    cin >> BaseName ;

  //TString FileName1 = baseName+".out";
  TString FileName1 = "gains_" + baseName + ".txt"; 
  TString FileName2 = "gains_" + baseName + ".gif";
  TString FileName3 = "gains_" + baseName + "_map.gif";
  
  //ofstream out_par("FitParameters_0_0.txt");
  ofstream out_par;
  out_par.open("FitParameters_0_0.txt", ios::out|ios::app); 
  ifstream input_file_1 (FileName1);
//  ifstream input_file_1 ("MilanoFit_0_0_0.out");

  input_file_1 >> dummy1  >> dummy2 >> dummy3  >> dummy4 >> dummy5  >> dummy6 >> dummy7  >> dummy8 >> dummy9 >> dummy10;
   cout << dummy1 << dummy2 << dummy3  << dummy4 << dummy5  << dummy6 << dummy7  << dummy8 << dummy9 << endl ;
    // Read files
  for(Int_t n = 0; n<4161; n++){
    input_file_1 >> St  >> Pl >> Chip >> Row >> Col >> P_P0 >> P_P1 >> P_P2 >> P_P3; 
    cout << St  <<"  " <<  Pl << "  " << Chip << "  " << Row << "  " << Col << "  " << P_P0 << "  " << P_P1 << "  " << P_P2 << endl ;
    if(P_P1>0.0){
     hp0->Fill(P_P0);
     hp1->Fill(P_P1);
     hp2->Fill(P_P2);
     hp3->Fill(P_P3);
   }
     hmap_p0->Fill(Col,Row,P_P0);
     hmap_p1->Fill(Col,Row,P_P1);
     hmap_p2->Fill(Col,Row,P_P2);
     hmap_p3->Fill(Col,Row,P_P3);
  }
  
out_par << St  << "  " <<  Pl << "  " << Chip << " " << hp0->GetMean() << " " << hp0->GetRMS() << " " << hp1->GetMean() << " " << hp1->GetRMS() << " " << hp2->GetMean() <<" " << hp2->GetRMS() << " " << hp3->GetMean() <<" " << hp3->GetRMS() << endl ;

gStyle->SetOptStat(111111); 
 TCanvas *c1 = new TCanvas ("c1","Parameters");
 c1->Divide(2,2);
 c1->cd(1); hp0->Draw();
 c1->cd(2); hp1->Draw();
 c1->cd(3); hp2->Draw();
 c1->cd(4); hp3->Draw();
 c1->SaveAs(FileName2);

 gStyle->SetOptStat(0);
 TCanvas *c2 = new TCanvas ("c2","Parameter maps");
 c2->Divide(2,2);
 c2->cd(1); hmap_p0->SetMinimum(500); hmap_p0->SetMaximum(700); hmap_p0->Draw("COLZ");
 c2->cd(2); hmap_p1->SetMinimum(50); hmap_p1->SetMaximum(250); hmap_p1->Draw("COLZ");
 c2->cd(3); hmap_p2->SetMinimum(0.01); hmap_p2->SetMaximum(0.06); hmap_p2->Draw("COLZ");
 c2->cd(4); hmap_p3->SetMinimum(-3); hmap_p3->SetMaximum(0); hmap_p3->Draw("COLZ");
 c2->SaveAs(FileName3);

out_par.close();

  return;
}

