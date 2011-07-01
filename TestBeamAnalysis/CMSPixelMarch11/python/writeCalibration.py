from readCalibration import readCalibration
import os,sys

if __name__ == '__main__':
    import optparse
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-d","--dir", dest="dir", help="Dir with calibration input files")
    parser.add_option("-o","--out", dest="out", help="Dir with calibration ROOT files")
    parser.add_option("-t","--type", dest="type", default="TELESCOPE", help="Uses file names matching TYPE (Default: 'TELESCOPE')")

    (input, args) = parser.parse_args()

    fileNames = os.listdir(input.dir)
    fileNames = [ '%s/%s' % (input.dir,item) for item in fileNames if item.find(input.type) != -1 ]

    outDir = ''
    if hasattr(input,'out'): outDir = input.out
    for file in fileNames:
        outFile = '%s.root' % os.path.basename(file).split('.')[0]
        if outDir: outFile = outDir + "/" + outFile
        (st,pl,ch) = file.split('.')[0].split('_')[-3:]
        print "St=%d Pl=%d Ch=%d" % (int(st),int(pl)/10,int(ch))
        print file, outFile
        readCalibration(file,outFile) 

    sys.exit(0)
