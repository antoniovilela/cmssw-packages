from workflow import replaceTemplate
from CrabTask import *
import os

class DTTTrigProd:
    def __init__(self,run,trial):
        pset_name = 'DTTTrigCalibration_cfg.py'

        self.crab_template = os.environ['CMSSW_BASE'] + '/src/Workflow/' + 'templates/crab/crab_ttrig_prod_TEMPL.cfg'
        self.pset_template = os.environ['CMSSW_BASE'] + '/src/Workflow/' + 'templates/config/DTTTrigCalibration_TEMPL_cfg.py'

        self.crab_opts = {'DATASETPATH':'/StreamExpress/CRAFT09-MuAlCalIsolatedMu-v1/ALCARECO',
                          'RUNNUMBER':run,
                          'PSET':pset_name, 
                          'USERDIRCAF':'TTRIGCalibration/Production/Run' + str(run) + '/v' + str(trial),
                          'EMAIL':'vilela@to.infn.it'}

        self.pset_opts = {'GLOBALTAG':'GR09_P_V1::All',
                          'MUDIGILABEL':'muonDTDigis'}

        self.crab_cfg = replaceTemplate(self.crab_template,**self.crab_opts)
        self.pset = replaceTemplate(self.pset_template,**self.pset_opts)

        desc = 'Run%s'%run
        desc += '/Ttrig/Production'
        self.desc = desc 

        self.task = CrabTask(self.desc,self.crab_cfg,self.pset,pset_name)

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

    dtTtrigProd = DTTTrigProd(run,trial) 
    project = dtTtrigProd.run()

    print "Sent production jobs with project",project
