void getCorrelations(){
	TTree* tree = (TTree*)_file0->Get("wToMuNuanalysis/data");

	// Define regions
	vector<string> regions(5);
	regions[0] = "A";
	regions[1] = "B";
	regions[2] = "C";
	regions[3] = "D";
	regions[4] = "All";
	double mtmin[5] = {50.,0.,50.,0.,0.};
	double mtmax[5] = {200.,50.,200.,50.,200.};
	double ptsummin[5] = {0.,0.,3.,3.,0.};
	double ptsummax[5] = {3.,3.,1000.,1000.,1000.};
	double normisomin[5] = {0.,0.,0.09,0.09,0.};
        double normisomax[5] = {0.09,0.09,1000.,1000.,1000.};
	for(int idx = 0; idx < 5; ++idx){
		cout << "Region " << regions[idx] << endl;
		cout << "MT = (" << mtmin[idx] << "," << mtmax[idx] << ")" << endl;
		cout << "Iso = (" << ptsummin[idx] << "," << ptsummax[idx] << ")" << endl; 
		getMTIsoCorrelation(tree,mtmin[idx],mtmax[idx],ptsummin[idx],ptsummax[idx],false);
		cout << "MT = (" << mtmin[idx] << "," << mtmax[idx] << ")" << endl;
                cout << "Iso = (" << normisomin[idx] << "," << normisomax[idx] << ")" << endl;
		getMTIsoCorrelation(tree,mtmin[idx],mtmax[idx],normisomin[idx],normisomax[idx],true);
	}
}

double getMTIsoCorrelation(TTree* tree,Double_t MTmin = 0.,Double_t MTmax = 1000.,Double_t Isomin = 0.,Double_t Isomax = 1000., bool useNormIso = false){

	Double_t evtWeight,muonIso,muonNormIso,MT;
	tree->SetBranchAddress("evtWeight",&evtWeight);
	tree->SetBranchAddress("muonIso",&muonIso);
	tree->SetBranchAddress("muonNormIso",&muonNormIso);
	tree->SetBranchAddress("MT",&MT);

	//TFile* file = new TFile("correlations.root","recreate");
	//TH2F* h_MTvsPtSum = new TH2F("h_MTvsPtSum","h_MTvsPtSum",100,0.,200.,100,0.,40.);
	//TH2F* h_MTvsNormIso = new TH2F("h_MTvsNormIso","h_MTvsNormIso",100,0.,200.,100,0.,0.3);
	
	Double_t sumw = 0;
	Double_t sumwx = 0;
	Double_t sumwx2 = 0;
	Double_t sumwy = 0;
        Double_t sumwy2 = 0;
	Double_t sumwxy = 0;
	for(Int_t i = 0; i < tree->GetEntries(); ++i){
		tree->GetEntry(i);
		// Calculate correlations -- r = Cov(x,y)/sig(x)sig(y)
		Double_t Iso = (useNormIso)?muonNormIso:muonIso;
		if((MT < MTmin)||(MT > MTmax)||(Iso < Isomin)||(Iso > Isomax)) continue;
		sumw += evtWeight;
		sumwx += evtWeight*MT;
		sumwy += evtWeight*Iso;
		sumwx2 += evtWeight*MT*MT;
		sumwy2 += evtWeight*Iso*Iso;
		sumwxy += evtWeight*MT*Iso;
	}
	Double_t covxy = sumwxy/sumw - (sumwx/sumw)*(sumwy/sumw);
	Double_t sig2x = TMath::Abs(sumwx2/sumw - (sumwx/sumw)*(sumwx/sumw));
	Double_t sigx = TMath::Sqrt(sig2x); 
	Double_t sig2y = TMath::Abs(sumwy2/sumw - (sumwy/sumw)*(sumwy/sumw));
        Double_t sigy = TMath::Sqrt(sig2y);

	Double_t rxy = ((sigx == 0.)||(sigy == 0.))?0.:covxy/(sigx*sigy);

	cout << ">>>>>>>>>> Pearson's r for (MT," << ((useNormIso)?"NormIso":"Iso") << ") = " << rxy << endl;

	//file->cd();
	//file->Write();
	//file->Close();
}
