from workflow import replaceTemplate
from CrabTask import *
import os

class DTTTrigProd:
    def __init__(run,trial):
        self.pset_name = 'DTTTrigCalibration_cfg.py'

        self.crab_template = 'workflow/templates/crab/crab_ttrig_prod_TEMPL.cfg'
        self.pset_template = 'workflow/templates/config/DTTTrigCalibration_TEMPL_cfg.py'

        self.crab_opts = {'DATASETPATH':'/Cosmics/Commissioning09-v2/RAW',
                          'RUNNUMBER':run,
                          'PSET':pset_name, 
                          'USERDIRCAF':'TTRIGCalibration/Run' + str(run) + '/v' + str(trial),
                          'EMAIL':'vilela@to.infn.it'}

        self.pset_opts = {'GLOBALTAG':'CRAFT_31X::All',
                          'MUDIGILABEL':'muonDTDigis'}

        self.crab_cfg = replaceTemplate(crab_template,**crab_opts)
        self.pset = replaceTemplate(pset_template,**pset_opts)

        desc = 'Run%s'%run
        desc += '/Ttrig/Production'
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

    dtTtrigProd = DTTTrigProd(run,trial): 
    project = dtTtrigProd.createCrab().run()

    print "Sent jobs with project",project
