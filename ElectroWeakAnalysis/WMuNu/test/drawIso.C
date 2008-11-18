{
	TFile* fileWmunu = TFile::Open("wToMuNuTree.root","read");
	TTree* treeWmunu = (TTree*)fileWmunu->Get("wToMuNuanalysis/data");
	TFile* fileZmumu = TFile::Open("ZmumuIsoTemplate.root","read");
	TTree* treeZmumu = (TTree*)fileZmumu->Get("data");

	TH1::SetDefaultSumw2(true);

	TH1F* hIsoWmunu = new TH1F("hIsoWmunu","hIsoWmunu",50,0.,10.0);
	//treeWmunu->Project("hIsoWmunu","muonIso","MT > 50.0");
	treeWmunu->Project("hIsoWmunu","muonIso");
	TH1F* hIsoZmumu = (TH1F*)hIsoWmunu->Clone("hIsoZmumu");
	hIsoZmumu->Reset();
	hIsoZmumu->SetTitle("hIsoZmumu");
	treeZmumu->Project("hIsoZmumu","PtSum");

	TH1F* hNormIsoWmunu = new TH1F("hNormIsoWmunu","hNormIsoWmunu",50,0.,0.4);
        //treeWmunu->Project("hNormIsoWmunu","muonNormIso","MT > 50.0");
	treeWmunu->Project("hNormIsoWmunu","muonNormIso");
        TH1F* hNormIsoZmumu = (TH1F*)hNormIsoWmunu->Clone("hNormIsoZmumu");
        hNormIsoZmumu->Reset();
        hNormIsoZmumu->SetTitle("hNormIsoZmumu");
        treeZmumu->Project("hNormIsoZmumu","NormIso");

	TH1F* hPtWmunu = new TH1F("hPtWmunu","hPtWmunu",50,0.,20.);
	treeWmunu->Project("hPtWmunu","genWPt");
	TH1F* hPtZmumu = (TH1F*)hPtWmunu->Clone("hPtZmumu");
	hPtZmumu->Reset();
	hPtZmumu->SetTitle("hPtZmumu");
	treeZmumu->Project("hPtZmumu","VBPt");

	hPtWmunu->Scale(1/hPtWmunu->Integral());
	hPtZmumu->Scale(1/hPtZmumu->Integral());

	TH1F* hRewPt = (TH1F*)hPtWmunu->Clone("hRewPt");
	hRewPt->Divide(hPtZmumu);

	TCanvas* canvas1 = new TCanvas("canvas1");
	canvas1->cd();
	hIsoWmunu->SetLineWidth(2);
	hIsoZmumu->SetLineColor(kRed);
	hIsoZmumu->SetLineStyle(2);
	hIsoZmumu->SetLineWidth(2);
	hIsoWmunu->DrawNormalized("EHIST");
	hIsoZmumu->DrawNormalized("EHISTSAMES");
	TCanvas* canvas2 = new TCanvas("canvas2");
        canvas2->cd();
	hNormIsoWmunu->SetLineWidth(2);
	hNormIsoZmumu->SetLineColor(kRed);
        hNormIsoZmumu->SetLineStyle(2);
	hNormIsoZmumu->SetLineWidth(2);
        hNormIsoWmunu->DrawNormalized("EHIST");
        hNormIsoZmumu->DrawNormalized("EHISTSAMES");
}
