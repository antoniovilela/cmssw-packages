import ROOT

def plotTTreeHistos():

    return -1

def plotEventSelection():

    variables = [
        'posRPrimVtx',
        'sumEnergyHFPlus',
        'sumEnergyHFMinus'
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

def plotEventSelectionMC():
    variables = [
        'multiplicityTracks',
        'sumEnergyHFPlus',
        'sumEnergyHFMinus'
    ]

    steps = [
        'minimumBiasTTreeAnalysisAll',
        'minimumBiasTTreeAnalysisL1CollBscOr',
        'minimumBiasTTreeAnalysishltBscMinBiasORBptxPlusORMinus',
        'minimumBiasTTreeAnalysisVertexFilter',
        'minimumBiasTTreeAnalysisFilterScraping',
        'minimumBiasTTreeAnalysisHBHENoiseFilter'
    ]
     
    prefix = 'analysisMinBiasTTree_PYTHIAD6T_MinBias_7TeV'
    dir = 'root/7TeV/EventSelection/PYTHIA6D6T'

    processIds = ['All','SD','DD','Inelastic'] 

    files = {}
    for selection in steps:
        for type in processIds:
            fileName = "%s/%s_%s_histos_%s.root" % (dir, prefix, selection, type)
            print "Accessing", fileName
            index = (selection,type)
            files[index] = ROOT.TFile(fileName) 
    
    # Loop over steps
    #for step in range(1,len(steps)):
   
    selectionRef = 'minimumBiasTTreeAnalysisAll'
    selectionEff = 'minimumBiasTTreeAnalysisL1CollBscOr'
   
    return plotEfficiencies(variables=variables,
                            files=files,
                            types=processIds,
                            selectionRef=selectionRef,
                            selectionEff=selectionEff)

def plotEfficiencies(variables, files, types, selectionRef, selectionEff):

    ROOT.TH1.AddDirectory(False)

    colors = (1,2,6,12,4,43,38,57)
    markers = (20,21,25,22,26,20,24,27)
    canvases = []
    graphsEff = {}
    for variable in variables:
        canvases.append(ROOT.TCanvas("c_" + variable))
        graphsEff[variable] = []
        idx_type = 0
        for type in types:
            indexRef = (selectionRef,type)
            h_Ref = files[indexRef].Get(variable)
            indexEff = (selectionEff,type)
            h_Eff = files[indexEff].Get(variable)

            graphsEff[variable].append(ROOT.TGraphAsymmErrors(h_Eff,h_Ref))
            graphsEff[variable][-1].SetName( h_Eff.GetName() + type + "_graph" )
            graphsEff[variable][-1].SetLineColor(colors[idx_type])
            graphsEff[variable][-1].SetMarkerStyle(markers[idx_type])
            print "  Created", graphsEff[variable][-1].GetName() 

            canvases[-1].cd()
            if idx_type == 0: graphsEff[variable][-1].Draw("AP")
            else: graphsEff[variable][-1].Draw("P")
            idx_type += 1

    return (canvases,graphsEff)

if __name__ == '__main__':
    
    sys.exit(0)
