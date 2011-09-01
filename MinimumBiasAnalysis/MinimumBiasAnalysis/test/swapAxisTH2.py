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

    for ibinX in range(nBinsX+2):
        for ibinY in range(nBinsY+2):
            histo_new.SetBinContent(ibinX, ibinY, histo.GetBinContent(ibinY,ibinX))  


    ROOT.TH1.AddDirectory(AddDirectoryStatus_)
    return histo_new
