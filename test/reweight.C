void reweight(){
	TFile fileZMET("ZMET.root","read");
	TFile fileWMET("WMET.root","update");

	TH1F* hVBPtInput = fileZMET.Get("hVBPt");
	TH1F* hVBPt = fileWMET.Get("hVBPt");
	TH1F* hPtNuInput = fileZMET.Get("hPtNu");
        TH1F* hPtNu = fileWMET.Get("hPtNu");
	TH2F* hMETvsVBPtInput = fileZMET.Get("hMETvsVBPt");
	TH2F* hMETvsVBPt = fileWMET.Get("hMETvsVBPt");
 	TH1F* hMETInput = fileZMET.Get("hMET");
	TH1F* hMET = fileWMET.Get("hMET");
	TH2F* hMTvsVBPtInput = fileZMET.Get("hMTvsVBPt");
        TH2F* hMTvsVBPt = fileWMET.Get("hMTvsVBPt");
	TH2F* hMTvsPtNuInput = fileZMET.Get("hMTvsPtNu");
        TH2F* hMTvsPtNu = fileWMET.Get("hMTvsPtNu");
        TH1F* hMTInput = fileZMET.Get("hMT");
        TH1F* hMT = fileWMET.Get("hMT");

	hVBPtInput->SetName("hVBPtInput");
	hPtNuInput->SetName("hPtNuInput");
	hMETvsVBPtInput->SetName("hMETvsVBPtInput");
	hMETInput->SetName("hMETInput");
	hMTvsVBPtInput->SetName("hMTvsVBPtInput");
        hMTInput->SetName("hMTInput");

	TH1F* hMETCorr = hMETInput->Clone("hMETCorr");
	hMETCorr->Reset();
	TH2F* hMETvsVBPtCorr = hMETvsVBPtInput->Clone("hMETvsVBPtCorr");
	hMETvsVBPtCorr->Reset();

	TH1F* hMTCorrPtNu = hMTInput->Clone("hMTCorrPtNu");
        hMTCorrPtNu->Reset();
        TH2F* hMTvsPtNuCorr = hMTvsPtNuInput->Clone("hMTvsPtNuCorr");
        hMTvsPtNuCorr->Reset();
 
	TH1F* hMTCorrVBPt = hMTInput->Clone("hMTCorrVBPt");
        hMTCorrVBPt->Reset();
        TH2F* hMTvsVBPtCorr = hMTvsVBPtInput->Clone("hMTvsVBPtCorr");
        hMTvsVBPtCorr->Reset();

	double contZ = hPtNuInput->Integral(0,hPtNuInput->GetNbinsX()+1);
        double contW = hPtNu->Integral(0,hPtNu->GetNbinsX()+1);
        if (contZ>0) hPtNuInput->Scale(1./contZ);
        if (contW>0) hPtNu->Scale(1./contW);
        TH1F* hPtNuRew = (TH1F*)hPtNu->Clone("hPtNuRew");
        hPtNuRew->Divide(hPtNuInput);

	for (int j=0; j<=hMTvsPtNuInput->GetNbinsY()+1; j++) {
                double entriesj = 0.;
                for (int i=0; i<=hMTvsPtNuInput->GetNbinsX()+1; i++) {
                        double entries = hMTvsPtNuInput->GetBinContent(i,j);
                        entries *= hPtNuRew->GetBinContent(i);
                        hMTvsPtNuCorr->SetBinContent(i,j,entries);
                        entriesj += hMTvsPtNuCorr->GetBinContent(i,j);
                }
                hMTCorrPtNu->SetBinContent(j,entriesj);
        }

	contZ = hVBPtInput->Integral(0,hVBPtInput->GetNbinsX()+1);
	contW = hVBPt->Integral(0,hVBPt->GetNbinsX()+1);
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

	for (int j=0; j<=hMTvsVBPtInput->GetNbinsY()+1; j++) {
                double entriesj = 0.;
                for (int i=0; i<=hMTvsVBPtInput->GetNbinsX()+1; i++) {
                        double entries = hMTvsVBPtInput->GetBinContent(i,j);
                        entries *= hVBPtRew->GetBinContent(i);
                        hMTvsVBPtCorr->SetBinContent(i,j,entries);
                        entriesj += hMTvsVBPtCorr->GetBinContent(i,j);
                }
                hMTCorrVBPt->SetBinContent(j,entriesj);
        }

	fileWMET.cd();
	hVBPtInput->Write();
	hPtNuInput->Write();
	hMETvsVBPtInput->Write();
        hMETInput->Write();
        hMTvsVBPtInput->Write();
	hMTvsPtNuInput->Write();
        hMTInput->Write();
	hMETvsVBPtCorr->Write();
	hMETCorr->Write();
	hMTCorrVBPt->Write();
	hMTvsVBPtCorr->Write();
	hMTCorrPtNu->Write();
	hMTvsPtNuCorr->Write();
	hVBPtRew->Write();
	hPtNuRew->Write();
	fileWMET.Write();

	fileZMET.Close();
	fileWMET.Close();	
}
