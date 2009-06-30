from workflow import replaceTemplate
from CrabTask import *
import os

class DTTTrigValid:
    def __init__(self,run,input_file,trial):
        pset_name = 'DTkFactValidation_1_cfg.py'

        self.crab_template = os.environ['CMSSW_BASE'] + '/src/Workflow/' + 'templates/crab/crab_Valid_TEMPL.cfg'
        self.pset_template = os.environ['CMSSW_BASE'] + '/src/Workflow/' + 'templates/config/DTkFactValidation_1_TEMPL_cfg.py'

        self.crab_opts = {'DATASETPATH':'/Cosmics/Commissioning09-v2/RAW',
                          'RUNNUMBER':run,
                          'PSET':pset_name, 
                          'USERDIRCAF':'TTRIGCalibration/Validation/First/Run' + str(run) + '/v' + str(trial), 
                          'INPUTFILE':input_file,
                          'EMAIL':'vilela@to.infn.it'}

        self.pset_opts = {'GLOBALTAG':'CRAFT_31X::All',
                          'INPUTFILE':input_file.split('/')[-1]}

        self.crab_cfg = replaceTemplate(self.crab_template,**self.crab_opts)
        self.pset = replaceTemplate(self.pset_template,**self.pset_opts)

        desc = 'Run%s'%run
        desc += '/Ttrig/Validation/First'
        self.desc = desc 

        self.task = CrabTask(self.desc,self.crab_cfg,self.pset,self.pset_name)

    def run(self):
        self.project = self.task.run()
        return self.project

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

    dtTtrigValid = DTTTrigValid(run,ttrig_second_db,trial) 
    project = dtTtrigValid.run()

    print "Sent validation jobs with project",project
