import ROOT

def truncateTH1F(histo,binXMax,hname=''):
    AddDirectoryStatus_ = ROOT.TH1.AddDirectoryStatus()
    ROOT.TH1.AddDirectory(False)

    if not hname: hname = histo.GetName() + "_range"
    nBinsX = histo.GetNbinsX()

    nBinsX_new = nBinsX
    if binXMax < nBinsX: nBinsX_new = binXMax
    
    #histo_new = histo.Clone(hname)
    #histo_new.Reset()
    lowEdgeX = histo.GetXaxis().GetBinLowEdge(1)
    highEdgeX = histo.GetXaxis().GetBinLowEdge(nBinsX_new) + histo.GetXaxis().GetBinWidth(nBinsX_new)
    histo_new = ROOT.TH1F(hname,hname,nBinsX_new,lowEdgeX,highEdgeX)

    for ibinX in range(1,nBinsX_new + 1):
        histo_new.SetBinContent(ibinX, histo.GetBinContent(ibinX))
        histo_new.SetBinError(ibinX, histo.GetBinError(ibinX))
          
    ROOT.TH1.AddDirectory(AddDirectoryStatus_)
    return histo_new

def truncateTH2F(histo,binXMax,binYMax,hname=''):
    AddDirectoryStatus_ = ROOT.TH1.AddDirectoryStatus()
    ROOT.TH1.AddDirectory(False)

    if not hname: hname = histo.GetName() + "_range"
    nBinsX = histo.GetNbinsX()
    nBinsY = histo.GetNbinsY()

    nBinsX_new = nBinsX
    if binXMax < nBinsX: nBinsX_new = binXMax
    nBinsY_new = nBinsY
    if binYMax < nBinsY: nBinsY_new = binYMax
    
    #histo_new = histo.Clone(hname)
    #histo_new.Reset()
    lowEdgeX = histo.GetXaxis().GetBinLowEdge(1)
    highEdgeX = histo.GetXaxis().GetBinLowEdge(nBinsX_new) + histo.GetXaxis().GetBinWidth(nBinsX_new)
    lowEdgeY = histo.GetYaxis().GetBinLowEdge(1)
    highEdgeY = histo.GetYaxis().GetBinLowEdge(nBinsY_new) + histo.GetYaxis().GetBinWidth(nBinsY_new)
    histo_new = ROOT.TH2F(hname,hname,nBinsX_new,lowEdgeX,highEdgeX,nBinsY_new,lowEdgeY,highEdgeY)

    for ibinX in range(1,nBinsX_new + 1):
        for ibinY in range(1,nBinsY_new + 1):
            histo_new.SetBinContent(ibinX, ibinY, histo.GetBinContent(ibinX,ibinY))  
            histo_new.SetBinError(ibinX, ibinY, histo.GetBinError(ibinX,ibinY))  

    ROOT.TH1.AddDirectory(AddDirectoryStatus_)
    return histo_new
