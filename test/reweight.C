void reweight(){
	TFile fileZMET("ZMET.root","read");
	TFile fileWMET("WMET.root","update");

	TH1F* hVBPtInput = fileZMET.Get("hVBPt");
	TH1F* hVBPt = fileWMET.Get("hVBPt");
	TH2F* hMETvsVBPtInput = fileZMET.Get("hMETvsVBPt");
	TH2F* hMETvsVBPt = fileWMET.Get("hMETvsVBPt");
 	TH1F* hMETInput = fileZMET.Get("hMET");
	TH1F* hMET = fileWMET.Get("hMET");

	TH1F* hMETCorr = hMETInput->Clone("hMETCorr");
	hMETCorr->Reset();
	TH2F* hMETvsVBPtCorr = hMETvsVBPtInput->Clone("hMETvsVBPtCorr");
	hMETvsVBPtCorr->Reset();

	double contZ = hVBPtInput->Integral(0,hVBPtInput->GetNbinsX()+1);
	double contW = hVBPt->Integral(0,hVBPt->GetNbinsX()+1);
	if (contZ>0) hVBPtInput->Scale(1./contZ);
	if (contW>0) hVBPt->Scale(1./contW);
	TH1F* hVBPtRew = (TH1F*)hVBPt->Clone("hVBPtRew");
	hVBPtRew->Divide(hVBPtInput);

	for (int j=0; j<=hMETvsVBPtInput->GetNbinsY()+1; j++) {
		double entriesj = 0.;
		for (int i=0; i<=hMETvsVBPtInput->GetNbinsX()+1; i++) {
			double entries = hMETvsVBPtInput->GetBinContent(i,j);
			entries *= hVBPtRew->GetBinContent(i);
			hMETvsVBPtCorr->SetBinContent(i,j,entries);
			entriesj += hMETvsVBPtCorr->GetBinContent(i,j);
		}
		hMETCorr->SetBinContent(j,entriesj);
  	}

	fileWMET.cd();
	hMETvsVBPtCorr->Write();
	hMETCorr->Write();
	hVBPtRew->Write();
	fileWMET.Write();

	fileZMET.Close();
	fileWMET.Close();	
}
