import ROOT

def scaleByWidth(histo):
    nBins = histo.GetNbinsX()
    for ibin in range(1,nBins+1):
        binContent = histo.GetBinContent(ibin);
        binError = histo.GetBinError(ibin);
        binWidth = histo.GetBinWidth(ibin);
        histo.SetBinContent(ibin,binContent/binWidth);
        histo.SetBinError(ibin,binError/binWidth)
