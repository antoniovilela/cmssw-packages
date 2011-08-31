import ROOT

def scale(graph,c):
    AddDirectoryStatus_ = ROOT.TH1.AddDirectoryStatus()
    ROOT.TH1.AddDirectory(False)

    graph_new = ROOT.TGraphAsymmErrors()
    graph_new.SetName(graph.GetName() + "_scaled")
    for ix in range(graph.GetN()):
        print ix,graph.GetX()[ix],graph.GetY()[ix]
        graph_new.SetPoint(ix,graph.GetX()[ix],c*graph.GetY()[ix])
        graph_new.SetPointError(ix,graph.GetErrorXlow(ix),graph.GetErrorXhigh(ix),
                                   c*graph.GetErrorYlow(ix),c*graph.GetErrorYhigh(ix))

    graph_new.SetLineColor( graph.GetLineColor() )
    graph_new.SetMarkerStyle( graph.GetMarkerStyle() )
    graph_new.SetMarkerColor( graph.GetMarkerColor() )
    graph_new.SetMarkerSize( graph.GetMarkerSize() )

    ROOT.TH1.AddDirectory(AddDirectoryStatus_)
    return graph_new
