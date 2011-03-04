import ROOT
from scaleByWidth import scaleByWidth

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
    h_logXiPurityVarBin.SetStats(0)
    h_logXiPurityVarBin.Draw()
    histos.append(h_logXiPurityVarBin)

    canvases.append(ROOT.TCanvas("c_logXiStabilityVarBin","logXiStabilityVarBin"))
    h_logXiStabilityVarBin.GetXaxis().SetTitle("log(#xi)")
    h_logXiStabilityVarBin.GetYaxis().SetTitle("p(#xi^{PF},#xi^{Gen} #epsilon bin i | #xi^{Gen} #epsilon bin i)")
    h_logXiStabilityVarBin.SetStats(0)
    h_logXiStabilityVarBin.Draw()
    histos.append(h_logXiStabilityVarBin)

    canvases.append(ROOT.TCanvas("c_effLogXiGenVarBin","effLogXiGenVarBin"))
    h_effLogXiGenVarBin.GetXaxis().SetTitle("log(#xi)")
    h_effLogXiGenVarBin.GetYaxis().SetTitle("#varepsilon^{MC}(#xi)")
    h_effLogXiGenVarBin.SetStats(0)
    h_effLogXiGenVarBin.Draw()
    histos.append(h_effLogXiGenVarBin) 

    canvases.append(ROOT.TCanvas("c_logXiCorrVarBin","logXiCorrVarBin"))
    h_logXiCorrVarBin.GetXaxis().SetTitle("log(#xi)")
    h_logXiCorrVarBin.GetYaxis().SetTitle("N(#xi^{Gen} #epsilon bin i)/N(#xi^{PF} #epsilon bin i)")
    h_logXiCorrVarBin.SetStats(0)
    h_logXiCorrVarBin.Draw()
    histos.append(h_logXiCorrVarBin)
  
    canvases.append(ROOT.TCanvas("c_logXiCorrFullVarBin","logXiCorrFullVarBin"))
    h_logXiCorrFullVarBin.GetXaxis().SetTitle("log(#xi)")
    h_logXiCorrFullVarBin.GetYaxis().SetTitle("#frac{1}{#varepsilon(#xi)}#frac{N(#xi^{Gen} #epsilon bin i)}{N(#xi^{PF} #epsilon bin i)}")
    h_logXiCorrFullVarBin.SetStats(0)
    h_logXiCorrFullVarBin.Draw()
    histos.append(h_logXiCorrFullVarBin)

    canvases.append(ROOT.TCanvas("c_logXiFromPFCandsVarBin_data_corr","logXiFromPFCandsVarBin_data_corr"))
    h_logXiFromPFCandsVarBin_data_corr.GetXaxis().SetTitle("log(#xi)")
    h_logXiFromPFCandsVarBin_data_corr.GetYaxis().SetTitle("d#sigma/dlog(#xi) (mb)")
    scaleByWidth(h_logXiFromPFCandsVarBin_data_corr)
    h_logXiFromPFCandsVarBin_data_corr.SetStats(0)
    h_logXiFromPFCandsVarBin_data_corr.Draw()
    scaleByWidth(h_logXiGenVarBin_ref_scaled)
    h_logXiGenVarBin_ref_scaled.SetLineColor(2)
    h_logXiGenVarBin_ref_scaled.SetStats(0)
    h_logXiGenVarBin_ref_scaled.Draw("HISTOSAME")
    histos.append(h_logXiFromPFCandsVarBin_data_corr)
    histos.append(h_logXiGenVarBin_ref_scaled)

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
  
