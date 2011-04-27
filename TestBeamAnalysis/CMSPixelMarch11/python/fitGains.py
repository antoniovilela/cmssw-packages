import ROOT
import os,sys

def fitGains(rootFileName,outputFileName):
    file = ROOT.TFile(rootFileName,'READ')
    outFile = open(outputFileName,'w')

    stationId = 0
    plaqId = 0
    chipId = 0

    nRows = 80
    nCols = 52
    minFracHits = 0.90
    minFracADC = 0.85
    outFile.write("st pq ch r c slope ped max\n")
    for row in range(nRows):
        for col in range(nCols):
            h_hits = file.Get( "hits_%d_%d" % (row,col) )
            h_adc = file.Get( "adc_%d_%d" % (row,col) )
            # Find linear fit range
            maxHits = h_hits.GetMaximum()
            iBinMin = -1
            for ibin in range( 1, (h_hits.GetNbinsX()+1) ):
                if (h_hits.GetBinContent(ibin)/maxHits) >= minFracHits:
                    iBinMin = ibin
                    break
    
            minADC = h_adc.GetBinContent(iBinMin)
            maxADC = h_adc.GetMaximum()
            iBinMax = -1
            for ibin in range( 1, (h_adc.GetNbinsX()+1) ):
                if ( (h_adc.GetBinContent(ibin) - minADC)/(maxADC - minADC) ) >= minFracADC:
                    iBinMax = ibin
                    break

            minFit = h_adc.GetBinLowEdge(iBinMin)
            maxFit = h_adc.GetBinLowEdge(iBinMax)
            fitFunc = ROOT.TF1("fit_adc_%d_%d" % (row,col),"pol1",minFit,maxFit)
            h_adc.Fit(fitFunc,"R")
            word = "%d %d %d %d %d %.3f %.3f %.3f\n" % (stationId,plaqId,chipId,row,col,fitFunc.GetParameter(1),fitFunc.GetParameter(0),maxADC)
            print word
            outFile.write(word)

    file.Close()
    outFile.close()

if __name__ == '__main__':
    import optparse
    parser = optparse.OptionParser(usage="usage: %prog [options] <file name>")
    parser.add_option("-o","--out", dest="out", help="Output file name")

    (input, args) = parser.parse_args()
    if len(args) != 1: parser.error('need to specify calibration file name')
    inputFile = args[0]  

    outFile = ''
    if hasattr(input,'out') and input.out:
        outFile = input.out
    else:
        outFile = 'gains_%s.txt' % os.path.basename(inputFile).split('.')[0]

    fitGains(inputFile,outFile)

    sys.exit(0)
