import sys

def parseInput(inputFields,requiredFields = ()):

    class options: pass
    for item in sys.argv:
        option = item.split('=')[0].strip()
        if option in inputFields:
            value = item.split('=')[1].strip()
            if value in ('true','True','yes','Yes'): value = True
            elif value in ('false','False','no','No'): value = False

            setattr(options,option,value)

    for item in requiredFields:
        if not hasattr(options,item):
            raise RuntimeError,'Need to set "%s"' % item

    return options

def hadd(dir,outfile,type = 'root',option=''):
    if not dir: raise ValueError,'Please specify valid castor dir'
    if not outfile: raise ValueError,'Please specify valid output file name'

    from subprocess import Popen,PIPE,call
    p1 = Popen(['ls',dir],stdout=PIPE)
    p2 = Popen(['grep',type],stdin=p1.stdout,stdout=PIPE)
    files = [dir + "/" + item[:-1] for item in p2.stdout]
    p2.stdout.close()
    #cmd = ['hadd',result_file]
    cmd = ['hadd']
    if option: cmd.append(option)
    cmd.append(outfile)
    cmd.extend(files)
    print cmd
    #retcode = call(cmd)
    return retcode

