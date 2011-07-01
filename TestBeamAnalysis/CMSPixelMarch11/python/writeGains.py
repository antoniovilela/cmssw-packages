from fitGains import fitGains
import os,sys

if __name__ == '__main__':
    import optparse
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-d","--dir", dest="dir", help="Dir with calibration ROOT files")
    parser.add_option("-o","--out", dest="out", help="Dir with output files")
    parser.add_option("-m","--mode", dest="mode", default='linear', help="Fit mode: 'linear' or 'tgh' (default: linear)")
    parser.add_option("-t","--type", dest="type", default="TELESCOPE", help="Uses file names matching TYPE (Default: 'TELESCOPE')")
    parser.add_option("-v","--verbose", action="store_true", dest="verbose", default=False, help="Verbose mode (default = False)")

    (input, args) = parser.parse_args()

    fileNames = os.listdir(input.dir)
    fileNames = [ '%s/%s' % (input.dir,item) for item in fileNames if item.find(input.type) != -1 ]

    verbose = input.verbose
    mode = input.mode
    outDir = ''
    if hasattr(input,'out'): outDir = input.out
    for file in fileNames:
        outFile = 'gains_%s.txt' % os.path.basename(file).split('.')[0]
        if outDir: outFile = outDir + "/" + outFile
        (st,pl,ch) = file.split('.')[0].split('_')[-3:]         
        print "St=%d Pl=%d Ch=%d" % (int(st),int(pl)/10,int(ch)) 
        print file, outFile
        fitGains(file,outFile,
                      stationId=int(st),
                      plaqId=int(pl)/10,
                      chipId=int(ch),
                      mode=mode,
                      verbose=verbose)

    sys.exit(0)
