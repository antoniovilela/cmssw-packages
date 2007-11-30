void reweight(){
	TFile fileZMET("ZMET.root","read");
	TFile fileWMET("WMET.root","update");

	TH1F* hMETInput = fileZMET.Get("hMET");
        //TH1F* hMET = fileWMET.Get("hMET");
	TH1F* hMTInput = fileZMET.Get("hMT");
        //TH1F* hMT = fileWMET.Get("hMT");
	TH1F* hEtaNuInput = fileZMET.Get("hEtaNu");
	
	TH1F* hVBPtInput = fileZMET.Get("hVBPt");
	TH1F* hVBPt = fileWMET.Get("hVBPt");
	TH1F* hPtNuInput = fileZMET.Get("hPtNu");
        TH1F* hPtNu = fileWMET.Get("hPtNu");
	TH2F* hPtNuvsEtaNuInput = fileZMET.Get("hPtNuvsEtaNu");
        TH2F* hPtNuvsEtaNu = fileWMET.Get("hPtNuvsEtaNu");

	TH2F* hMETvsVBPtInput = fileZMET.Get("hMETvsVBPt");
	//TH2F* hMETvsVBPt = fileWMET.Get("hMETvsVBPt");
	TH2F* hMETvsPtNuInput = fileZMET.Get("hMETvsPtNu");
        //TH2F* hMETvsPtNu = fileWMET.Get("hMETvsPtNu");	
	TH2F* hMTvsVBPtInput = fileZMET.Get("hMTvsVBPt");
        //TH2F* hMTvsVBPt = fileWMET.Get("hMTvsVBPt");
	TH2F* hMTvsPtNuInput = fileZMET.Get("hMTvsPtNu");
        //TH2F* hMTvsPtNu = fileWMET.Get("hMTvsPtNu");
	TH3F* hMETvsPtNuvsEtaNuInput = fileZMET.Get("hMETvsPtNuvsEtaNu");
	TH3F* hMTvsPtNuvsEtaNuInput = fileZMET.Get("hMTvsPtNuvsEtaNu");
	
	hVBPtInput->SetName("hVBPtInput");
	hPtNuInput->SetName("hPtNuInput");
	hEtaNuInput->SetName("hEtaNuInput");
	hMETvsVBPtInput->SetName("hMETvsVBPtInput");
	hMETInput->SetName("hMETInput");
	hMTvsVBPtInput->SetName("hMTvsVBPtInput");
	hMTvsPtNuInput->SetName("hMTvsPtNuInput");
        hMTInput->SetName("hMTInput");
	hPtNuvsEtaNuInput->SetName("hPtNuvsEtaNuInput");

	TH1F* hMETCorrPtNu = hMETInput->Clone("hMETCorrPtNu");
        hMETCorrPtNu->Reset();
        TH2F* hMETvsPtNuCorr = hMETvsPtNuInput->Clone("hMETvsPtNuCorr");
        hMETvsPtNuCorr->Reset();

	TH1F* hMETCorrVBPt = hMETInput->Clone("hMETCorrVBPt");
	hMETCorrVBPt->Reset();
	TH2F* hMETvsVBPtCorr = hMETvsVBPtInput->Clone("hMETvsVBPtCorr");
	hMETvsVBPtCorr->Reset();

	TH1F* hMETCorrPtNuEtaNu = hMETInput->Clone("hMETCorrPtNuEtaNu");
        hMETCorrPtNuEtaNu->Reset();

	TH1F* hMTCorrPtNu = hMTInput->Clone("hMTCorrPtNu");
        hMTCorrPtNu->Reset();
        TH2F* hMTvsPtNuCorr = hMTvsPtNuInput->Clone("hMTvsPtNuCorr");
        hMTvsPtNuCorr->Reset();
 
	TH1F* hMTCorrVBPt = hMTInput->Clone("hMTCorrVBPt");
        hMTCorrVBPt->Reset();
        TH2F* hMTvsVBPtCorr = hMTvsVBPtInput->Clone("hMTvsVBPtCorr");
        hMTvsVBPtCorr->Reset();

	TH1F* hMTCorrPtNuEtaNu = hMTInput->Clone("hMTCorrPtNuEtaNu");
        hMTCorrPtNuEtaNu->Reset();

	//============================================================================
	double contZ = hPtNuInput->Integral(0,hPtNuInput->GetNbinsX()+1);
        double contW = hPtNu->Integral(0,hPtNu->GetNbinsX()+1);
        if (contZ>0) hPtNuInput->Scale(1./contZ);
        if (contW>0) hPtNu->Scale(1./contW);
        TH1F* hPtNuRew = (TH1F*)hPtNu->Clone("hPtNuRew");
        hPtNuRew->Divide(hPtNuInput);

	for (int j=0; j<=hMETvsPtNuInput->GetNbinsY()+1; j++) {
                double entriesj = 0.;
                for (int i=0; i<=hMETvsPtNuInput->GetNbinsX()+1; i++) {
                        double entries = hMETvsPtNuInput->GetBinContent(i,j);
                        entries *= hPtNuRew->GetBinContent(i);
                        hMETvsPtNuCorr->SetBinContent(i,j,entries);
                        entriesj += hMETvsPtNuCorr->GetBinContent(i,j);
                }
                hMETCorrPtNu->SetBinContent(j,entriesj);
        }

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

	//============================================================================
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
		hMETCorrVBPt->SetBinContent(j,entriesj);
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

	//============================================================================
	contZ = hPtNuvsEtaNuInput->Integral(0,hPtNuvsEtaNuInput->GetNbinsX()+1,0,hPtNuvsEtaNuInput->GetNbinsY()+1);
        contW = hPtNuvsEtaNu->Integral(0,hPtNuvsEtaNu->GetNbinsX()+1,0,hPtNuvsEtaNu->GetNbinsY()+1);
        if (contZ>0) hPtNuvsEtaNuInput->Scale(1./contZ);
        if (contW>0) hPtNuvsEtaNu->Scale(1./contW);
        TH2F* hPtNuvsEtaNuRew = (TH2F*)hPtNuvsEtaNu->Clone("hPtNuvsEtaNuRew");
        hPtNuvsEtaNuRew->Divide(hPtNuvsEtaNuInput);

	for (int k=0; k<=hMETvsPtNuvsEtaNuInput->GetNbinsZ()+1; k++) {//MET bins
		double entriesk = 0.;
		for (int i=0; i<=hMETvsPtNuvsEtaNuInput->GetNbinsX()+1; i++) {//PtNu bins
			for (int j=0; j<=hMETvsPtNuvsEtaNuInput->GetNbinsY()+1; j++) {//EtaNu bins
				double entries = hMETvsPtNuvsEtaNu->GetBinContent(i,j,k);
				entries *= hPtNuvsEtaNuRew->GetBinContent(j,i);
				entriesk += entries;
			}
		}
		hMETCorrPtNuEtaNu->SetBinContent(k,entriesk);
	}

	for (int k=0; k<=hMTvsPtNuvsEtaNuInput->GetNbinsZ()+1; k++) {//MT bins
                double entriesk = 0.;
                for (int i=0; i<=hMTvsPtNuvsEtaNuInput->GetNbinsX()+1; i++) {//PtNu bins
                        for (int j=0; j<=hMTvsPtNuvsEtaNuInput->GetNbinsY()+1; j++) {//EtaNu bins
                                double entries = hMTvsPtNuvsEtaNu->GetBinContent(i,j,k);
                                entries *= hPtNuvsEtaNuRew->GetBinContent(j,i);
                                entriesk += entries;
                        }
                }
                hMTCorrPtNuEtaNu->SetBinContent(k,entriesk);
        }
 	
	//============================================================================
	fileWMET.cd();
	hVBPtInput->Write();
	hPtNuInput->Write();
	hEtaNuInput->Write();
	hMETvsVBPtInput->Write();
        hMETInput->Write();
        hMTvsVBPtInput->Write();
	hMTvsPtNuInput->Write();
        hMTInput->Write();
	hMETCorrVBPt->Write();
	hMETvsVBPtCorr->Write();
	hMETCorrPtNu->Write();
        hMETvsPtNuCorr->Write();
	hMETCorrPtNuEtaNu->Write();
	hMTCorrVBPt->Write();
	hMTvsVBPtCorr->Write();
	hMTCorrPtNu->Write();
	hMTvsPtNuCorr->Write();
	hMTCorrPtNuEtaNu->Write();
	hVBPtRew->Write();
	hPtNuRew->Write();
	hPtNuvsEtaNuRew->Write();
	//fileWMET.Write();

	fileZMET.Close();
	fileWMET.Close();	
}
