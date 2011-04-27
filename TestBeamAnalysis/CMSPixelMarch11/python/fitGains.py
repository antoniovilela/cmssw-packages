import ROOT
import os,sys

def fitGains(rootFileName,outputFileName,stationId,plaqId,chipId,verbose=True):
    file = ROOT.TFile(rootFileName,'READ')
    outFile = open(outputFileName,'w')

    option = ""
    if not verbose: option = "Q"

    #stationId = 0
    #plaqId = 0
    #chipId = 0
    def writeWord(word):
        if verbose: print word
        outFile.write(word)

    nRows = 80
    nCols = 52
    minFracHits = 0.90
    minFracADC = 0.85
    outFile.write("st pl ch r c slope pedestal max\n")
    for row in range(nRows):
        for col in range(nCols):
            h_hits = file.Get( "hits_%d_%d" % (row,col) )
            h_adc = file.Get( "adc_%d_%d" % (row,col) )
            # Find linear fit range
            maxHits = h_hits.GetMaximum()
            if maxHits == 0:
                word = "%d %d %d %d %d %.3f %.3f %.3f\n" % (stationId,plaqId,chipId,row,col,0.0,0.0,0.0)
                writeWord(word)
                continue
 
            iBinMin = -1
            for ibin in range( 1, (h_hits.GetNbinsX()+1) ):
                if (h_hits.GetBinContent(ibin)/maxHits) >= minFracHits:
                    iBinMin = ibin
                    break
    
            minADC = h_adc.GetBinContent(iBinMin)
            maxADC = h_adc.GetMaximum()
            if maxADC == 0 or (maxADC - minADC) == 0: 
                word = "%d %d %d %d %d %.3f %.3f %.3f\n" % (stationId,plaqId,chipId,row,col,0.0,0.0,0.0)
                writeWord(word)
                continue

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
            if fit_slope <= 0:
                word = "%d %d %d %d %d %.3f %.3f %.3f\n" % (stationId,plaqId,chipId,row,col,0.0,0.0,0.0)
                writeWord(word)
                continue

            # Write output
            word = "%d %d %d %d %d %.3f %.3f %.3f\n" % (stationId,plaqId,chipId,row,col,fit_slope,fit_ped,maxADC)
            writeWord(word)

    file.Close()
    outFile.close()

if __name__ == '__main__':
    import optparse
    parser = optparse.OptionParser(usage="usage: %prog [options] <file name>")
    parser.add_option("-o","--out", dest="out", help="Output file name")
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
                       chipId=input.chip)

    sys.exit(0)
