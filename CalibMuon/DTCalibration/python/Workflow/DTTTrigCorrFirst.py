from workflow import replaceTemplate
from CmsswTask import *
import os

class DTTTrigCorrFirst:
    def __init__(self, run, result_dir):
        desc = 'Run%s'%run
        desc += '/Ttrig/Exec'
        self.desc = desc 

        self.common_opts = {'GLOBALTAG':'GR09_P_V1::All'}

        self.configs = ['DTTTrigWriter_cfg.py','DumpDBToFile_first_cfg.py','DTTTrigCorrection_cfg.py','DumpDBToFile_second_cfg.py']

        base = os.environ['CMSSW_BASE'] + '/src/' 
        self.pset_templates = {'DTTTrigWriter_cfg.py':base+'Workflow/templates/config/DTTTrigWriter_TEMPL_cfg.py',
                               'DumpDBToFile_first_cfg.py':base+'Workflow/templates/config/DumpDBToFile_ttrig_TEMPL_cfg.py',
                               'DTTTrigCorrection_cfg.py':base+'Workflow/templates/config/DTTTrigCorrection_TEMPL_cfg.py',
                               'DumpDBToFile_second_cfg.py':base+'Workflow/templates/config/DumpDBToFile_ttrig_TEMPL_cfg.py'}

        timeBoxes = os.path.abspath(result_dir + '/' + 'DTTimeBoxes_' + run + '.root')
        ttrig_first = result_dir + '/' + 'ttrig_first_' + run
        ttrig_first_db = os.path.abspath(ttrig_first + '.db')
        ttrig_first_txt = os.path.abspath(ttrig_first + '.txt')
        ttrig_second = result_dir + '/' + 'ttrig_second_' + run
        ttrig_second_db = os.path.abspath(ttrig_second + '.db')
        ttrig_second_txt = os.path.abspath(ttrig_second + '.txt')

        self.pset_opts = {'DTTTrigWriter_cfg.py':{'INPUTFILE':timeBoxes,'OUTPUTFILE':ttrig_first_db},
                          'DumpDBToFile_first_cfg.py':{'INPUTFILE':ttrig_first_db,'OUTPUTFILE':ttrig_first_txt},
                          'DTTTrigCorrection_cfg.py':{'INPUTFILE':ttrig_first_db,
                                                      'OUTPUTFILE':ttrig_second_db,
                                                      'RUNNUMBER':run},
                          'DumpDBToFile_second_cfg.py':{'INPUTFILE':ttrig_second_db,'OUTPUTFILE':ttrig_second_txt}}

        self.task = CmsswTask(self.desc,self.configs,self.common_opts,self.pset_templates,self.pset_opts)

    def run(self):
        self.task.run()

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

    dtTtrigCorrFirst = DTTTrigCorrFirst(run,result_dir)  
    dtTtrigCorrFirst.run()

    print "Finished processing:"
    for pset in dtTtrigCorrFirst.configs: print "--->",pset