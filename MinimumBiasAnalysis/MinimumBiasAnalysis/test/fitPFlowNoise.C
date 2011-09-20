#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1.h>

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooPlot.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#endif

using namespace RooFit;

void fitPFlowNoise(std::string const& fileName){
   TFile* file = TFile::Open(fileName.c_str(),"READ");

   std::vector<std::string> variables;
   variables.push_back("energyVsEtaGamma_proj_Barrel_zplus");
   variables.push_back("energyVsEtaGamma_proj_Barrel_zminus");
   variables.push_back("energyVsEtaGamma_proj_Transition_zplus");
   variables.push_back("energyVsEtaGamma_proj_Transition_zminus");
   variables.push_back("energyVsEtaGamma_proj_Endcap_zplus");
   variables.push_back("energyVsEtaGamma_proj_Endcap_zminus");
   variables.push_back("energyVsEtaNeutralHadron_proj_Barrel_zplus");
   variables.push_back("energyVsEtaNeutralHadron_proj_Barrel_zminus");
   variables.push_back("energyVsEtaNeutralHadron_proj_Transition_zplus");
   variables.push_back("energyVsEtaNeutralHadron_proj_Transition_zminus");
   variables.push_back("energyVsEtaNeutralHadron_proj_Endcap_zplus");
   variables.push_back("energyVsEtaNeutralHadron_proj_Endcap_zminus");
   variables.push_back("energyVsEtaHadronHF_proj_Forward_zplus");
   variables.push_back("energyVsEtaHadronHF_proj_Forward_zminus");
   variables.push_back("energyVsEtaHadronHF_proj_Transition_zplus");
   variables.push_back("energyVsEtaHadronHF_proj_Transition_zminus");
   variables.push_back("energyVsEtaEGammaHF_proj_Forward_zplus");
   variables.push_back("energyVsEtaEGammaHF_proj_Forward_zminus");
   variables.push_back("energyVsEtaEGammaHF_proj_Transition_zplus");
   variables.push_back("energyVsEtaEGammaHF_proj_Transition_zminus");

   std::map<std::string,double> thresholds;
   std::vector<TCanvas*> canvases;
   std::vector<TLegend*> legends;
   std::vector<std::string>::const_iterator var = variables.begin();
   std::vector<std::string>::const_iterator vars_end = variables.end();
   for(;var != vars_end; ++var){
      TH1* histo = static_cast<TH1*>( file->Get( (*var).c_str() ) );
      int iBinMax = histo->GetMaximumBin();
      int iBinLow = -1;
      for(int ibin = 1; ibin <= iBinMax; ++ibin){
         if( histo->GetBinContent(ibin) >= 0.15*histo->GetMaximum() ) {iBinLow = ibin; break;}
      }

      double maxPeak = histo->GetBinCenter(iBinMax);
    
      double minHisto = histo->GetBinLowEdge(1);
      double maxHisto = histo->GetBinLowEdge(histo->GetNbinsX()) + histo->GetBinWidth(histo->GetNbinsX());

      double rangeFitLow = histo->GetBinLowEdge(iBinLow);
      double rangeFitHigh = maxHisto;

      std::cout << "Histogram range, maximum " << minHisto << ", " << maxHisto << ", " 
                                               << maxPeak << ", "
                                               << histo->GetMaximum() << std::endl;
      std::cout << "Fit limits "  << rangeFitLow << ", " << rangeFitHigh << std::endl;

      RooRealVar x((std::string(histo->GetName()) + "_var").c_str(),"energy",minHisto,maxHisto);
      RooDataHist hdata((std::string(histo->GetName()) + "_hdata").c_str(),"Binned data",RooArgList(x),histo);

      // First fit single Gaussian around peak
      double initRMS = histo->GetRMS();
      if(initRMS < histo->GetBinWidth(iBinMax)) initRMS = histo->GetBinWidth(iBinMax);
  
      double minFit = maxPeak - 2*initRMS;
      if(minFit < rangeFitLow) minFit = rangeFitLow; 
      double maxFit = maxPeak + 2*initRMS;
      std::cout << "Fit range "  << minFit << ", " << maxFit << std::endl;

      RooRealVar mean_singleGaus("mean_singleGaus","mean_singleGaus",maxPeak,minFit,maxFit);
      RooRealVar sigma_singleGaus("sigma_singleGaus","sigma_singleGaus",initRMS,0.,5.0);
      RooGaussian singleGaus((std::string(histo->GetName()) + "_singleGaus").c_str(),"Gaussian distribution",x,mean_singleGaus,sigma_singleGaus);
      singleGaus.fitTo(hdata,Range(minFit,maxFit));

      // Double gaussian
      minFit = mean_singleGaus.getVal() - 3*sigma_singleGaus.getVal();
      if(minFit < rangeFitLow) minFit = rangeFitLow;
      maxFit = mean_singleGaus.getVal() + 15*sigma_singleGaus.getVal();
      std::cout << "Fit range "  << minFit << ", " << maxFit << std::endl;

      RooRealVar sigma1("sigma1","sigma1",sigma_singleGaus.getVal(),0.,0.1);
      RooRealVar sigma2("sigma2","sigma2",5*sigma_singleGaus.getVal(),0.,5.0);
      RooRealVar mean1("mean1","mean1",mean_singleGaus.getVal(),minFit,maxFit);
      RooRealVar mean2("mean2","mean2",mean_singleGaus.getVal(),minFit,maxFit);
      RooRealVar frac("frac","frac",0.25,0.,1.);

      RooGaussian gaus1((std::string(histo->GetName()) + "_gaus1").c_str(),"Gaussian distribution",x,mean1,sigma1);
      RooGaussian gaus2((std::string(histo->GetName()) + "_gaus2").c_str(),"Gaussian distribution",x,mean2,sigma2);
      RooAddPdf model((std::string(histo->GetName()) + "_model").c_str(),"Model",RooArgList(gaus1,gaus2),RooArgList(frac));

      //model.fitTo( hdata,ROOT.RooFit.Minos(0) )
      //model.fitTo(hdata,ROOT.RooFit.Minos(0),ROOT.RooFit.Range(rangeFitLow,rangeFitHigh))
      model.fitTo(hdata,Range(minFit,maxFit));
      //model.fitTo(hdata);

      double fitMean = mean1.getVal();
      double fitSigma = sigma1.getVal();
      double thresholdNew = fitMean + 5*fitSigma;      
      thresholds[*var] = thresholdNew;

      RooPlot* frame = x.frame();
      hdata.plotOn(frame);
      //singleGaus.plotOn(frame,LineStyle(3));
      model.plotOn(frame);
      model.plotOn(frame,Components(gaus2),LineColor(2),LineStyle(2));

      canvases.push_back( new TCanvas( (std::string(histo->GetName()) + "_canvas").c_str() ) );
      canvases.back()->cd();
      frame->Draw();
      legends.push_back( new TLegend(0.45,0.55,0.95,0.80) );
      legends.back()->SetHeader( histo->GetName() );
      legends.back()->AddEntry(&hdata,"Data","LP");
      legends.back()->AddEntry(&model,"Fit","LF");
      legends.back()->SetFillColor( canvases.back()->GetFillColor() );
      legends.back()->Draw("SAME"); 
   }

   for(size_t k = 0; k < variables.size(); ++k) std::cout << variables[k] << " --> " << thresholds[variables[k]] << std::endl;
}
