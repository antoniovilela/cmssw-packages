import ROOT
from convertTGraph import convert

def graphName(variable,type):
    return variable + "_clone_" + type + "_graph"

def plotReweight(variables, fileNameData, fileNameMC, rebin=1, scale=1., outputFileName=""):

    AddDirectoryStatus_ = ROOT.TH1.AddDirectoryStatus()
    ROOT.TH1.AddDirectory(False)

    fileData = ROOT.TFile(fileNameData,'read')
    fileMC = ROOT.TFile(fileNameMC,'read')
    
    colors = (1,2)
    markers = (20,24) 
    canvases = {}
    histosRef = {}
    histosEff = {}
    histosReweight = {}
    types = ('All',)
    for variable in variables:
        canvases[variable] = []
        histosRef[variable] = []
        histosEff[variable] = [] 
        histosReweight[variable] = []
        idx_type = 0
        for type in types:
            graph_Ref_tmp = fileMC.Get( graphName(variable,type) )
            print "  Accessing", graph_Ref_tmp.GetName() 
            histosRef[variable].append( convert(graph_Ref_tmp,variable + "_mc_histo") )

            graph_Eff_tmp = fileData.Get( graphName(variable,'') )
            print "  Accessing", graph_Eff_tmp.GetName()
            histosEff[variable].append( convert(graph_Eff_tmp,variable + "_data_histo") )

            if rebin != 1:  histosRef[variable][-1].Rebin(rebin)
            if rebin != 1:  histosEff[variable][-1].Rebin(rebin)
            if scale != 1.: histosEff[variable][-1].Scale(scale)

            histosReweight[variable].append( histosEff[variable][-1].Clone(variable + "_reweight") ) 
            histosReweight[variable][-1].Divide( histosRef[variable][-1] )
            histosReweight[variable][-1].GetYaxis().SetRangeUser(0.,1.05)
            histosReweight[variable][-1].GetYaxis().SetTitle("Reweight")
            print "  Created", histosReweight[variable][-1].GetName() 

            canvases[variable].append( ROOT.TCanvas("c_" + variable,variable) ) 
            canvases[variable][-1].cd()
            histosEff[variable][-1].SetMarkerStyle(20)
            histosEff[variable][-1].SetMarkerColor(1)
            histosEff[variable][-1].SetMarkerSize(1.4)
            histosEff[variable][-1].SetLineColor(1) 
            histosRef[variable][-1].SetMarkerStyle(24)
            histosRef[variable][-1].SetMarkerColor(2)
            histosRef[variable][-1].SetMarkerSize(1.4) 
            histosRef[variable][-1].SetLineColor(2) 
            histosEff[variable][-1].Draw()
            histosRef[variable][-1].Draw("SAME")

            canvases[variable].append( ROOT.TCanvas("c_" + variable + "_reweight",variable) )
            canvases[variable][-1].cd()
            histosReweight[variable][-1].Draw()
 
            idx_type += 1

    ROOT.TH1.AddDirectory(AddDirectoryStatus_)

    if outputFileName:
        outputFile = ROOT.TFile(outputFileName,'recreate')
        outputFile.cd()
        for key in histosReweight:
            for histo in histosReweight[key]: histo.Write()
            for histo in histosRef[key]: histo.Write()
            for histo in histosEff[key]: histo.Write()
            
        return 0
    else:       
        return (canvases,histosReweight,histosRef,histosEff)

if __name__ == '__main__':
    
    sys.exit(0)
