import ROOT
from MinimumBiasAnalysis.MinimumBiasAnalysis.pfThresholds_cfi import pfThresholds

"""
def getThreshold(pset,region,type,var):
    regionPSet = getattr(pset,region)
    typePSet = getattr(regionPSet,type)
    value = getattr(typePSet,var).value()
    return value
"""

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
    thresholdsNew = {}
    for type in particleTypes:
        histo = file.Get("%s/%s" % (selection,histoNames[type]))
        canvas = ROOT.TCanvas("c_" + histo.GetName())
        #canvas.SetGridy()
        canvas.cd() 
        
        histo.SetMarkerSize(0.1)
        #histo.SetMarkerColor(4)
        histo.GetXaxis().SetTitle("#eta")
        histo.GetYaxis().SetTitle("Energy (GeV)")
        histo.SetStats(0)
        histo.Draw("colz")

        lines = []
        canvasesProj = []         
        projections = []
        linesProj = []
        thresholdsNew[type] = {}
        for region in thresholds[type]:
            xrange = regions[region]
            ythreshold = thresholds[type][region]
            print "Threshold for %s, %s region, eta=[%.1f,%.1f): %.1f" % (type,region,xrange[0],xrange[1],ythreshold)

            canvas.cd()
            lines.append(ROOT.TLine(xrange[0],ythreshold,xrange[1],ythreshold))
            lines[-1].SetLineColor(2)
            lines[-1].SetLineStyle(2)
            lines[-1].SetLineWidth(5)
            lines[-1].Draw("SAME")
            lines.append(ROOT.TLine(-xrange[1],ythreshold,-xrange[0],ythreshold))
            lines[-1].SetLineColor(2) 
            lines[-1].SetLineStyle(2)
            lines[-1].SetLineWidth(4)
            lines[-1].Draw("SAME")

            # Projections
            canvasProj = ROOT.TCanvas("c_" + histo.GetName() + "_proj_" + region + "_zplus")
            canvasesProj.append(canvasProj)
            canvasesProj[-1].SetGridy()
            canvasesProj[-1].SetLogy()
            canvasesProj[-1].cd()

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
            nSigmas = 4
            fitOption = "R"
            iBinMax = projections[-1].GetMaximumBin()
            minFit = projections[-1].GetBinCenter(iBinMax) - projections[-1].GetRMS()
            maxFit = projections[-1].GetBinCenter(iBinMax) + projections[-1].GetRMS()

	    funcName = projections[-1].GetName() + "_gaus"
	    fitFunc = ROOT.TF1(funcName,"gaus",minFit,maxFit)
	    projections[-1].Fit(fitFunc,fitOption)

	    minFit = fitFunc.GetParameter(1) - nSigmas*fitFunc.GetParameter(2)
	    maxFit = fitFunc.GetParameter(1) + nSigmas*fitFunc.GetParameter(2)
	    fitFunc.SetRange(minFit,maxFit)
	    projections[-1].Fit(fitFunc,fitOption)
            thresholdsNew[type][region] = fitFunc.GetParameter(1) + 5*fitFunc.GetParameter(2)  
            ###
            #projections[-1].DrawNormalized() 
            projections[-1].Draw()
   
            linesProj.append(ROOT.TLine(ythreshold,0.,
                                        ythreshold,0.15))
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
            projections[-1].DrawNormalized()

            linesProj.append(ROOT.TLine(ythreshold,0.,
                                        ythreshold,0.15))
            linesProj[-1].SetLineColor(2)
            linesProj[-1].SetLineStyle(2)
            linesProj[-1].SetLineWidth(4)
            linesProj[-1].Draw("SAME")     
 
        canvases.append(canvas)
        canvases.append(canvasesProj)
        histos.append(histo)
        histos.append(projections)
        objects.append(lines)
        objects.append(linesProj)

    ROOT.TH1.AddDirectory(AddDirectoryStatus_)

    for type in particleTypes:
        for region in thresholds[type]:
            xrange = regions[region]
            ythreshold = thresholds[type][region]
            ythresholdNew = thresholdsNew[type][region]
            print "New threshold for %s, %s region, eta=[%.1f,%.1f): %.2f --> %.2f" % (type,region,xrange[0],xrange[1],ythreshold,ythresholdNew)

    if outputFileName:
        outputFile = ROOT.TFile(outputFileName,'recreate')
        outputFile.cd()
        for item in histos: item.Write()
        return 0
    else:       
        return (canvases,histos,objects)   
