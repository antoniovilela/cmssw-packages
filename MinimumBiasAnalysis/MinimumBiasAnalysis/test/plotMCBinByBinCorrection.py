import ROOT

def plotMCBinByBinCorrection(fileNameData, fileNameMCRef, fileNameMCEff, side = "plus"):
    ROOT.TH1.AddDirectory(False)

    intLumi = 20.322 # /mub
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

    file_mc_ref = ROOT.TFile(fileNameMCRef,'read')
    h_logXiGenVarBin_ref = file_mc_ref.Get(histoNames["logXiGenVarBin"])
    h_EventSelection_ref = file_mc_ref.Get("EventSelection")

    file_mc_eff = ROOT.TFile(fileNameMCEff,'read')
    h_logXiFromPFCandsVarBin = file_mc_eff.Get(histoNames["logXiFromPFCandsVarBin"])
    h_logXiGenVarBin = file_mc_eff.Get(histoNames["logXiGenVarBin"])
    h_logXiFromPFCandsANDXiGenVarBin = file_mc_eff.Get(histoNames["logXiFromPFCandsANDXiGenVarBin"])

    ###############################
    # Purity & stability
    h_logXiPurityVarBin = h_logXiFromPFCandsANDXiGenVarBin.Clone('logXiPurityVarBin')
    h_logXiPurityVarBin.Divide(h_logXiFromPFCandsVarBin)
    h_logXiStabilityVarBin = h_logXiFromPFCandsANDXiGenVarBin.Clone('logXiStabilityVarBin')
    h_logXiStabilityVarBin.Divide(h_logXiGenVarBin)
 
    # Correction factor
    h_logXiCorrVarBin = h_logXiGenVarBin.Clone('logXiCorrVarBin')
    h_logXiCorrVarBin.Divide(h_logXiFromPFCandsVarBin)

    # Efficiency
    # FIXME: use graph and compute proper errors 
    h_effLogXiGenVarBin = h_logXiGenVarBin.Clone('effLogXiGenVarBin')
    h_effLogXiGenVarBin.Divide(h_logXiGenVarBin_ref)

    # Full bin by bin correction
    h_logXiCorrFullVarBin = h_logXiCorrVarBin.Clone('logXiCorrFullVarBin')
    h_logXiCorrFullVarBin.Divide(h_effLogXiGenVarBin)

    # Correct data
    h_logXiFromPFCandsVarBin_data_corr = h_logXiFromPFCandsVarBin_data.Clone('logXiFromPFCandsVarBin_data_corr')
    h_logXiFromPFCandsVarBin_data_corr.Multiply(h_logXiCorrFullVarBin)
    h_logXiFromPFCandsVarBin_data_corr.Scale(0.001/intLumi) # mb

    # MC before selection
    h_logXiGenVarBin_ref_scaled = h_logXiGenVarBin_ref.Clone('logXiGenVarBin_ref_scaled')
    nEventsAll_ref = h_EventSelection_ref.GetBinContent(1)
    h_logXiGenVarBin_ref_scaled.Scale(sigmaMC/nEventsAll_ref)    

    canvases = []
    histos = []
    canvases.append(ROOT.TCanvas("c_logXiPurityVarBin","logXiPurityVarBin"))
    h_logXiPurityVarBin.GetXaxis().SetTitle("log(#xi)") 
    h_logXiPurityVarBin.GetYaxis().SetTitle("p(#xi^{PF},#xi^{Gen} #epsilon bin i | #xi^{PF} #epsilon bin i)")
    h_logXiPurityVarBin.Draw()
    histos.append(h_logXiPurityVarBin)

    canvases.append(ROOT.TCanvas("c_logXiStabilityVarBin","logXiStabilityVarBin"))
    h_logXiStabilityVarBin.GetXaxis().SetTitle("log(#xi)")
    h_logXiStabilityVarBin.GetYaxis().SetTitle("p(#xi^{PF},#xi^{Gen} #epsilon bin i | #xi^{Gen} #epsilon bin i)")
    h_logXiStabilityVarBin.Draw()
    histos.append(h_logXiStabilityVarBin)

    canvases.append(ROOT.TCanvas("c_effLogXiGenVarBin","effLogXiGenVarBin"))
    h_effLogXiGenVarBin.GetXaxis().SetTitle("log(#xi)")
    h_effLogXiGenVarBin.GetYaxis().SetTitle("#varepsilon^{MC}(#xi)")
    h_effLogXiGenVarBin.Draw()
    histos.append(h_effLogXiGenVarBin) 

    canvases.append(ROOT.TCanvas("c_logXiCorrVarBin","logXiCorrVarBin"))
    h_logXiCorrVarBin.GetXaxis().SetTitle("log(#xi)")
    h_logXiCorrVarBin.GetYaxis().SetTitle("N(#xi^{Gen} #epsilon bin i)/N(#xi^{PF} #epsilon bin i)")
    h_logXiCorrVarBin.Draw()
    histos.append(h_logXiCorrVarBin)
  
    canvases.append(ROOT.TCanvas("c_logXiCorrFullVarBin","logXiCorrFullVarBin"))
    h_logXiCorrFullVarBin.GetXaxis().SetTitle("log(#xi)")
    h_logXiCorrFullVarBin.GetYaxis().SetTitle("#frac{1}{#varepsilon(#xi)}#frac{N(#xi^{Gen} #epsilon bin i)}{N(#xi^{PF} #epsilon bin i)}")
    h_logXiCorrFullVarBin.Draw()
    histos.append(h_logXiCorrFullVarBin)

    canvases.append(ROOT.TCanvas("c_logXiFromPFCandsVarBin_data_corr","logXiFromPFCandsVarBin_data_corr"))
    h_logXiFromPFCandsVarBin_data_corr.GetXaxis().SetTitle("log(#xi)")
    h_logXiFromPFCandsVarBin_data_corr.GetYaxis().SetTitle("#sigma^{D}(mb)")
    h_logXiFromPFCandsVarBin_data_corr.Draw()
    h_logXiGenVarBin_ref_scaled.SetLineColor(2)
    h_logXiGenVarBin_ref_scaled.Draw("HISTOSAME")
    histos.append(h_logXiFromPFCandsVarBin_data_corr)
    histos.append(h_logXiGenVarBin_ref_scaled)

    return (canvases,histos)
