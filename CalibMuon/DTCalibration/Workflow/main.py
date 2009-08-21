from DTTTrigProd import DTTTrigProd
from DTTTrigCorrFirst import DTTTrigCorrFirst
from DTTTrigValid import DTTTrigValid
from DTTTrigResidualCorr import DTTTrigResidualCorr
from DTValidSummary import DTValidSummary
from CrabWatch import CrabWatch
from workflow import haddInCastor,crabWatch,getOutput
import sys,os,time

#FIXME: This should come from some configuration module
castor_prefix = '/castor/cern.ch/cms/store/caf/user/antoniov/'
 
def_crab_opts = {'DATASETPATH':'/StreamExpress/CRAFT09-MuAlCalIsolatedMu-v1/ALCARECO',
                 'EMAIL':'vilela@to.infn.it'}

def_pset_opts = {'GLOBALTAG':'GR09_P_V1::All'}

def runTtrigProd(run,runselection,trial,result_dir):

    crab_opts = {'RUNSELECTION':runselection,
                 'USERDIRCAF':'TTRIGCalibration/Production/Run' + str(run) + '/v' + str(trial)}
    crab_opts.update(def_crab_opts)

    pset_opts = {'MUDIGILABEL':'muonDTDigis'}
    pset_opts.update(def_pset_opts)

    dtTtrigProd = DTTTrigProd(run,crab_opts,pset_opts) 
    project_prod = dtTtrigProd.run()

    print "Sent production jobs with project",project_prod

    #crabWatch(getOutput,project_prod) 
    crabProd = CrabWatch(project_prod)
    crabProd.start()
    crabProd.join()

    castor_dir = castor_prefix + dtTtrigProd.crab_opts['USERDIRCAF']

    result_file = result_dir + '/DTTimeBoxes_%s.root'%run
    haddInCastor(castor_dir,result_file)

def runTtrigCorrFirst(run,result_dir):
    dtTtrigCorrFirst = DTTTrigCorrFirst(run,result_dir)  
    dtTtrigCorrFirst.run()

    print "Finished processing:"
    for pset in dtTtrigCorrFirst.configs: print "--->",pset

def runTtrigValid(run,runselection,trial,input_file,result_dir):
    #ttrig_second_db = os.path.abspath(result_dir + '/' + 'ttrig_second_' + run + '.db')

    crab_opts = {'RUNSELECTION':runselection,
                 'USERDIRCAF':'TTRIGCalibration/Validation/First/Run' + str(run) + '/v' + str(trial),
                 'INPUTFILE':input_file}

    crab_opts.update(def_crab_opts)
 
    pset_opts = {'INPUTFILE':input_file.split('/')[-1]}
    pset_opts.update(def_pset_opts)

    dtTtrigValid = DTTTrigValid(run,crab_opts,pset_opts) 
    project_valid_first = dtTtrigValid.run()

    print "Sent validation jobs with project",project_valid_first

    #crabWatch(getOutput,project_valid_first)
    crabValidFirst = CrabWatch(project_valid_first)
    crabValidFirst.start()
    crabValidFirst.join()

    result_file = result_dir + '/DTkFactValidation_%s.root'%run
    castor_dir = castor_prefix + dtTtrigValid.crab_opts['USERDIRCAF']
    haddInCastor(castor_dir,result_file,'residuals')

def runTtrigResidualCorr(run,result_dir):
    dtTtrigResidualCorr = DTTTrigResidualCorr(run,result_dir)  
    dtTtrigResidualCorr.run()

    print "Finished processing:"
    for pset in dtTtrigResidualCorr.configs: print "--->",pset

def runTtrigValidResidCorr(run,runselection,trial,input_file,result_dir):
    #ttrig_ResidCorr_db = os.path.abspath(result_dir + '/' + 'ttrig_ResidCorr_' + run + '.db')

    crab_opts = {'RUNSELECTION':runselection,
                 'USERDIRCAF':'TTRIGCalibration/Validation/ResidCorr/Run' + str(run) + '/v' + str(trial),
                 'INPUTFILE':input_file}

    crab_opts.update(def_crab_opts)

    pset_opts = {'INPUTFILE':input_file.split('/')[-1]}
    pset_opts.update(def_pset_opts)

    dtTtrigValid_ResidCorr = DTTTrigValid(run,crab_opts,pset_opts) 
    project_valid_ResidCorr = dtTtrigValid_ResidCorr.run()

    print "Sent validation jobs with project",project_valid_ResidCorr

    #crabWatch(getOutput,project_valid_ResidCorr)
    crabValidResidCorr = CrabWatch(project_valid_ResidCorr)
    crabValidResidCorr.start()
    crabValidResidCorr.join()

    result_file = result_dir + '/DTkFactValidation_ResidCorr_%s.root'%run
    castor_dir = castor_prefix + dtTtrigValid_ResidCorr.crab_opts['USERDIRCAF']
    haddInCastor(castor_dir,result_file,'residuals')

def runTtrigValidSummary(run,input_file,result_dir):
    #input_file = os.path.abspath(result_file)

    dtTtrigValidSummary = DTValidSummary(run,input_file,result_dir)
    dtTtrigValidSummary.run()

    print "Finished processing:"
    for pset in dtTtrigValidSummary.configs: print "--->",pset

if __name__ == '__main__':
    start = time.time()
    
    run = None
    runselection = None
    trial = None
    #castor_prefix = 'rfio:/castor/cern.ch/cms/store/caf/user/antoniov/'
    for opt in sys.argv:
        if opt[:4] == 'run=':
            run = opt[4:]
        if opt[:13] == 'runselection=':
            runselection = opt[13:] 
        if opt[:6] == 'trial=':
            trial = opt[6:]
        if opt[:7] == 'castor=':
            castor_prefix = opt[7:]

    if not run: raise ValueError,'Need to set run number'
    if not trial: raise ValueError,'Need to set trial number'
    #if not castor_prefix: raise ValueError,'Need to set castor dir'

    if not runselection: runselection = run

    result_dir = 'Run%s'%run
    result_dir += '/Ttrig/Results'
    if not os.path.exists(result_dir): os.makedirs(result_dir)

    log_dir = 'Run%s'%run
    log_dir += '/Ttrig/Log'
    if not os.path.exists(log_dir): os.makedirs(log_dir)
 
    #logFileName = os.path.abspath('%s/Run_%s_v%s.log' % (log_dir,run,trial))
    #logOut = open(logFileName,'w',1)

    print "DT Calibration starting for Run",run
    print "Using runs",runselection
    print "Results at",result_dir 
    #print "Log file at",logFileName
 
    #stdout_original = sys.stdout
    #sys.stdout = logOut

    timeBoxes = os.path.abspath(result_dir + '/' + 'DTTimeBoxes_' + run + '.root')

    if not os.path.exists(timeBoxes): runTtrigProd(run,runselection,trial,result_dir)
   
    if not os.path.exists(timeBoxes): raise RuntimeError,'Could not produce %s'%timeBoxes

    ttrig_second_db = os.path.abspath(result_dir + '/' + 'ttrig_second_' + run + '.db')

    if not os.path.exists(ttrig_second_db): runTtrigCorrFirst(run,result_dir)

    if not os.path.exists(ttrig_second_db): raise RuntimeError,'Could not produce %s'%ttrig_second_db

    residualsFirst = os.path.abspath(result_dir + '/' + 'DTkFactValidation_' + run + '.root')

    if not os.path.exists(residualsFirst): runTtrigValid(run,runselection,trial,ttrig_second_db,result_dir)

    if not os.path.exists(residualsFirst): raise RuntimeError,'Could not produce %s'%residualsFirst

    ttrig_ResidCorr_db = os.path.abspath(result_dir + '/' + 'ttrig_ResidCorr_' + run + '.db')

    if not os.path.exists(ttrig_ResidCorr_db): runTtrigResidualCorr(run,result_dir)

    if not os.path.exists(ttrig_ResidCorr_db): raise RuntimeError,'Could not produce %s'%ttrig_ResidCorr_db      

    residualsResidCorr = os.path.abspath(result_dir + '/' + 'DTkFactValidation_ResidCorr_' + run + '.root')
    
    if not os.path.exists(residualsResidCorr): runTtrigValidResidCorr(run,runselection,trial,ttrig_ResidCorr_db,result_dir)

    if not os.path.exists(residualsResidCorr): raise RuntimeError,'Could not produce %s'%residualsResidCorr

    summaryResiduals = os.path.abspath(result_dir + '/' + 'SummaryResiduals_' + run + '.root')

    if not os.path.exists(summaryResiduals): runTtrigValidSummary(run,residualsResidCorr,result_dir)

    if not os.path.exists(summaryResiduals): raise RuntimeError,'Could not produce %s'%summaryResiduals

    #sys.stdout = stdout_original
 
    stop = time.time() 
    print "DT Calibration finished for Run",run
    print "Time elapsed was %.1f seconds"%(stop-start)
