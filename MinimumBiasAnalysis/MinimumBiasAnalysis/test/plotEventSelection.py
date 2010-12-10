import ROOT

def plotTTreeHistos():

    return -1

def plotEventSelection():

    variables = [
        'posRPrimVtx',
        'sumEnergyHFPlus',
        'sumEnergyHFMinus',
        'etaMaxFromPFCands',
        'etaMinFromPFCands'
    ]

    selectionRef = 'BscOr'
    selectionEff = 'VertexFilter'
    dir = 'root/7TeV/Data/Run132605/eventSelection/test_031210'
    fileNameRef = dir + "/" + "analysisMinBiasTTree_MinimumBias_7TeV_minimumBiasTTreeAnalysisAll_histos.root"
    fileNameEff = dir + "/" + "analysisMinBiasTTree_MinimumBias_7TeV_minimumBiasTTreeAnalysisBscOr_histos.root"

    files = {}
    indexRef = (selectionRef,"")
    files[indexRef] = ROOT.TFile(fileNameRef)
    indexEff = (selectionEff,"")
    files[indexEff] = ROOT.TFile(fileNameEff)

    return plotEfficiencies(variables=variables,
                            files=files,
                            types=("",),
                            selectionRef=selectionRef,
                            selectionEff=selectionEff)

def plotEventSelectionMC(dir,prefix,rebin=1):

    #dir = 'root/7TeV/Pythia8/eventSelection'
    #prefix = 'analysisMinBiasTTree_PYTHIA8_MinBias_7TeV'

    #selectionRef = 'minimumBiasTTreeAnalysishltBscMinBiasORBptxPlusORMinus'
    #selectionEff = 'minimumBiasTTreeAnalysisBscOr'

    selectionRef = 'minimumBiasTTreeAnalysisBscOr'
    selectionEff = 'minimumBiasTTreeAnalysisVertexFilter'

    #selectionRef = 'minimumBiasTTreeAnalysisVertexFilter'
    #selectionEff = 'minimumBiasTTreeAnalysisHBHENoiseFilterHcalNoiseSelection'

    variables = [
        'multiplicityTracks',
        'sumEnergyHFPlus',
        'sumEnergyHFMinus',
        'etaMaxFromPFCands',
        'etaMinFromPFCands'
    ]

    """
    steps = [
        'minimumBiasTTreeAnalysisAll',
        'minimumBiasTTreeAnalysisBscOr',
        'minimumBiasTTreeAnalysisVertexFilter',
        'minimumBiasTTreeAnalysisHBHENoiseFilterHcalNoiseSelection'
    ]
    """
    steps = [selectionRef,selectionEff]

    files = {}
    processIds = ['All','SD','DD','Inelastic'] 
    for selection in steps:
        for type in processIds:
            fileName = "%s/%s_%s_histos_%s.root" % (dir, prefix, selection, type)
            print "Accessing", fileName
            index = (selection,type)
            files[index] = ROOT.TFile(fileName) 
    
    # Loop over steps
    #for step in range(1,len(steps)):
   
    return plotEfficiencies(variables=variables,
                            files=files,
                            types=processIds,
                            selectionRef=selectionRef,
                            selectionEff=selectionEff,
                            rebin=rebin)

def plotEfficiencies(variables, files, types, selectionRef, selectionEff, rebin=1):

    ROOT.TH1.AddDirectory(False)

    colors = (1,2,6,12,4,43,38,57)
    markers = (20,21,25,22,26,20,24,27)
    canvases = []
    legends = []
    graphsEff = {}
    for variable in variables:
        canvases.append(ROOT.TCanvas("c_" + variable))
        legends.append(ROOT.TLegend(0.4,0.7,0.95,0.8))
        graphsEff[variable] = []
        idx_type = 0
        for type in types:
            indexRef = (selectionRef,type)
            h_Ref_tmp = files[indexRef].Get(variable)
            h_Ref = h_Ref_tmp.Clone( h_Ref_tmp.GetName() + "_clone" )
            indexEff = (selectionEff,type)
            h_Eff_tmp = files[indexEff].Get(variable)
            h_Eff = h_Eff_tmp.Clone( h_Eff_tmp.GetName() + "_clone" )

            h_Ref.Rebin(rebin)
            h_Eff.Rebin(rebin)

            graphsEff[variable].append(ROOT.TGraphAsymmErrors(h_Eff,h_Ref))
            graphsEff[variable][-1].SetName( h_Eff.GetName() + "_" + type + "_graph" )
            graphsEff[variable][-1].SetLineColor(colors[idx_type])
            graphsEff[variable][-1].SetMarkerStyle(markers[idx_type])
            print "  Created", graphsEff[variable][-1].GetName() 

            legends[-1].AddEntry(graphsEff[variable][-1],
                                 graphsEff[variable][-1].GetName(),"LP");

            canvases[-1].cd()
            if idx_type == 0: graphsEff[variable][-1].Draw("AP")
            else: graphsEff[variable][-1].Draw("P")
            idx_type += 1
        canvases[-1].cd()
        legends[-1].SetFillColor( canvases[-1].GetFillColor() ) 
        legends[-1].SetHeader( variable ) 
        legends[-1].Draw("SAME")

    return (canvases,legends,graphsEff)

if __name__ == '__main__':
    
    sys.exit(0)
