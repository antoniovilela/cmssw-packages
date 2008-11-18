{
	TFile* fileZHistos = TFile::Open("ZmumuIsoTemplate.root","read");
	fileZHistos->cd();
	TH2F* hIsovsVBPtZmumu = (TH2F*)fileZHistos->Get("hMuIsovsVBPt");
	TH2F* hNormIsovsVBPtZmumu = (TH2F*)fileZHistos->Get("hMuNormIsovsVBPt");
	TH2F* hIsovsMuPtZmumu = (TH2F*)fileZHistos->Get("hMuIsovsMuPt");
	TH2F* hNormIsovsMuPtZmumu = (TH2F*)fileZHistos->Get("hMuNormIsovsMuPt");
	TH1F* hIsoZmumu = (TH1F*)fileZHistos->Get("hMuIsofromZmumu");
	TH1F* hNormIsoZmumu = (TH1F*)fileZHistos->Get("hMuNormIsofromZmumu");

	TFile* fileWTree = TFile::Open("/tmp/antoniov/WMETTree.root","read");
	fileWTree->cd();
	TTree* treeW = (TTree*)fileWTree->Get("data");

	Double_t VBPt,muonPt,muonIso,muonNormIso,MTfromZmumuv2,MT;
	treeW->SetBranchAddress("VBPt",&VBPt);
	treeW->SetBranchAddress("muonPt",&muonPt);
	treeW->SetBranchAddress("muonIso",&muonIso);
	treeW->SetBranchAddress("muonNormIso",&muonNormIso);
	treeW->SetBranchAddress("MTfromZmumuv2",&MTfromZmumuv2);
	treeW->SetBranchAddress("MT",&MT);

	TH1F* hIso = (TH1F*)hIsoZmumu->Clone("hIso");
	hIso->SetTitle("hIso");
	hIso->Reset();
	TH1F* hIsoMC = (TH1F*)hIsoZmumu->Clone("hIsoMC");
        hIsoMC->SetTitle("hIsoMC");
        hIsoMC->Reset();

	TH1F* hNormIso = (TH1F*)hNormIsoZmumu->Clone("hNormIso");
        hNormIso->SetTitle("hNormIso");
        hNormIso->Reset();
        TH1F* hNormIsoMC = (TH1F*)hNormIsoZmumu->Clone("hNormIsoMC");
        hNormIsoMC->SetTitle("hNormIsoMC");
        hNormIsoMC->Reset();

	TH2F* hIsovsMT = new TH2F("hIsovsMT","hIsovsMT",80,0.,200.,50,0.,10.);
	TH2F* hIsovsMTMC = (TH2F*)hIsovsMT->Clone("hIsovsMTMC");
	hIsovsMTMC->SetTitle("hIsovsMTMC");	
	TH2F* hNormIsovsMT = new TH2F("hNormIsovsMT","hNormIsovsMT",80,0.,200.,50,0.,0.3);
	TH2F* hNormIsovsMTMC = (TH2F*)hNormIsovsMT->Clone("hNormIsovsMTMC");
        hNormIsovsMTMC->SetTitle("hNormIsovsMTMC");

	bool usemuPt = false;
	TH1D* hProj_aux;
	for(Int_t entry = 0; entry < treeW->GetEntries(); ++entry){
		treeW->GetEntry(entry);
		
		if(usemuPt){
			int binmupt = (hIsovsMuPtZmumu->GetXaxis())->FindBin(muonPt);
			cout << ">>> Muon pt, bin in Z histo= " << muonPt << ", " << binmupt << endl;
			hProj_aux = hIsovsMuPtZmumu->ProjectionY("_pbinmupt",binmupt,binmupt);
		} else{
			int binWpt = (hIsovsVBPtZmumu->GetXaxis())->FindBin(VBPt);
			cout << ">>> W pt, bin in Z histo= " << VBPt << ", " << binWpt << endl;
			hProj_aux = hIsovsVBPtZmumu->ProjectionY("_pbinWpt",binWpt,binWpt);
		}
		//Get Iso value
		double isoFromZmumu = hProj_aux->GetRandom();
		cout << ">>>> Iso value from Zmumu= " << isoFromZmumu << endl;

		delete hProj_aux;

		if(usemuPt){
			binmupt = (hNormIsovsMuPtZmumu->GetXaxis())->FindBin(muonPt);
			cout << ">>> Muon pt, bin in Z histo= " << muonPt << ", " << binmupt << endl;
			hProj_aux = hNormIsovsMuPtZmumu->ProjectionY("_pbinmupt",binmupt,binmupt);
                } else{
			 binWpt = (hNormIsovsVBPtZmumu->GetXaxis())->FindBin(VBPt);
			cout << ">>> W pt, bin in Z histo= " << VBPt << ", " << binWpt << endl;
			hProj_aux = hNormIsovsVBPtZmumu->ProjectionY("_pbinWpt",binWpt,binWpt);
		}
                //Get Iso value
                double normIsoFromZmumu = hProj_aux->GetRandom();
                cout << ">>>> Norm. Iso value from Zmumu= " << normIsoFromZmumu << endl;		

		delete hProj_aux;

		//Fill reweighted histo
		hIso->Fill(isoFromZmumu);
		hIsoMC->Fill(muonIso);
		hNormIso->Fill(normIsoFromZmumu);
                hNormIsoMC->Fill(muonNormIso);
		hIsovsMT->Fill(MTfromZmumuv2,isoFromZmumu);
		hNormIsovsMT->Fill(MTfromZmumuv2,normIsoFromZmumu);
		hIsovsMTMC->Fill(MT,muonIso);
		hNormIsovsMTMC->Fill(MT,muonNormIso);
	}  

	//Draw histos
	TCanvas* canvas1 = new TCanvas("canvas1");
	canvas1->cd();
	hIsoZmumu->SetLineWidth(2);
	hIsoZmumu->SetLineStyle(3);
	hIso->SetLineWidth(2);
	hIso->SetLineStyle(2);
	hIsoMC->SetLineWidth(2);
	hIsoMC->DrawNormalized("HISTO");
	hIsoZmumu->DrawNormalized("SAMESHISTO");
	hIso->DrawNormalized("SAMESHISTO");

	TCanvas* canvas2 = new TCanvas("canvas2");
        canvas2->cd();
        hNormIsoZmumu->SetLineWidth(2);
        hNormIsoZmumu->SetLineStyle(3);
        hNormIso->SetLineWidth(2);
        hNormIso->SetLineStyle(2);
        hNormIsoMC->SetLineWidth(2);
        hNormIsoMC->DrawNormalized("HISTO");
        hNormIsoZmumu->DrawNormalized("SAMESHISTO");
        hNormIso->DrawNormalized("SAMESHISTO");

	TCanvas* canvas31 = new TCanvas("canvas31");
        canvas31->cd();
	hIsovsMT->Draw();

	TCanvas* canvas32 = new TCanvas("canvas32");
        canvas32->cd();
        hIsovsMTMC->Draw();

	TCanvas* canvas41 = new TCanvas("canvas41");
        canvas41->cd();
        hNormIsovsMT->Draw();

	TCanvas* canvas42 = new TCanvas("canvas42");
        canvas42->cd();
        hNormIsovsMTMC->Draw();
}
