import ROOT
import os,sys

def fitLinear(file,stationId,plaqId,chipId,row,col,option=""):
    minFracHits = 0.90
    minFracADC = 0.85

    h_hits = file.Get( "hits_%d_%d" % (row,col) )
    h_adc = file.Get( "adc_%d_%d" % (row,col) )

    def defaultWord():
        word = "%d %d %d %d %d %.3f %.3f %.3f\n" % (stationId,plaqId,chipId,row,col,0.0,0.0,0.0)
        return word

    maxHits = h_hits.GetMaximum()
    if maxHits == 0: return defaultWord()

    # Find linear fit range 
    iBinMin = -1
    for ibin in range( 1, (h_hits.GetNbinsX()+1) ):
        if (h_hits.GetBinContent(ibin)/maxHits) >= minFracHits:
            iBinMin = ibin
            break

    minADC = h_adc.GetBinContent(iBinMin)
    maxADC = h_adc.GetMaximum()
    if maxADC == 0 or (maxADC - minADC) == 0: return defaultWord()

    iBinMax = -1
    for ibin in range( 1, (h_adc.GetNbinsX()+1) ):
        if ( (h_adc.GetBinContent(ibin) - minADC)/(maxADC - minADC) ) >= minFracADC:
            iBinMax = ibin
            break

    minFit = h_adc.GetBinLowEdge(iBinMin)
    maxFit = h_adc.GetBinLowEdge(iBinMax)
    fitFunc = ROOT.TF1("fit_adc_%d_%d" % (row,col),"pol1",minFit,maxFit)
    h_adc.Fit(fitFunc,"R" + option)
    fit_slope = fitFunc.GetParameter(1)
    fit_ped = fitFunc.GetParameter(0)
    if fit_slope <= 0: return defaultWord()

    # Format output
    word = "%d %d %d %d %d %.3f %.3f %.3f\n" % (stationId,plaqId,chipId,row,col,fit_slope,fit_ped,maxADC)
    return word

def fitTgh(file,stationId,plaqId,chipId,row,col,option=""):
    minFracHits = 0.10

    h_hits = file.Get( "hits_%d_%d" % (row,col) )
    h_adc = file.Get( "adc_%d_%d" % (row,col) )
     
    def defaultWord():
        word = "%d %d %d %d %d %.3f %.3f %.3f %.3f\n" % (stationId,plaqId,chipId,row,col,-1.,-1.,-1.,-1.)
        return word

    maxHits = h_hits.GetMaximum()
    if maxHits == 0:
        #print "Row ",row,"Col ",col,"has zero hits"
        return defaultWord()

    iBinMin = -1
    for ibin in range( 1, (h_hits.GetNbinsX()+1) ):
        if (h_hits.GetBinContent(ibin)/maxHits) >= minFracHits:
            iBinMin = ibin
            break

    iBinMax = h_adc.GetMaximumBin()
    #print "Fit from bin ",iBinMin,"to bin  ",iBinMax
    minFit = h_adc.GetBinLowEdge(iBinMin)
    maxFit = h_adc.GetBinLowEdge(iBinMax)
    # Define fit function
    fitFunc = ROOT.TF1("fit_adc_%d_%d" % (row,col),"[0]+[1]*TMath::TanH([2]*x+[3])",minFit,maxFit)
    fitFunc.SetParameter(2,0.00382)
    fitFunc.SetParameter(3,-0.886)
    fitFunc.SetParameter(1,112.7)
    fitFunc.SetParameter(0,113.0)
    h_adc.Fit(fitFunc,"R" + option)
    if fitFunc.GetParameter(3) == -0.886:
         fit_p0 = -1.0
         fit_p1 = -1.0
         fit_p2 = -1.0
         fit_p3 = -1.0
    else:
         fit_p0 = fitFunc.GetParameter(0)
         fit_p1 = fitFunc.GetParameter(1)
         fit_p2 = fitFunc.GetParameter(2)
         fit_p3 = fitFunc.GetParameter(3)

    word = "%d %d %d %d %d %.3f %.3f %.3f %.3f\n" % (stationId,plaqId,chipId,row,col,fit_p0,fit_p1,fit_p2,fit_p3)
    return word

def fitGains(rootFileName,outputFileName,stationId,plaqId,chipId,mode='linear',verbose=True):
    file = ROOT.TFile(rootFileName,'READ')
    outFile = open(outputFileName,'w')

    option = ""
    if not verbose: option = "QN"

    #stationId = 0
    #plaqId = 0
    #chipId = 0
    def writeWord(word):
        if verbose: print word
        outFile.write(word)

    nRows = 80
    nCols = 52
    if mode == 'linear': outFile.write("st pl ch r c slope pedestal max\n")
    elif mode == 'tgh': outFile.write("st pl ch r c p0 p1 p2 p3\n")
    for row in range(nRows):
        for col in range(nCols):
            if mode == 'linear':
                word = fitLinear(file,stationId,plaqId,chipId,row,col,option)
                writeWord(word)
            elif mode == 'tgh':
                word = fitTgh(file,stationId,plaqId,chipId,row,col,option)
                writeWord(word)

    file.Close()
    outFile.close()

if __name__ == '__main__':
    import optparse
    parser = optparse.OptionParser(usage="usage: %prog [options] <file name>")
    parser.add_option("-o","--out", dest="out", help="Output file name")
    parser.add_option("-m","--mode", dest="mode", default='linear', help="Fit mode: 'linear' or 'tgh' (default: linear)")
    parser.add_option("-s","--station", dest="station", type='int', default=0, help="Station Id")
    parser.add_option("-p","--plaq", dest="plaq", type='int', default=0, help="Plaquette Id")
    parser.add_option("-c","--chip", dest="chip", type='int', default=0, help="Chip Id")

    (input, args) = parser.parse_args()
    if len(args) != 1: parser.error('need to specify calibration file name')
    inputFile = args[0]  

    outFile = ''
    if hasattr(input,'out') and input.out:
        outFile = input.out
    else:
        outFile = 'gains_%s.txt' % os.path.basename(inputFile).split('.')[0]

    fitGains(inputFile,outFile,
                       stationId=input.station,
                       plaqId=input.plaq,
                       chipId=input.chip,
                       mode=input.mode)

    sys.exit(0)
