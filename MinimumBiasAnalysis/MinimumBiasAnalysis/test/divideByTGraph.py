import ROOT
import math

def divide(histo,graph):
    
    nBins = histo.GetNbinsX()
    for ix in range(nBins):
        print ix,graph.GetX()[ix],graph.GetY()[ix]
        valX = histo.GetBinContent(ix+1)
        errX = histo.GetBinError(ix+1)
        valY = graph.GetY()[ix]
        errY = graph.GetErrorY(ix) 
        val = valX/valY
        err = ( (errX*errX) + (valX*valX*errY*errY)/(valY*valY) )/(valY*valY)
        err = math.sqrt(err)
        histo.SetBinContent( (ix+1), val )
        histo.SetBinError( (ix+1), err )
