from workflow import replaceTemplate
from CrabTask import *
import os

class DTTTrigValid:
    def __init__(run,input_file,trial):
        self.pset_name = 'DTkFactValidation_1_cfg.py'

        self.crab_template = 'workflow/templates/crab/crab_Valid_TEMPL.cfg'
        self.pset_template = 'workflow/templates/config/DTkFactValidation_1_TEMPL_cfg.py'

        self.crab_opts = {'DATASETPATH':'/Cosmics/Commissioning09-v2/RAW',
                          'RUNNUMBER':run,
                          'PSET':pset_name, 
                          'USERDIRCAF':'TTRIGCalibration/Validation/First/Run' + str(run) + '/v' + str(trial), 
                          'INPUTFILE':input_file,
                          'EMAIL':'vilela@to.infn.it'}

        self.pset_opts = {'GLOBALTAG':'CRAFT_31X::All',
                          'INPUTFILE':input_file.split('/')[-1]}

        self.crab_cfg = replaceTemplate(crab_template,**crab_opts)
        self.pset = replaceTemplate(pset_template,**pset_opts)

        desc = 'Run%s'%run
        desc += '/Ttrig/Validation/First'
        self.desc = desc 

    def createCrab():
        prod = CrabTask(self.desc,self.crab_cfg,self.pset,self.pset_name)
        return prod

if __name__ == '__main__':

    run = None
    trial = None
    import sys
    for opt in sys.argv:
        if opt[:4] == 'run=':
            run = opt[4:]
        if opt[:6] == 'trial=':
            trial = opt[6:]

    if not run: raise ValueError,'Need to set run number'
    if not trial: raise ValueError,'Need to set trial number'

    result_dir = 'Run%s'%run
    result_dir += '/Ttrig/Results'

    ttrig_second_db = result_dir + '/' + 'ttrig_second_' + run + '.db'

    dtTtrigValid = DTTTrigValid(run,trial,ttrig_second_db): 
    project = dtTtrigValid.createCrab().run()

    print "Sent validation jobs with project",project
