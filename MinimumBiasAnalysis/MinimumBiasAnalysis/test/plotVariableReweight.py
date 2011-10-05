import ROOT

def plotReweight(variables, fileNameData, fileNameMC, rebin=1, outputFileName=""):

    AddDirectoryStatus_ = ROOT.TH1.AddDirectoryStatus()
    ROOT.TH1.AddDirectory(False)

    # Ref --> Reweight
    fileReweight = ROOT.TFile(fileNameData,'read')
    fileRef = ROOT.TFile(fileNameMC,'read')
    
    colors = (1,2)
    markers = (20,24) 
    canvases = {}
    histosRef = {}
    histosReweight = {}
    histosReweightFraction = {}
    for variable in variables:
        canvases[variable] = []
        histosRef[variable] = []
        histosReweight[variable] = []
        histosReweightFraction[variable] = []

	histo_Ref_tmp = fileRef.Get( variable )
	print "  Accessing", histo_Ref_tmp.GetName()
        histosRef[variable].append( histo_Ref_tmp.Clone( histo_Ref_tmp.GetName() + "_ref" ) ) 

	histo_Reweight_tmp = fileReweight.Get( variable )
	print "  Accessing", histo_Reweight_tmp.GetName()
        histosReweight[variable].append( histo_Reweight_tmp.Clone( histo_Reweight_tmp.GetName() + "_refnew" ) ) 

	if rebin != 1:  histosRef[variable][-1].Rebin(rebin)
	if rebin != 1:  histosReweight[variable][-1].Rebin(rebin)
        histosRef[variable][-1].Scale( 1./histosRef[variable][-1].GetSumOfWeights() )
        histosReweight[variable][-1].Scale( 1./histosReweight[variable][-1].GetSumOfWeights() )

	histosReweightFraction[variable].append( histosReweight[variable][-1].Clone(variable + "_reweight") ) 
	histosReweightFraction[variable][-1].Divide( histosRef[variable][-1] )
	print "  Created", histosReweightFraction[variable][-1].GetName() 

	canvases[variable].append( ROOT.TCanvas("c_" + variable,variable) ) 
	canvases[variable][-1].cd()
	histosReweight[variable][-1].SetMarkerStyle(20)
	histosReweight[variable][-1].SetMarkerColor(1)
	histosReweight[variable][-1].SetMarkerSize(1.4)
	histosReweight[variable][-1].SetLineColor(1) 
	histosRef[variable][-1].SetMarkerStyle(24)
	histosRef[variable][-1].SetMarkerColor(2)
	histosRef[variable][-1].SetMarkerSize(1.4) 
	histosRef[variable][-1].SetLineColor(2)
        histosRef[variable][-1].GetYaxis().SetRangeUser(0.,histosRef[variable][-1].GetMaximum()*1.2) 
	histosRef[variable][-1].Draw("HISTOE2")
	histosReweight[variable][-1].Draw("HISTOE2SAME")

	canvases[variable].append( ROOT.TCanvas("c_" + variable + "_reweight",variable) )
	canvases[variable][-1].cd()
	histosReweightFraction[variable][-1].GetYaxis().SetRangeUser(0.,1.5)
	histosReweightFraction[variable][-1].GetYaxis().SetTitle("Reweight")
	histosReweightFraction[variable][-1].Draw()

    ROOT.TH1.AddDirectory(AddDirectoryStatus_)

    if outputFileName:
        outputFile = ROOT.TFile(outputFileName,'recreate')
        outputFile.cd()
        for key in histosReweight:
            for histo in histosReweightFraction[key]: histo.Write()
            for histo in histosRef[key]: histo.Write()
            for histo in histosReweight[key]: histo.Write()
            
        return 0
    else:       
        return (canvases,histosReweightFraction,histosRef,histosReweight)

if __name__ == '__main__':
    
    sys.exit(0)
