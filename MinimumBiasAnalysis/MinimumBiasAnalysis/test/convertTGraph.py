import ROOT
from array import array

def convert(graph,hname=''):
    AddDirectoryStatus_ = ROOT.TH1.AddDirectoryStatus()
    ROOT.TH1.AddDirectory(False)

    #histo = graph.GetHistogram().Clone( graph.GetName() + "_histo" )
    
    if not hname: hname = graph.GetName() + "_histo"
    nBins = graph.GetN()
    x_bins = []
    for ix in range(nBins): x_bins.append( graph.GetX()[ix] - graph.GetErrorXlow(ix) )
    x_bins.append( graph.GetX()[nBins-1] + graph.GetErrorXhigh(nBins-1) )
    histo = ROOT.TH1F( hname,graph.GetTitle(),nBins,array('f',x_bins) )
    #histo = ROOT.TH1F(hname,graph.GetTitle(),nBins,
    #                                         graph.GetX()[0] - graph.GetErrorXlow(0),
    #                                         graph.GetX()[nBins-1] + graph.GetErrorXhigh(nBins-1) )
    print "  Created", histo.GetName()

    for ix in range(graph.GetN()):
        print ix,'(%f,%f)' % (graph.GetX()[ix] - graph.GetErrorXlow(ix),graph.GetX()[ix] + graph.GetErrorXhigh(ix)),graph.GetX()[ix],graph.GetY()[ix]
        histo.SetBinContent(ix+1,graph.GetY()[ix])
        histo.SetBinError(ix+1,graph.GetErrorY(ix))

    for ibin in range(1,histo.GetNbinsX()+1):
        print ibin,'(%f,%f)' % (histo.GetBinLowEdge(ibin),histo.GetBinLowEdge(ibin)+histo.GetBinWidth(ibin)),histo.GetBinCenter(ibin),histo.GetBinContent(ibin),histo.GetBinError(ibin)

    histo.SetLineColor( graph.GetLineColor() )
    histo.SetMarkerStyle( graph.GetMarkerStyle() )
    histo.SetMarkerColor( graph.GetMarkerColor() )
    histo.SetMarkerSize( graph.GetMarkerSize() )

    ROOT.TH1.AddDirectory(AddDirectoryStatus_)
    return histo
