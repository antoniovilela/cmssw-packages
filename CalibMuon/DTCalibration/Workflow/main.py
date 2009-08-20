from DTTTrigProd import DTTTrigProd
from DTTTrigCorrFirst import DTTTrigCorrFirst
from DTTTrigValid import DTTTrigValid
from DTTTrigResidualCorr import DTTTrigResidualCorr
from DTValidSummary import DTValidSummary
from CrabWatch import CrabWatch
from workflow import haddInCastor,crabWatch,getOutput
import sys,os,time

start = time.time()

run = None
trial = None
castor_prefix = '/castor/cern.ch/cms/store/caf/user/antoniov/'
for opt in sys.argv:
    if opt[:4] == 'run=':
        run = opt[4:]
    if opt[:6] == 'trial=':
        trial = opt[6:]
    if opt[:7] == 'castor=':
        castor_prefix = opt[7:]

if not run: raise ValueError,'Need to set run number'
if not trial: raise ValueError,'Need to set trial number'
#if not castor_prefix: raise ValueError,'Need to set castor dir'

dtTtrigProd = DTTTrigProd(run,trial) 
project_prod = dtTtrigProd.run()

print "Sent production jobs with project",project_prod

#crabWatch(getOutput,project_prod) 
crabProd = CrabWatch(project_prod)
crabProd.start()
crabProd.join()

castor_dir = castor_prefix + dtTtrigProd.crab_opts['USERDIRCAF']

result_dir = 'Run%s'%run
result_dir += '/Ttrig/Results'
if not os.path.exists(result_dir): os.makedirs(result_dir)

result_file = result_dir + '/DTTimeBoxes_%s.root'%run
haddInCastor(castor_dir,result_file)

dtTtrigCorrFirst = DTTTrigCorrFirst(run,result_dir)  
dtTtrigCorrFirst.run()

print "Finished processing:"
for pset in dtTtrigCorrFirst.configs: print "--->",pset

ttrig_second_db = os.path.abspath(result_dir + '/' + 'ttrig_second_' + run + '.db')

opts = {'USERDIRCAF':'TTRIGCalibration/Validation/First/Run' + str(run) + '/v' + str(trial)}

dtTtrigValid = DTTTrigValid(run,ttrig_second_db,opts,trial) 
project_valid_first = dtTtrigValid.run()

print "Sent validation jobs with project",project_valid_first

#crabWatch(getOutput,project_valid_first)
crabValidFirst = CrabWatch(project_valid_first)
crabValidFirst.start()
crabValidFirst.join()

result_file = result_dir + '/DTkFactValidation_%s.root'%run
castor_dir = castor_prefix + dtTtrigValid.crab_opts['USERDIRCAF']
haddInCastor(castor_dir,result_file,'residuals')

dtTtrigResidualCorr = DTTTrigResidualCorr(run,result_dir)  
dtTtrigResidualCorr.run()

print "Finished processing:"
for pset in dtTtrigResidualCorr.configs: print "--->",pset

ttrig_ResidCorr_db = os.path.abspath(result_dir + '/' + 'ttrig_ResidCorr_' + run + '.db')

opts = {'USERDIRCAF':'TTRIGCalibration/Validation/ResidCorr/Run' + str(run) + '/v' + str(trial)}

dtTtrigValid_ResidCorr = DTTTrigValid(run,ttrig_ResidCorr_db,opts,trial) 
project_valid_ResidCorr = dtTtrigValid_ResidCorr.run()

print "Sent validation jobs with project",project_valid_ResidCorr

#crabWatch(getOutput,project_valid_ResidCorr)
crabValidResidCorr = CrabWatch(project_valid_ResidCorr)
crabValidResidCorr.start()
crabValidResidCorr.join()

result_file = result_dir + '/DTkFactValidation_ResidCorr_%s.root'%run
castor_dir = castor_prefix + dtTtrigValid_ResidCorr.crab_opts['USERDIRCAF']
haddInCastor(castor_dir,result_file,'residuals')

input_file = os.path.abspath(result_file)

dtTtrigValidSummary = DTValidSummary(run,input_file,result_dir)
dtTtrigValidSummary.run()

print "Finished processing:"
for pset in dtTtrigValidSummary.configs: print "--->",pset

stop = time.time() 
print "DT Calibration finished for Run",run
print "Time elapsed was %.1f seconds"%(stop-start)
