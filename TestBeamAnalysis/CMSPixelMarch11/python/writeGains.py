from fitGains import fitGains
import os,sys

if __name__ == '__main__':
    import optparse
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-d","--dir", dest="dir", help="Dir with calibration ROOT files")

    (input, args) = parser.parse_args()

    fileNames = os.listdir(input.dir)
    fileNames = [ '%s/%s' % (input.dir,item) for item in fileNames if item.find('.root') != -1 ]

    for file in fileNames:
        outFile = 'gains_%s.txt' % os.path.basename(file).split('.')[0]
        (st,pl,ch) = file.split('.')[0].split('_')[-3:]         
        print file, outFile
        print "St=%d Pl=%d Ch=%d" % (int(st),int(pl)/10,int(ch)) 
        fitGains(file,outFile,
                      stationId=int(st),
                      plaqId=int(pl)/10,
                      chipId=int(ch),
                      verbose=False)

    sys.exit(0)
