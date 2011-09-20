#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TFile.h>
#include <TCanvas.h>
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

   std::map<std::string,double> thresholds;
   std::vector<TCanvas*> canvases;
   std::vector<std::string>::const_iterator var = variables.begin();
   std::vector<std::string>::const_iterator vars_end = variables.end();
   for(;var != vars_end; ++var){
      TH1* histo = static_cast<TH1*>( file->Get( (*var).c_str() ) );
      int iBinMax = histo->GetMaximumBin();
      int iBinLow = -1;
      for(int ibin = 1; ibin <= iBinMax; ++ibin){
         if( histo->GetBinContent(ibin) >= 0.15*histo->GetMaximum() ) {iBinLow = ibin; break;}
      }
    
      double minHisto = histo->GetBinCenter(1);
      double maxHisto = histo->GetBinCenter( histo->GetNbinsX() );
      double rangeFitLow = histo->GetBinCenter(iBinLow);
      double rangeFitHigh = histo->GetBinCenter( histo->GetNbinsX() );
      std::cout << "Histogram range, maximum" << minHisto << ", " << maxHisto << ", " 
                                              << histo->GetBinCenter(iBinMax) << ", " << histo->GetMaximum() << std::endl;
      std::cout << "Fit range"  << rangeFitLow << ", " << rangeFitHigh << std::endl;
  
      RooRealVar x((std::string(histo->GetName()) + "_var").c_str(),"energy",minHisto,maxHisto);
      RooRealVar sigma1("sigma1","sigma1",0.,0.7);
      RooRealVar sigma2("sigma2","sigma2",0.,0.7);
      RooRealVar mean1("mean1","mean1",histo->GetBinCenter(iBinMax),rangeFitLow,rangeFitHigh);
      RooRealVar mean2("mean2","mean2",histo->GetBinCenter(iBinMax),rangeFitLow,rangeFitHigh);

      RooRealVar frac("frac","frac",0.,1.);

      RooGaussian gaus1((std::string(histo->GetName()) + "_gaus1").c_str(),"Gaussian distribution",x,mean1,sigma1);
      RooGaussian gaus2((std::string(histo->GetName()) + "_gaus2").c_str(),"Gaussian distribution",x,mean2,sigma2);
      RooAddPdf model((std::string(histo->GetName()) + "_model").c_str(),"Model",RooArgList(gaus1,gaus2),RooArgList(frac));

      RooDataHist hdata((std::string(histo->GetName()) + "_hdata").c_str(),"Binned data",RooArgList(x),histo);
      //model.fitTo( hdata,ROOT.RooFit.Minos(0) )
      //model.fitTo(hdata,ROOT.RooFit.Minos(0),ROOT.RooFit.Range(rangeFitLow,rangeFitHigh))
      //model.fitTo(hdata,ROOT.RooFit.Range(rangeFitLow,rangeFitHigh))
      model.fitTo(hdata);

      double fitMean = mean1.getVal();
      double fitSigma = sigma1.getVal();
      double thresholdNew = fitMean + 3*fitSigma;      
      thresholds[*var] = thresholdNew;

      RooPlot* frame = x.frame();
      hdata.plotOn(frame);
      model.plotOn(frame);
      model.plotOn(frame,Components(gaus2),LineColor(2),LineStyle(2));

      canvases.push_back( new TCanvas( (std::string(histo->GetName()) + "_canvas").c_str() ) );
      canvases.back()->cd();
      frame->Draw();
   }

   for(size_t k = 0; k < variables.size(); ++k) std::cout << variables[k] << ", " << thresholds[variables[k]] << std::endl;
}
