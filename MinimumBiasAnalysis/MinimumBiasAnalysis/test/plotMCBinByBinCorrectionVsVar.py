import ROOT
import math
from scaleByWidth import scaleByWidth

def unfoldData(h_VarData,h_VarRecoMC,h_VarGenMC,h_VarRecoVsGenMC,iterations=4):

    nBinsReco = 7
    nBinsGen = 7
    from truncateHisto import truncateTH1F,truncateTH2F
    h_VarReco_range = truncateTH1F(h_VarRecoMC,nBinsReco)
    h_VarGen_range = truncateTH1F(h_VarGenMC,nBinsGen)
 
    from swapAxisTH2 import swapAxisTH2
    h_VarGenVsReco_swap = swapAxisTH2(h_VarRecoVsGenMC)
    h_VarGenVsReco_range = truncateTH2F(h_VarGenVsReco_swap,nBinsReco,nBinsGen)

    #response = ROOT.RooUnfoldResponse(h_VarReco_range,h_VarGen_range,h_VarGenVsReco_range)
    response = ROOT.RooUnfoldResponse(0,0,h_VarGenVsReco_range)
    unfold = ROOT.RooUnfoldBayes(response, h_VarData, iterations)
    #unfold = ROOT.RooUnfoldBinByBin(response, h_VarData)

    h_VarData_unfold = unfold.Hreco()
    h_VarData_unfold_errors = h_VarData_unfold.Clone(h_VarData_unfold.GetName() + "_errors")
    varData_unfold_errors = unfold.ErecoV()
    for ibin in range(0,h_VarData_unfold_errors.GetNbinsX()):
        print "Bin",ibin+1,"error",varData_unfold_errors[ibin]
        h_VarData_unfold_errors.SetBinError(ibin+1,varData_unfold_errors[ibin])

    return (h_VarData_unfold,h_VarData_unfold_errors)

def scaleFromHisto(histo,histoCorr):
    for ibin in range(1,histo.GetNbinsX()+1):
        scale = histoCorr.GetBinContent(ibin)
        histo.SetBinContent(ibin, histo.GetBinContent(ibin)*scale)
        histo.SetBinError(ibin, histo.GetBinError(ibin)*scale)

def addHistoErrors(histo,histoErr):
    for ibin in range(1,histo.GetNbinsX()+1):
        error = histo.GetBinError(ibin)
        errorAdd = histoErr.GetBinError(ibin)
        errorNew = math.sqrt(error*error + errorAdd*errorAdd)
        print "Bin",ibin,"error orig.",error,"add error",errorAdd,"total",errorNew
        histo.SetBinError(ibin, errorNew)

def setErrorsFromHisto(histo,histoErr):
    for ibin in range(1,histo.GetNbinsX()+1):
	binContent = histo.GetBinContent(ibin)
	corrValue = histoErr.GetBinContent(ibin)
	corrError = histoErr.GetBinError(ibin)
        errorNew = 0.
        if corrValue > 0.: errorNew = binContent*(corrError/corrValue)
        print "Bin",ibin,"value",binContent,"set error",errorNew
	histo.SetBinError(ibin,errorNew)

def plotMCBinByBinCorrection(fileNameData, fileNameMCRef, fileNameMCEff, fileNameUnfold = "", side = "plus"):
    if not fileNameUnfold: fileNameUnfold = fileNameMCEff

    ROOT.TH1.AddDirectory(False)

    #intLumi = 20.322 # /mub
    intLumi = 49.156 # /mub
    #intLumi = 500000./71260.
    #intLumi = 1816992./71260.
    sigmaMC = 71.26 # mb
    nLogXiBins = 2
    ###############################
    histoNames = {}
    if side == "plus":
        histoNames["VarRecoLogXi"]   = "multiplicityTracks_LogXiPlus"
        histoNames["VarGenLogXi"]    = "multiplicityTracksGen_LogXiPlus"
        histoNames["VarGenLogXiGen"] = "multiplicityTracksGen_LogXiGenPlus"
    elif side == "minus":
        histoNames["VarRecoLogXi"]   = "multiplicityTracks_LogXiMinus"
        histoNames["VarGenLogXi"]    = "multiplicityTracksGen_LogXiMinus"
        histoNames["VarGenLogXiGen"] = "multiplicityTracksGen_LogXiGenMinus"

    histoNames["VarReco"]      = "multiplicityTracks"
    histoNames["VarGen"]       = "multiplicityTracksGen"        
    histoNames["VarRecoVsGen"] = "multiplicityTracksVsGen"
    #histoNames["VarReco"]      = "multiplicityTracks_LogXiMinus_0"
    #histoNames["VarGen"]       = "multiplicityTracksGen_LogXiMinus_0"        
    #histoNames["VarRecoVsGen"] = "multiplicityTracksVsGen_LogXiGenMinus_0"
    ###############################
    file_data = ROOT.TFile(fileNameData,'read')
    #histos_VarLogXi_data = []
    #for idx in range(nLogXiBins):
    #    histos_VarLogXi_data.append( file_data.Get("%s_%d" % (histoNames["VarLogXi"],idx) ) )

    #files_mc_ref = []
    #files_mc_eff = [] 
    file_mc_ref = ROOT.TFile(fileNameMCRef,'read')
    file_mc_eff = ROOT.TFile(fileNameMCEff,'read')
    file_unfold = ROOT.TFile(fileNameUnfold,'read')
    h_EventSelection_mc_ref = file_mc_ref.Get("EventSelection")
    h_VarReco_unfold        = file_unfold.Get( histoNames["VarReco"] )
    h_VarGen_unfold         = file_unfold.Get( histoNames["VarGen"] )
    h_VarRecoVsGen_unfold   = file_unfold.Get( histoNames["VarRecoVsGen"] )

    histos_VarLogXi_data = []
    histos_VarLogXi_data_unfold = []
    histos_VarLogXi_data_unfold_errors = []
    histos_VarLogXi_data_corr_errors = []
    histos_VarLogXi_data_corr = []
    histos_VarLogXi_data_errors = []
    histos_VarLogXiGen_ref = []
    histos_VarLogXiGen = []
    histos_VarLogXi = []
    histos_corrVarLogXi = []
    histos_effVarLogXi = []
    histos_corrFullVarLogXi = []
    histos_VarLogXiGen_ref_scaled = []
    for idx in range(nLogXiBins):
        histos_VarLogXi_data.append( file_data.Get("%s_%d" % (histoNames["VarRecoLogXi"],idx)) )  

        histos_VarLogXiGen_ref.append( file_mc_ref.Get("%s_%d" % (histoNames["VarGenLogXiGen"],idx)) )
        histos_VarLogXiGen.append( file_mc_eff.Get("%s_%d" % (histoNames["VarGenLogXiGen"],idx)) )
        histos_VarLogXi.append( file_mc_eff.Get("%s_%d" % (histoNames["VarGenLogXi"],idx)) )

        ###############################
        # Unfolding
        unfoldResult = unfoldData(histos_VarLogXi_data[-1],
                                                       h_VarReco_unfold,
                                                       h_VarGen_unfold,
                                                       h_VarRecoVsGen_unfold, 5)  
        histos_VarLogXi_data_unfold.append( unfoldResult[0] )
        histos_VarLogXi_data_unfold_errors.append( unfoldResult[1] )
        ###############################
        # Correction factor
        histos_corrVarLogXi.append( histos_VarLogXiGen[-1].Clone('corrVarLogXi_%d' % idx) )
        histos_corrVarLogXi[-1].Divide( histos_VarLogXi[-1] )

        # Efficiency
        # FIXME: use graph and compute proper errors 
        histos_effVarLogXi.append( histos_VarLogXiGen[-1].Clone('effVarLogXi_%d' % idx) )
        histos_effVarLogXi[-1].Divide( histos_VarLogXiGen_ref[-1] )

        # Full bin by bin correction
        histos_corrFullVarLogXi.append( histos_corrVarLogXi[-1].Clone('corrFullVarLogXi_%d' % idx) )
        histos_corrFullVarLogXi[-1].Divide( histos_effVarLogXi[-1] )

        # Correct data
        #histos_VarLogXi_data_corr.append( histos_VarLogXi_data[idx].Clone('VarLogXi_data_%d' % idx) )
        histos_VarLogXi_data_corr.append( histos_VarLogXi_data_unfold[idx].Clone('VarLogXi_data_%d' % idx) ) 
        #histos_VarLogXi_data_corr[-1].Multiply( histos_corrFullVarLogXi[-1] ) 
        scaleFromHisto(histos_VarLogXi_data_corr[-1],histos_corrFullVarLogXi[-1])
        histos_VarLogXi_data_corr[-1].Scale(0.001/intLumi) # mb
        # Errors
        # Errors from correction factors
        histos_VarLogXi_data_corr_errors.append( histos_VarLogXi_data_corr[-1].Clone('VarLogXi_data_corr_errors_%d' % idx) )
        setErrorsFromHisto(histos_VarLogXi_data_corr_errors[-1],histos_corrFullVarLogXi[-1])
        # Unfolding errors
        scaleFromHisto(histos_VarLogXi_data_unfold_errors[-1],histos_corrFullVarLogXi[-1])
        histos_VarLogXi_data_unfold_errors[-1].Scale(0.001/intLumi) # mb
        # Sum all errors 
        histos_VarLogXi_data_errors.append( histos_VarLogXi_data_corr_errors[-1].Clone('VarLogXi_data_errors_%d' % idx) )
        addHistoErrors(histos_VarLogXi_data_errors[-1],histos_VarLogXi_data_unfold_errors[-1])

        # MC before selection
        histos_VarLogXiGen_ref_scaled.append( histos_VarLogXiGen_ref[-1].Clone('VarLogXiGen_ref_scaled_%d' % idx) )
        nEventsAll_ref = h_EventSelection_mc_ref.GetBinContent(1)
        histos_VarLogXiGen_ref_scaled[-1].Scale(sigmaMC/nEventsAll_ref)    
    
    # Plotting
    canvases = []
    histos = []
    lineStyles = (1,2,3)
    markerStylesData = (21,24,25)
    fillColorsData = (5,38,9)
    canvases.append(ROOT.TCanvas("c_effVarLogXi","effVarLogXi"))
    for idx in range(len(histos_effVarLogXi)):
        histos_effVarLogXi[idx].GetXaxis().SetTitle("N_{chg}")
        histos_effVarLogXi[idx].GetYaxis().SetTitle("#varepsilon^{MC}(N_{chg};#xi)")
        histos_effVarLogXi[idx].SetLineStyle( lineStyles[idx] )
        histos_effVarLogXi[idx].SetStats(0)
        if idx == 0: histos_effVarLogXi[idx].Draw()
        else:        histos_effVarLogXi[idx].Draw("SAME")
    histos.append(histos_effVarLogXi) 

    canvases.append(ROOT.TCanvas("c_corrVarLogXi","corrVarLogXi"))
    for idx in range(len(histos_corrVarLogXi)):
        histos_corrVarLogXi[idx].GetXaxis().SetTitle("N_{chg}")
        histos_corrVarLogXi[idx].GetYaxis().SetTitle("N(#xi^{Gen} #epsilon bin i)/N(#xi^{PF} #epsilon bin i)")
        histos_corrVarLogXi[idx].SetLineStyle( lineStyles[idx] )
        histos_corrVarLogXi[idx].SetStats(0)
        if idx == 0: histos_corrVarLogXi[idx].Draw()
        else:        histos_corrVarLogXi[idx].Draw("SAME")
    histos.append(histos_corrVarLogXi)
  
    canvases.append(ROOT.TCanvas("c_corrFullVarLogXi","corrFullVarLogXi"))
    for idx in range(len(histos_corrFullVarLogXi)):
        histos_corrFullVarLogXi[idx].GetXaxis().SetTitle("N_{chg}")
        histos_corrFullVarLogXi[idx].GetYaxis().SetTitle("#frac{1}{#varepsilon(N_{chg};#xi)}#frac{N(#xi^{Gen} #epsilon bin i)}{N(#xi^{PF} #epsilon bin i)}")
        histos_corrFullVarLogXi[idx].SetLineStyle( lineStyles[idx] )
        histos_corrFullVarLogXi[idx].SetStats(0)
        if idx == 0: histos_corrFullVarLogXi[idx].Draw()
        else:        histos_corrFullVarLogXi[idx].Draw("SAME")
    histos.append(histos_corrFullVarLogXi)

    canvases.append(ROOT.TCanvas("c_VarLogXi_data_corr","VarLogXi_data_corr"))
    for idx in range(len(histos_VarLogXi_data_corr)):
        histos_VarLogXi_data_errors[idx].GetXaxis().SetTitle("N_{chg}")
        histos_VarLogXi_data_errors[idx].GetYaxis().SetTitle("d#sigma/dN_{chg} (#xi) (mb)")
        histos_VarLogXi_data_errors[idx].SetMarkerSize(0.)
        histos_VarLogXi_data_errors[idx].SetFillColor( fillColorsData[idx] )
        histos_VarLogXi_data_errors[idx].SetStats(0)

        histos_VarLogXi_data_corr[idx].GetXaxis().SetTitle("N_{chg}")
        histos_VarLogXi_data_corr[idx].GetYaxis().SetTitle("d#sigma/dN_{chg} (#xi) (mb)")
        histos_VarLogXi_data_corr[idx].SetMarkerStyle( markerStylesData[idx] )
        histos_VarLogXi_data_corr[idx].SetMarkerSize(1.6)
        histos_VarLogXi_data_corr[idx].SetStats(0)

        scaleByWidth(histos_VarLogXi_data_corr[idx])
        scaleByWidth(histos_VarLogXi_data_errors[idx])
        #if idx == 0: histos_VarLogXi_data_corr[idx].Draw()
        #else:        histos_VarLogXi_data_corr[idx].Draw("SAME")
        if idx == 0: histos_VarLogXi_data_errors[idx].Draw("E2")
        else:        histos_VarLogXi_data_errors[idx].Draw("E2SAME")
        histos_VarLogXi_data_corr[idx].Draw("SAME")

        histos_VarLogXiGen_ref_scaled[idx].SetLineStyle( lineStyles[idx] )
        histos_VarLogXiGen_ref_scaled[idx].SetLineColor(2)
        histos_VarLogXiGen_ref_scaled[idx].SetStats(0)
        scaleByWidth(histos_VarLogXiGen_ref_scaled[idx])
        histos_VarLogXiGen_ref_scaled[idx].Draw("HISTOSAME")
    histos.append(histos_VarLogXi_data_corr)
    histos.append(histos_VarLogXi_data_errors)
    histos.append(histos_VarLogXiGen_ref_scaled)

    return (canvases,histos)
