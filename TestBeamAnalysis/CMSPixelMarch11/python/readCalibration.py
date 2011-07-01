import ROOT
import os,sys
import re

def readCalibration(fileName,outputFileName):
    """
    Convert values in input calibration file in ROOT histograms
    """

    file = open(fileName,'r')
    if not file:
        raise RuntimeError, "Could not open file '%s'" % fileName

    rootFile = ROOT.TFile.Open(outputFileName, 'recreate')
    if not rootFile:
        raise RuntimeError, "Could not open output file '%s'" % outputFileName

    histosHits = {}
    histosADC = {}
    for row in range(80):
        for col in range(52):
            index = (row,col)
            title = "Hits in pixel versus register value"
            histosHits[index] = ROOT.TH1F("hits_%d_%d" % (row,col), title,256,0,256)
            title = "ADC in pixel versus register value"
            histosADC[index] = ROOT.TH1F("adc_%d_%d" % (row,col), title,256,0,256)  

    iter = -1
    reg = -1
    for line in file:
        m_iter = re.match('Iteration (?P<iter>\d+) --- reg = (?P<reg>\d+)',line)   
        if m_iter:
            iter = int( m_iter.group('iter') )
            reg = int( m_iter.group('reg') )
            continue 
        if iter == -1: continue

        # Read entry
        (t_row,v_row,t_col,v_col,t_hits,v_hits,t_adc,v_adc) = line.rstrip().split(' ')
        index = (int(v_row),int(v_col))
        histosHits[index].Fill(reg,int(v_hits))
        histosADC[index].Fill(reg,int(v_adc))


    rootFile.Write()
    rootFile.Close()

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
        outFile =  os.path.basename(inputFile).split('.')[0] + '.root'

    readCalibration(inputFile,outFile)

    sys.exit(0)
