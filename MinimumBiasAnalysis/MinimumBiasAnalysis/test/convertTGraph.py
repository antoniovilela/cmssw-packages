import ROOT

def convert(graph):
    AddDirectoryStatus_ = ROOT.TH1.AddDirectoryStatus()
    ROOT.TH1.AddDirectory(False)

    #histo = graph.GetHistogram().Clone( graph.GetName() + "_histo" )
    nBins = graph.GetN()
    histo = ROOT.TH1F( graph.GetName() + "_histo",graph.GetTitle(),
                                                  nBins,
                                                  graph.GetX()[0] - graph.GetErrorXlow(0),
                                                  graph.GetX()[nBins-1] + graph.GetErrorXhigh(nBins-1) )

    for ix in range(graph.GetN()):
        print ix,graph.GetX()[ix],graph.GetY()[ix]
        histo.SetBinContent(ix+1,graph.GetY()[ix])
        histo.SetBinError(ix+1,graph.GetErrorY(ix))

    histo.SetLineColor( graph.GetLineColor() )
    histo.SetMarkerStyle( graph.GetMarkerStyle() )
    histo.SetMarkerColor( graph.GetMarkerColor() )
    histo.SetMarkerSize( graph.GetMarkerSize() )

    ROOT.TH1.AddDirectory(AddDirectoryStatus_)
    return histo
