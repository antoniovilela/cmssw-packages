from workflow import replaceTemplate
import os

class CmsswTask:
    def __init__(self, desc, common_opts, pset_templates, pset_opts):
        self.desc = desc 
        self.common_opts = common_opts
        self.pset_templates = pset_templates
        self.pset_opts = pset_opts

        self.pset_list = None
        self.makePSetList()
  
    def makePSetList(self):
        pset_list = {}
        for pset_name in self.pset_templates:
            opts = self.pset_opts[pset_name]
            opts.update(self.common_opts)
            pset = replaceTemplate(self.pset_templates[pset_name],**opts)
            pset_list[pset_name] = pset

        self.pset_list = pset_list

    def run(self):
        dir = self.desc
        if not os.path.exists(dir): os.makedirs(dir)

        cwd = os.getcwd()
        for pset in self.pset_list:
            open(dir + '/' + pset,'w').write(self.pset_list[pset])

            os.chdir(dir)
            cmd = 'cmsRun %s'%pset
            print "Running", cmd, "in dir", dir
            #os.system(cmd)
            os.chdir(cwd)       
     
if __name__ == '__main__':

    run = None
    import sys
    for opt in sys.argv:
        if opt[:4] == 'run=':
            run = opt[4:] 
 
    if not run: raise ValueError,'Need to set run number' 
 
    desc = 'Run%s'%run
    desc += '/Ttrig/Exec'

    common_opts = {'GLOBALTAG':'CRAFT_31X::All'}

    pset_templates = {'DTTTrigWriter_cfg.py':'Workflow/templates/config/DTTTrigWriter_TEMPL_cfg.py',
                      'DumpDBToFile_first_cfg.py':'Workflow/templates/config/DumpDBToFile_ttrig_TEMPL_cfg.py',
                      'DTTTrigCorrection_cfg.py':'Workflow/templates/config/DTTTrigCorrection_TEMPL_cfg.py',
                      'DumpDBToFile_second_cfg.py':'Workflow/templates/config/DumpDBToFile_ttrig_TEMPL_cfg.py'}

    result_dir = 'Run%s'%run
    result_dir += '/Ttrig/Results'
    if not os.path.exists(result_dir): os.makedirs(result_dir)

    timeBoxes = os.path.abspath(result_dir + '/' + 'DTTimeBoxes_' + run + '.root')
    ttrig_first = result_dir + '/' + 'ttrig_first_' + run
    ttrig_first_db = os.path.abspath(ttrig_first + '.db')
    ttrig_first_txt = os.path.abspath(ttrig_first + '.txt')
    ttrig_second = result_dir + '/' + 'ttrig_second_' + run
    ttrig_second_db = os.path.abspath(ttrig_second + '.db')
    ttrig_second_txt = os.path.abspath(ttrig_second + '.txt')

    pset_opts = {'DTTTrigWriter_cfg.py':{'INPUTFILE':timeBoxes,'OUTPUTFILE':ttrig_first_db},
                 'DumpDBToFile_first_cfg.py':{'INPUTFILE':ttrig_first_db,'OUTPUTFILE':ttrig_first_txt},
                 'DTTTrigCorrection_cfg.py':{'INPUTFILE':ttrig_first_db,'OUTPUTFILE':ttrig_second_db},
                 'DumpDBToFile_second_cfg.py':{'INPUTFILE':ttrig_second_db,'OUTPUTFILE':ttrig_second_txt}}

    task = CmsswTask(desc,common_opts,pset_templates,pset_opts)
    task.run()

    print "Finished processing:"
    for pset_name in pset_templates: print "--->",pset_name
