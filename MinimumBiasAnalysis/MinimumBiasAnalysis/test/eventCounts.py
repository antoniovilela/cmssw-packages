import ROOT

def eventCounts(fileName):

    steps = ['countsAll',
             'countshltBscMinBiasORBptxPlusORMinus',
             'countsBPTX',
             'countsBscOr',
             'countsVertexFilter',
             'countsBeamHaloVeto',
             'countsFilterScraping',
             'countsHBHENoiseFilter']

    file = ROOT.TFile(fileName,'READ')

    nEvents = []
    for step in steps:
        h_counts = file.Get("%s/NCounts" % step)
        nEvents.append(h_counts.GetBinContent(1))

        nCounts = nEvents[-1] 
        effWrtPrevious = 100.
        if len(nEvents) > 1: effWrtPrevious = 100.*(nCounts/nEvents[-2]) 
        print "Step %s --> %d  %.2f%%" % (step,nCounts,effWrtPrevious)           

    return 0

def eventCountHisto(file,histoName="EventSelection",binNumber=-1):
    h_counts = file.Get(histoName)
    if binNumber == -1: binNumber = h_counts.GetNbinsX()
    nEvents = h_counts.GetBinContent(binNumber)

    return nEvents

def eventCountsFiles(fileNames,histoName="EventSelection",binNumber=-1):

   nEvents = []
   for fileName in fileNames:
       file = ROOT.TFile(fileName,'read')
       nEvents.append( eventCountHisto(file,histoName,binNumber) )
        
   return nEvents

def eventCountsProcessIds(processIds,dir,prefix):

   steps = [
        'minimumBiasTTreeAnalysishltBscMinBiasORBptxPlusORMinus',
        'minimumBiasTTreeAnalysisBscOr',
        'minimumBiasTTreeAnalysisVertexFilter',
        'minimumBiasTTreeAnalysisHBHENoiseFilterHcalNoiseSelection',
        'minimumBiasTTreeAnalysisHBHENoiseFilterHcalNoiseSelectionEtaMaxSelection'
   ]

   #processIds = ('All','SD','DD','Inelastic')
   #processIds = ('SDPlus','SDMinus')

   for type in processIds:
       fileNames = ["%s/%s_%s_histos_%s.root" % (dir,prefix,item,type) for item in steps]
       nEvents = eventCountsFiles(fileNames=fileNames,
                                  histoName="EventSelection",binNumber=-1)
       print fileNames
       print "Process",type
       for k in range(len(nEvents)):
           effWrtPrevious = 100.
           if k: effWrtPrevious = 100.*(nEvents[k]/nEvents[k-1])
           print "Step %s --> %d  %.2f%%" % (steps[k],nEvents[k],effWrtPrevious)
