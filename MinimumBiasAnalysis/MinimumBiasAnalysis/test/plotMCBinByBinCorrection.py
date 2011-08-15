import ROOT
from scaleByWidth import scaleByWidth

def plotMCBinByBinCorrection(fileNameData, fileNamesMCRef, fileNamesMCEff, side = "plus"):
    ROOT.TH1.AddDirectory(False)

    #intLumi = 20.322 # /mub
    intLumi = 500000./71260.;
    sigmaMC = 71.26 # mb
    ###############################
    histoNames = {}
    if side == "plus":
        histoNames["logXiFromPFCandsVarBin"] = "logXiPlusFromPFCandsVarBin"
        histoNames["logXiGenVarBin"] = "logXiGenPlusVarBin"
        histoNames["logXiFromPFCandsANDXiGenVarBin"] = "logXiFromPFCandsANDXiGenPlusVarBin" 
    elif side == "minus":
        histoNames["logXiFromPFCandsVarBin"] = "logXiMinusFromPFCandsVarBin"
        histoNames["logXiGenVarBin"] = "logXiGenMinusVarBin"
        histoNames["logXiFromPFCandsANDXiGenVarBin"] = "logXiFromPFCandsANDXiGenMinusVarBin"

    ###############################
    file_data = ROOT.TFile(fileNameData,'read')
    h_logXiFromPFCandsVarBin_data = file_data.Get(histoNames["logXiFromPFCandsVarBin"])

    files_mc_ref = []
    files_mc_eff = []
    histos_logXiGenVarBin_ref = []
    histos_EventSelection_ref = []
    histos_logXiFromPFCandsVarBin = []
    histos_logXiGenVarBin = []
    histos_logXiFromPFCandsANDXiGenVarBin = []
    histos_logXiPurityVarBin = []
    histos_logXiStabilityVarBin = []
    histos_logXiCorrVarBin = []
    histos_effLogXiGenVarBin = []
    histos_logXiCorrFullVarBin = []
    histos_logXiFromPFCandsVarBin_data_corr = []
    histos_logXiGenVarBin_ref_scaled = []
    for idx in range(len(fileNamesMCRef)):
        files_mc_ref.append( ROOT.TFile(fileNamesMCRef[idx],'read') )
        histos_logXiGenVarBin_ref.append( files_mc_ref[-1].Get(histoNames["logXiGenVarBin"]) )
        histos_EventSelection_ref.append( files_mc_ref[-1].Get("EventSelection") )

        files_mc_eff.append( ROOT.TFile(fileNamesMCEff[idx],'read') )
        histos_logXiFromPFCandsVarBin.append( files_mc_eff[-1].Get(histoNames["logXiFromPFCandsVarBin"]) )
        histos_logXiGenVarBin.append( files_mc_eff[-1].Get(histoNames["logXiGenVarBin"]) )
        histos_logXiFromPFCandsANDXiGenVarBin.append( files_mc_eff[-1].Get(histoNames["logXiFromPFCandsANDXiGenVarBin"]) )

        ###############################
        # Purity & stability
        histos_logXiPurityVarBin.append( histos_logXiFromPFCandsANDXiGenVarBin[-1].Clone('logXiPurityVarBin_%d' % idx) )
        histos_logXiPurityVarBin[-1].Divide( histos_logXiFromPFCandsVarBin[-1] )
        histos_logXiStabilityVarBin.append( histos_logXiFromPFCandsANDXiGenVarBin[-1].Clone('logXiStabilityVarBin_%d' % idx) )
        histos_logXiStabilityVarBin[-1].Divide(histos_logXiGenVarBin[-1])
 
        # Correction factor
        histos_logXiCorrVarBin.append( histos_logXiGenVarBin[-1].Clone('logXiCorrVarBin_%d' % idx) )
        histos_logXiCorrVarBin[-1].Divide( histos_logXiFromPFCandsVarBin[-1] )

        # Efficiency
        # FIXME: use graph and compute proper errors 
        histos_effLogXiGenVarBin.append( histos_logXiGenVarBin[-1].Clone('effLogXiGenVarBin_%d' % idx) )
        histos_effLogXiGenVarBin[-1].Divide( histos_logXiGenVarBin_ref[-1] )

        # Full bin by bin correction
        histos_logXiCorrFullVarBin.append( histos_logXiCorrVarBin[-1].Clone('logXiCorrFullVarBin_%d' % idx) )
        histos_logXiCorrFullVarBin[-1].Divide( histos_effLogXiGenVarBin[-1] )

        # Correct data
        histos_logXiFromPFCandsVarBin_data_corr.append( h_logXiFromPFCandsVarBin_data.Clone('logXiFromPFCandsVarBin_data_corr_%d' % idx) )
        histos_logXiFromPFCandsVarBin_data_corr[-1].Multiply( histos_logXiCorrFullVarBin[-1] )
        histos_logXiFromPFCandsVarBin_data_corr[-1].Scale(0.001/intLumi) # mb

        # MC before selection
        histos_logXiGenVarBin_ref_scaled.append( histos_logXiGenVarBin_ref[-1].Clone('logXiGenVarBin_ref_scaled_%d' % idx) )
        nEventsAll_ref = histos_EventSelection_ref[-1].GetBinContent(1)
        histos_logXiGenVarBin_ref_scaled[-1].Scale(sigmaMC/nEventsAll_ref)    
    
    # Plotting
    canvases = []
    histos = []
    lineStyles = (1,2,3)
    markerStylesData = (21,24,25)
    canvases.append(ROOT.TCanvas("c_logXiPurityVarBin","logXiPurityVarBin"))
    for idx in range(len(histos_logXiPurityVarBin)):
        histos_logXiPurityVarBin[idx].GetXaxis().SetTitle("log(#xi)") 
        histos_logXiPurityVarBin[idx].GetYaxis().SetTitle("p(#xi^{PF},#xi^{Gen} #epsilon bin i | #xi^{PF} #epsilon bin i)")
        histos_logXiPurityVarBin[idx].SetLineStyle( lineStyles[idx] )
        histos_logXiPurityVarBin[idx].SetStats(0)
        if idx == 0: histos_logXiPurityVarBin[idx].Draw()
        else:        histos_logXiPurityVarBin[idx].Draw("SAME")
    histos.append(histos_logXiPurityVarBin)

    canvases.append(ROOT.TCanvas("c_logXiStabilityVarBin","logXiStabilityVarBin"))
    for idx in range(len(histos_logXiStabilityVarBin)):
        histos_logXiStabilityVarBin[idx].GetXaxis().SetTitle("log(#xi)")
        histos_logXiStabilityVarBin[idx].GetYaxis().SetTitle("p(#xi^{PF},#xi^{Gen} #epsilon bin i | #xi^{Gen} #epsilon bin i)")
        histos_logXiStabilityVarBin[idx].SetLineStyle( lineStyles[idx] )
        histos_logXiStabilityVarBin[idx].SetStats(0)
        if idx == 0: histos_logXiStabilityVarBin[idx].Draw()
        else:        histos_logXiStabilityVarBin[idx].Draw("SAME")
    histos.append(histos_logXiStabilityVarBin)

    canvases.append(ROOT.TCanvas("c_effLogXiGenVarBin","effLogXiGenVarBin"))
    for idx in range(len(histos_effLogXiGenVarBin)):
        histos_effLogXiGenVarBin[idx].GetXaxis().SetTitle("log(#xi)")
        histos_effLogXiGenVarBin[idx].GetYaxis().SetTitle("#varepsilon^{MC}(#xi)")
        histos_effLogXiGenVarBin[idx].SetLineStyle( lineStyles[idx] )
        histos_effLogXiGenVarBin[idx].SetStats(0)
        if idx == 0: histos_effLogXiGenVarBin[idx].Draw()
        else:        histos_effLogXiGenVarBin[idx].Draw("SAME")
    histos.append(histos_effLogXiGenVarBin) 

    canvases.append(ROOT.TCanvas("c_logXiCorrVarBin","logXiCorrVarBin"))
    for idx in range(len(histos_logXiCorrVarBin)):
        histos_logXiCorrVarBin[idx].GetXaxis().SetTitle("log(#xi)")
        histos_logXiCorrVarBin[idx].GetYaxis().SetTitle("N(#xi^{Gen} #epsilon bin i)/N(#xi^{PF} #epsilon bin i)")
        histos_logXiCorrVarBin[idx].SetLineStyle( lineStyles[idx] )
        histos_logXiCorrVarBin[idx].SetStats(0)
        if idx == 0: histos_logXiCorrVarBin[idx].Draw()
        else:        histos_logXiCorrVarBin[idx].Draw("SAME")
    histos.append(histos_logXiCorrVarBin)
  
    canvases.append(ROOT.TCanvas("c_logXiCorrFullVarBin","logXiCorrFullVarBin"))
    for idx in range(len(histos_logXiCorrFullVarBin)):
        histos_logXiCorrFullVarBin[idx].GetXaxis().SetTitle("log(#xi)")
        histos_logXiCorrFullVarBin[idx].GetYaxis().SetTitle("#frac{1}{#varepsilon(#xi)}#frac{N(#xi^{Gen} #epsilon bin i)}{N(#xi^{PF} #epsilon bin i)}")
        histos_logXiCorrFullVarBin[idx].SetLineStyle( lineStyles[idx] )
        histos_logXiCorrFullVarBin[idx].SetStats(0)
        if idx == 0: histos_logXiCorrFullVarBin[idx].Draw()
        else:        histos_logXiCorrFullVarBin[idx].Draw("SAME")
    histos.append(histos_logXiCorrFullVarBin)

    canvases.append(ROOT.TCanvas("c_logXiFromPFCandsVarBin_data_corr","logXiFromPFCandsVarBin_data_corr"))
    for idx in range(len(histos_logXiFromPFCandsVarBin_data_corr)):
        histos_logXiFromPFCandsVarBin_data_corr[idx].GetXaxis().SetTitle("log(#xi)")
        histos_logXiFromPFCandsVarBin_data_corr[idx].GetYaxis().SetTitle("d#sigma/dlog(#xi) (mb)")
        histos_logXiFromPFCandsVarBin_data_corr[idx].SetMarkerStyle( markerStylesData[idx] )
        histos_logXiFromPFCandsVarBin_data_corr[idx].SetMarkerSize(1.6)
        histos_logXiFromPFCandsVarBin_data_corr[idx].SetStats(0)

        scaleByWidth(histos_logXiFromPFCandsVarBin_data_corr[idx])
        if idx == 0: histos_logXiFromPFCandsVarBin_data_corr[idx].Draw()
        else:        histos_logXiFromPFCandsVarBin_data_corr[idx].Draw("SAME")

        histos_logXiGenVarBin_ref_scaled[idx].SetLineStyle( lineStyles[idx] )
        histos_logXiGenVarBin_ref_scaled[idx].SetLineColor(2)
        histos_logXiGenVarBin_ref_scaled[idx].SetStats(0)
        scaleByWidth(histos_logXiGenVarBin_ref_scaled[idx])
        histos_logXiGenVarBin_ref_scaled[idx].Draw("HISTOSAME")
    histos.append(histos_logXiFromPFCandsVarBin_data_corr)
    histos.append(histos_logXiGenVarBin_ref_scaled)

    return (canvases,histos)

def plotMCEfficiencySD(fileNameMCSDPlusRef, fileNameMCSDPlusEff, fileNameMCSDMinusRef, fileNameMCSDMinusEff):
    ROOT.TH1.AddDirectory(False)

    ###############################
    histoNamesSDPlus = {}
    histoNamesSDMinus = {}
    histoNamesSDPlus["logXiFromPFCandsVarBin"] = "logXiPlusFromPFCandsVarBin"
    histoNamesSDPlus["logXiGenVarBin"] = "logXiGenPlusVarBin"
    histoNamesSDPlus["logXiFromPFCandsANDXiGenVarBin"] = "logXiFromPFCandsANDXiGenPlusVarBin"
    histoNamesSDMinus["logXiFromPFCandsVarBin"] = "logXiMinusFromPFCandsVarBin"
    histoNamesSDMinus["logXiGenVarBin"] = "logXiGenMinusVarBin"
    histoNamesSDMinus["logXiFromPFCandsANDXiGenVarBin"] = "logXiFromPFCandsANDXiGenMinusVarBin"

    ###############################
    file_SDPlus_ref = ROOT.TFile(fileNameMCSDPlusRef,'read')
    h_logXiGenVarBin_SDPlus_ref = file_SDPlus_ref.Get(histoNamesSDPlus["logXiGenVarBin"])
    h_EventSelection_SDPlus_ref = file_SDPlus_ref.Get("EventSelection")

    file_SDMinus_ref = ROOT.TFile(fileNameMCSDMinusRef,'read')
    h_logXiGenVarBin_SDMinus_ref = file_SDMinus_ref.Get(histoNamesSDMinus["logXiGenVarBin"])
    h_EventSelection_SDMinus_ref = file_SDMinus_ref.Get("EventSelection")

    file_SDPlus_eff = ROOT.TFile(fileNameMCSDPlusEff,'read')
    h_logXiFromPFCandsVarBin_SDPlus = file_SDPlus_eff.Get(histoNamesSDPlus["logXiFromPFCandsVarBin"])
    h_logXiGenVarBin_SDPlus = file_SDPlus_eff.Get(histoNamesSDPlus["logXiGenVarBin"])
    h_logXiFromPFCandsANDXiGenVarBin_SDPlus = file_SDPlus_eff.Get(histoNamesSDPlus["logXiFromPFCandsANDXiGenVarBin"])

    file_SDMinus_eff = ROOT.TFile(fileNameMCSDMinusEff,'read')
    h_logXiFromPFCandsVarBin_SDMinus = file_SDMinus_eff.Get(histoNamesSDMinus["logXiFromPFCandsVarBin"])
    h_logXiGenVarBin_SDMinus = file_SDMinus_eff.Get(histoNamesSDMinus["logXiGenVarBin"])
    h_logXiFromPFCandsANDXiGenVarBin_SDMinus = file_SDMinus_eff.Get(histoNamesSDMinus["logXiFromPFCandsANDXiGenVarBin"])

    ###############################
    # Purity & stability
    h_logXiPurityVarBin_SDPlus = h_logXiFromPFCandsANDXiGenVarBin_SDPlus.Clone('logXiPurityVarBin_SDPlus')
    h_logXiPurityVarBin_SDPlus.Divide(h_logXiFromPFCandsVarBin_SDPlus)
    h_logXiStabilityVarBin_SDPlus = h_logXiFromPFCandsANDXiGenVarBin_SDPlus.Clone('logXiStabilityVarBin_SDPlus')
    h_logXiStabilityVarBin_SDPlus.Divide(h_logXiGenVarBin_SDPlus)

    h_logXiPurityVarBin_SDMinus = h_logXiFromPFCandsANDXiGenVarBin_SDMinus.Clone('logXiPurityVarBin_SDMinus')
    h_logXiPurityVarBin_SDMinus.Divide(h_logXiFromPFCandsVarBin_SDMinus)
    h_logXiStabilityVarBin_SDMinus = h_logXiFromPFCandsANDXiGenVarBin_SDMinus.Clone('logXiStabilityVarBin_SDMinus')
    h_logXiStabilityVarBin_SDMinus.Divide(h_logXiGenVarBin_SDMinus)

    # Correction factor
    h_logXiCorrVarBin_SDPlus = h_logXiGenVarBin_SDPlus.Clone('logXiCorrVarBin_SDPlus')
    h_logXiCorrVarBin_SDPlus.Divide(h_logXiFromPFCandsVarBin_SDPlus)

    h_logXiCorrVarBin_SDMinus = h_logXiGenVarBin_SDMinus.Clone('logXiCorrVarBin_SDMinus')
    h_logXiCorrVarBin_SDMinus.Divide(h_logXiFromPFCandsVarBin_SDMinus)

    # Efficiency
    # FIXME: use graph and compute proper errors 
    h_effLogXiGenVarBin_SDPlus = h_logXiGenVarBin_SDPlus.Clone('effLogXiGenVarBin_SDPlus')
    h_effLogXiGenVarBin_SDPlus.Divide(h_logXiGenVarBin_SDPlus_ref)

    h_effLogXiGenVarBin_SDMinus = h_logXiGenVarBin_SDMinus.Clone('effLogXiGenVarBin_SDMinus')
    h_effLogXiGenVarBin_SDMinus.Divide(h_logXiGenVarBin_SDMinus_ref)

    # Full bin by bin correction
    h_logXiCorrFullVarBin_SDPlus = h_logXiCorrVarBin_SDPlus.Clone('logXiCorrFullVarBin_SDPlus')
    h_logXiCorrFullVarBin_SDPlus.Divide(h_effLogXiGenVarBin_SDPlus)

    h_logXiCorrFullVarBin_SDMinus = h_logXiCorrVarBin_SDMinus.Clone('logXiCorrFullVarBin_SDMinus')
    h_logXiCorrFullVarBin_SDMinus.Divide(h_effLogXiGenVarBin_SDMinus)

    canvases = []
    histos = []
    canvases.append(ROOT.TCanvas("c_logXiPurityVarBin","logXiPurityVarBin"))
    h_logXiPurityVarBin_SDPlus.GetXaxis().SetTitle("log(#xi)")
    h_logXiPurityVarBin_SDPlus.GetYaxis().SetTitle("p(#xi^{PF},#xi^{Gen} #epsilon bin i | #xi^{PF} #epsilon bin i)")
    h_logXiPurityVarBin_SDPlus.SetStats(0)
    h_logXiPurityVarBin_SDPlus.Draw()
    h_logXiPurityVarBin_SDMinus.SetLineStyle(2)
    h_logXiPurityVarBin_SDMinus.SetStats(0)
    h_logXiPurityVarBin_SDMinus.Draw("SAME")
    histos.append(h_logXiPurityVarBin_SDPlus)
    histos.append(h_logXiPurityVarBin_SDMinus)

    canvases.append(ROOT.TCanvas("c_logXiStabilityVarBin","logXiStabilityVarBin"))
    h_logXiStabilityVarBin_SDPlus.GetXaxis().SetTitle("log(#xi)")
    h_logXiStabilityVarBin_SDPlus.GetYaxis().SetTitle("p(#xi^{PF},#xi^{Gen} #epsilon bin i | #xi^{Gen} #epsilon bin i)")
    h_logXiStabilityVarBin_SDPlus.SetStats(0)
    h_logXiStabilityVarBin_SDPlus.Draw()
    h_logXiStabilityVarBin_SDMinus.SetLineStyle(2)
    h_logXiStabilityVarBin_SDMinus.SetStats(0)
    h_logXiStabilityVarBin_SDMinus.Draw("SAME")
    histos.append(h_logXiStabilityVarBin_SDPlus)
    histos.append(h_logXiStabilityVarBin_SDMinus)

    canvases.append(ROOT.TCanvas("c_effLogXiGenVarBin","effLogXiGenVarBin"))
    h_effLogXiGenVarBin_SDPlus.GetXaxis().SetTitle("log(#xi)")
    h_effLogXiGenVarBin_SDPlus.GetYaxis().SetTitle("#varepsilon^{MC}(#xi)")
    h_effLogXiGenVarBin_SDPlus.SetStats(0)
    h_effLogXiGenVarBin_SDPlus.Draw()
    h_effLogXiGenVarBin_SDMinus.SetLineStyle(2)
    h_effLogXiGenVarBin_SDMinus.SetStats(0)
    h_effLogXiGenVarBin_SDMinus.Draw("SAME")
    histos.append(h_effLogXiGenVarBin_SDPlus)
    histos.append(h_effLogXiGenVarBin_SDMinus)

    canvases.append(ROOT.TCanvas("c_logXiCorrVarBin","logXiCorrVarBin"))
    h_logXiCorrVarBin_SDPlus.GetXaxis().SetTitle("log(#xi)")
    h_logXiCorrVarBin_SDPlus.GetYaxis().SetTitle("N(#xi^{Gen} #epsilon bin i)/N(#xi^{PF} #epsilon bin i)")
    h_logXiCorrVarBin_SDPlus.SetStats(0)
    h_logXiCorrVarBin_SDPlus.Draw()
    h_logXiCorrVarBin_SDMinus.SetLineStyle(2)
    h_logXiCorrVarBin_SDMinus.SetStats(0)
    h_logXiCorrVarBin_SDMinus.Draw("SAME")
    histos.append(h_logXiCorrVarBin_SDPlus)
    histos.append(h_logXiCorrVarBin_SDMinus) 

    canvases.append(ROOT.TCanvas("c_logXiCorrFullVarBin","logXiCorrFullVarBin"))
    h_logXiCorrFullVarBin_SDPlus.GetXaxis().SetTitle("log(#xi)")
    h_logXiCorrFullVarBin_SDPlus.GetYaxis().SetTitle("#frac{1}{#varepsilon(#xi)}#frac{N(#xi^{Gen} #epsilon bin i)}{N(#xi^{PF} #epsilon bin i)}")
    h_logXiCorrFullVarBin_SDPlus.SetStats(0)
    h_logXiCorrFullVarBin_SDPlus.Draw()
    h_logXiCorrFullVarBin_SDMinus.SetLineStyle(2)
    h_logXiCorrFullVarBin_SDMinus.SetStats(0)
    h_logXiCorrFullVarBin_SDMinus.Draw("SAME")
    histos.append(h_logXiCorrFullVarBin_SDPlus)
    histos.append(h_logXiCorrFullVarBin_SDMinus)

    return (canvases,histos)
  
