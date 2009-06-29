from workflow import replaceTemplate
from CmsswTask import *
import os

class DTTTrigResidualCorr:
    def __init__(self, run, result_dir):
        desc = 'Run%s'%run
        desc += '/Ttrig/Exec'
        desc = desc 

        common_opts = {'GLOBALTAG':'CRAFT_31X::All'}

        pset_templates = {'DTTTrigResidualCorrection_cfg.py':'Workflow/templates/config/DTTTrigResidualCorrection_TEMPL_cfg.py',
                          'DumpDBToFile_ResidCorr_cfg.py':'Workflow/templates/config/DumpDBToFile_ttrig_TEMPL_cfg.py'}

        ttrig_second_db = result_dir + '/' + 'ttrig_second_' + run + '.db'
        ttrig_ResidCorr = result_dir + '/' + 'ttrig_ResidCorr_' + run
        ttrig_ResidCorr_db = os.path.abspath(ttrig_ResidCorr + '.db')
        ttrig_ResidCorr_txt = os.path.abspath(ttrig_ResidCorr + '.txt')

        pset_opts = {'DTTTrigResidualCorrection_cfg.py':{'INPUTDBFILE':ttrig_second_db,'OUTPUTDBFILE':ttrig_ResidCorr_db},
                     'DumpDBToFile_ResidCorr_cfg.py':{'INPUTFILE':ttrig_ResidCorr_db,'OUTPUTFILE':ttrig_ResidCorr_txt}}

        self.task = CmsswTask(common_opts,pset_templates,pset_opts)

        return

    def run(self):
        self.task.run()
        return

if __name__ == '__main__':

    run = None
    import sys
    for opt in sys.argv:
        if opt[:4] == 'run=':
            run = opt[4:] 
 
    if not run: raise ValueError,'Need to set run number' 
 
    result_dir = 'Run%s'%run
    result_dir += '/Ttrig/Results'
    if not os.path.exists(result_dir): os.makedirs(result_dir)

    dtTtrigResidualCorr = DTTTrigResidualCorr(run,result_dir)  
    dtTtrigResidualCorr.run()

    print "Finished processing:"
    for pset_name in dtTtrigResidualCorr.pset_templates: print "--->",pset_name
