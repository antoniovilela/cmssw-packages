{
	gROOT->ProcessLine(".L ~antoniov/tdrstyle.C");
	setTDRStyle();
        gStyle->SetOptStat(1111111);
        gStyle->SetHistLineWidth(2);
        //gStyle->SetHistFillStyle(1);
        gStyle->SetMarkerStyle(23);
        gStyle->SetMarkerSize(0.3);
        gStyle->SetErrorX(0.5);

        /*//Style settings
  	gDirectory->DeleteAll();
  	gROOT->SetStyle("Plain");

        gStyle->SetOptStat(1111111);
  	gStyle->SetOptFit(1);
        //gStyle->SetOptStat(0);

	gStyle->SetTitleFillColor(0);
	gStyle->SetMarkerStyle(23);
        gStyle->SetMarkerSize(0.3);
	gStyle->SetPadColor(0);
	gStyle->SetCanvasColor(0);
	gStyle->SetCanvasBorderMode(0);
	gStyle->SetPadBorderMode(0);
	gStyle->SetFrameFillStyle(0);
        gStyle->SetHistLineWidth(2);*/

        gSystem->Load("libFWCoreFWLite");
        AutoLibraryLoader::enable();
        gSystem->Load("libDataFormatsFWLite.so");
        gSystem->Load("libDataFormatsPatCandidates.so");
        gSystem->Load("libDataFormatsMETReco.so");
        //gSystem->Load("libExclusiveDijetsAnalysisExclusiveDijetsAnalysis.so");
        gSystem->Load("libMinimumBiasAnalysisMinimumBiasAnalysis.so");
        //gSystem->Load("libPlottingTools.so");
        gSystem->Load("libUtilitiesPlottingTools.so");
}
