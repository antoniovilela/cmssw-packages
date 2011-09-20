import ROOT
from MinimumBiasAnalysis.MinimumBiasAnalysis.pfThresholds_cfi import pfThresholds

"""
def getThreshold(pset,region,type,var):
    regionPSet = getattr(pset,region)
    typePSet = getattr(regionPSet,type)
    value = getattr(typePSet,var).value()
    return value
"""

def fitHisto(histo):
    nSigmas = 4
    fitOption = "R"
    iBinMax = histo.GetMaximumBin()
    iBinLow = -1
    for ibin in range(1,iBinMax + 1):
	if histo.GetBinContent(ibin) >= 0.15*histo.GetMaximum():
	    iBinLow = ibin
	    break
    rangeFitLow = histo.GetBinCenter(iBinLow)
    print "Histogram maximum",histo.GetBinCenter(iBinMax),histo.GetMaximum()
    print "Fit low range",rangeFitLow

    minFit = histo.GetBinCenter(iBinMax) - histo.GetRMS()
    if minFit < rangeFitLow: minFit = rangeFitLow
    maxFit = histo.GetBinCenter(iBinMax) + histo.GetRMS()

    funcName = histo.GetName() + "_gaus"
    fitFunc = ROOT.TF1(funcName,"gaus",minFit,maxFit)
    histo.Fit(fitFunc,fitOption)

    minFit = fitFunc.GetParameter(1) - nSigmas*fitFunc.GetParameter(2)
    if minFit < rangeFitLow: minFit = rangeFitLow 
    maxFit = fitFunc.GetParameter(1) + nSigmas*fitFunc.GetParameter(2)
    fitFunc.SetRange(minFit,maxFit)
    histo.Fit(fitFunc,fitOption)

    return (fitFunc.GetParameter(1),fitFunc.GetParameter(2))

def fitHistoDoubleGaus(histo):
    fitOption = "R"
    iBinMax = histo.GetMaximumBin()
    iBinLow = -1
    for ibin in range(1,iBinMax + 1):
	if histo.GetBinContent(ibin) >= 0.15*histo.GetMaximum():
	    iBinLow = ibin
	    break
    rangeFitLow = histo.GetBinCenter(iBinLow)
    maximumHisto = histo.GetMaximum()
    print "Histogram maximum",histo.GetBinCenter(iBinMax),maximumHisto
    print "Fit low range",rangeFitLow

    minFit = histo.GetBinCenter(iBinMax) - histo.GetRMS()
    if minFit < rangeFitLow: minFit = rangeFitLow
    maxFit = histo.GetBinCenter(iBinMax) + histo.GetRMS()

    funcName = histo.GetName() + "_gaus"
    fitFunc = ROOT.TF1(funcName,"gaus",minFit,maxFit)
    histo.Fit(fitFunc,fitOption)

    mean = fitFunc.GetParameter(1)
    sigma = fitFunc.GetParameter(2)
    if mean < rangeFitLow:
        mean = histo.GetBinCenter(iBinMax)

    minFit = mean - 3*sigma
    if minFit < rangeFitLow: minFit = rangeFitLow 
    maxFit = mean + 10*sigma

    funcName = histo.GetName() + "_doubleGaus"
    fitFuncDoubleGaus = ROOT.TF1(funcName,"gaus(0) + gaus(3)",minFit,maxFit)
    fitFuncDoubleGaus.SetParameters(fitFunc.GetParameter(0),mean,sigma,
                                    0.05*fitFunc.GetParameter(0),mean,10*sigma)
 
    fixMean = True
    if fixMean:
        fitFuncDoubleGaus.FixParameter(1,mean)
        fitFuncDoubleGaus.FixParameter(4,mean)

    histo.Fit(fitFuncDoubleGaus,fitOption)

    meanFinal = fitFuncDoubleGaus.GetParameter(4)
    sigmaFinal = fitFuncDoubleGaus.GetParameter(5)
    return (meanFinal,sigmaFinal)

def fitHistoRooFit(histo, plot=False):

    iBinMax = histo.GetMaximumBin()
    iBinLow = -1
    for ibin in range(1,iBinMax + 1):
	if histo.GetBinContent(ibin) >= 0.15*histo.GetMaximum():
	    iBinLow = ibin
	    break

    minHisto = histo.GetBinCenter(1)
    maxHisto = histo.GetBinCenter( histo.GetNbinsX() )
    rangeFitLow = histo.GetBinCenter(iBinLow)
    rangeFitHigh = histo.GetBinCenter( histo.GetNbinsX() )  
    print "Histogram range, maximum",minHisto,maxHisto,histo.GetBinCenter(iBinMax),histo.GetMaximum()
    print "Fit range",rangeFitLow,rangeFitHigh
    
    x = ROOT.RooRealVar(histo.GetName() + "_var","energy",minHisto,maxHisto)
    sigma1 = ROOT.RooRealVar("sigma1","sigma1",0.,0.7)
    sigma2 = ROOT.RooRealVar("sigma2","sigma2",0.,0.7)
    mean1 = ROOT.RooRealVar("mean1","mean1",histo.GetBinCenter(iBinMax),rangeFitLow,rangeFitHigh)
    mean2 = ROOT.RooRealVar("mean2","mean2",histo.GetBinCenter(iBinMax),rangeFitLow,rangeFitHigh)
    
    frac = ROOT.RooRealVar("frac","frac",0.,1.)
   
    gaus1 = ROOT.RooGaussian(histo.GetName() + "_gaus1","Gaussian distribution",x,mean1,sigma1)
    gaus2 = ROOT.RooGaussian(histo.GetName() + "_gaus2","Gaussian distribution",x,mean2,sigma2)
    model = ROOT.RooAddPdf(histo.GetName() + "_model","Model",ROOT.RooArgList(gaus1,gaus2),ROOT.RooArgList(frac))

    hdata = ROOT.RooDataHist(histo.GetName() + "_hdata","Binned data",ROOT.RooArgList(x),histo)
    #model.fitTo( hdata,ROOT.RooFit.Minos(0) )
    #model.fitTo(hdata,ROOT.RooFit.Minos(0),ROOT.RooFit.Range(rangeFitLow,rangeFitHigh))
    #model.fitTo(hdata,ROOT.RooFit.Range(rangeFitLow,rangeFitHigh))
    model.fitTo(hdata)

    objects = []
    if plot:
        frame = x.frame()
	hdata.plotOn(frame)
        model.plotOn(frame)
        model.plotOn(frame,ROOT.RooFit.Components( gaus2.GetName() ),
                           ROOT.RooFit.LineColor(2),ROOT.RooFit.LineStyle(2))
        
	#gaus1.plotOn(frame,ROOT.RooFit.LineStyle(3),ROOT.RooFit.LineColor(2))
	#gaus2.plotOn(frame,ROOT.RooFit.LineStyle(2),ROOT.RooFit.LineColor(3))
        
	canvas = ROOT.TCanvas("c_" + histo.GetName() + "_fit")
	canvas.SetGridy()
	#canvas.SetLogy()
	canvas.cd()
        frame.Draw()

        objects.append(x)
        objects.append(frame)
        objects.append(hdata)
        objects.append(model)
        objects.append(gaus1)
        objects.append(gaus2)
        objects.append(canvas) 

    return (mean1,mean2,sigma1,sigma2,frac,objects)

def plot(fileName,selection="pFlowAnalysisNoCollNoVtx",rebin=1,outputFileName=""):

    AddDirectoryStatus_ = ROOT.TH1.AddDirectoryStatus() 
    ROOT.TH1.AddDirectory(False)
    
    file = ROOT.TFile(fileName,'READ')

    particleTypes = ("all","gamma","hadronNeutral","hadronHF","emHF")
    histoNames = {}
    histoNames["all"] = "energyVsEtaAllTypes" 
    histoNames["gamma"] = "energyVsEtaGamma"
    histoNames["hadronNeutral"] = "energyVsEtaNeutralHadron"
    histoNames["hadronHF"] = "energyVsEtaHadronHF"
    histoNames["emHF"] = "energyVsEtaEGammaHF"
    regions = {}
    regions["Barrel"] = (0.,1.4)
    regions["Endcap"] = (1.4,2.6) 
    regions["Transition"] = (2.6,3.2)
    regions["Forward"] = (3.2,5.205)
    """
    thresholds = {}
    thresholds["all"] = {}
    thresholds["gamma"] = {"Barrel":0.9,
                           "Endcap":2.5,
                           "Transition":2.5}
    thresholds["hadronNeutral"] = {"Barrel":1.4,
                                   "Endcap":2.7,
                                   "Transition":3.8}
    thresholds["hadronHF"] = {"Transition":4.0,
                              "Forward":4.0}
    thresholds["emHF"] = {"Transition":3.5,
                          "Forward":3.5}
    """
    thresholds = {}
    for type in particleTypes:
        thresholds[type] = {} 
        if type == 'all': continue
        for region in regions:
	    regionPSet = getattr(pfThresholds,region)
            if hasattr(regionPSet,type):
		typePSet = getattr(regionPSet,type)
                if hasattr(typePSet,'energy'):
		    value = getattr(typePSet,'energy').value()
		    thresholds[type][region] = value

    canvases = []
    histos = []
    objects = []
    thresholdsPlus = {}
    thresholdsMinus = {}
    for type in particleTypes:
        if type == 'all': continue
        histo = file.Get("%s/%s" % (selection,histoNames[type]))
        #canvas = ROOT.TCanvas("c_" + histo.GetName())
        #canvas.SetGridy()
        #canvas.cd() 
        
        histo.SetMarkerSize(0.1)
        #histo.SetMarkerColor(4)
        histo.GetXaxis().SetTitle("#eta")
        histo.GetYaxis().SetTitle("Energy (GeV)")
        histo.SetStats(0)
        #histo.Draw("colz")

        lines = []
        canvasesProj = []         
        projections = []
        linesProj = []
        fitObjects = []
        frames = []
        thresholdsPlus[type] = {}
        thresholdsMinus[type] = {}
        for region in thresholds[type]:
            xrange = regions[region]
            ythreshold = thresholds[type][region]
            print "Threshold for %s, %s region, eta=[%.1f,%.1f): %.1f" % (type,region,xrange[0],xrange[1],ythreshold)

            #canvas.cd()
            lines.append(ROOT.TLine(xrange[0],ythreshold,xrange[1],ythreshold))
            lines[-1].SetLineColor(2)
            lines[-1].SetLineStyle(2)
            lines[-1].SetLineWidth(5)
            #lines[-1].Draw("SAME")
            lines.append(ROOT.TLine(-xrange[1],ythreshold,-xrange[0],ythreshold))
            lines[-1].SetLineColor(2) 
            lines[-1].SetLineStyle(2)
            lines[-1].SetLineWidth(4)
            #lines[-1].Draw("SAME")

            # Projections
            #canvasProj = ROOT.TCanvas("c_" + histo.GetName() + "_proj_" + region + "_zplus")
            #canvasesProj.append(canvasProj)
            #canvasesProj[-1].SetGridy()
            #canvasesProj[-1].SetLogy()
            #canvasesProj[-1].cd() 
            
            iBinLow = histo.GetXaxis().FindBin(xrange[0])
            iBinUpp = histo.GetXaxis().FindBin(xrange[1]) - 1
            print "Bin range for %s, %s region, eta=[%.1f,%.1f): [%d,%d]" % (type,region,xrange[0],xrange[1],iBinLow,iBinUpp)
            projectionY = histo.ProjectionY(histo.GetName() + "_proj_" + region + "_zplus", iBinLow, iBinUpp)
            projections.append(projectionY)
            projections[-1].SetMarkerStyle(20)
            projections[-1].SetMarkerSize(1.2) 
            projections[-1].Rebin(rebin)
            projections[-1].Scale( 1./projections[-1].GetSumOfWeights() )
            ###
            #fitMean,fitSigma = fitHistoDoubleGaus(projections[-1])
            #thresholdsPlus[type][region] = fitMean + 3*fitSigma  
            #ythresholdNew = thresholdsPlus[type][region]

            #returnFit = fitHistoRooFit(projections[-1],True) 
            #fitObjects.append( returnFit )
            #fitMean,fitSigma = returnFit[0].getVal(),returnFit[2].getVal()

	    iBinMax = projections[-1].GetMaximumBin()
	    iBinLow = -1
	    for ibin in range(1,iBinMax + 1):
		if projections[-1].GetBinContent(ibin) >= 0.15*projections[-1].GetMaximum():
		    iBinLow = ibin
		    break

	    minHisto = projections[-1].GetBinCenter(1)
	    maxHisto = projections[-1].GetBinCenter( projections[-1].GetNbinsX() )
	    rangeFitLow = projections[-1].GetBinCenter(iBinLow)
	    rangeFitHigh = projections[-1].GetBinCenter( projections[-1].GetNbinsX() )  
	    print "Histogram range, maximum",minHisto,maxHisto,projections[-1].GetBinCenter(iBinMax),projections[-1].GetMaximum()
	    print "Fit range",rangeFitLow,rangeFitHigh

	    x = ROOT.RooRealVar(projections[-1].GetName() + "_var","energy",minHisto,maxHisto)
	    sigma1 = ROOT.RooRealVar(projections[-1].GetName() + "_sigma1","sigma1",0.,0.7)
	    sigma2 = ROOT.RooRealVar(projections[-1].GetName() + "_sigma2","sigma2",0.,0.7)
	    mean1 = ROOT.RooRealVar(projections[-1].GetName() + "_mean1","mean1",projections[-1].GetBinCenter(iBinMax),rangeFitLow,rangeFitHigh)
	    mean2 = ROOT.RooRealVar(projections[-1].GetName() + "_mean2","mean2",projections[-1].GetBinCenter(iBinMax),rangeFitLow,rangeFitHigh)
	    
	    frac = ROOT.RooRealVar(projections[-1].GetName() + "_frac","frac",0.,1.)

	    gaus1 = ROOT.RooGaussian(projections[-1].GetName() + "_gaus1","Gaussian distribution",x,mean1,sigma1)
	    gaus2 = ROOT.RooGaussian(projections[-1].GetName() + "_gaus2","Gaussian distribution",x,mean2,sigma2)
	    model = ROOT.RooAddPdf(projections[-1].GetName() + "_model","Model",ROOT.RooArgList(gaus1,gaus2),ROOT.RooArgList(frac))

	    hdata = ROOT.RooDataHist(projections[-1].GetName() + "_hdata","Binned data",ROOT.RooArgList(x),projections[-1])
	    #model.fitTo( hdata,ROOT.RooFit.Minos(0) )
	    #model.fitTo(hdata,ROOT.RooFit.Minos(0),ROOT.RooFit.Range(rangeFitLow,rangeFitHigh))
	    #model.fitTo(hdata,ROOT.RooFit.Range(rangeFitLow,rangeFitHigh))
	    model.fitTo(hdata)
            
            fitMean,fitSigma = mean1.getVal(),sigma1.getVal()
            thresholdsPlus[type][region] = fitMean + 3*fitSigma  
            ythresholdNew = thresholdsPlus[type][region]
            fitObjects.append([mean1,mean2,sigma1,sigma2,x,hdata,model,gaus1,gaus2])

            #fitDrawObjs = fitObjects[-1][-1]
	    #frame = fitDrawObjs[0].frame()
	    #fitDrawObjs[1].plotOn(frame)
	    #fitDrawObjs[2].plotOn(frame)
	    #fitDrawObjs[2].plotOn(frame,ROOT.RooFit.Components( fitDrawObjs[4].GetName() ),
            #                            ROOT.RooFit.LineColor(2),ROOT.RooFit.LineStyle(2))

            frames.append( x.frame() )
	    hdata.plotOn(frames[-1])
            #model.plotOn(frame)
            #model.plotOn(frame,ROOT.RooFit.Components( gaus2.GetName() ),
            #                   ROOT.RooFit.LineColor(2),ROOT.RooFit.LineStyle(2))

            canvasProj = ROOT.TCanvas("c_" + histo.GetName() + "_proj_" + region + "_zplus",projections[-1].GetName())
            canvasesProj.append(canvasProj)
            canvasesProj[-1].SetGridy()
            #canvasesProj[-1].SetLogy()
            canvasesProj[-1].cd() 
	    frames[-1].Draw()
            break
            ### 
            #projections[-1].DrawNormalized() 
            #if len(fitObjects[-1]): fitObjects[-1][0].Draw()
            #projections[-1].Draw()
   
            linesProj.append(ROOT.TLine(ythresholdNew,0.,
                                        ythresholdNew,0.15))
            linesProj[-1].SetLineColor(2)
            linesProj[-1].SetLineStyle(2)
            linesProj[-1].SetLineWidth(4)
            linesProj[-1].Draw("SAME")

            canvasProj = ROOT.TCanvas("c_" + histo.GetName() + "_proj_" + region + "_zminus")
            canvasesProj.append(canvasProj)
            canvasesProj[-1].SetGridy()
            canvasesProj[-1].SetLogy()
            canvasesProj[-1].cd()

            iBinLow = histo.GetXaxis().FindBin(-xrange[1]) + 1
            iBinUpp = histo.GetXaxis().FindBin(-xrange[0])
            print "Bin range for %s, %s region, eta=(%.1f,%.1f]: [%d,%d]" % (type,region,-xrange[1],-xrange[0],iBinLow,iBinUpp)
            projectionY = histo.ProjectionY(histo.GetName() + "_proj_" + region + "_zminus", iBinLow, iBinUpp)
            projections.append(projectionY)
            projections[-1].SetMarkerStyle(20)
            projections[-1].SetMarkerSize(1.2)
            projections[-1].Rebin(rebin)
            projections[-1].Scale( 1./projections[-1].GetSumOfWeights() )
            ###
            fitMean,fitSigma = fitHistoDoubleGaus(projections[-1])
            thresholdsMinus[type][region] = fitMean + 3*fitSigma  
            ythresholdNew = thresholdsMinus[type][region]
            ###
            #projections[-1].DrawNormalized()
            projections[-1].Draw()

            linesProj.append(ROOT.TLine(ythresholdNew,0.,
                                        ythresholdNew,0.15))
            linesProj[-1].SetLineColor(2)
            linesProj[-1].SetLineStyle(2)
            linesProj[-1].SetLineWidth(4)
            linesProj[-1].Draw("SAME")
 
        #canvases.append(canvas)
        canvases.append(canvasesProj)
        histos.append(histo)
        for proj in projections: histos.append(proj)
        objects.append(fitObjects)
        objects.append(frames)
        objects.append(lines)
        objects.append(linesProj)
        break

    #ROOT.TH1.AddDirectory(AddDirectoryStatus_)

    for type in particleTypes:
        if not thresholdsPlus.has_key(type): continue
        for region in thresholdsPlus[type]:
            xrange = regions[region]
            ythreshold = thresholds[type][region]
            ythresholdNew = thresholdsPlus[type][region]
            print "New threshold for %s, %s region, eta=[%.1f,%.1f): %.2f --> %.2f" % (type,region,xrange[0],xrange[1],ythreshold,ythresholdNew)

    for type in particleTypes:
        if not thresholdsMinus.has_key(type): continue
        for region in thresholdsMinus[type]:
            xrange = regions[region]
            ythreshold = thresholds[type][region]
            ythresholdNew = thresholdsMinus[type][region]
            print "New threshold for %s, %s region, eta=(%.1f,%.1f]: %.2f --> %.2f" % (type,region,-xrange[1],xrange[0],ythreshold,ythresholdNew)

    if outputFileName:
        outputFile = ROOT.TFile(outputFileName,'recreate')
        outputFile.cd()
        for item in histos: item.Write()
        return 0
    else:       
        return (canvases,histos,objects)   
