from workflow import replaceTemplate
from CmsswTask import *
import os

class DTValidSummary:
    def __init__(self, run, input_file, result_dir):
        desc = 'Run%s'%run
        desc += '/Ttrig/Exec'
        self.desc = desc 

        self.common_opts = {'GLOBALTAG':'GR09_P_V1::All'}

        self.configs = ['DTkFactValidation_2_cfg.py']

        base = os.environ['CMSSW_BASE'] + '/src/' 
        self.pset_templates = {}
        self.pset_templates['DTkFactValidation_2_cfg.py'] = base + 'Workflow/templates/config/DTkFactValidation_2_TEMPL_cfg.py'

        output_file = os.path.abspath(result_dir + '/' + 'SummaryResiduals_' + run + '.root')

        self.pset_opts = {} 
        self.pset_opts['DTkFactValidation_2_cfg.py'] = {'INPUTFILE':input_file,
                                                        'OUTPUTFILE':output_file}

        self.task = CmsswTask(self.desc,self.configs,self.common_opts,self.pset_templates,self.pset_opts)

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

    input_file = os.path.abspath(result_dir + '/' + 'DTkFactValidation_' + run + '.root')

    dtTtrigValidSummary = DTValidSummary(run,input_file,result_dir)  
    dtTtrigValidSummary.run()

    print "Finished processing:"
    for pset in dtTtrigValidSummary.configs: print "--->",pset
