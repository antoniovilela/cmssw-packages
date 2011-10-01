import ROOT

def swapAxisTH2(histo,hname=''):
    AddDirectoryStatus_ = ROOT.TH1.AddDirectoryStatus()
    ROOT.TH1.AddDirectory(False)

    if not hname: hname = histo.GetName() + "_swapAxis"
    histo_new = histo.Clone(hname)
    histo_new.Reset() 

    nBinsX = histo_new.GetNbinsX()
    nBinsY = histo_new.GetNbinsY()
    if nBinsX != nBinsY: raise RuntimeError,'Only valid for same number of bins in X,Y'

    for ibinX in range(1,nBinsX+1):
        for ibinY in range(1,nBinsY+1):
            histo_new.SetBinContent(ibinX, ibinY, histo.GetBinContent(ibinY,ibinX))  
            histo_new.SetBinError(ibinX, ibinY, histo.GetBinError(ibinY,ibinX))  

    ROOT.TH1.AddDirectory(AddDirectoryStatus_)
    return histo_new
